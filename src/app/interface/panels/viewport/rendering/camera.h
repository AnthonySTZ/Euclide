#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace euclide {

/// @brief Represents a 3D camera with view and projection controls for a rendering system.
///
/// Supports both perspective and orthographic projections. Provides orbit, pan, and dolly
/// controls for interactive manipulation, as well as getters for matrices, position, and target.
class Camera {
  public:
    /// @brief Construct a Camera with default perspective settings.
    Camera();
    ~Camera() = default;

    /// @brief Set an orthographic projection matrix.
    /// @param t_left Left boundary of the view volume.
    /// @param t_right Right boundary of the view volume.
    /// @param t_top Top boundary of the view volume.
    /// @param t_bottom Bottom boundary of the view volume.
    /// @param t_near Near clipping plane.
    /// @param t_far Far clipping plane.
    void setOrthographicProjection(float t_left, float t_right, float t_top, float t_bottom, float t_near, float t_far);

    /// @brief Set a perspective projection matrix.
    /// @param t_fov Vertical field of view in radians.
    /// @param t_aspect Aspect ratio (width / height).
    /// @param t_near Near clipping plane distance.
    /// @param t_far Far clipping plane distance.
    void setPerspectiveProjection(float t_fov, float t_aspect, float t_near, float t_far);

    /// @brief Update the current perspective projection based on stored parameters.
    void updatePerpectiveProjection();

    /// @brief Update the aspect ratio of the projection matrix.
    /// @param t_aspect New aspect ratio (width / height).
    void updateAspectRatio(float t_aspect);

    /// @brief Set the camera's position and the point it looks at.
    /// @param position Camera world position.
    /// @param target Target point the camera is looking at.
    /// @param up Up vector for the camera (default is {0,1,0}).
    void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{.0f, 1.f, .0f});

    /// @brief Orbit the camera around its target.
    /// @param t_deltaYaw Horizontal rotation in radians.
    /// @param t_deltaPitch Vertical rotation in radians.
    void orbit(float t_deltaYaw, float t_deltaPitch);

    /// @brief Move the camera closer or further from its target.
    /// @param t_delta Positive values move forward, negative values move backward.
    void dolly(float t_delta);

    /// @brief Pan the camera in the plane perpendicular to the view direction.
    /// @param t_deltaX Horizontal translation.
    /// @param t_deltaY Vertical translation.
    void pan(float t_deltaX, float t_deltaY);

    /// @brief Get the projection matrix.
    /// @return 4x4 projection matrix in column-major order.
    [[nodiscard]] const glm::mat4x4& getProjection() const noexcept { return m_projectionMatrix; };

    /// @brief Get the view matrix.
    /// @return 4x4 view matrix in column-major order.
    [[nodiscard]] const glm::mat4x4& getView() const noexcept { return m_viewMatrix; };

    /// @brief Get the target position the camera is looking at.
    /// @return Target position in world coordinates.
    glm::vec3 getTarget() const noexcept { return m_targetPosition; };

    /// @brief Get the camera position in world coordinates.
    /// @return Camera position vector.
    glm::vec3 getPosition() const noexcept { return m_cameraPosition; };

    /// @brief Update the camera to look at a new target from the current position.
    /// @param target New target point in world coordinates.
    void updateTarget(glm::vec3 target) { setViewTarget(m_cameraPosition, target); }

  private:
    /// @brief Recompute the view matrix using the current position and orientation.
    void updateViewMatrix();

  private:
    glm::mat4x4 m_projectionMatrix{1.f}; ///< Projection matrix (orthographic or perspective)
    glm::mat4x4 m_viewMatrix{1.f};       ///< View matrix computed from position and orientation

    glm::vec3 m_cameraPosition{0.f, 1.f, 3.f}; ///< Camera world position
    glm::vec3 m_targetPosition{0.f};           ///< Target point the camera is looking at
    glm::quat m_orientation{};                 ///< Camera orientation as a quaternion

    float m_distanceToTarget = 10.f;  ///< Distance from camera to target point
    float m_fov = glm::radians(50.f); ///< Field of view (radians)
    float m_near = 0.1f;              ///< Near clipping plane distance
    float m_far = 1000.f;             ///< Far clipping plane distance
    float m_aspect = 1.77f;           ///< Aspect ratio (width / height)

    float m_translateSpeed = 0.003f; ///< Speed factor for panning translation
    float m_viewSpeed = 0.005f;      ///< Speed factor for orbit rotation
    float m_zoomSpeed = 0.01f;       ///< Speed factor for dolly/zoom
};

} // namespace euclide