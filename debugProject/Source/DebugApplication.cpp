#include "DebugApplication.h"

#include <Application\Input.h>
#include <Application\Renderer.h>

#include <Rendering\Lighting\DirectionalLight.h>
#include <Rendering\Lighting\PointLight.h>

#include <glm\ext.hpp>

#include <iostream>

using namespace mods;

DebugApplication::DebugApplication()
{
}

DebugApplication::~DebugApplication()
{
}

bool DebugApplication::Startup()
{
	m_ModelShader.Load("Resources/Shaders/ModelVertex.vert", "Resources/Shaders/ModelFragment.frag");
	m_Nanosuit.Load("Resources/Models/Nanosuit/nanosuit.obj", eProcessModel::Triangulate);

	m_Camera.Position = glm::vec3(10.f);
	m_Camera.LookAt(glm::vec3(0.f));

	// Simple directional light
	{
		mods::DirectionalLight dirlight;

		dirlight.Color.r = 0.f;
		dirlight.SetDirection(glm::vec3(0.f, -1.f, 0.f));

		Renderer::AddLight(dirlight);
	}

	// Simple point light
	{
		mods::PointLight pntlight;

		pntlight.Color = glm::vec3(1.f, 0.f, 0.f);
		//pntlight.Position = glm::vec3(3.f, 12.f, 4.f);
		
		Renderer::AddLight(pntlight);
	}

	float constant = 1.f;
	float linear = 0.022f;
	float quadratic = 0.0019f;
	float lightMax = 1.f;
	float radius =
		(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
		/ (2 * quadratic);

	m_Flashlight.Color = glm::vec3(1.f, 1.f, 1.f);
	m_Flashlight.Position = m_Camera.Position;
	m_Flashlight.SetDirection(m_Camera.GetHeading());
	m_Flashlight.SetInnerCutoff(12.5f);
	m_Flashlight.SetOuterCutoff(17.5f);

	m_FlashlightIndex = Renderer::AddLight(m_Flashlight);

	return true;
}

bool DebugApplication::Shutdown()
{
	return true;
}

void DebugApplication::Tick(float deltaTime)
{
	// Update camera
	{
		float speed = 5.f * deltaTime;

		glm::vec3 forward = m_Camera.GetHeading() * speed;
		if (Input::IsKeyDown(eInputKey::W))
			m_Camera.Position += forward;
		if (Input::IsKeyDown(eInputKey::S))
			m_Camera.Position -= forward;

		glm::vec3 right = glm::normalize(glm::cross(m_Camera.GetHeading(), glm::vec3(0.f, 1.f, 0.f))) * speed;
		if (Input::IsKeyDown(eInputKey::D))
			m_Camera.Position += right;
		if (Input::IsKeyDown(eInputKey::A))
			m_Camera.Position -= right;

		glm::vec3 up(0.f, speed, 0.f);
		if (Input::IsKeyDown(eInputKey::E))
			m_Camera.Position += up;
		if (Input::IsKeyDown(eInputKey::Q))
			m_Camera.Position -= up;

		if (Input::IsKeyDown(eInputKey::MouseRight))
		{
			float DeltaX = Input::GetDeltaX() * 0.3f;
			float DeltaY = Input::GetDeltaY() * 0.3f;

			glm::vec3 rot = m_Camera.GetRotation();

			rot.y = glm::mod(rot.y + DeltaX, 360.f);
			rot.x = glm::clamp(rot.x + DeltaY, -89.f, 89.f);
			
			m_Camera.SetRotation(rot);
		}	

		m_Flashlight.Position = m_Camera.Position;
		m_Flashlight.SetDirection(m_Camera.GetHeading());
		Renderer::UpdateLight(m_Flashlight, m_FlashlightIndex);
	}
}

void DebugApplication::Draw()
{
	Renderer::SetCamera(m_Camera);

	Renderer::DrawModel(m_Nanosuit, m_ModelShader, glm::mat4(1.f));
}
