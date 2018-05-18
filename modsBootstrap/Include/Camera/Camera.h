#pragma once

#include "Types.h"

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

namespace mods
{
	enum class eProjectionMode : byte
	{
		Orthographic,
		Perspective
	};

	// Camera that only contains a position and rotation.
	// Perspective and orthographic variables are not in a union to
	// allow for the easy transition between different projection modes
	// TODO: implement a rotator class similar to ue4 FRotator
	// it should be able handle limiting view
	class Camera
	{
	public:

		Camera();
		virtual ~Camera() = default;

	public:

		// Sets cameras rotation to face the given targets location
		void LookAt(const glm::vec3& location);

	public:

		// Get the view matrix of this camera
		glm::mat4 GetViewMatrix() const;

		// Get the projection matrix of this camera.
		// This matrix will be different based on ProjectionMode
		glm::mat4 GetProjectionMatrix() const;

		// Get the projection matrix specified to the given mode
		glm::mat4 GetProjectionMatrix(eProjectionMode mode) const;

		// Get the projection view matrix of this camera.
		// This matrix will be different based on ProjectionMode
		glm::mat4 GetProjectionView() const;

		// Get the projection view matrix specified to the given mode
		glm::mat4 GetProjectionView(eProjectionMode mode) const;

	public:

		// Set the cameras rotation
		void SetRotation(const glm::vec3& rotation);

		// Set the near clipping plane of the camera
		void SetNearClippingPlane(float near);

		// Set the far clipping plane of the camera
		void SetFarClippingPlane(float far);

		// Set the width of the cameras orthographic projection
		void SetOrthographicWidth(float width);

		// Set the height of the cameras orthographic projection
		void SetOrthographicHeight(float height);

		// Set the aspect ratio of the cameras perspective projection
		void SetAspectRatio(float ratio);

		// Set aspect ratio using size of the viewport
		void SetAspectRatio(int32 width, int32 height);

		// Set the field of view of the cameras perspective projection
		void SetFieldOfView(float fov);

	public:

		// Get this cameras rotation
		const glm::vec3& GetRotation() const { return m_Rotation; }

		// Get the direction this camera is facing
		const glm::vec3& GetHeading() const { return m_Heading; }

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

	protected:

		// Calculates the cameras heading based of the current rotation
		void CalculateHeading();

	public:

		// The position of the camera in world space
		glm::vec3 Position;

		// The projection mode of the camera
		eProjectionMode ProjectionMode;

	protected:

		// The rotation of the camera in world space
		glm::vec3 m_Rotation;

		// The direction the camera is facing
		glm::vec3 m_Heading;

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