#include "Camera\Camera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace mods
{
	mdCamera::mdCamera()
		: Position(0.f)
		, ProjectionMode(mdProjectionMode::Perspective)
		, m_Rotation(1.f, 0.f, 0.f, 0.f)
		, m_NearClippingPlane(0.001f)
		, m_FarClippingPlane(10000.f)
		, m_OrthographicWidth(1280.f)
		, m_OrthographicHeight(720.f)
		, m_AspectRatio(16.f / 9.f)
		, m_FieldOfView(75.f)
	{
		
	}

	void mdCamera::SetEulerRotation(const glm::vec3& euler)
	{
		m_Rotation = glm::quat(euler);
	}

	glm::vec3 mdCamera::GetEulerRotation() const
	{
		return glm::eulerAngles(m_Rotation);
	}

	glm::vec3 mdCamera::GetHeading() const
	{
		// TODO: * ForwardVector
		return m_Rotation * glm::vec3(0.f, 0.f, 1.f);
	}

	glm::mat4 mdCamera::GetViewMatrix() const
	{
		// TODO: Have optional up variable
		return glm::lookAt(Position, Position + GetHeading(), glm::vec3(0.f, 1.f, 0.f));
	}

	glm::mat4 mdCamera::GetProjectionMatrix() const
	{
		return GetProjectionMatrix(ProjectionMode);
	}

	glm::mat4 mdCamera::GetProjectionMatrix(mdProjectionMode mode) const
	{
		if (mode == mdProjectionMode::Orthographic)
			return glm::ortho(0.f, 0.f, m_OrthographicWidth, m_OrthographicHeight, m_NearClippingPlane, m_FarClippingPlane);
		else
			return glm::perspective(glm::radians(m_FieldOfView), m_AspectRatio, m_NearClippingPlane, m_FarClippingPlane);
	}

	glm::mat4 mdCamera::GetProjectionView() const
	{
		return GetProjectionView(ProjectionMode);
	}

	glm::mat4 mdCamera::GetProjectionView(mdProjectionMode mode) const
	{
		return GetViewMatrix() * GetProjectionMatrix(mode);
	}

	void mdCamera::SetNearClippingPlane(float near)
	{
		m_NearClippingPlane = glm::max(0.f, near);
	}

	void mdCamera::SetFarClippingPlane(float far)
	{
		m_FarClippingPlane = glm::max(m_NearClippingPlane, far);
	}

	void mdCamera::SetOrthographicWidth(float width)
	{
		m_OrthographicWidth = glm::max(0.f, width);
	}

	void mdCamera::SetOrthographicHeight(float height)
	{
		m_OrthographicHeight = glm::max(0.f, height);
	}

	void mdCamera::SetAspectRatio(float ratio)
	{
		m_AspectRatio = glm::max(0.f, ratio);
	}

	void mdCamera::SetAspectRatio(int width, int height)
	{
		SetAspectRatio((float)width / (float)height);
	}

	void mdCamera::SetFieldOfView(float fov)
	{
		m_FieldOfView = glm::clamp(fov, 0.f, 180.f);
	}
}