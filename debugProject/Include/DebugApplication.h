#pragma once

#include <Application\Application.h>

#include <Rendering\Meshes\Model.h>
#include <Rendering\Shaders\Shader.h>

#include <Rendering\Lighting\PointLight.h>

#include <Rendering\Particles\ParticleEmitter.h>

#include <Rendering\Textures\Texture2D.h>

#include <Camera\Camera.h>

class DebugApplication : public mods::Application
{
public:

	DebugApplication();
	virtual ~DebugApplication();

protected:

	virtual bool Startup() override;
	virtual bool Shutdown() override;
	
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:

	mods::Camera m_Camera;

	mods::ShaderProgram m_ModelShader;
	mods::Model m_Nanosuit;

	mods::ShaderProgram m_ParticleShader;
	mods::Texture2D m_ParticleTexture;
	mods::ParticleEmitter m_TestEmitter;

	glm::mat4 m_ModelTransform;
};