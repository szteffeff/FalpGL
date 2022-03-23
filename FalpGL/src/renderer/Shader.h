#pragma once
#include <string>
#include <unordered_map>

#include"glm/glm.hpp"
#include "Log.h"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader
{
private:
	std::string m_filepath;
	unsigned int m_renderer_id;
	std::unordered_map<std::string, int> m_uniform_location_cache;
public:
	Shader(const std::string& filepath);
	Shader() {}
	~Shader();

	void Bind() const;

	void Unbind() const;

	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int i);
	void SetUniform1f(const std::string& name, float i);
	void SetUniform1iv(const std::string& name, int count, int values[]);

	void SetUniformMat4f(const std::string& name, glm::mat4 m);

private:
	int GetUnifromLocation(const std::string& name);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int createShader(ShaderProgramSource source);
	unsigned int compileShader(unsigned int type, const std::string& source);
	ShaderProgramSource parseShader(const std::string& filepath);
};