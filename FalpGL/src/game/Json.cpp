#include "Json.h"

Json_loader::Json_loader()
{
}

bool Json_loader::init()
{
	try 
	{
		std::ifstream file(animation_filepath);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		animations = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing animation json!\n";
		return false;
	}

	try 
	{
		std::ifstream file(entity_filepath);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		entities = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing enitity json!\n";
		return false;
	}

	try
	{
		std::ifstream file(tile_filepath);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		tiles = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing tile json!\n";
		return false;
	}

	try
	{
		std::ifstream file(map_filepath);
		std::stringstream buf;
		buf << file.rdbuf();
		std::string file_string(buf.str());
		map = json::parse(file_string);
		file.close();
	}
	catch (json::exception)
	{
		std::cout << "error parsing map json!\n";
		return false;
	}

	return true;
}
