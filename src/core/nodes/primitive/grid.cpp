#include "grid.h"

#include "fields/float2field.h"
#include "fields/float3field.h"
#include "fields/int2field.h"

#include "utils/timer.h"

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

std::shared_ptr<Mesh> Grid::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs)
{
    Timer timer{"grid"}; // 65ms 1000x1000 grid
    auto output = std::make_shared<Mesh>();
    
    const float3 position = getField<Float3Field>("position")->getValue();
    const float2 size = getField<Float2Field>("size")->getValue();
    const int2 divisions = getField<Int2Field>("divisions")->getValue();
    const int orientation = getField<NodeField<int>>("orientation")->getValue();

    const int rows = divisions[0];
    const int columns = divisions[1];

    const int columnsPoints = columns + 1;
    const int rowsPoints = rows + 1;


    const float size_cols = size[0];
    const float size_rows = size[1];
    const float columnSpacing = size_cols / static_cast<float>(columns);
    const float rowSpacing = size_rows / static_cast<float>(rows);

    float3 basePos{
        position[0],
        position[1],
        position[2]
    };

    switch (orientation) {
        case GridOrientation::XY:
            basePos[0] -= size_cols / 2;
            basePos[1] -= size_rows / 2;
            break;
        case GridOrientation::YZ:
            basePos[1] -= size_rows / 2;
            basePos[2] -= size_cols / 2;
            break;
        case GridOrientation::ZX:
            basePos[0] -= size_cols / 2;
            basePos[2] -= size_rows / 2;
            break;
    }

    // Create Points
    std::vector<float> posCols(columnsPoints);
    for (size_t col = 0; col < columnsPoints; ++col){
        posCols[col] = col * columnSpacing;
    }
    std::vector<float> posRows(rowsPoints);
    for (size_t row = 0; row < rowsPoints; ++row){
        posRows[row] = static_cast<float>(row) * rowSpacing;
    }

    auto& points = output->points;
    size_t pointIdx = points.size();

    points.reserve(points.size() + rowsPoints * columnsPoints);
    points.resize(points.size() + rowsPoints * columnsPoints);

    for (size_t row = 0; row < rowsPoints; ++row) {
        const float rowOffset = posRows[row];
        for (size_t col = 0; col < columnsPoints; ++col) {
            const float colOffset = posCols[col];
            float3 pos = basePos;
            switch (orientation) {
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

            points.normalX[pointIdx] = 0.0f;
            points.normalY[pointIdx] = 1.0f;
            points.normalZ[pointIdx] = 0.0f;

            points.colorR[pointIdx] = 1.0f;
            points.colorG[pointIdx] = 1.0f;
            points.colorB[pointIdx] = 1.0f;
            pointIdx++;
        }
    }

    // Create Primitives
    auto& primitives = output->primitives;
    auto& vertices = output->vertices;
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

    return output;
}

}