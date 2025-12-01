#include "camera.h"

#include <iostream>

namespace butter {

Camera::Camera() {
    updateTarget(m_targetPosition);
    updatePerpectiveProjection();
}

void Camera::setOrthographicProjection(float t_left, float t_right, float t_top, float t_bottom, float t_near,
                                       float t_far) {
    m_projectionMatrix = glm::ortho(t_left, t_right, t_bottom, t_top, t_near, t_far);
}

void Camera::setPerspectiveProjection(float t_fov, float t_aspect, float t_near, float t_far) {
    m_fov = t_fov;
    m_near = t_near;
    m_far = t_far;
    m_aspect = t_aspect;

    m_projectionMatrix = glm::perspective(t_fov, t_aspect, t_near, t_far);
    m_projectionMatrix[1][1] *= -1.0f; // Setting +Y axis to up
}

void Camera::updatePerpectiveProjection() {
    m_projectionMatrix = glm::perspective(m_fov, m_aspect, m_near, m_far);
    m_projectionMatrix[1][1] *= -1.0f; // Setting +Y axis to up
}

void Camera::updateAspectRatio(float t_aspect) {
    m_aspect = t_aspect;
    updatePerpectiveProjection();
}

void Camera::setViewTarget(glm::vec3 t_position, glm::vec3 t_target, glm::vec3 t_up) {
    m_cameraPosition = t_position;
    m_targetPosition = t_target;

    glm::vec3 direction = glm::normalize(t_target - t_position);
    m_distanceToTarget = glm::length(t_target - t_position);

    m_orientation = glm::quatLookAt(direction, t_up);

    updateViewMatrix();
}

void Camera::orbit(float t_deltaYaw, float t_deltaPitch) {
    t_deltaYaw *= m_viewSpeed;
    t_deltaPitch *= m_viewSpeed;

    glm::vec3 yAxis{0.0, 1.0, 0.0};
    glm::mat4x4 rotMat{1.f};

    rotMat = glm::rotate(rotMat, t_deltaYaw, yAxis);

    glm::vec3 rightAxis = glm::normalize(m_orientation * glm::vec3{1.0, 0.0, 0.0});
    glm::vec3 camUp = glm::normalize(m_orientation * glm::vec3{0.0, 1.0, 0.0});
    glm::vec3 targetDir = glm::normalize(m_orientation * glm::vec3{0.0, 0.0, 1.0});

    if (glm::dot(camUp, yAxis) < 0.1f) {
        if (targetDir[1] < 0.0f && t_deltaPitch < 0.0f) {
            rotMat = glm::rotate(rotMat, t_deltaPitch, rightAxis);
        } else if (targetDir[1] > 0.0f && t_deltaPitch > 0.0f) {
            rotMat = glm::rotate(rotMat, t_deltaPitch, rightAxis);
        }
    } else {
        rotMat = glm::rotate(rotMat, t_deltaPitch, rightAxis);
    }

    glm::vec3 localPosition = m_cameraPosition - m_targetPosition;
    glm::vec4 local = rotMat * glm::vec4{localPosition[0], localPosition[1], localPosition[2], 1.0f};
    localPosition[0] = local[0];
    localPosition[1] = local[1];
    localPosition[2] = local[2];

    m_cameraPosition = m_targetPosition + localPosition;

    glm::vec3 direction = glm::normalize(m_targetPosition - m_cameraPosition);
    m_orientation = glm::quatLookAt(direction, glm::vec3{0.0, 1.0, 0.0});

    updateViewMatrix();
}

void Camera::dolly(float t_delta) {
    t_delta *= m_zoomSpeed;
    m_distanceToTarget = std::max(0.1f, m_distanceToTarget + t_delta);

    glm::vec3 targetAxis = m_orientation * glm::vec3{0.f, 0.f, 1.0};
    m_cameraPosition = m_targetPosition + (targetAxis * m_distanceToTarget);

    updateViewMatrix();
}

void Camera::pan(float t_deltaX, float t_deltaY) {
    t_deltaX *= m_translateSpeed;
    t_deltaY *= m_translateSpeed;

    glm::vec3 upAxis = glm::normalize(m_orientation * glm::vec3{0.0, 1.0, 0.0});
    glm::vec3 rightAxis = glm::normalize(m_orientation * glm::vec3{1.0, 0.0, 0.0});

    glm::vec3 move = upAxis * t_deltaY;
    move += rightAxis * t_deltaX;

    m_cameraPosition += move;
    m_targetPosition += move;

    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    m_viewMatrix = glm::lookAt(m_cameraPosition, m_targetPosition, m_orientation * glm::vec3{0.f, 1.f, 0.f});
}

} // namespace butter