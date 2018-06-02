#include "DebugApplication.h"

#include <Application\Input.h>
#include <Application\Renderer.h>

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
	return true;
}

bool DebugApplication::Shutdown()
{
	return true;
}

void DebugApplication::Tick(float deltaTime)
{
	float x, y;
	mods::Input::GetMouseDelta(x, y);
	std::cout << "X: " << x << " Y: " << y << std::endl;
}

void DebugApplication::Draw()
{
}
