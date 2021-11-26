#include "Entitiy.h"

Entity::Entity(VertexBuffer *a)
	: m_quad(*a, 0.0f, 0.0f, 1)
{
}
