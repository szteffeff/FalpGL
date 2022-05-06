#include "Presentation.h"


class Leos_Presentation : public Capstone_Project {
private:

	std::string Presentor = "Leo Panganiban";
	std::string Project = "Game Software Development";
	std::string Group_Members = "'Leo', 'Alex', 'Liam', 'Max', 'Jackson'";
	std::string Mentor = "Wilbert Chua";

	std::string What_Is_The_Capstone = "A 2D game created through C++ and OpenGL";

public:
	void What_And_Why_I_Chose(float reason_for_project);
	void Who_Is_My_Mentor(float mentor);
	void Problems_I_Faced(float obstales);
	void What_I_Enjoyed(float The_Fun_Parts);
	void The_end();
};


void Leos_Presentation::What_And_Why_I_Chose(float reason_for_project)
{
	text = R"(
			Hello, my name is Leo Panganiban and this is our capstone project, 
			we are attempting to create a game demo or create the start of a game. 

			The reason I chose to participate in this project 
			is because I am personally interested in becoming a 
			game developer in the future. Working on a game 
			like this helps me learn some of the basics of 
			programming that I will need for the future. 
			
			I Worked on:
			- Creating the enemy AI
			- Writing out the code to make sound and music play in the bacakground
			- Creating the user interface for the game like the health bar
)";
}


void Leos_Presentation::Who_Is_My_Mentor(float mentor)
{
	text = R"(
			My mentor is Wilbert Chua. He is working as a software developer in Vancouver. 
			Even through a many difficulties, he was stil able to assist me during
			The creation of this project.

			Some of the ways he has helped include:
			- Giving advice for the creation of certain code
			- Supplied many resources to help strengthen my knowledge
			  on diffrent aspects of coding
			- Provided much moral support.

)";
}


void Leos_Presentation::Problems_I_Faced(float obstales)
{
	text = R"(
			I have faced Many obstacles during the creation of this game, these inlcude:
			- A severe Lack of motivation
			- A increadibly busy school schedule
			- Misunderstandings or a lack of knowledge on certain code
			- Mentor had to move

			After much consideration, I realized that there are many things 
			that I could have done better, including:
			- Reached out to my mentor more via email, text, or call
			- Taking the time to learn C++ more indept to have smoother 
			  time programming
			- Communicating better with my team as to clarify what things 
			  we wanted and could get done

)";
}


void Leos_Presentation::What_I_Enjoyed(float The_Fun_Parts)
{
	text = R"(
			Overall, I enjoyed the process of making this game. 
			Even if there were some difficulties, I very much enjoyed
			working on a big project with my friends, coding the monsters ai
			and also using my problem solving skills to figure out how to
			make certain things work.
			
			Other things I enjoyed include:
			- The huge sense of accomplishment I get after figuring
			  out how to make piece of code work.
			- The feeling of relief as I finish manualy creating 
			  many, many mobs.
			- Making the weapons actually move as intended 
)";
}

void Leos_Presentation::The_end()
{
	text = R"(
			Thank you very much for reading! Have a great day!
)";
}
