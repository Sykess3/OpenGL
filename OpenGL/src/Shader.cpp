#include "Shader.h"

#include <fstream>
#include <sstream>
#include "Renderer.h"
#include <iostream>

Shader::Shader(const std::string& path)
	:m_RendererID(0), m_Path(path)
{
    ShaderProgramSource programSource = ParseShader(path);
    m_RendererID = CreateShader(programSource.VertexSource, programSource.FragmentSource);
}

Shader::~Shader()
{
    GlCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GlCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GlCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GlCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GlCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GlCall(glUniform1f(GetUniformLocation(name), value));
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];

    ShaderType shaderType = ShaderType::NONE;
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                shaderType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                shaderType = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[static_cast<int>(shaderType)] << line << '\n';
        }
    }

    return { ss[static_cast<int>(ShaderType::VERTEX)].str(), ss[static_cast<int>(ShaderType::FRAGMENT)].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GlCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, nullptr));
    GlCall(glCompileShader(id));

    int result;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        int length;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        GlCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        GlCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    	GlCall(unsigned int program = glCreateProgram());
    GlCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GlCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));

    GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));

    GlCall(glDeleteShader(vs));
    GlCall(glDeleteShader(fs));

    return program;
}

int Shader::GetUniformLocation(const std::string& name) 
{
	if (m_UniformlocationCache.find(name) != m_UniformlocationCache.end())
	{
        return m_UniformlocationCache[name];
	}
    GlCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if(location == -1)
    {
        std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
    }

    m_UniformlocationCache[name] = location;

    return location;
}
