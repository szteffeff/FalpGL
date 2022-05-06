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

class Capstone_Project {};
void What_And_Why_I_Chose(float reason_for_project);
void Who_Is_My_Mentor(float mentor);
void Problems_I_Faced(float obstales);
void What_I_Enjoyed(float The_Fun_Parts);
std::string text;