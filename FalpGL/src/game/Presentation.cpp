#include "Presentation.h"

Project* introduction()
{
	Project* capstone = new Project;

	capstone->name =          "Alex Stefani";
	capstone->project =       "Game Engine Programing";
	capstone->group_members = "'Alex', 'Leo', 'Liam', 'Jackson', 'Max'";
	capstone->mentor =        "Anthony Wagner";

	capstone->description =   "A 2D pixel art game engine written in C++ with Opengl";

	return capstone;
}


void presentaion(Project capstone) {
	const char* text;

	switch (capstone.myInfo)
	{
	case (What_I_Chose):
		text = R"(
				For my capstone project, I chose to program a game. I focused
				on creating the game engine and graphics programming.

				I worked on:
					- Rendering graphics drawn by other members of my group
					- Loading and properly displaying a map
					- Maintaining a version control so I can work with others
			)";
		break;

	case (Why_I_Chose):
		text = R"(
				I chose my capstone project because I wanted to learn more about
				programming and commit to doing a big project. In my opinion, the
				best way to learn something is to do it, so I thought this project
				would be a great way to learn about some things I'm interested in.

				Some things I wanted to learn about were:
					- Graphics programing and writing shaders
					- More advanced C++ programing
					- Using Git and Github
					- Matrix and vector math
			)";
		break;


	case (My_Mentor):
		text = R"(
				My mentor, Anthony Wagner, does freelance software development
				work and has developed games himself. He has lots of experience
				with the same software and tools I used for my capstone.

				Anthony helped me by:
					- Directing me to resources to learn about programming
					- Suggesting software to use
					- Helping me when I was stuck on a problem
			)";
		break;


	case (Obstacles_and_Refelections):
		text = R"(
				Some challenges I faced while completing my capstone were:
					- Staying motivated when faced with a big problem
					- Knowing what to do next after I finished a step
					- Effectivly working with the rest of my group
					- Keeping everything organized

				Looking back, some things I could have done to make my
				work easier would be:
					- Use a schedual to organize and spread out work
					- Look at tutorials for software instead of guessing
					- Leave more comments to make code easier to understand	
			)";
		break;


	case (What_I_Enjoyed):
		text = R"(
				My favorite part of my capstone was definitly solving problems
				with things that I had learned. Solving problems in interesting
				and creative ways is my favorite part of programming and I had
				lots of opportunity for that in this project.

				Some other things I enjoyed are:
					- Learing how to use new software
					- Seeing something I made work properly on the screen
					- Looking back at silly mistakes and old code 
			)";
		break;


	default:
		text = "Thanks for reading!";
		break;
	}


	std::cout << std::string(text).append("\n").c_str();
}