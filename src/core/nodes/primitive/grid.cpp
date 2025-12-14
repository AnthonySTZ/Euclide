#include "grid.h"

#include "fields/float2field.h"
#include "fields/float3field.h"
#include "fields/int2field.h"

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

namespace euclide {

Grid::Grid() : Node(0, 1, "Grid") {
    auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    positionField->setMetadata(NodeFieldMetadata{displayName : "Position", step : 0.02f});
    addField("position", positionField);

    auto sizeField = std::make_shared<Float2Field>(1.0, 1.0);
    sizeField->setMetadata(NodeFieldMetadata{displayName : "Size", min : 0.0f, step : 0.02f});
    addField("size", sizeField);

    auto divisionsField = std::make_shared<Int2Field>(1, 1);
    divisionsField->setMetadata(NodeFieldMetadata{displayName : "Divisions", min : 1.0f, step : 1.0f});
    addField("divisions", divisionsField);

    auto orientationField = std::make_shared<NodeField<int>>(0);
    orientationField->setMetadata(NodeFieldMetadata{
        displayName : "Orientation",
        is_combo : true,
        choices : std::move(std::vector<std::string>{"ZX Plane", "XY Plane", "YZ Plane"})
    });
    addField("orientation", orientationField);
}

std::shared_ptr<Mesh> Grid::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    auto output = std::make_shared<Mesh>();

    const float3 position = getField<Float3Field>("position")->getValue();
    const float2 size = getField<Float2Field>("size")->getValue();
    const int2 divisions = getField<Int2Field>("divisions")->getValue();
    const int orientation = getField<NodeField<int>>("orientation")->getValue();

    const GridSettings settings{position, size, divisions, orientation};

    createGrid(*output, settings);

    return output;
}

void Grid::createGrid(Mesh& t_mesh, const GridSettings& t_settings) {
    const int rows = t_settings.divisions[0];
    const int columns = t_settings.divisions[1];

    const int columnsPoints = columns + 1;
    const int rowsPoints = rows + 1;

    const float size_cols = t_settings.size[0];
    const float size_rows = t_settings.size[1];
    const float columnSpacing = size_cols / static_cast<float>(columns);
    const float rowSpacing = size_rows / static_cast<float>(rows);

    float3 basePos{t_settings.position[0], t_settings.position[1], t_settings.position[2]};

    float3 normal{0.0f, 1.0f, 0.0f};

    switch (t_settings.orientation) {
    case GridOrientation::XY:
        basePos[0] -= size_cols / 2;
        basePos[1] -= size_rows / 2;
        normal[1] = 0.0f;
        normal[2] = 1.0f;
        break;
    case GridOrientation::YZ:
        basePos[1] -= size_rows / 2;
        basePos[2] -= size_cols / 2;
        normal[1] = 0.0f;
        normal[0] = 1.0f;
        break;
    case GridOrientation::ZX:
        basePos[0] -= size_cols / 2;
        basePos[2] -= size_rows / 2;
        break;
    }

    // Create Points
    std::vector<float, AlignedAllocator<float, 32>> posCols(columnsPoints);
    for (size_t col = 0; col < columnsPoints; ++col) {
        posCols[col] = col * columnSpacing;
    }
    std::vector<float, AlignedAllocator<float, 32>> posRows(rowsPoints);
    for (size_t row = 0; row < rowsPoints; ++row) {
        posRows[row] = row * rowSpacing;
    }

    size_t pointIdx = 0;
    t_mesh.pointAttribs.resize(rowsPoints * columnsPoints);
    auto positions = t_mesh.pointAttribs.findOrCreate<float, 3>("P");
    float* posX = positions->component<float>(0);
    float* posY = positions->component<float>(1);
    float* posZ = positions->component<float>(2);

    auto normals = t_mesh.pointAttribs.findOrCreate<float, 3>("N");
    float* normalX = normals->component<float>(0);
    float* normalY = normals->component<float>(1);
    float* normalZ = normals->component<float>(2);

#ifdef USE_SIMD
    __m256 __normalX = _mm256_set1_ps(normal[0]);
    __m256 __normalY = _mm256_set1_ps(normal[1]);
    __m256 __normalZ = _mm256_set1_ps(normal[2]);
#endif

    for (size_t row = 0; row < rowsPoints; ++row) {
        size_t col = 0;

#ifdef USE_SIMD
        __m256 __rowOffsets = _mm256_set1_ps(posRows[row]);
        for (; col + 8 <= columnsPoints; col += 8) {
            __m256 __colOffsets = _mm256_load_ps(&posCols[col]);

            __m256 __posX = _mm256_set1_ps(basePos[0]);
            __m256 __posY = _mm256_set1_ps(basePos[1]);
            __m256 __posZ = _mm256_set1_ps(basePos[2]);

            switch (t_settings.orientation) {
            case GridOrientation::XY:
                __posX = _mm256_add_ps(__posX, __colOffsets);
                __posY = _mm256_add_ps(__posY, __rowOffsets);
                break;
            case GridOrientation::YZ:
                __posY = _mm256_add_ps(__posY, __rowOffsets);
                __posZ = _mm256_add_ps(__posZ, __colOffsets);
                break;
            case GridOrientation::ZX:
                __posX = _mm256_add_ps(__posX, __colOffsets);
                __posZ = _mm256_add_ps(__posZ, __rowOffsets);
                break;
            }

            _mm256_storeu_ps(&posX[pointIdx], __posX);
            _mm256_storeu_ps(&posY[pointIdx], __posY);
            _mm256_storeu_ps(&posZ[pointIdx], __posZ);

            _mm256_storeu_ps(&normalX[pointIdx], __normalX);
            _mm256_storeu_ps(&normalY[pointIdx], __normalY);
            _mm256_storeu_ps(&normalZ[pointIdx], __normalZ);

            pointIdx += 8;
        }
#endif

        const float rowOffset = posRows[row];

        for (; col < columnsPoints; ++col) {
            const float colOffset = posCols[col];
            float3 pos = basePos;
            switch (t_settings.orientation) {
            case GridOrientation::XY:
                pos[0] += colOffset;
                pos[1] += rowOffset;
                break;
            case GridOrientation::YZ:
                pos[1] += rowOffset;
                pos[2] += colOffset;
                break;
            case GridOrientation::ZX:
                pos[0] += colOffset;
                pos[2] += rowOffset;
                break;
            }

            posX[pointIdx] = pos[0];
            posY[pointIdx] = pos[1];
            posZ[pointIdx] = pos[2];

            normalX[pointIdx] = normal[0];
            normalY[pointIdx] = normal[1];
            normalZ[pointIdx] = normal[2];

            pointIdx++;
        }
    }

    // Create Vertices
    const size_t numOfPrims = rows * columns;
    auto& vertices = t_mesh.vertices;
    vertices.resize(numOfPrims * 4);

#pragma omp parallel for
    for (uint32_t row = 0; row < rows; ++row) {
        float rowVal = row * columnsPoints;
        const uint32_t vertIdxOffset = row * columns * 4;

        for (size_t column = 0; column < columns; ++column) {
            const uint32_t top_left = rowVal + column;
            const uint32_t bottom_left = top_left + columnsPoints;

            const uint32_t vertIdx = vertIdxOffset + column * 4;

            vertices[vertIdx] = Vertex{top_left};
            vertices[vertIdx + 1] = Vertex{top_left + 1};
            vertices[vertIdx + 2] = Vertex{bottom_left + 1};
            vertices[vertIdx + 3] = Vertex{bottom_left};
        }
    }

    // Create Primitives
    auto& primitives = t_mesh.primitives;
    primitives.resize(numOfPrims);

    uint32_t primVertIdx = 0;
    for (size_t i = 0; i < numOfPrims; ++i) {
        primitives[i] = Primitive{primVertIdx, 4};
        primVertIdx += 4;
    }
}

} // namespace euclide