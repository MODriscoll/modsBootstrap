#pragma once

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\quaternion.hpp>

namespace mods
{
	enum class mdProjectionMode : char
	{
		Orthographic,
		Perspective
	};

	// Camera that only contains a position and rotation.
	// Perspective and orthographic variables are not in a union to
	// allow for the easy transition between different projection modes
	class mdCamera
	{
	public:

		mdCamera();
		virtual ~mdCamera() = default;

	public:

		// Set the near clipping plane of the camera
		virtual void SetNearClippingPlane(float near);

		// Set the far clipping plane of the camera
		virtual void SetFarClippingPlane(float far);

		// Set the width of the cameras orthographic projection
		virtual void SetOrthographicWidth(float width);

		// Set the height of the cameras orthographic projection
		virtual void SetOrthographicHeight(float height);

		// Set the aspect ratio of the cameras perspective projection
		virtual void SetAspectRatio(float ratio);

		// Set aspect ratio using size of the viewport
		void SetAspectRatio(int width, int height);

		// Set the field of view of the cameras perspective projection
		virtual void SetFieldOfView(float fov);

	public:

		// Get the cameras near clipping plane
		inline float GetNearClippingPlane() const { return m_NearClippingPlane; }

		// Get the cameras far clipping plane
		inline float GetFarClippingPlane() const { return m_FarClippingPlane; }

		// Get the cameras orthographic width
		inline float GetOrthographicWidth() const { return m_OrthographicWidth; }

		// Get the cameras orthographic height
		inline float GetOrthographicHeight() const { return m_OrthographicHeight; }

		// Get the cameras apsect ratio
		inline float GetAspectRatio() const { return m_AspectRatio; }

		// Get the cameras field of view
		inline float GetFieldOfView() const { return m_FieldOfView; }

	public:

		// The position of the camera in world space
		glm::vec3 Position;

		// The projection mode of the camera
		mdProjectionMode ProjectionMode;

	protected:

		// The rotation of the camera in world space
		glm::quat m_Rotation;

	protected:

		// Near clipping plane of the camera
		float m_NearClippingPlane;

		// Far clipping plane of the camera
		float m_FarClippingPlane;

		// Width of the camera in orthographic view.
		// Only applies if projection mode is set to orthographic
		float m_OrthographicWidth;

		// Height of the camera in orthographic view.
		// Only applies if projection mode is set to orthographic
		float m_OrthographicHeight;

		// Aspect ratio of the camera.
		// Only applies if projection mode is set to perspective
		float m_AspectRatio;

		// Field of view of the camera.
		// Only apllies if projection mode is set to perspective
		float m_FieldOfView;
	};
}