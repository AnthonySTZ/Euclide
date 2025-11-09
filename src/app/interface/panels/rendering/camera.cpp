#include "camera.h"

#include <iostream>

namespace butter {

Camera::Camera() {
    updateTarget(m_targetPosition);
    updatePerpectiveProjection();
}

void Camera::setOrthographicProjection(float t_left, float t_right, float t_top, float t_bottom, float t_near,
                                       float t_far) {
    m_projectionMatrix = py::ortho(t_left, t_right, t_bottom, t_top, t_near, t_far);
}

void Camera::setPerspectiveProjection(float t_fov, float t_aspect, float t_near, float t_far) {
    m_fov = t_fov;
    m_near = t_near;
    m_far = t_far;
    m_aspect = t_aspect;

    m_projectionMatrix = py::perspective(t_fov, t_aspect, t_near, t_far);
    m_projectionMatrix(1,1) *= -1.0f; // Setting +Y axis to up
}

void Camera::updatePerpectiveProjection() {
    m_projectionMatrix = py::perspective(m_fov, m_aspect, m_near, m_far);
    m_projectionMatrix(1,1) *= -1.0f; // Setting +Y axis to up
}

void Camera::updateAspectRatio(float t_aspect) {
    m_aspect = t_aspect;
    updatePerpectiveProjection();
}

void Camera::setViewTarget(py::vec3 t_position, py::vec3 t_target, py::vec3 t_up) {
    m_cameraPosition = t_position;
    m_targetPosition = t_target;

    py::vec3 direction = py::normalize(t_target - t_position);
    m_distanceToTarget = py::length(t_target - t_position);

    m_orientation = py::quatLookAt(direction, t_up);

    updateViewMatrix();
}

void Camera::orbit(float t_deltaYaw, float t_deltaPitch) {

    t_deltaYaw *= m_viewSpeed;
    t_deltaPitch *= m_viewSpeed;

    py::vec3 yAxis{0.0, 1.0, 0.0};
    py::mat4 rotMat{1.f};

    rotMat = py::rotate(rotMat, t_deltaYaw, yAxis);

    py::vec3 rightAxis = py::normalize(m_orientation * py::vec3{1.0, 0.0, 0.0});
    py::vec3 camUp = py::normalize(m_orientation * py::vec3{0.0, 1.0, 0.0});
    py::vec3 targetDir = py::normalize(m_orientation * py::vec3{0.0, 0.0, 1.0});

    if (py::dot(camUp, yAxis) < 0.1f) {
        if (targetDir[1] < 0.0f && t_deltaPitch < 0.0f) {
            rotMat = py::rotate(rotMat, t_deltaPitch, rightAxis);
        } else if (targetDir[1] > 0.0f && t_deltaPitch > 0.0f) {
            rotMat = py::rotate(rotMat, t_deltaPitch, rightAxis);
        }
    } else {
        rotMat = py::rotate(rotMat, t_deltaPitch, rightAxis);
    }

    py::vec3 localPosition = m_cameraPosition - m_targetPosition;
    py::vec4 local = rotMat * py::vec4{localPosition[0], localPosition[1], localPosition[2], 1.0f};
    localPosition[0] = local[0];
    localPosition[1] = local[1];
    localPosition[2] = local[2];

    m_cameraPosition = m_targetPosition + localPosition;

    py::vec3 direction = py::normalize(m_targetPosition - m_cameraPosition);
    m_orientation = py::quatLookAt(direction, py::vec3{0.0, 1.0, 0.0});

    updateViewMatrix();
}

void Camera::dolly(float t_delta) {

    t_delta *= m_zoomSpeed;
    m_distanceToTarget = std::max(0.1f, m_distanceToTarget + t_delta);

    py::vec3 targetAxis = m_orientation * py::vec3{0.f, 0.f, 1.0};
    m_cameraPosition = m_targetPosition + (targetAxis * m_distanceToTarget);

    updateViewMatrix();
}

void Camera::pan(float t_deltaX, float t_deltaY) {

    t_deltaX *= m_translateSpeed;
    t_deltaY *= m_translateSpeed;

    py::vec3 upAxis = py::normalize(m_orientation * py::vec3{0.0, 1.0, 0.0});
    py::vec3 rightAxis = py::normalize(m_orientation * py::vec3{1.0, 0.0, 0.0});

    py::vec3 move = upAxis * t_deltaY;
    move += rightAxis * t_deltaX;

    m_cameraPosition += move;
    m_targetPosition += move;

    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    m_viewMatrix = py::lookAt(m_cameraPosition, m_targetPosition, m_orientation * py::vec3{0.f, 1.f, 0.f});
}

}