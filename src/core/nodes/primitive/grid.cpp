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
    Timer timer{"grid"}; // 200ms 1000x1000 grid
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

    float3 base{
        position[0],
        position[1],
        position[2]
    };

    // Create Points
    std::vector<float> posCols(columnsPoints);
    for (size_t col = 0; col < columnsPoints; ++col){
        posCols[col] = col * columnSpacing;
    }
    std::vector<float> posRows(rowsPoints);
    for (size_t row = 0; row < rowsPoints; ++row){
        posRows[row] = static_cast<float>(row) * rowSpacing;
    }

    std::function<float3(float rowOffset, float colOffset)> makePoint;
    switch (orientation) {
        case GridOrientation::XY:
            base[0] -= size_cols / 2;
            base[1] -= size_rows / 2;
            makePoint = [&](float t_row, float t_col) {
                return float3{base[0] + t_col, base[1] + t_row, position[2]};
            };
            break;
            
        case GridOrientation::YZ:
            base[1] -= size_rows / 2;
            base[2] -= size_cols / 2;
            makePoint = [&](float t_row, float t_col) {
                return float3{position[0], base[1] + t_row, base[2] + t_col};
            };
            break;
            
        case GridOrientation::ZX:
            base[0] -= size_cols / 2;
            base[2] -= size_rows / 2;
            makePoint = [&](float t_row, float t_col) {
                return float3{base[0] + t_col, position[1], base[2] + t_row};
            };
            break;
    }

    output->points.reserve(rowsPoints * columnsPoints);
    for (size_t row = 0; row < rowsPoints; ++row) {
        for (size_t col = 0; col < columnsPoints; ++col) {
            output->addPoint(makePoint(posRows[row], posCols[col]));
        }
    }

    // Create Primitives
    output->primitives.reserve(rows * columns);
    for (uint32_t row = 0; row < rows; ++row) {
        for (uint32_t column = 0; column < columns; ++column) {
            const uint32_t top_left = row * (columnsPoints) + column;
            const uint32_t bottom_left = top_left + columnsPoints;
            output->addPrimitive({top_left, top_left + 1, bottom_left + 1, bottom_left});
        }
    }

    return output;
}

}