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
	m_ParticleShader.Load("Resources/Shaders/Particles/TestParticle.vert", "Resources/Shaders/Particles/TestParticle.frag");
	m_ParticleTexture.Load("Resources/Textures/AwesomeFace.png", eTextureChannels::RGB);
	m_TestEmitter.Initialise(1000, 500, 0.1f, 1.f, 1, 5, 1, 0.1f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 0.f));

	m_TestEmitter.m_Position.x = 5.f;

	m_ModelShader.Load("Resources/Shaders/Meshes/ModelVertex.vert", "Resources/Shaders/Meshes/ModelFragment.frag");
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

	m_TestGPUParticleShader.Load("Resources/Shaders/Particles/pt.vert", "Resources/Shaders/Particles/pt.geom", "Resources/Shaders/Particles/pt.frag");
	m_TestGPUParticles.Init(1028 * 1028);
	m_TestGPUParticles.m_EmitRate = 1.f;
	m_TestGPUParticles.m_EmitterPosition.y = -200.f;

	m_Font = new mods::AltFont("Resources/Fonts/consolas.ttf", 24);

	EnableVSync(false);

	return true;
}

bool DebugApplication::Shutdown()
{
	delete m_Font;

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

	if (Input::IsKeyDown(eInputKey::MouseRight))
	{
		float DeltaX = Input::GetDeltaX() * 0.3f;
		float DeltaY = Input::GetDeltaY() * 0.3f;

		glm::vec3 rot = m_Camera.GetRotation();

		rot.y = glm::mod(rot.y + DeltaX, 360.f);
		rot.x = glm::clamp(rot.x + DeltaY, -89.f, 89.f);

		m_Camera.SetRotation(rot);
	}

	if (Input::WasKeyPressed(eInputKey::F1))
		Renderer::EnableWireframe(!Renderer::IsWireframeEnabled());

	if (Input::WasKeyPressed(eInputKey::F2))
		Renderer::EnableGammaCorrection(!Renderer::IsGammaCorrectionEnabled());

	if (Input::WasKeyPressed(eInputKey::F3))
		Renderer::EnableBloom(!Renderer::IsBloomEnabled());

	m_ModelTransform = glm::rotate(m_ModelTransform, glm::radians(45.f) * deltaTime, glm::vec3(0.f, 1.f, 0.f));

	m_TestEmitter.Update(deltaTime, m_Camera.GetTransformMatrix());

	m_TestGPUParticles.Update(deltaTime);
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

	Renderer::DrawString(std::string("Controls:"), *m_Font, glm::ivec2(20, 680));
	Renderer::DrawString(std::string("Forward = W, Backwards = S"), *m_Font, glm::ivec2(20, 660));
	Renderer::DrawString(std::string("Left = A, Right = D"), *m_Font, glm::ivec2(20, 640));
	Renderer::DrawString(std::string("Up = Q, Down = E (In Worldspace)"), *m_Font, glm::ivec2(20, 620));
	Renderer::DrawString(std::string("Hold Right Mouse then move mouse to rotate camera"), *m_Font, glm::ivec2(20, 600));
	Renderer::DrawString(std::string("F1 to toggle Wireframe, F2 to toggle Gamma Correction, F3 to toggle Bloom"), *m_Font, glm::ivec2(20, 580));

	Renderer::DrawString(std::string("FPS: ") + std::to_string(GetFPS()), *m_Font, glm::ivec2(20));
	Renderer::DrawString(std::string("Wireframe Enabled: ") + std::string(Renderer::IsWireframeEnabled() ? "True" : "False"), *m_Font, glm::ivec2(20, 40));
	Renderer::DrawString(std::string("Gamma Correction Enabled: ") + std::string(Renderer::IsGammaCorrectionEnabled() ? "True" : "False"), *m_Font, glm::ivec2(20, 60));
	Renderer::DrawString(std::string("Bloom Enabled: ") + std::string(Renderer::IsBloomEnabled() ? "True" : "False"), *m_Font, glm::ivec2(20, 80));
}
