#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "renderer.h"

Shader::Shader(const std::string& filepath)
	: m_filepath(filepath), m_renderer_id(0)
{
    ShaderProgramSource source = parseShader(filepath);
    m_renderer_id = createShader(source);

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
        std::stringstream stream;
        stream << "[SHADER]! uniform '" << name << "' does not exist in shader: " << m_filepath.substr(m_filepath.rfind("/") + 1);
        console_log(stream.str());
    } 

     m_uniform_location_cache[name] = location;

    return location;
}



ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class shaderType {
        NONE = -1, VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER, GEOMETRY = GL_GEOMETRY_SHADER
    };

    shaderType type = shaderType::NONE;

    ShaderProgramSource source;
    std::string line;
    while (getline(stream, line))
    {
        /* Determine what shader type is currently being parsed */
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
            else if (line.find("geometry") != std::string::npos)
            {
                type = shaderType::GEOMETRY;
            }

            /* Don't write type to source */
            continue;
        }


        /* Remove all data after a comment */
        if (line.find("//") != std::string::npos)
        {
            line.erase(line.find("//"));
        }
        
        
        /* Write line to appropriate shader source */
        switch (type)
        {
        case shaderType::NONE:
            break;

        case shaderType::VERTEX:
            source.VertexSource.append(std::string("\n").append(line));
            break;

        case shaderType::FRAGMENT:
            source.FragmentSource.append(std::string("\n").append(line));
            break;

        case shaderType::GEOMETRY:
            source.GeometrySource.append(std::string("\n").append(line));
            break;

        default:
            break;
        }
        

    }

    return source;
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

        std::stringstream stream;
        switch (type)
        {
        case (GL_VERTEX_SHADER):
            stream << "[SHADER]! Failed to compile " << "vertex" << " shader '" << m_filepath.substr(m_filepath.rfind("/") + 1) << "':" << message;
            console_log(stream.str());
            stream.clear();
            break;

        case (GL_FRAGMENT_SHADER):
            stream << "[SHADER]! Failed to compile " << "fragment" << " shader '" << m_filepath.substr(m_filepath.rfind("/") + 1) << "':" << message;
            console_log(stream.str());
            stream.clear();
            break;

        case (GL_GEOMETRY_SHADER):
            stream << "[SHADER]! Failed to compile " << "geometry" << " shader '" << m_filepath.substr(m_filepath.rfind("/") + 1) << "':" << message;
            console_log(stream.str());
            stream.clear();
            break;

        default:
            break;
        }
        

        glDeleteShader(id);
        return 0;
    }
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


unsigned int Shader::createShader(ShaderProgramSource source)
{
    unsigned int program = glCreateProgram();
    unsigned int fs = 0, vs = 0, gs = 0;
    bool geometry = false;
    std::stringstream stream;

    if (source.VertexSource != "")
    {
        vs = compileShader(GL_VERTEX_SHADER, source.VertexSource);
    }
    else
    {
        stream << "[SHADER]! No vertex shader source provided for shader: '" << m_filepath.substr(m_filepath.rfind("/") + 1) << "'!";
        console_log(stream.str());
        stream.clear();
    }

    if (source.FragmentSource != "")
    {
        fs = compileShader(GL_FRAGMENT_SHADER, source.FragmentSource);
    }
    else
    {
        stream << "[SHADER]! No fragment shader source provided for shader: '" << m_filepath.substr(m_filepath.rfind("/") + 1) << "'!";
        console_log(stream.str());
        stream.clear();
    }

    if (source.GeometrySource != "")
    {
        gs = compileShader(GL_GEOMETRY_SHADER, source.GeometrySource);
        geometry = true;
    }


    if (vs)
        stream << "[SHADER]: Vertex";

    if (fs)
        stream << " + Fragment";

    if (geometry && gs)
        stream << " + Geometry";
    
    if (vs || fs || (geometry && gs))
        stream << " shader '" << m_filepath.substr(m_filepath.rfind("/") + 1) << "' compiled successfuly!";

    console_log(stream.str());

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    if (geometry) 
    {
        glAttachShader(program, gs);
    }

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    if (geometry)
    {
        glDeleteShader(gs);
    }

    return program;
}


