#pragma once
#include <vector>

struct Firefly{
	/* Position 2, HSL 3, alpha 1, size 1, age 1*/
	float vertex_data[8];

	float momentum[2];
	float hue_momentum;
	float saturation_momentum;
	float lighness_momentum;

};


class Fireflies {
	const unsigned int firefly_amount;
	std::vector<float> firefly_vertex_data;


public:
	Fireflies(const unsigned int amount);

	void tick(float delta_time = 1);
};