#pragma once
#include <string>
#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_Path;
	std::unordered_map<std::string, int> m_UniformlocationCache;
public:
	Shader(const std::string& path);
	~Shader();

	Shader(const Shader& other);
	Shader(Shader&& other);
	Shader& operator=(const Shader& other);
	Shader& operator=(Shader&& other);

	void Bind() const;
	void Unbind() const;

	void SetUniform3f(const std::string& name, glm::vec3 vec);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};

