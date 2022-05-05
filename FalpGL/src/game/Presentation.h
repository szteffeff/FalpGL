#pragma once
#include "../renderer/RendererIncludes.h"


enum t {
	What_I_Chose,
	Why_I_Chose,
	My_Mentor,
	Obstacles_and_Refelections,
	What_I_Enjoyed
};

struct Project {
	t myInfo;
	std::string name;
	std::string project;
	std::string group_members;
	std::string mentor;
	std::string description;
};


void presentaion(Project capstone);

Project* introduction();
