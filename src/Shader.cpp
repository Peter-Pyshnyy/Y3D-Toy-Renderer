#include "shader.h"
#include <string>
#include <fstream>
#include <iostream>

//this allocates memory!
char *readEntireFile(const char *source)
{
	std::ifstream file;
	file.open(source);

	if (!file.is_open())
	{
		std::cout << "Error openning file: " << source << "\n";
		return nullptr;
	}

	GLint size = 0;
	file.seekg(0, file.end);
	size = file.tellg();
	file.seekg(0, file.beg);

	char *fileContent = new char[size + 1] {};

	file.read(fileContent, size);

	file.close();

	return fileContent;
}

bool Shader::loadShaderProgramFromFile(const char *vertexShaderPath,
	const char *fragmentShaderPath)
{
	char *vertexData = readEntireFile(vertexShaderPath);
	char *fragmentData = readEntireFile(fragmentShaderPath);

  	if (vertexData == nullptr || fragmentData == nullptr)
	{
		delete[] vertexData;
		delete[] fragmentData;

		return 0;
	}

	bool rez = loadShaderProgramFromData(vertexData, fragmentData);


	delete[] vertexData;
	delete[] fragmentData;

	return rez;
}


GLint createShaderFromData(const char *data, GLenum shaderType)
{
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &data, nullptr);
	glCompileShader(shaderId);

	GLint rezult = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &rezult);

	if (!rezult)
	{
		char *message = 0;
		int   l = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &l);

		if (l)
		{
			message = new char[l];

			glGetShaderInfoLog(shaderId, l, &l, message);

			message[l - 1] = 0;

			std::cout << data << ":\n" << message << "\n";

			delete[] message;

		}
		else
		{
			std::cout << data << ":\n" << "unknown error" << "\n";
		}

		glDeleteShader(shaderId);

		shaderId = 0;
		return shaderId;
	}

	return shaderId;

}


bool Shader::loadShaderProgramFromData(const char *vertexShaderData,
	const char *fragmentShaderData)
{
	auto vertexId = createShaderFromData(vertexShaderData, GL_VERTEX_SHADER);

	if (vertexId == 0) { return 0; }

	auto fragmentId = createShaderFromData(fragmentShaderData, GL_FRAGMENT_SHADER);

	if (fragmentId == 0)
	{
		glDeleteShader(vertexId);
		return 0;
	}

	id = glCreateProgram();

	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);

	glLinkProgram(id);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	GLint info = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &info);

	if (info != GL_TRUE)
	{
		char *message = 0;
		int   l = 0;

		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &l);

		message = new char[l];

		glGetProgramInfoLog(id, l, &l, message);

		message[l - 1] = 0;

		std::cout << "Link error: " << message << "\n";

		delete[] message;

		glDeleteProgram(id);
		id = 0;
		return 0;
	}

	glValidateProgram(id);

	return true;
}

GLuint Shader::getUniformLocation(const char *name)
{
	GLuint location = glGetUniformLocation(id, name);
	if (location == GL_INVALID_INDEX || location == -1)
	{
		std::cout << "Uniform error: " << name << " not found in shader program.\n";
	}
	return location;
}

void Shader::bind()
{
	glUseProgram(id);
}

void Shader::clear()
{
	glDeleteProgram(id);

	*this = {};
}

//https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setiVec3(const std::string& name, const glm::ivec3& value) const
{
	glUniform3iv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setiVec3(const std::string& name, int x, int y, int z) const
{
	glUniform3i(glGetUniformLocation(id, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setLightSource(const LightSource& light) const
{
	if (!LightSource::incrementLightCount(light.type)) {
		std::cout << "Maximum number of lights of this type reached.\n";
		return;
	}

	switch (light.type) {
	case LightType::DIRECTIONAL: {
		setVec3("dirLight.direction", light.direction);
		setVec3("dirLight.ambient", light.ambient);
		setVec3("dirLight.diffuse", light.diffuse);
		setVec3("dirLight.specular", light.specular);
		break;
	}
	case LightType::POINT: {
		std::string name = "pointLights[" + std::to_string(LightSource::getLightCount()[LightType::POINT]) + "]";
		setVec3(name + ".position", light.position);
		setVec3(name + ".ambient", light.ambient);
		setVec3(name + ".diffuse", light.diffuse);
		setVec3(name + ".specular", light.specular);
		setFloat(name + ".constant", light.constant);
		setFloat(name + ".linear", light.linear);
		setFloat(name + ".quadratic", light.quadratic);
		break;
	}
	case LightType::SPOT: {
		std::string name = "spotlights[" + std::to_string(LightSource::getLightCount()[LightType::POINT]) + "]";
		setVec3(name + ".position", light.position);
		setVec3(name + ".ambient", light.ambient);
		setVec3(name + ".diffuse", light.diffuse);
		setVec3(name + ".specular", light.specular);
		setVec3(name + ".direction", light.direction);
		setFloat(name + ".cutOff", light.cutOff);
		break;
	}
	default:
		break;
	}
}
