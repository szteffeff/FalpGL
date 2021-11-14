#pragma once
#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h"
#include "Animation.h"

class Entity {
private:
	std::vector<Animation> animations;
	quad quad;
};