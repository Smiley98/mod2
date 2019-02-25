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
	m2ShaderProgram& setInt(const std::string&, unsigned int, int*);

	m2ShaderProgram& setFloat(const std::string&, float);
	m2ShaderProgram& setFloat(const std::string&, unsigned int, float*);

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
	m2ShaderProgram& setVec4(const std::string&, unsigned int, float*);
	m2ShaderProgram& setVec4(const std::string&, unsigned int, glm::vec4*);

	m2ShaderProgram& setMat3(const std::string&, float[9]);
	m2ShaderProgram& setMat3(const std::string&, const glm::mat3&);
	m2ShaderProgram& setMat3(const std::string&, unsigned int, float*);
	m2ShaderProgram& setMat3(const std::string&, unsigned int, glm::mat3*);

	m2ShaderProgram& setMat4(const std::string&, float*);
	m2ShaderProgram& setMat4(const std::string&, const glm::mat4&);
	m2ShaderProgram& setMat4(const std::string&, unsigned int, float*);
	m2ShaderProgram& setMat4(const std::string&, unsigned int, glm::mat4*);

	//GLint getAttribLocation(std::string);
	//glBindAttribLocation or glBindFragDataLocation specifies input/output.
	//It might be worth while to do this for each shader so that we're guarenteed we're reading/writing what we expec to.
	//ie ensure we're not pointing to normals when we think we're pointing to uvs.

private:
	//Cache uniform locations.
	std::unordered_map<std::string, GLuint> m_uniforms;
	//Must store shaders in order to detatch them. More efficient than using glGetProgramiv to retrieve shader objects.
	std::vector<m2Shader> m_shaders;
	GLuint m_programHandle;

	inline GLint getUniformHandle(const std::string&);
};

//I overload functions rather than pass default parameters for sending 1 element vs multiple elements because it is faster to call 1f when possible rather than 1fv
//when possible. Moreover, only floats are sent. unsigned ints, doubles, and weird matrices (m * n where m != n and non-float matrices) aren't included because supporting
//those additional types seems unnecessary. *Still not a good idea to template cause gl needs to differentiate between float and double. At the same time, we could hack
//together something that makse a gl call based on the memory size of the data to send rather than the actual type. But like most hacks, we run into errors like treating
//a as b ie floats as ints when we shouldn't be. Thus, of all the areas to get stupid, this isn't one of them (just like loading images or making a math library).

//As for memory, shaders are like shared pointers. They won't be deleted until they are no longer attached to a program object. Thus, either delete the program or
//detatch the shaders. Can detatch/delete (detach in my case cause we want to share our shaders) after [successful] link.

//For reference, deleteShader() queues shader for deletion, won't delete if attached to a program. deleteProgram() won't delete if bound. Detatches (but doesn't
//delete) shaders linked to the program.
//Basically, have programs delete shaders if you don't wanna explicitly control the lifetimes of shaders. However, be mindful if having programs dictate shader
//lifetimes when sharing shaders. The best thing to do in both cases is put all your shaders and programs in the same scope. Contruct all shaders before any programs.