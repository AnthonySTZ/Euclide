#include "cylinder.h"

#include <cmath>

namespace butter {

 float3 position = {0.0, 0.0, 0.0};
    float2 radius = {1.0, 1.0};
    float height = 1.0;
    int divisions = 8;
    bool capped = true; 

Cylinder::Cylinder()
    : Node(0, 1, "Cylinder") 
{
    auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    positionField->setMetadata(NodeFieldMetadata{
        displayName: "Position",
        step: 0.02f
    });
    addField("position", positionField);
    
    auto radiusField = std::make_shared<Float2Field>(1.0, 1.0);
    radiusField->setMetadata(NodeFieldMetadata{
        displayName: "Radius",
        min: 0.0f,
        step: 0.02f
    });
    addField("radius", radiusField);
    
    auto heightField = std::make_shared<NodeField<float>>(1.0);
    heightField->setMetadata(NodeFieldMetadata{
        displayName: "Height",
        min: 0.0f,
        step: 0.02f
    });
    addField("height", heightField);
    
    auto divisionsField = std::make_shared<NodeField<int>>(8);
    divisionsField->setMetadata(NodeFieldMetadata{
        displayName: "Divisions",
        min: 3,
        step: 1
    });
    addField("divisions", divisionsField);
    
    auto cappedField = std::make_shared<NodeField<bool>>(true);
    cappedField->setMetadata(NodeFieldMetadata{
        displayName: "Capped"
    });
    addField("capped", cappedField);
}

std::shared_ptr<Mesh> Cylinder::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs) const
{
    auto output = std::make_shared<Mesh>();

    float3 position = getField<Float3Field>("position")->getValue();
    float2 radius = getField<Float2Field>("radius")->getValue();
    
    float height = getField<NodeField<float>>("height")->getValue();
    int divisions = getField<NodeField<int>>("divisions")->getValue();
    bool capped = getField<NodeField<bool>>("capped")->getValue();

    const CylinderSettings settings{
        .position = position,
        .radius = radius,
        .height = height,
        .divisions = divisions,
        .capped = capped
    };

    createCylinder(*output, settings);

    return output;
}

void Cylinder::createCylinder(Mesh &t_mesh, const CylinderSettings &t_settings)
{
    if (t_settings.divisions < 3) return;

    const float angleStep = (2.0f * M_PI) / static_cast<float>(t_settings.divisions);
    
    const float radiusTop = t_settings.radius[0];
    const float radiusBottom = t_settings.radius[1];
    const float heightOffset = t_settings.height * 0.5f;

    float3 position = t_settings.position; 
    auto& points = t_mesh.points;
    auto& vertices = t_mesh.vertices;
    auto& primitives = t_mesh.primitives;

    points.resize(t_settings.divisions * 2);
    std::fill(points.colorR.begin(), points.colorR.end(), 1.0);
    std::fill(points.colorG.begin(), points.colorG.end(), 1.0);
    std::fill(points.colorB.begin(), points.colorB.end(), 1.0);

    // Top
    for (size_t i = 0; i < t_settings.divisions; ++i) {
        float angle = i * angleStep;
        float cosAngle = std::cos(angle);
		float sinAngle = std::sin(angle);

        float posX = cosAngle * radiusTop + position[0];
        float posY = heightOffset + position[1];
        float posZ = sinAngle * radiusTop + position[2];

        points.posX[i] = posX;
        points.posY[i] = posY;
        points.posZ[i] = posZ;

        points.normalX[i] = cosAngle;
        points.normalY[i] = 0.0f;
        points.normalZ[i] = sinAngle;
    }

    // Bottom
    for (size_t i = 0; i < t_settings.divisions; ++i) {
        size_t idx = i + t_settings.divisions;
        points.posX[idx] = points.posX[i];
        points.posY[idx] = -heightOffset + position[1];
        points.posZ[idx] = points.posZ[i];

        points.normalX[idx] = points.normalX[i];
        points.normalY[idx] = points.normalY[i];
        points.normalZ[idx] = points.normalZ[i];
    }

    if (t_settings.capped) {
        vertices.resize(t_settings.divisions * 2);

        for (uint32_t i = 0; i < t_settings.divisions * 2; ++i) {
            vertices[i] = Vertex{i};
        }

        primitives.emplace_back(Primitive{
            0, static_cast<uint32_t>(t_settings.divisions)
        });
        primitives.emplace_back(Primitive{
            static_cast<uint32_t>(t_settings.divisions), static_cast<uint32_t>(t_settings.divisions)
        });
    }

    for (size_t i = 0; i < t_settings.divisions; i++) {
		uint32_t topFirstPt = i;
		uint32_t topSecondPt = (i + 1) % t_settings.divisions;

		uint32_t bottomFirstPt = topFirstPt + t_settings.divisions;
		uint32_t bottomSecondPt = topSecondPt + t_settings.divisions;

		t_mesh.addPrimitive({ topFirstPt, topSecondPt, bottomSecondPt, bottomFirstPt });
	}
}

}