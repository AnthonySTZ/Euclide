#include "cylinder.h"

// #include "nodes/geometry/smooth_normals.h"

#include <cmath>

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

namespace euclide {

float3 position = {0.0, 0.0, 0.0};
float2 radius = {1.0, 1.0};
float height = 1.0;
int divisions = 8;
bool capped = true;

Cylinder::Cylinder() : Node(0, 1, "Cylinder") {
    auto positionField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    positionField->setMetadata(NodeFieldMetadata{displayName : "Position", step : 0.02f});
    addField("position", positionField);

    auto radiusField = std::make_shared<Float2Field>(1.0, 1.0);
    radiusField->setMetadata(NodeFieldMetadata{displayName : "Radius", min : 0.0f, step : 0.02f});
    addField("radius", radiusField);

    auto heightField = std::make_shared<NodeField<float>>(1.0);
    heightField->setMetadata(NodeFieldMetadata{displayName : "Height", min : 0.0f, step : 0.02f});
    addField("height", heightField);

    auto divisionsField = std::make_shared<NodeField<int>>(8);
    divisionsField->setMetadata(NodeFieldMetadata{displayName : "Divisions", min : 3, step : 1});
    addField("divisions", divisionsField);

    auto cappedField = std::make_shared<NodeField<bool>>(true);
    cappedField->setMetadata(NodeFieldMetadata{displayName : "Capped"});
    addField("capped", cappedField);
}

std::shared_ptr<Mesh> Cylinder::compute(const size_t t_index,
                                        const std::vector<std::shared_ptr<Mesh>>& t_inputs) const {
    auto output = std::make_shared<Mesh>();

    float3 position = getField<Float3Field>("position")->getValue();
    float2 radius = getField<Float2Field>("radius")->getValue();

    float height = getField<NodeField<float>>("height")->getValue();
    int divisions = getField<NodeField<int>>("divisions")->getValue();
    bool capped = getField<NodeField<bool>>("capped")->getValue();

    const CylinderSettings settings{
        .position = position, .radius = radius, .height = height, .divisions = divisions, .capped = capped};

    createCylinder(*output, settings);

    return output;
}

void Cylinder::createCylinder(Mesh& t_mesh, const CylinderSettings& t_settings) {
    if (t_settings.divisions < 3)
        return;

    const float angleStep = (2.0f * M_PI) / static_cast<float>(t_settings.divisions);

    const float radiusTop = t_settings.radius[0];
    const float radiusBottom = t_settings.radius[1];
    const float heightOffset = t_settings.height * 0.5f;

    float3 position = t_settings.position;
    t_mesh.pointAttribs.resize(t_settings.divisions * 2);

    auto positions = t_mesh.pointAttribs.findOrCreate("P", 3, AttributeType::ATTR_TYPE_FLOAT);
    float* pointsPosX = positions->component<float>(0);
    float* pointsPosY = positions->component<float>(1);
    float* pointsPosZ = positions->component<float>(2);

    auto normals = t_mesh.pointAttribs.findOrCreate("N", 3, AttributeType::ATTR_TYPE_FLOAT);
    float* pointsNormalX = normals->component<float>(0);
    float* pointsNormalY = normals->component<float>(1);
    float* pointsNormalZ = normals->component<float>(2);

    // Top
    std::vector<float, AlignedAllocator<float, 32>> cosAngles(t_settings.divisions);
    std::vector<float, AlignedAllocator<float, 32>> sinAngles(t_settings.divisions);
    for (size_t i = 0; i < t_settings.divisions; ++i) {
        float angle = i * angleStep;
        cosAngles[i] = std::cos(angle);
        sinAngles[i] = std::sin(angle);
    }

    size_t div = 0;
#ifdef USE_SIMD
    if (t_settings.divisions >= 8) {
        __m256 __translateX = _mm256_set1_ps(position[0]);
        __m256 __translateY = _mm256_set1_ps(position[1]);
        __m256 __translateZ = _mm256_set1_ps(position[2]);

        __m256 __radiusTop = _mm256_set1_ps(radiusTop);
        __m256 __height = _mm256_set1_ps(heightOffset);

        for (; div + 8 <= t_settings.divisions; div += 8) {
            __m256 __cos = _mm256_load_ps(&cosAngles[div]);
            __m256 __sin = _mm256_load_ps(&sinAngles[div]);

            _mm256_store_ps(&pointsPosX[div], _mm256_fmadd_ps(__cos, __radiusTop, __translateX));
            _mm256_store_ps(&pointsPosY[div], _mm256_add_ps(__height, __translateY));
            _mm256_store_ps(&pointsPosZ[div], _mm256_fmadd_ps(__sin, __radiusTop, __translateZ));

            _mm256_store_ps(&pointsNormalX[div], __cos);
            _mm256_store_ps(&pointsNormalZ[div], __sin);
        }
    }
#endif

    for (; div < t_settings.divisions; ++div) {
        const float cosAngle = cosAngles[div];
        const float sinAngle = sinAngles[div];

        float posX = cosAngle * radiusTop + position[0];
        float posY = heightOffset + position[1];
        float posZ = sinAngle * radiusTop + position[2];

        pointsPosX[div] = posX;
        pointsPosY[div] = posY;
        pointsPosZ[div] = posZ;

        pointsNormalX[div] = cosAngle;
        pointsNormalZ[div] = sinAngle;
    }

    div = 0;
#ifdef USE_SIMD
    if (t_settings.divisions >= 8) {
        __m256 __translateX = _mm256_set1_ps(position[0]);
        __m256 __translateY = _mm256_set1_ps(position[1]);
        __m256 __translateZ = _mm256_set1_ps(position[2]);

        __m256 __radiusBottom = _mm256_set1_ps(radiusBottom);
        __m256 __height = _mm256_set1_ps(-heightOffset);

        for (; div + 8 <= t_settings.divisions; div += 8) {
            __m256 __cos = _mm256_load_ps(&cosAngles[div]);
            __m256 __sin = _mm256_load_ps(&sinAngles[div]);

            _mm256_storeu_ps(&pointsPosX[div + t_settings.divisions],
                             _mm256_fmadd_ps(__cos, __radiusBottom, __translateX));
            _mm256_storeu_ps(&pointsPosY[div + t_settings.divisions], _mm256_add_ps(__height, __translateY));
            _mm256_storeu_ps(&pointsPosZ[div + t_settings.divisions],
                             _mm256_fmadd_ps(__sin, __radiusBottom, __translateZ));

            _mm256_storeu_ps(&pointsNormalX[div + t_settings.divisions], __cos);
            _mm256_storeu_ps(&pointsNormalZ[div + t_settings.divisions], __sin);
        }
    }
#endif

    for (; div < t_settings.divisions; ++div) {
        size_t idx = div + t_settings.divisions;

        const float cosAngle = cosAngles[div];
        const float sinAngle = sinAngles[div];

        float posX = cosAngle * radiusBottom + position[0];
        float posY = -heightOffset + position[1];
        float posZ = sinAngle * radiusBottom + position[2];

        pointsPosX[idx] = posX;
        pointsPosY[idx] = posY;
        pointsPosZ[idx] = posZ;

        pointsNormalX[idx] = pointsNormalX[div];
        pointsNormalY[idx] = pointsNormalY[div];
        pointsNormalZ[idx] = pointsNormalZ[div];
    }

    auto& vertices = t_mesh.vertices;
    auto& primitives = t_mesh.primitives;

    size_t totalVertices = t_settings.divisions * 4;
    vertices.resize(totalVertices);

    size_t vertIdx = 0;
    for (size_t i = 0; i < t_settings.divisions; i++) {
        const uint32_t topFirstPt = i;
        const uint32_t topSecondPt = (i + 1) % t_settings.divisions;
        const uint32_t bottomFirstPt = topFirstPt + t_settings.divisions;
        const uint32_t bottomSecondPt = topSecondPt + t_settings.divisions;

        vertices[vertIdx++] = Vertex{topFirstPt};
        vertices[vertIdx++] = Vertex{topSecondPt};
        vertices[vertIdx++] = Vertex{bottomSecondPt};
        vertices[vertIdx++] = Vertex{bottomFirstPt};
    }

    uint32_t primVertIdx = 0;
    primitives.resize(t_settings.divisions);
    for (size_t i = 0; i < t_settings.divisions; i++) {
        primitives[i] = Primitive{primVertIdx, 4};
        primVertIdx += 4;
    }

    if (t_settings.capped) {
        totalVertices += t_settings.divisions * 2;
        vertices.reserve(totalVertices);
        vertices.resize(totalVertices);

        for (uint32_t i = 0; i < t_settings.divisions; i++) {
            vertices[vertIdx++] = Vertex{i};
        }
        for (uint32_t i = t_settings.divisions; i < t_settings.divisions * 2; i++) {
            vertices[vertIdx++] = Vertex{i};
        }

        primitives.emplace_back(Primitive{primVertIdx, static_cast<uint32_t>(t_settings.divisions)});
        primitives.emplace_back(
            Primitive{primVertIdx + t_settings.divisions, static_cast<uint32_t>(t_settings.divisions)});
    }

    // SmoothNormals::smoothNormals(t_mesh);
}

} // namespace euclide