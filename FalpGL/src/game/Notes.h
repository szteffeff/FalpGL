#pragma once

#include "Animation.h"

/* Need to:
* 
*	eliminate float for positioning vericies
*   framebuffer for pixel-perferect windowed mode
*	animation frame oreder
*/

/*
*    MATRICES:
*
* Zoom: scales everything 2x
* Player Transform: Moves around everthing to follow player
* Map Transform: Centers Everthing
* Projection: Fits output to window
*
*/

/*
* 
* file for stuff I don't know where to put yet
* and notes and things
* 
* probably don't include this anywhere
*  
* t i l e   d e p t h
*  
* 2 3
* 0 1
* 
* 	if (color.w == 0)
*		discard;
*
* s  s  y  t  t  i
* 00 01 02 03 04 05
* 06 07 08 09 10 11
* 12 13 14 15 16 17
* 18 19 20 21 22 23
* 
* 
*	X Y Z TX TY TI
*	X Y Z TX TY TI
*	X Y Z TX TY TI
*	X Y Z TX TY TI
* 
* 
*/


/* old up/down shift
static int tile = 0;
print = true;
std::vector<Tile*> buffer;
buffer.resize(width);
current_center[1] += 1;
for (int i = 0; i < width; i++) {
	map_vector[i]->translate(0, height * 32);
	map_vector[i]->change_type(tile_id(((int)abs(current_center[1]) % 20) + 1), loader);
	buffer[i] = map_vector[i];
}
std::shift_left(map_vector.begin(), map_vector.end(), width);
for (int i = 0; i < width; i++)
{
	map_vector[((width * height)) - width + i] = buffer[i];
}
*/


json get_object(int depth)