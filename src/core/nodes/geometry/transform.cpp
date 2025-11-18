#include "transform.h"

#include "utils/math.h"
#include "utils/timer.h"

// #define USE_SIMD

#ifdef USE_SIMD
#include <immintrin.h>
#endif

namespace butter {
    
Transform::Transform()
    : Node(1, 1, "Transform")
{
    auto translateField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    translateField->setMetadata(NodeFieldMetadata{
        displayName: "Translate",
        step: 0.02f
    });
    addField("translate", translateField);
    
    auto scaleField = std::make_shared<Float3Field>(1.0, 1.0, 1.0);
    scaleField->setMetadata(NodeFieldMetadata{
        displayName: "Scale",
        step: 0.02f
    });
    addField("scale", scaleField);

    auto rotateField = std::make_shared<Float3Field>(0.0, 0.0, 0.0);
    rotateField->setMetadata(NodeFieldMetadata{
        displayName: "Rotate",
        step: 0.02f
    });
    addField("rotate", rotateField);
}

std::shared_ptr<Mesh> Transform::compute(const size_t t_index, const std::vector<std::shared_ptr<Mesh>> &t_inputs)
{
    if (t_inputs[0] == nullptr) return std::make_shared<Mesh>();
    auto output = std::make_shared<Mesh>(*t_inputs[0]);

    float3 translate = getField<Float3Field>("translate")->getValue();
    float3 scale = getField<Float3Field>("scale")->getValue();
    float3 rotate = getField<Float3Field>("rotate")->getValue();

    scaleMesh(output, scale);
    rotateMesh(output, rotate);
    translateMesh(output, translate);
        
    return output;
}

void Transform::scaleMesh(std::shared_ptr<Mesh> t_mesh, const float3& t_scale) {

    auto& points = t_mesh->points;

    size_t i = 0;
    #ifdef USE_SIMD
    __m256 __scaleX = _mm256_set1_ps(t_scale[0]);
    __m256 __scaleY = _mm256_set1_ps(t_scale[1]);
    __m256 __scaleZ = _mm256_set1_ps(t_scale[2]);
    for (; i + 8 < points.size(); i += 8) {
        __m256 __posX = _mm256_load_ps(&points.posX[i]); // 8 posX
        __m256 __posY = _mm256_load_ps(&points.posY[i]); // 8 posY
        __m256 __posZ = _mm256_load_ps(&points.posZ[i]); // 8 posZ

        __m256 __posXp = _mm256_mul_ps(__posX, __scaleX);
        __m256 __posYp = _mm256_mul_ps(__posY, __scaleY);
        __m256 __posZp = _mm256_mul_ps(__posZ, __scaleZ);

        _mm256_store_ps(&points.posX[i], __posXp);
        _mm256_store_ps(&points.posY[i], __posYp);
        _mm256_store_ps(&points.posZ[i], __posZp);
    }
    #endif
    for (; i < points.size(); ++i) {
        points.posX[i] *= t_scale[0];
        points.posY[i] *= t_scale[1];
        points.posZ[i] *= t_scale[2];
    }
}

void Transform::rotateMesh(std::shared_ptr<Mesh> t_mesh, const float3& t_rotation) {
    
    const double rotX = radians(t_rotation[0]);
    const double rotY = radians(t_rotation[1]);
    const double rotZ = radians(t_rotation[2]);
    
    const float cosX = std::cos(rotX);
    const float cosY = std::cos(rotY);
    const float cosZ = std::cos(rotZ);
    
    const float sinX = std::sin(rotX);
    const float sinY = std::sin(rotY);
    const float sinZ = std::sin(rotZ);
    
    const float row_0[3] = {
        cosY * cosX, (sinZ * sinY * cosX) - (cosZ * sinX), (cosZ * sinY * cosX) + (sinZ * sinX)
    };
    const float row_1[3] = {
        cosY * sinX, (sinZ * sinY * sinX) + (cosZ * cosX), (cosZ * sinY * sinX) - (sinZ * cosX)
    };
    const float row_2[3] = {
        -sinY, sinZ * cosY, cosZ * cosY
    };
    
    auto& points = t_mesh->points;
    
    size_t i = 0;
    #ifdef USE_SIMD
    __m256 __row_00 = _mm256_set1_ps(row_0[0]);
    __m256 __row_01 = _mm256_set1_ps(row_0[1]);
    __m256 __row_02 = _mm256_set1_ps(row_0[2]);
    __m256 __row_10 = _mm256_set1_ps(row_1[0]);
    __m256 __row_11 = _mm256_set1_ps(row_1[1]);
    __m256 __row_12 = _mm256_set1_ps(row_1[2]);
    __m256 __row_20 = _mm256_set1_ps(row_2[0]);
    __m256 __row_21 = _mm256_set1_ps(row_2[1]);
    __m256 __row_22 = _mm256_set1_ps(row_2[2]);
    for (; i + 8 < points.size(); i += 8) {
        __m256 __posX = _mm256_load_ps(&points.posX[i]); // 8 posX
        __m256 __posY = _mm256_load_ps(&points.posY[i]); // 8 posY
        __m256 __posZ = _mm256_load_ps(&points.posZ[i]); // 8 posZ
        
        __m256 __posXp = _mm256_fmadd_ps(__posZ, __row_02, _mm256_fmadd_ps(__posY, __row_01, _mm256_mul_ps(__posX, __row_00)));
        __m256 __posYp = _mm256_fmadd_ps(__posZ, __row_12, _mm256_fmadd_ps(__posY, __row_11, _mm256_mul_ps(__posX, __row_10)));
        __m256 __posZp = _mm256_fmadd_ps(__posZ, __row_22, _mm256_fmadd_ps(__posY, __row_21, _mm256_mul_ps(__posX, __row_20)));
        
        _mm256_store_ps(&points.posX[i], __posXp);
        _mm256_store_ps(&points.posY[i], __posYp);
        _mm256_store_ps(&points.posZ[i], __posZp);
    }
    #endif
    for (; i < points.size(); ++i) {
        const float posX = points.posX[i];
        const float posY = points.posY[i];
        const float posZ = points.posZ[i];
        points.posX[i] = posX * row_0[0] + posY * row_0[1] + posZ * row_0[2];
        points.posY[i] = posX * row_1[0] + posY * row_1[1] + posZ * row_1[2];
        points.posZ[i] = posX * row_2[0] + posY * row_2[1] + posZ * row_2[2];
    }
}

void Transform::translateMesh(std::shared_ptr<Mesh> t_mesh, const float3& t_translation) {

    auto& points = t_mesh->points;

    size_t i = 0;
    #ifdef USE_SIMD
    __m256 __translateX = _mm256_set1_ps(t_translation[0]);
    __m256 __translateY = _mm256_set1_ps(t_translation[1]);
    __m256 __translateZ = _mm256_set1_ps(t_translation[2]);
    for (; i + 8 < points.size(); i += 8) {
        __m256 __posX = _mm256_load_ps(&points.posX[i]); // 8 posX
        __m256 __posY = _mm256_load_ps(&points.posY[i]); // 8 posY
        __m256 __posZ = _mm256_load_ps(&points.posZ[i]); // 8 posZ

        __m256 __posXp = _mm256_add_ps(__posX, __translateX);
        __m256 __posYp = _mm256_add_ps(__posY, __translateY);
        __m256 __posZp = _mm256_add_ps(__posZ, __translateZ);

        _mm256_store_ps(&points.posX[i], __posXp);
        _mm256_store_ps(&points.posY[i], __posYp);
        _mm256_store_ps(&points.posZ[i], __posZp);
    }
    #endif
    for (; i < points.size(); ++i) {
        points.posX[i] += t_translation[0];
        points.posY[i] += t_translation[1];
        points.posZ[i] += t_translation[2];
    }
    
}

}