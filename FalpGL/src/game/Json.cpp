#include "Json.h"

Json_loader::Json_loader()
{
	{
		std::ifstream file(animation_filepath);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		animations = json::parse(file_string);
		file.close();
	}
	{
		std::ifstream file(entity_filepath);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		entities = json::parse(file_string);
		file.close();
	}
}
