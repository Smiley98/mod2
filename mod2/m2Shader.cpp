#include "m2Shader.h"
#include "m2Utilities.h"

//Again, can't have static members that require gl to be initialized.
//m2ShaderProgram m2ShaderProgram::s_programs[Shaders::NUM_SHADERS];
m2ShaderProgram* m2ShaderProgram::s_programs = nullptr;

m2Shader::m2Shader(m2Shaders ShaderType, const std::string& path)
{	//Create shader.
	m_shaderHandle = glCreateShader(ShaderType);
	//Store shader source as an l-value c-string (gl needs a string double pointer).
	std::string source = m2Utils::loadTextFile(path);
	const GLchar* const sourceCstr = source.c_str();

	//Add source to m2Shader.
	glShaderSource(m_shaderHandle, 1, &sourceCstr, nullptr);

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
		fprintf(stderr, "Shader compilation error: %s\n", errorLog.data());
	}
}

m2Shader::~m2Shader()
{
	glDeleteShader(m_shaderHandle);
}

m2ShaderProgram::m2ShaderProgram()
{
	m_programHandle = glCreateProgram();
}

m2ShaderProgram::~m2ShaderProgram()
{
	glDeleteProgram(m_programHandle);
}

m2ShaderProgram& m2ShaderProgram::add(const m2Shader& shader) {
	glAttachShader(m_programHandle, shader.m_shaderHandle);
	m_shaders.push_back(shader);
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
		glDeleteProgram(m_programHandle);
		std::printf("Shader linker error: %s\n", errorLog.data());
	}
	//Can release shader objects (shader pointers) after [successful] link.
	for (m2Shader& m2Shader : m_shaders)
		glDetachShader(m_programHandle, m2Shader.m_shaderHandle);
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

m2ShaderProgram& m2ShaderProgram::setInt(const std::string& index, unsigned int amount, int* values)
{
	glUniform1iv(getUniformHandle(index), amount, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setFloat(const std::string& index, float value) {
	glUniform1f(getUniformHandle(index), value);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setFloat(const std::string& index, unsigned int amount, float* values)
{
	glUniform1fv(getUniformHandle(index), amount, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec2(const std::string& index, float values[2]) {
	glUniform2f(getUniformHandle(index), values[0], values[1]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec2(const std::string& index, const glm::vec2& values)
{
	glUniform2f(getUniformHandle(index), values[0], values[1]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec2(const std::string& index, unsigned int amount, float values[2])
{
	glUniform2fv(getUniformHandle(index), amount, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec2(const std::string& index, unsigned int amount, const glm::vec2& values)
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

m2ShaderProgram& m2ShaderProgram::setVec3(const std::string& index, unsigned int amount, const glm::vec3& values)
{
	glUniform3fv(getUniformHandle(index), amount, &values[0]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec4(const std::string& index, float values[4]) {
	glUniform4f(getUniformHandle(index), values[0], values[1], values[2], values[3]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec4(const std::string& index, const glm::vec4& values)
{
	glUniform4f(getUniformHandle(index), values[0], values[1], values[2], values[3]);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec4(const std::string& index, unsigned int amount, float* values)
{
	glUniform4fv(getUniformHandle(index), amount, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setVec4(const std::string& index, unsigned int amount, glm::vec4* values)
{
	glUniform4fv(getUniformHandle(index), amount, &values[0].x);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat3(const std::string& index, float values[9])
{
	glUniformMatrix3fv(getUniformHandle(index), 1, GL_FALSE, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat3(const std::string& index, const glm::mat3& values)
{
	glUniformMatrix3fv(getUniformHandle(index), 1, GL_FALSE, &values[0].x);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat3(const std::string& index, unsigned int amount, float* values)
{
	glUniformMatrix3fv(getUniformHandle(index), amount, GL_FALSE, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat3(const std::string& index, unsigned int amount, glm::mat3* values)
{
	glUniformMatrix3fv(getUniformHandle(index), amount, GL_FALSE, &values[0][0].x);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat4(const std::string& index, float* values) {
	glUniformMatrix4fv(getUniformHandle(index), 1, GL_FALSE, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat4(const std::string& index, const glm::mat4& values)
{
	glUniformMatrix4fv(getUniformHandle(index), 1, GL_FALSE, &values[0].x);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat4(const std::string& index, unsigned int amount, float *values)
{
	glUniformMatrix4fv(getUniformHandle(index), amount, GL_FALSE, values);
	return *this;
}

m2ShaderProgram& m2ShaderProgram::setMat4(const std::string& index, unsigned int amount, glm::mat4* values)
{
	glUniformMatrix4fv(getUniformHandle(index), amount, GL_FALSE, &values[0][0].x);
	return *this;
}

void m2ShaderProgram::init()
{
	s_programs = new m2ShaderProgram[NUM_SHADERS];
	std::string sdir = "Shaders/";
//Vertex Shaders:
	m2Shader v_passThrough(VERTEX, sdir + "PassThrough.vert");
	m2Shader v_ray(VERTEX, sdir + "Ray.vert");
	m2Shader v_screenQuad(VERTEX, sdir + "ScreenQuad.vert");

//Geometry Shaders:
	m2Shader g_line(GEOMETRY, sdir + "Line.geom");
	m2Shader g_ray(GEOMETRY, sdir + "Ray.geom");

//Fragment Shaders:
	m2Shader f_uniformColour(FRAGMENT, sdir + "UniformColour.frag");
	m2Shader f_ray(FRAGMENT, sdir + "Ray.frag");
	m2Shader f_randomColour(FRAGMENT, sdir + "RandomColour.frag");
	m2Shader f_screenQuadTest(FRAGMENT, sdir + "ScreenQuadTest.frag");

//Programs:
	s_programs[LINE].add(v_passThrough);
	s_programs[LINE].add(g_line);
	s_programs[LINE].add(f_uniformColour);
	s_programs[LINE].link();

	s_programs[RAY].add(v_ray);
	s_programs[RAY].add(g_ray);
	s_programs[RAY].add(f_ray);
	//s_programs[RAY].add(f_randomColour);
	s_programs[RAY].link();

	s_programs[QUAD_TEST].add(v_screenQuad);
	s_programs[QUAD_TEST].add(f_screenQuadTest);
	s_programs[QUAD_TEST].link();
}

void m2ShaderProgram::shutdown()
{
	delete[] s_programs;
}

m2ShaderProgram& m2ShaderProgram::getProgram(m2ShaderPrograms shader)
{
	return s_programs[shader];
}

void m2ShaderProgram::drawLine()
{	//Must bind a random vao cause you're not allowed to render if the null vao is bound.
	static GLuint vao;
	static bool generated = false;
	if (!generated) {
		glGenVertexArrays(1, &vao);
		generated = true;
	}

	m2ShaderProgram& sp = getProgram(LINE);
	sp.bind();
	sp.setVec3("u_colour", glm::vec3(1.0f));

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, 1);
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
