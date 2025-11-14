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
    
    float2 size = getField<Float2Field>("size")->getValue();
    const int2 divisions = getField<Int2Field>("divisions")->getValue();

    const int columns = divisions[0] + 1;
    const int rows = divisions[1] + 1;

    const float columnSpacing = size[0] / static_cast<float>(columns - 1);
    const float rowSpacing = size[1] / static_cast<float>(rows - 1);

    output->points.reserve(rows * columns);

    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            const float posX = static_cast<float>(column) * columnSpacing;
            const float posZ = static_cast<float>(row) * rowSpacing;
            output->addPoint(posX, 0, posZ);
        }
    }

    return output;
}

}