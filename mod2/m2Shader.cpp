#include "m2Shader.h"
#include "m2Utilities.h"

m2Shader::m2Shader(GLenum m2ShaderType, const std::string& path)
{	//Create m2Shader.
	m_shaderHandle = glCreateShader(m2ShaderType);
	//Store m2Shader source as an l-value c-string (gl needs a string double pointer).
	const char* const sourceCstr = m2Utils::loadTextFile(path).c_str();
	//Add source to m2Shader.
	glShaderSource(m_shaderHandle, 1, &reinterpret_cast<const GLchar*const>(sourceCstr), 0);

	//Compile m2Shader, log error if compilation error.
	glCompileShader(m_shaderHandle);
	GLint isCompiled = GL_FALSE;
	glGetShaderiv(m_shaderHandle, GL_COMPILE_STATUS, &isCompiled);
	if (!isCompiled) {
		GLint maxLength = 0;
		glGetShaderiv(m_shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(m_shaderHandle, maxLength, &maxLength, errorLog.data());
		glDeleteShader(m_shaderHandle);
		fprintf(stderr, "m2Shader compilation error: %s\n", errorLog.data());
	}
}

m2Shader::~m2Shader()
{
}

m2ShaderProgram::m2ShaderProgram()
{
	m_programHandle = glCreateProgram();
}

m2ShaderProgram::~m2ShaderProgram()
{
	deleteProgram();
}

m2ShaderProgram& m2ShaderProgram::add(const m2Shader& m2Shader) {
	glAttachShader(m_programHandle, m2Shader.m_shaderHandle);
	m_shaders.push_back(m2Shader);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::link() {
	//Link m2Shader, log error if linker error.
	glLinkProgram(m_programHandle);
	GLint isLinked = GL_FALSE;
	glGetProgramiv(m_programHandle, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		GLint maxLength = 0;
		glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(m_programHandle, maxLength, &maxLength, &errorLog[0]);
		deleteProgram();
		std::printf("ERROR: %s\n", &(errorLog[0]));
	}
	return *this;
}

m2ShaderProgram& m2ShaderProgram::bind() {
	glUseProgram(m_programHandle);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::unbind() {
	glUseProgram(GL_NONE);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setInt(const std::string& index, int value) {
	glUniform1i(getUniformHandle(index), value);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setFloat(const std::string& index, float value) {
	glUniform1f(getUniformHandle(index), value);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec2(const std::string& index, float values[2]) {
	glUniform2f(getUniformHandle(index), values[0], values[1]);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setVec2(const std::string& index, const glm::vec2& values)
{
	glUniform2f(getUniformHandle(index), values[0], values[1]);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setVec2(const std::string& index, unsigned int amount, float values[2])
{
	glUniform2fv(getUniformHandle(index), amount, values);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setVec2(const std::string& index, unsigned int amount, const glm::vec2& values)
{
	glUniform2fv(getUniformHandle(index), amount, &values[0]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec3(const std::string& index, float values[3]) {
	glUniform3f(getUniformHandle(index), values[0], values[1], values[2]);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setVec3(const std::string& index, const glm::vec3& values)
{
	glUniform3f(getUniformHandle(index), values[0], values[1], values[2]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec3(const std::string& index, unsigned int amount, float values[3]) {
	glUniform3fv(getUniformHandle(index), amount, values);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setVec3(const std::string& index, unsigned int amount, const glm::vec3& values)
{
	glUniform3fv(getUniformHandle(index), amount, &values[0]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec4(const std::string& index, float values[4]) {
	glUniform4f(getUniformHandle(index), values[0], values[1], values[2], values[3]);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setVec4(const std::string& index, const glm::vec4& values)
{
	glUniform4f(getUniformHandle(index), values[0], values[1], values[2], values[3]);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setVec4(const std::string& index, unsigned int amount, float values[4])
{
	glUniform4fv(getUniformHandle(index), amount, &values[0]);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setVec4(const std::string& index, unsigned int amount, const glm::vec4& values)
{
	glUniform4fv(getUniformHandle(index), amount, &values[0]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat3(const std::string& index, float values[9]) {
	glUniform1fv(getUniformHandle(index), 9, values);
	return *this;
}

m2ShaderProgram & m2ShaderProgram::setMat3(const std::string& index, const glm::mat3& values)
{
	const float* memory = &values[0].x;
	for (int i = 1; i < 10; i++) {
		printf("%f ", memory[i-1]);
		if (i % 3 == 0)
			printf("\n");
	}
	printf("Columnar: \n");
	printf("%f %f %f ", memory[0], memory[3], memory[6]);
	printf("%f %f %f ", memory[1], memory[4], memory[7]);
	printf("%f %f %f ", memory[2], memory[5], memory[8]);
	printf("\n");
	glUniform1fv(getUniformHandle(index), 9, &values[0].x);
}

m2ShaderProgram& m2ShaderProgram::setMat4(const std::string& index, float* values) {
	glUniformMatrix4fv(getUniformHandle(index), 1, GL_FALSE, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat4(const std::string& index, unsigned int amount, float *values) {
	glUniformMatrix4fv(getUniformHandle(index), amount, GL_FALSE, values);
	return *this;
}

/*GLint m2ShaderProgram::getAttribLocation(std::string attribName) {
	if (m_attributes.find(attribName) == m_attributes.end())
		m_attributes[attribName] = glGetAttribLocation(m_programHandle, attribName.c_str());
	return m_attributes[attribName];
}*/

GLint m2ShaderProgram::getUniformHandle(const std::string& uniformName) {
	if (m_uniforms.find(uniformName) == m_uniforms.end())
		m_uniforms[uniformName] = glGetUniformLocation(m_programHandle, uniformName.c_str());
	return m_uniforms[uniformName];
}

inline void m2ShaderProgram::deleteProgram() {
	for (m2Shader& m2Shader : m_shaders)
		glDetachShader(m_programHandle, m2Shader.m_shaderHandle);
	glDeleteProgram(m_programHandle);
}
