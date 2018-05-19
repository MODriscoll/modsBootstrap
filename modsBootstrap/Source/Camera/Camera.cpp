#include "Camera\Camera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace mods
{
	Camera::Camera()
		: Position(0.f)
		, ProjectionMode(eProjectionMode::Perspective)
		, m_Rotation(0.f)
		, m_Heading(0.f)
		, m_NearClippingPlane(0.001f)
		, m_FarClippingPlane(10000.f)
		, m_OrthographicWidth(1280.f)
		, m_OrthographicHeight(720.f)
		, m_AspectRatio(16.f / 9.f)
		, m_FieldOfView(75.f)
	{
		CalculateHeading();
	}

	void Camera::LookAt(const glm::vec3& location)
	{
		glm::vec3 direction = glm::normalize(location - Position);

		glm::vec3 rotation(0.f);
		rotation.x = glm::degrees(glm::asin(direction.y));
		rotation.y = glm::degrees(atan2(direction.z, direction.x));
		rotation.z = m_Rotation.z;

		SetRotation(rotation);
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		// TODO: Have optional up variable
		return glm::lookAt(Position, Position + m_Heading, glm::vec3(0.f, 1.f, 0.f));
	}

	glm::mat4 Camera::GetProjectionMatrix() const
	{
		return GetProjectionMatrix(ProjectionMode);
	}

	glm::mat4 Camera::GetProjectionMatrix(eProjectionMode mode) const
	{
		if (mode == eProjectionMode::Orthographic)
			return glm::ortho(0.f, m_OrthographicWidth, 0.f, m_OrthographicHeight);
		else
			return glm::perspective(glm::radians(m_FieldOfView), m_AspectRatio, m_NearClippingPlane, m_FarClippingPlane);
	}

	glm::mat4 Camera::GetProjectionView() const
	{
		return GetProjectionView(ProjectionMode);
	}

	glm::mat4 Camera::GetProjectionView(eProjectionMode mode) const
	{
		return GetViewMatrix() * GetProjectionMatrix(mode);
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{
		m_Rotation = rotation;
		CalculateHeading();
	}

	void Camera::SetNearClippingPlane(float near)
	{
		m_NearClippingPlane = glm::max(0.f, near);
	}

	void Camera::SetFarClippingPlane(float far)
	{
		m_FarClippingPlane = glm::max(m_NearClippingPlane, far);
	}

	void Camera::SetOrthographicWidth(float width)
	{
		m_OrthographicWidth = glm::max(0.f, width);
	}

	void Camera::SetOrthographicHeight(float height)
	{
		m_OrthographicHeight = glm::max(0.f, height);
	}

	void Camera::SetAspectRatio(float ratio)
	{
		m_AspectRatio = glm::max(0.f, ratio);
	}

	void Camera::SetAspectRatio(int32 width, int32 height)
	{
		SetAspectRatio((float)width / (float)height);
	}

	void Camera::SetFieldOfView(float fov)
	{
		m_FieldOfView = glm::clamp(fov, 0.f, 180.f);
	}

	void Camera::CalculateHeading()
	{
		glm::vec3 heading(0.f);
		heading.x = glm::cos(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x));
		heading.y = glm::sin(glm::radians(m_Rotation.x));
		heading.z = glm::sin(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x));

		m_Heading = glm::normalize(heading);
	}
}