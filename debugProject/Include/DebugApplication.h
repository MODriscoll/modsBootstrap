#pragma once

#include <Application\Application.h>

#include <Rendering\Meshes\Model.h>
#include <Rendering\Shaders\Shader.h>

#include <Rendering\Lighting\SpotLight.h>

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

	mods::SpotLight m_Flashlight;
	mods::int32 m_FlashlightIndex;
};