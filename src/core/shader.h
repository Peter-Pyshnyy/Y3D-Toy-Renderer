#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "lightSource.h"


struct Shader
{
	GLuint id = 0;

	bool loadShaderProgramFromFile(const char *vertexShaderPath, const char *fragmentShaderPath);
	
	bool loadShaderProgramFromData(const char *vertexShaderData, const char *fragmentShaderData);

	void bind();

	void clear();

	GLuint getUniformLocation(const char* name);

	//https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader.h
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, float x, float y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setuVec3(const std::string& name, const glm::uvec3& value) const;
	void setuVec3(const std::string& name, unsigned int x, unsigned int y, unsigned int z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w);
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;
	void setLightSource(const LightSource& light) const;
};
