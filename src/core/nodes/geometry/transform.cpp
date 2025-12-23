#include "transform.h"

#include "utils/math.h"
#include "utils/timer.h"

#ifdef USE_SIMD
#include "utils/simd.h"
#endif

namespace euclide {

Transform::Transform() : Node(1, 1, "Transform") {
    auto translateField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    translateField->setMetadata(NodeFieldMetadata{displayName : "Translate", step : 0.02f});
    addField("translate", translateField);

    auto scaleField = std::make_shared<Float3Field>(1.0, 1.0, 1.0);
    scaleField->setMetadata(NodeFieldMetadata{displayName : "Scale", step : 0.02f});
    addField("scale", scaleField);

    auto rotateField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    rotateField->setMetadata(NodeFieldMetadata{displayName : "Rotate", step : 0.02f});
    addField("rotate", rotateField);
}

std::shared_ptr<Mesh> Transform::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>>& t_inputs) {
    if (t_inputs[0] == nullptr)
        return std::make_shared<Mesh>();
    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    Timer timer{"Transform"};

    float3 translate = getField<Float3Field>("translate")->getValue();
    float3 scale = getField<Float3Field>("scale")->getValue();
    float3 rotate = getField<Float3Field>("rotate")->getValue();

    TransformSettings settings{translate, scale, rotate};

    transform(*output, settings);

    return output;
}

void Transform::transform(Mesh& t_mesh, const TransformSettings& t_settings) {
    if (t_settings.scale[0] != 1.0f || t_settings.scale[1] != 1.0f || t_settings.scale[2] != 1.0f) {
        scaleMesh(t_mesh, t_settings.scale);
    }
    if (t_settings.rotation[0] != 0.0f || t_settings.rotation[1] != 0.0f || t_settings.rotation[2] != 0.0f) {
        rotateMesh(t_mesh, t_settings.rotation);
    }
    if (t_settings.translation[0] != 0.0f || t_settings.translation[1] != 0.0f || t_settings.translation[2] != 0.0f) {
        translateMesh(t_mesh, t_settings.translation);
    }
}

void Transform::scaleMesh(Mesh& t_mesh, const float3& t_scale) {
    const size_t numPoints = t_mesh.pointAttribs.size();

    auto positions = t_mesh.pointAttribs.find("P");
    float* pointsPosX = positions->component<float>(0);
    float* pointsPosY = positions->component<float>(1);
    float* pointsPosZ = positions->component<float>(2);

    size_t i = 0;
#ifdef USE_SIMD
    if (numPoints >= 8) {
        __m256 __scaleX = _mm256_set1_ps(t_scale[0]);
        __m256 __scaleY = _mm256_set1_ps(t_scale[1]);
        __m256 __scaleZ = _mm256_set1_ps(t_scale[2]);
        for (; i + 8 <= numPoints; i += 8) {
            __m256 __posX = _mm256_load_ps(&pointsPosX[i]); // 8 posX
            __m256 __posY = _mm256_load_ps(&pointsPosY[i]); // 8 posY
            __m256 __posZ = _mm256_load_ps(&pointsPosZ[i]); // 8 posZ

            __m256 __posXp = _mm256_mul_ps(__posX, __scaleX);
            __m256 __posYp = _mm256_mul_ps(__posY, __scaleY);
            __m256 __posZp = _mm256_mul_ps(__posZ, __scaleZ);

            _mm256_store_ps(&pointsPosX[i], __posXp);
            _mm256_store_ps(&pointsPosY[i], __posYp);
            _mm256_store_ps(&pointsPosZ[i], __posZp);
        }
    }
#endif
    for (; i < t_mesh.pointAttribs.size(); ++i) {
        pointsPosX[i] *= t_scale[0];
        pointsPosY[i] *= t_scale[1];
        pointsPosZ[i] *= t_scale[2];
    }
}

void Transform::rotateMesh(Mesh& t_mesh, const float3& t_rotation) {
    const double rotX = radians(t_rotation[0]);
    const double rotY = radians(t_rotation[1]);
    const double rotZ = radians(t_rotation[2]);

    const float cosX = std::cos(rotX);
    const float cosY = std::cos(rotY);
    const float cosZ = std::cos(rotZ);

    const float sinX = std::sin(rotX);
    const float sinY = std::sin(rotY);
    const float sinZ = std::sin(rotZ);

    const float row_0[3] = {cosY * cosZ, cosZ * sinX * sinY - cosX * sinZ, sinX * sinZ + cosX * cosZ * sinY};

    const float row_1[3] = {cosY * sinZ, cosX * cosZ + sinX * sinY * sinZ, cosX * sinY * sinZ - cosZ * sinX};

    const float row_2[3] = {-sinY, cosY * sinX, cosX * cosY};

    const size_t numPoints = t_mesh.pointAttribs.size();
    auto positions = t_mesh.pointAttribs.find("P");
    float* pointsPosX = positions->component<float>(0);
    float* pointsPosY = positions->component<float>(1);
    float* pointsPosZ = positions->component<float>(2);

    size_t i = 0;
#ifdef USE_SIMD
    if (numPoints >= 8) {
        __m256 __row_00 = _mm256_set1_ps(row_0[0]);
        __m256 __row_01 = _mm256_set1_ps(row_0[1]);
        __m256 __row_02 = _mm256_set1_ps(row_0[2]);
        __m256 __row_10 = _mm256_set1_ps(row_1[0]);
        __m256 __row_11 = _mm256_set1_ps(row_1[1]);
        __m256 __row_12 = _mm256_set1_ps(row_1[2]);
        __m256 __row_20 = _mm256_set1_ps(row_2[0]);
        __m256 __row_21 = _mm256_set1_ps(row_2[1]);
        __m256 __row_22 = _mm256_set1_ps(row_2[2]);
        for (; i + 8 <= numPoints; i += 8) {
            __m256 __posX = _mm256_load_ps(&pointsPosX[i]); // 8 posX
            __m256 __posY = _mm256_load_ps(&pointsPosY[i]); // 8 posY
            __m256 __posZ = _mm256_load_ps(&pointsPosZ[i]); // 8 posZ

            __m256 __posXp =
                _mm256_fmadd_ps(__posZ, __row_02, _mm256_fmadd_ps(__posY, __row_01, _mm256_mul_ps(__posX, __row_00)));
            __m256 __posYp =
                _mm256_fmadd_ps(__posZ, __row_12, _mm256_fmadd_ps(__posY, __row_11, _mm256_mul_ps(__posX, __row_10)));
            __m256 __posZp =
                _mm256_fmadd_ps(__posZ, __row_22, _mm256_fmadd_ps(__posY, __row_21, _mm256_mul_ps(__posX, __row_20)));

            _mm256_store_ps(&pointsPosX[i], __posXp);
            _mm256_store_ps(&pointsPosY[i], __posYp);
            _mm256_store_ps(&pointsPosZ[i], __posZp);
        }
    }
#endif
    for (; i < t_mesh.pointAttribs.size(); ++i) {
        const float posX = pointsPosX[i];
        const float posY = pointsPosY[i];
        const float posZ = pointsPosZ[i];
        pointsPosX[i] = posX * row_0[0] + posY * row_0[1] + posZ * row_0[2];
        pointsPosY[i] = posX * row_1[0] + posY * row_1[1] + posZ * row_1[2];
        pointsPosZ[i] = posX * row_2[0] + posY * row_2[1] + posZ * row_2[2];
    }
}

void Transform::translateMesh(Mesh& t_mesh, const float3& t_translation) {
    const size_t numPoints = t_mesh.pointAttribs.size();

    auto positions = t_mesh.pointAttribs.find("P");
    float* pointsPosX = positions->component<float>(0);
    float* pointsPosY = positions->component<float>(1);
    float* pointsPosZ = positions->component<float>(2);

    size_t i = 0;
#ifdef USE_SIMD
    if (numPoints >= 8) {
        __m256 __translateX = _mm256_set1_ps(t_translation[0]);
        __m256 __translateY = _mm256_set1_ps(t_translation[1]);
        __m256 __translateZ = _mm256_set1_ps(t_translation[2]);
        for (; i + 8 <= numPoints; i += 8) {
            __m256 __posX = _mm256_load_ps(&pointsPosX[i]); // 8 posX
            __m256 __posY = _mm256_load_ps(&pointsPosY[i]); // 8 posY
            __m256 __posZ = _mm256_load_ps(&pointsPosZ[i]); // 8 posZ

            __m256 __posXp = _mm256_add_ps(__posX, __translateX);
            __m256 __posYp = _mm256_add_ps(__posY, __translateY);
            __m256 __posZp = _mm256_add_ps(__posZ, __translateZ);

            _mm256_store_ps(&pointsPosX[i], __posXp);
            _mm256_store_ps(&pointsPosY[i], __posYp);
            _mm256_store_ps(&pointsPosZ[i], __posZp);
        }
    }
#endif
    for (; i < t_mesh.pointAttribs.size(); ++i) {
        pointsPosX[i] += t_translation[0];
        pointsPosY[i] += t_translation[1];
        pointsPosZ[i] += t_translation[2];
    }
}

} // namespace euclide