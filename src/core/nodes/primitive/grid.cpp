#include "grid.h"

#include "fields/float2field.h"
#include "fields/float3field.h"
#include "fields/int2field.h"

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

namespace butter {

Grid::Grid()
    : Node(0, 1, "Grid") 
{
    auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    positionField->setMetadata(NodeFieldMetadata{
        displayName: "Position",
        step: 0.02f
    });
    addField("position", positionField);
    
    auto sizeField = std::make_shared<Float2Field>(1.0, 1.0);
    sizeField->setMetadata(NodeFieldMetadata{
        displayName: "Size",
        min: 0.0f,
        step: 0.02f
    });
    addField("size", sizeField);

    auto divisionsField = std::make_shared<Int2Field>(1, 1);
    divisionsField->setMetadata(NodeFieldMetadata{
        displayName: "Divisions",
        min: 1.0f,
        step: 1.0f
    });
    addField("divisions", divisionsField);

    auto orientationField = std::make_shared<NodeField<int>>(0);
    orientationField->setMetadata(NodeFieldMetadata{
        displayName: "Orientation",
        is_combo: true,
        choices: std::move(std::vector<std::string>{"ZX Plane", "XY Plane", "YZ Plane"})
    });
    addField("orientation", orientationField);
}


std::shared_ptr<Mesh> Grid::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs) const
{
    auto output = std::make_shared<Mesh>();
    
    const float3 position = getField<Float3Field>("position")->getValue();
    const float2 size = getField<Float2Field>("size")->getValue();
    const int2 divisions = getField<Int2Field>("divisions")->getValue();
    const int orientation = getField<NodeField<int>>("orientation")->getValue();

    const GridSettings settings{
        position,
        size,
        divisions,
        orientation
    };

    createGrid(*output, settings);

    return output;
}

void Grid::createGrid(Mesh& t_mesh, const GridSettings& t_settings)
{
    const int rows = t_settings.divisions[0];
    const int columns = t_settings.divisions[1];

    const int columnsPoints = columns + 1;
    const int rowsPoints = rows + 1;

    const float size_cols = t_settings.size[0];
    const float size_rows = t_settings.size[1];
    const float columnSpacing = size_cols / static_cast<float>(columns);
    const float rowSpacing = size_rows / static_cast<float>(rows);

    float3 basePos{
        t_settings.position[0],
        t_settings.position[1],
        t_settings.position[2]
    };

    float3 normal{
        0.0f,
        1.0f,
        0.0f
    };

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
    for (size_t col = 0; col < columnsPoints; ++col){
        posCols[col] = col * columnSpacing;
    }
    std::vector<float, AlignedAllocator<float, 32>> posRows(rowsPoints);
    for (size_t row = 0; row < rowsPoints; ++row){
        posRows[row] = row * rowSpacing;
    }

    auto& points = t_mesh.points;
    size_t pointIdx = 0;

    points.reserve(points.size() + rowsPoints * columnsPoints);
    points.resize(points.size() + rowsPoints * columnsPoints);

    #ifdef USE_SIMD
    __m256 __normalX = _mm256_set1_ps(normal[0]);
    __m256 __normalY = _mm256_set1_ps(normal[1]);
    __m256 __normalZ = _mm256_set1_ps(normal[2]);

    __m256 __colorR = _mm256_set1_ps(1.0f);
    __m256 __colorG = _mm256_set1_ps(1.0f);
    __m256 __colorB = _mm256_set1_ps(1.0f);
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

            _mm256_storeu_ps(&points.posX[pointIdx], __posX);
            _mm256_storeu_ps(&points.posY[pointIdx], __posY);
            _mm256_storeu_ps(&points.posZ[pointIdx], __posZ);

            _mm256_storeu_ps(&points.normalX[pointIdx], __normalX);
            _mm256_storeu_ps(&points.normalY[pointIdx], __normalY);
            _mm256_storeu_ps(&points.normalZ[pointIdx], __normalZ);

            _mm256_storeu_ps(&points.colorR[pointIdx], __colorR);
            _mm256_storeu_ps(&points.colorG[pointIdx], __colorG);
            _mm256_storeu_ps(&points.colorB[pointIdx], __colorB);

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

            points.posX[pointIdx] = pos[0];
            points.posY[pointIdx] = pos[1];
            points.posZ[pointIdx] = pos[2];

            points.normalX[pointIdx] = normal[0];
            points.normalY[pointIdx] = normal[1];
            points.normalZ[pointIdx] = normal[2];

            points.colorR[pointIdx] = 1.0f;
            points.colorG[pointIdx] = 1.0f;
            points.colorB[pointIdx] = 1.0f;
            pointIdx++;
        }
    }

    // Create Primitives
    auto& primitives = t_mesh.primitives;
    auto& vertices = t_mesh.vertices;
    size_t primIdx = primitives.size();
    size_t vertIdx = vertices.size();

    const size_t numOfPrims = rows * columns;
    primitives.reserve(primitives.size() + numOfPrims);
    primitives.resize(primitives.size() + numOfPrims);

    vertices.reserve(vertices.size() + numOfPrims * 4);
    vertices.resize(vertices.size() + numOfPrims * 4);
    
    for (uint32_t row = 0; row < rows; ++row) {
        for (uint32_t column = 0; column < columns; ++column) {
            const uint32_t top_left = row * (columnsPoints) + column;
            const uint32_t bottom_left = top_left + columnsPoints;

            primitives[primIdx++] = Primitive{
                static_cast<uint32_t>(vertIdx), 4
            };

            vertices[vertIdx++] = Vertex{top_left};
            vertices[vertIdx++] = Vertex{top_left + 1};
            vertices[vertIdx++] = Vertex{bottom_left + 1};
            vertices[vertIdx++] = Vertex{bottom_left};
        }
    }
}

}