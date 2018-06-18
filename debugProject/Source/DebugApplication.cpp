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
	m_ModelTransform = glm::mat4(1.f);
}

DebugApplication::~DebugApplication()
{
}

bool DebugApplication::Startup()
{
	m_ParticleShader.Load("Resources/Shaders/TestParticle.vert", "Resources/Shaders/TestParticle.frag");
	m_ParticleTexture.Load("Resources/Textures/AwesomeFace.png", eTextureChannels::RGB);
	m_TestEmitter.Initialise(1000, 500, 0.1f, 1.f, 1, 5, 1, 0.1f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 0.f));

	m_TestEmitter.m_Position.x = 5.f;

	m_ModelShader.Load("Resources/Shaders/ModelVertex.vert", "Resources/Shaders/ModelFragment.frag");
	m_Nanosuit.Load("Resources/Models/Nanosuit/nanosuit.obj", eProcessModel::Triangulate);

	m_Camera.Position = glm::vec3(10.f);
	m_Camera.LookAt(glm::vec3(0.f));

	// Simple directional light
	{
		mods::DirectionalLight dirlight;

		dirlight.Color = glm::vec3(0.f, 1.f, 1.f);
		dirlight.SetDirection(glm::vec3(1.f, -1.f, 0.f));

		Renderer::AddLight(dirlight);
	}

	// Simple point light
	{
		mods::PointLight pntlight;

		pntlight.Color = glm::vec3(0.f, 1.f, 0.f);
		pntlight.Position = glm::vec3(3.f, 12.f, 4.f);
		pntlight.SetIntensity(100.f);
		pntlight.SetRadius(15.f);
		
		Renderer::AddLight(pntlight);
	}

	Renderer::SetCamera(m_Camera);
	//Renderer::SetBloomThreshold(20.f);

	//m_TestGPUParticleShader.Load("Resources/Shaders/pt.vert", "Resources/Shaders/pt.frag");
	m_TestGPUParticleShader.Load("Resources/Shaders/pt.vert", "Resources/Shaders/pt.geom", "Resources/Shaders/pt.frag");
	m_TestGPUParticles.Init(1028 * 1028);
	//m_TestGPUParticles.m_EmitterPosition.y = 7.f;

	return true;
}

bool DebugApplication::Shutdown()
{
	return true;
}

void DebugApplication::Tick(float deltaTime)
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

	if (Input::WasKeyPressed(eInputKey::F1))
		Renderer::EnableGammaCorrection(true);
	if (Input::WasKeyPressed(eInputKey::F2))
		Renderer::EnableGammaCorrection(false);

	if (Input::IsKeyDown(eInputKey::MouseRight))
	{
		float DeltaX = Input::GetDeltaX() * 0.3f;
		float DeltaY = Input::GetDeltaY() * 0.3f;

		glm::vec3 rot = m_Camera.GetRotation();

		rot.y = glm::mod(rot.y + DeltaX, 360.f);
		rot.x = glm::clamp(rot.x + DeltaY, -89.f, 89.f);

		m_Camera.SetRotation(rot);
	}

	static bool wire = false;
	if (Input::WasKeyPressed(eInputKey::F5))
	{
		wire = !wire;
		Renderer::EnableWireframe(wire);
	}

	static bool bloom = false;
	if (Input::WasKeyPressed(eInputKey::F6))
	{
		bloom = !bloom;
		Renderer::EnableBloom(bloom);
	}

	m_ModelTransform = glm::rotate(m_ModelTransform, glm::radians(45.f) * deltaTime, glm::vec3(0.f, 1.f, 0.f));

	m_TestEmitter.Update(deltaTime, m_Camera.GetTransformMatrix());

	m_TestGPUParticles.Update();
}

void DebugApplication::Draw()
{
	Renderer::DrawModel(m_Nanosuit, m_ModelShader, m_ModelTransform);

	m_ParticleTexture.Bind(0);

	m_ParticleShader.Bind();
	m_ParticleShader.SetUniformValue("image", 0);
	m_TestEmitter.Draw();

	m_TestGPUParticleShader.Bind();
	m_TestGPUParticleShader.SetUniformValue("image", 0);
	m_TestGPUParticles.Draw(m_TestGPUParticleShader);
}
