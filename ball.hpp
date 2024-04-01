#pragma once

#include <iostream>
#include <vector>
#include <array>

#include "specifications.hpp"

class Ball {
public:
	Ball(
		double g_velocity,
		double g_angle,
		float  g_x,
		float  g_y,
		float  g_radius,
		float  g_mass,
		RGBA_t g_color
	) : velocity(g_velocity),angle(g_angle),x(g_x),y(g_y),
		radius(g_radius),mass(g_mass),color(g_color) {}

private:
	double velocity = 0;
	double angle = 0;
	float x = 0, y = 0;
	float radius = 0;
	float mass = 0;
	// 256 bits up until here
	
	RGBA_t color = {0,0,0,0};
	// stores the last number of positions of the ball
	//std::array<std::pair<float,float>,ball::TRAIL_LENGTH> trails;
};