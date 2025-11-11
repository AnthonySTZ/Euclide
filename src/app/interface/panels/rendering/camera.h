#pragma once 

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace butter {

class Camera {

public:
    Camera();
    ~Camera() = default;

    // Projection settings
    void setOrthographicProjection(float t_left, float t_right, float t_top, float t_bottom, float t_near, float t_far);
    void setPerspectiveProjection(float t_fov, float t_aspect, float t_near, float t_far);

    void updatePerpectiveProjection();
    void updateAspectRatio(float t_aspect);

    // View controls
    void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{.0f, 1.f, .0f});

    // Orientation
    void orbit(float t_deltaYaw, float t_deltaPitch);
    void dolly(float t_delta);
    void pan(float t_deltaX, float t_deltaY);

    // View and projection matrices
    const glm::mat4x4 &getProjection() const { return m_projectionMatrix; };
    const glm::mat4x4 &getView() const { return m_viewMatrix; };

    // Getters for position and target
    glm::vec3 getTarget() const { return m_targetPosition; };
    glm::vec3 getPosition() const { return m_cameraPosition; };

    void updateTarget(glm::vec3 target) { setViewTarget(m_cameraPosition, target); }

private:
    void updateViewMatrix();

    glm::mat4x4 m_projectionMatrix{1.f};
    glm::mat4x4 m_viewMatrix{1.f};

    glm::vec3 m_cameraPosition{0.f, 1.f, 3.f};
    glm::vec3 m_targetPosition{0.f};
    glm::quat m_orientation{}; // Camera orientation as a quaternion

    float m_distanceToTarget = 10.f;
    float m_fov = glm::radians(50.f);
    float m_near = 0.1f;
    float m_far = 1000.f;
    float m_aspect = 1.77; // 16:9 at default

    float m_translateSpeed = 0.003f;
    float m_viewSpeed = 0.005f;
    float m_zoomSpeed = 0.01f;


};


}