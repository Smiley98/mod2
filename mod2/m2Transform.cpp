#include "m2Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cstdio>

glm::mat4 m2Transform::s_mIdentity = glm::mat4(1.0f);
glm::vec4 m2Transform::s_vIdentity = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec3 m2Transform::s_up = glm::vec3(0.0f, 1.0f, 0.0f);

m2Transform::m2Transform() : m_localTransformation(s_mIdentity), m_parent(nullptr)
{
}

m2Transform::~m2Transform()
{
}

const glm::mat4 m2Transform::getWorldTransformation()
{
	if (m_parent)
		return m_parent->getWorldTransformation() * m_localTransformation;
	return m_localTransformation;
}

const glm::vec3 m2Transform::getWorldPosition()
{
	return getWorldTransformation() * s_vIdentity;
}

const glm::vec3 m2Transform::getWorldTranslation()
{
	return getWorldTransformation()[3];
}

const glm::vec3 m2Transform::getWorldRotation()
{
	glm::vec3 result;
	glm::mat4 worldTransformation = getWorldTransformation();
	//Takes a reference parameter so saving it ensures we're not taking in a null reference.
	glm::extractEulerAngleXYZ(worldTransformation, result.x, result.y, result.z);
	return result;
}

const glm::mat4 & m2Transform::getLocalTransformation()
{
	return m_localTransformation;
}

const glm::vec3 m2Transform::getLocalPosition()
{
	return m_localTransformation * s_vIdentity;
}

const glm::vec3 m2Transform::getLocalTranslation()
{
	return m_localTransformation[3];
}

const glm::vec3 m2Transform::getLocalRotation()
{	//Pretty sure this accounts for scale.
	glm::vec3 result;
	glm::extractEulerAngleXYZ(m_localTransformation, result.x, result.y, result.z);
	return result;
}

float m2Transform::getLocalRotationX()
{	//X component of extractEulerAnglesXYZ().
	return -glm::atan2<float, glm::defaultp>(m_localTransformation[2][1], m_localTransformation[2][2]);
}

float m2Transform::getLocalRotationY()
{	//Y component of extractEulerAnglesXYZ(). Returns the negated angle 
	float magnitude = glm::sqrt(m_localTransformation[0][0] * m_localTransformation[0][0] + m_localTransformation[1][0] * m_localTransformation[1][0]);
	return -glm::atan2<float, glm::defaultp>(-m_localTransformation[2][0], magnitude);
}

float m2Transform::getLocalRotationZ()
{	//Z component of extractEulerAnglesXYZ().
	float xRotation = getLocalRotationX();
	float xSin = sin(xRotation);
	float xCos = cos(xRotation);
	return -glm::atan2<float, glm::defaultp>(xSin * m_localTransformation[0][2] - xCos * m_localTransformation[0][1], xCos * m_localTransformation[1][1] - xSin * m_localTransformation[1][2]);
}

glm::vec3 m2Transform::getScale()
{
	//return glm::vec3(glm::length(m_localTransformation[0]), glm::length(m_localTransformation[1]), glm::length(m_localTransformation[2]));
	return glm::vec3(getScaleX(), getScaleY(), getScaleZ());
}

float m2Transform::getScaleX()
{
	return m_localTransformation[0][0] / _extractRotation00();
}

float m2Transform::getScaleY()
{
	return m_localTransformation[1][1] / _extractRotation11();
}

float m2Transform::getScaleZ()
{
	return m_localTransformation[2][2] / _extractRotation22();
}

const glm::vec3 m2Transform::getFront()
{
	return getWorldTransformation()[2] / getScaleZ();
}

const glm::vec3 m2Transform::getRight()
{
	return getWorldTransformation()[0] / getScaleX();
}

const glm::vec3 m2Transform::getAbove()
{
	return getWorldTransformation()[1] / getScaleY();
}

void m2Transform::setFront(const glm::vec3 front)
{
	glm::vec3 right(glm::cross(s_up, front));
	glm::vec3 above(glm::cross(front, right));
	setDirections(front, right, above);
}

void m2Transform::setRight(const glm::vec3 right)
{
	glm::vec3 front(glm::cross(right, s_up));
	glm::vec3 above(glm::cross(front, right));
	setDirections(front, right, above);
}

void m2Transform::setAbove(const glm::vec3 above)
{
	glm::vec3 right(glm::cross(s_up, above));
	glm::vec3 front(glm::cross(right, above));
	setDirections(front, right, above);
}

void m2Transform::setTranslation(const glm::vec3 translation)
{
	m_localTransformation[3].xyz = translation;
}

void m2Transform::setRotation(const glm::vec3 rotation)
{
	glm::vec3 translation = getLocalTranslation();
	glm::vec3 scale = getScale();
	m_localTransformation = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
	setScale(scale);
	setTranslation(translation);
}

void m2Transform::setRotationX(float x)
{
	glm::vec3 rotation = getLocalRotation();
	rotation.x = x;
	setRotation(rotation);
}

void m2Transform::setRotationY(float y)
{
	glm::vec3 rotation = getLocalRotation();
	rotation.y = y;
	setRotation(rotation);
}

void m2Transform::setRotationZ(float z)
{
	glm::vec3 rotation = getLocalRotation();
	rotation.z = z;
	setRotation(rotation);
}

void m2Transform::setDeltaTranslation(const glm::vec3 translation)
{
	m_localTransformation[3].xyz += translation;
}

void m2Transform::setDeltaRotation(const glm::vec3 rotation)
{
	glm::mat4 rotationMatrix = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
	m_localTransformation *= rotationMatrix;
}

void m2Transform::setDeltaRotationX(float x)
{
	m_localTransformation *= glm::eulerAngleX(x);
}

void m2Transform::setDeltaRotationY(float y)
{
	m_localTransformation *= glm::eulerAngleY(y);
}

void m2Transform::setDeltaRotationZ(float z)
{
	m_localTransformation *= glm::eulerAngleZ(z);
}

void m2Transform::setScale(glm::vec3 scale)
{
	_scale(scale);
}

void m2Transform::setScale(float scale)
{
	_scale(glm::vec3(scale));
}

void m2Transform::setScaleX(float sx)
{
	m_localTransformation[0][0] = sx * _extractRotation00();
}

void m2Transform::setScaleY(float sy)
{
	m_localTransformation[1][1] = sy * _extractRotation11();
}

void m2Transform::setScaleZ(float sz)
{
	m_localTransformation[2][2] = sz * _extractRotation22();
}

inline void m2Transform::_scale(glm::vec3 scale)
{
	glm::vec3 rotation = _extractRotations();
	m_localTransformation[0][0] = scale.x * rotation.x;
	m_localTransformation[1][1] = scale.y * rotation.y;
	m_localTransformation[2][2] = scale.z * rotation.z;
}

inline glm::vec3 m2Transform::_extractRotations()
{
	return glm::vec3(_extractRotation00(), _extractRotation11(), _extractRotation22());
}

inline float m2Transform::_extractRotation00()
{
	return 1.0f - glm::sqrt(m_localTransformation[0][1] * m_localTransformation[0][1] + m_localTransformation[0][2] * m_localTransformation[0][2]);
}

inline float m2Transform::_extractRotation11()
{
	return 1.0f - glm::sqrt(m_localTransformation[1][0] * m_localTransformation[1][0] + m_localTransformation[1][2] * m_localTransformation[1][2]);
}

inline float m2Transform::_extractRotation22()
{
	return 1.0f - glm::sqrt(m_localTransformation[2][0] * m_localTransformation[2][0] + m_localTransformation[2][1] * m_localTransformation[2][1]);
}

inline void m2Transform::setDirections(glm::vec3 front, glm::vec3 right, glm::vec3 up)
{
	glm::vec3 scale = getScale();
	m_localTransformation[0].xyz = right;
	m_localTransformation[1].xyz = up;
	m_localTransformation[2].xyz = front;
	_scale(scale);
}