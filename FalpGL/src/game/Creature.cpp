#include "Creature.h"




Creature::Creature() {}





Health_Bar::Health_Bar(BatchRenderer* for_active_buffer, Json_loader* l)
	: frame(&for_active_buffer->vertex_buffer, l->entities["HEALTH_BAR_FRAME"]["flask"]),
	red_bar(&for_active_buffer->vertex_buffer, l->entities["HEALTH_BAR_RED"]["red_bar"]),
	green_bar(&for_active_buffer->vertex_buffer, l->entities["HEALTH_BAR_GREEN"]["green_bar"])
{}