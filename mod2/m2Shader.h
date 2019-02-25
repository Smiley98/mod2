#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <unordered_map>

class m2ShaderProgram;
class m2Shader {
	friend m2ShaderProgram;

public:
	m2Shader(GLenum, const std::string&);
	~m2Shader();

private:
	GLuint m_shaderHandle;
};

class m2ShaderProgram {

public:
	m2ShaderProgram();
	~m2ShaderProgram();

	m2ShaderProgram& add(const m2Shader&);
	m2ShaderProgram& link();

	m2ShaderProgram& bind();
	m2ShaderProgram& unbind();

	m2ShaderProgram& setInt(const std::string&, int);
	m2ShaderProgram& setFloat(const std::string&, float);

	m2ShaderProgram& setVec2(const std::string&, float[2]);
	m2ShaderProgram& setVec2(const std::string&, const glm::vec2&);
	m2ShaderProgram& setVec2(const std::string&, unsigned int, float[2]);
	m2ShaderProgram& setVec2(const std::string&, unsigned int, const glm::vec2&);

	m2ShaderProgram& setVec3(const std::string&, float[3]);
	m2ShaderProgram& setVec3(const std::string&, const glm::vec3&);
	m2ShaderProgram& setVec3(const std::string&, unsigned int, float[3]);
	m2ShaderProgram& setVec3(const std::string&, unsigned int, const glm::vec3&);

	m2ShaderProgram& setVec4(const std::string&, float[4]);
	m2ShaderProgram& setVec4(const std::string&, const glm::vec4&);
	m2ShaderProgram& setVec4(const std::string&, unsigned int, float[4]);
	m2ShaderProgram& setVec4(const std::string&, unsigned int, const glm::vec4&);

	m2ShaderProgram& setMat3(const std::string&, float[9]);
	m2ShaderProgram& setMat3(const std::string&, const glm::mat3&);

	m2ShaderProgram& setMat4(const std::string&, float*);
	m2ShaderProgram& setMat4(const std::string&, unsigned int, float*);

	//GLint getAttribLocation(std::string);
	//glBindAttribLocation or glBindFragDataLocation specifies input/output.
	//It might be worth while to do this for each shader so that we're guarenteed we're reading/writing what we expec to.
	//ie ensure we're not pointing to normals when we think we're pointing to uvs.

private:
	//Cache uniform locations.
	std::unordered_map<std::string, GLuint> m_uniforms;
	//Must store shaders in order to detatch them.
	std::vector<m2Shader> m_shaders;
	GLuint m_programHandle;

	inline GLint getUniformHandle(const std::string&);
	inline void deleteProgram();

	template<typename T>
	inline void _setVec2(const std::string&, const T&);
	template<typename T>
	inline void _setVec3(const std::string&, const T&);
	template<typename T>
	inline void _setVec4(const std::string&, const T&);

	template<typename T>
	inline void _setVec2(const std::string&, unsigned int, const T&);
	template<typename T>
	inline void _setVec3(const std::string&, unsigned int, const T&);
	template<typename T>
	inline void _setVec4(const std::string&, unsigned int, const T&);
};

