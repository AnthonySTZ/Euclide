#include "grid.h"

#include "fields/float2field.h"
#include "fields/float3field.h"
#include "fields/int2field.h"

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
    auto output = std::make_shared<Mesh>();
    
    float3 position = getField<Float3Field>("position")->getValue();
    float2 size = getField<Float2Field>("size")->getValue();
    const int2 divisions = getField<Int2Field>("divisions")->getValue();

    const int columns = divisions[0];
    const int rows = divisions[1];

    const int columnsPoints = columns + 1;
    const int rowsPoints = rows + 1;

    const float columnSpacing = size[0] / static_cast<float>(columns);
    const float rowSpacing = size[1] / static_cast<float>(rows);

    const float startX = position[0] - (size[0] * 0.5f);
    const float startZ = position[2] - (size[1] * 0.5f);
    const float posY = position[1];

    // Create Points
    std::vector<float> posXs(columnsPoints);
    for (size_t col = 0; col < columnsPoints; ++col){
        posXs[col] = startX + col * columnSpacing;
    }

    output->points.reserve(rowsPoints * columnsPoints);
    for (size_t row = 0; row < rowsPoints; ++row) {
        const float posZ = startZ + static_cast<float>(row) * rowSpacing;
        for (size_t column = 0; column < columnsPoints; ++column) {
            output->addPoint(posXs[column], posY, posZ);
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