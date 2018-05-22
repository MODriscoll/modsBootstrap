#pragma once

#include <Application\Application.h>

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
};