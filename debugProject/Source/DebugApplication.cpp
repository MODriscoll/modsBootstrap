#include "DebugApplication.h"

#include <Application\Input.h>
#include <Application\Renderer.h>

#include <Rendering\Lighting\PointLight.h>

#include <glm\ext.hpp>

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
	m_Nanosuit = new mods::Model("Resources/Models/Nanosuit/nanosuit.obj");

	//m_DirLight.SetDirection(glm::vec3(0.f, -1.f, 0.f));
	m_DirLight.SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
	m_DirLight.Color = glm::vec4(1.f, 0.f, 0.f, 1.f);

	m_Camera.Position = glm::vec3(0.f, 5.f, 8.f);
	m_Camera.LookAt(glm::vec3(0.f));

	Renderer::AddLight(m_DirLight);

	return true;
}

bool DebugApplication::Shutdown()
{
	delete m_Nanosuit;

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
	}
}

void DebugApplication::Draw()
{
	Renderer::SetCamera(m_Camera);

	Renderer::DrawModel(*m_Nanosuit, m_ModelShader, glm::mat4(1.f));
}
