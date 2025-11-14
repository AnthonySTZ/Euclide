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

    // Create Points
    output->points.reserve(rowsPoints * columnsPoints);
    for (size_t row = 0; row < rowsPoints; ++row) {
        for (size_t column = 0; column < columnsPoints; ++column) {
            const float offsetX = static_cast<float>(column) * columnSpacing;
            const float offsetZ = static_cast<float>(row) * rowSpacing;
            output->addPoint(startX + offsetX, position[1], startZ + offsetZ);
        }
    }

    // Create Primitives
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