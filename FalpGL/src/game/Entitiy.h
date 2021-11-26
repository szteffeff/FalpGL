#pragma once
#include "../renderer/RendererIncludes.h"
#include "BatchRenderer.h"
#include "Animation.h"

class Entity {
private:
public:
	Entity(VertexBuffer *a);

	std::vector<Animation> animations;
	quad m_quad;
};