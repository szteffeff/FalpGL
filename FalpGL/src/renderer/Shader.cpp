#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "renderer.h"

Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_renderer_id(0)
{
    ShaderProgramSource source = parseShader(filepath);
    m_renderer_id = createShader(source.VertexSource, source.FragmentSource);

}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_renderer_id));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_renderer_id));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUnifromLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1f(const std::string& name, float i)
{
    GLCall(glUniform1f(GetUnifromLocation(name), i));
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4 m)
{
    GLCall(glUniformMatrix4fv(GetUnifromLocation(name), 1, GL_FALSE, &m[0][0]));
}

void Shader::SetUniform1i(const std::string& name, int i) 
{
    GLCall(glUniform1i(GetUnifromLocation(name), i));
}

void Shader::SetUniform1iv(const std::string& name, int count, int values[])
{
    GLCall(glUniform1iv(GetUnifromLocation(name), count, values));
}

int Shader::GetUnifromLocation(const std::string& name)
{
    if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
    {
        return m_uniform_location_cache[name];
    }

    GLCall(int location = glGetUniformLocation(m_renderer_id, name.c_str()));
    if (location == -1)
    {
        std::cout << "[WARN] uniform '" << name << "' does not exist" << std::endl;
    } 

     m_uniform_location_cache[name] = location;

    return location;
}



ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class shaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    shaderType type = shaderType::NONE;

    std::stringstream ss[2];
    std::string line;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = shaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }

    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader '" << m_filepath << "':" << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    std::cout << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader '" << m_filepath << "' compiled succesfully!" << std::endl;
    return id;
}

unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


