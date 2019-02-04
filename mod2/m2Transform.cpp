#include "m2Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <cstdio>

glm::mat4 m2Transform::s_mIdentity = glm::mat4(1.0f);
glm::vec4 m2Transform::s_vIdentity = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
glm::vec3 m2Transform::s_up = glm::vec3(0.0f, 1.0f, 0.0f);

m2Transform::m2Transform() : m_localTransformation(s_mIdentity), m_parent(nullptr)
{
	s_mIdentity * s_mIdentity;
}

m2Transform::~m2Transform()
{
}

glm::mat4 m2Transform::getWorldTransformation()
{
	if (m_parent)
		return m_parent->getWorldTransformation() * m_localTransformation;
	return m_localTransformation;
}

glm::vec3 m2Transform::getWorldPosition()
{	//Chances are no math is actually being done with the position ie not gonna multiply by an external matrix, probably just gonna compare.
	//Although technically incorrect, its not that hard to 
	return getWorldTransformation() * s_vIdentity;
}

glm::vec3 m2Transform::getWorldTranslation()
{
	return getWorldTransformation()[3];
}

glm::vec3 m2Transform::getWorldRotation()
{
	glm::vec3 result;
	glm::mat4 worldTransformation = getWorldTransformation();
	glm::extractEulerAngleXYZ(worldTransformation, result.x, result.y, result.z);
	return glm::degrees(result);
}

const glm::mat4 & m2Transform::getLocalTransformation()
{
	return m_localTransformation;
}

glm::vec3 m2Transform::getLocalPosition()
{
	return m_localTransformation * s_vIdentity;
}

glm::vec3 m2Transform::getLocalTranslation()
{
	return m_localTransformation[3];
}

//If this doesn't account for scale, this has the potential to have significantly even worse performance.
glm::vec3 m2Transform::getLocalRotation()
{	//Pretty sure this doesn't account for scale. If it doesn't, we'll have to form a new matrix with a scale of 1.
	glm::vec3 result;
	glm::extractEulerAngleXYZ(m_localTransformation, result.x, result.y, result.z);
	return glm::degrees(result);
}

float m2Transform::getLocalRotationX()
{	//X component of extractEulerAnglesXYZ().
	return glm::degrees(-glm::atan2<float, glm::defaultp>(m_localTransformation[2][1], m_localTransformation[2][2]));
}

float m2Transform::getLocalRotationY()
{	//Y component of extractEulerAnglesXYZ(). Returns the negated angle 
	float magnitude = glm::sqrt(m_localTransformation[0][0] * m_localTransformation[0][0] + m_localTransformation[1][0] * m_localTransformation[1][0]);
	return glm::degrees(-glm::atan2<float, glm::defaultp>(-m_localTransformation[2][0], magnitude));
}

float m2Transform::getLocalRotationZ()
{	//Z component of extractEulerAnglesXYZ().
	float xRotation = getLocalRotationX();
	float xSin = sin(xRotation);
	float xCos = cos(xRotation);
	return glm::degrees(-glm::atan2<float, glm::defaultp>(xSin * m_localTransformation[0][2] - xCos * m_localTransformation[0][1], xCos * m_localTransformation[1][1] - xSin * m_localTransformation[1][2]));
}

glm::vec3 m2Transform::getScale()
{
	return glm::vec3(getScaleX(), getScaleY(), getScaleZ());
}

float m2Transform::getScaleX()
{
	return m_localTransformation[0][0] / _extractRotation00(m_localTransformation);
}

float m2Transform::getScaleY()
{
	return m_localTransformation[1][1] / _extractRotation11(m_localTransformation);
}

float m2Transform::getScaleZ()
{
	return m_localTransformation[2][2] / _extractRotation22(m_localTransformation);
}

glm::vec3 m2Transform::getFront()
{
	glm::vec3 front = m_localTransformation[2];
	front.z /= getScaleZ();
	return front;
}

glm::vec3 m2Transform::getRight()
{
	glm::vec3 right = m_localTransformation[0];
	right.x /= getScaleX();
	return right;
}

glm::vec3 m2Transform::getAbove()
{
	glm::vec3 above = m_localTransformation[1];
	above.y /= getScaleY();
	return above;
}

void m2Transform::setFront(glm::vec3 front)
{
	glm::vec3 right(glm::cross(s_up, front));
	glm::vec3 above(glm::cross(front, right));
	_setDirections(front, right, above);
}

void m2Transform::setRight(glm::vec3 right)
{
	glm::vec3 front(glm::cross(right, s_up));
	glm::vec3 above(glm::cross(front, right));
	_setDirections(front, right, above);
}

void m2Transform::setAbove(glm::vec3 above)
{
	glm::vec3 right(glm::cross(s_up, above));
	glm::vec3 front(glm::cross(right, above));
	_setDirections(front, right, above);
}

void m2Transform::setTranslation(glm::vec3 translation)
{
	m_localTransformation[3].xyz = translation;
}

void m2Transform::setTranslation(float x, float y, float z)
{
	m_localTransformation[3].x = x;
	m_localTransformation[3].y = y;
	m_localTransformation[3].z = z;
}

void m2Transform::setRotation(glm::vec3 rotation)
{
	glm::vec3 translation = getLocalTranslation();
	rotation = glm::radians(rotation);
	glm::vec3 scale = getScale();	//Sqrt x3 due to per-column extract rotation.
	//Might have to add extra logic to preserve the orientation ie get the difference between current orientation, and desired orientation, then
	//apply that to the current orientation.
	m_localTransformation = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
	_scaleUnsafe(scale);			//Safe in this case because there is currently a uniform scale of 1.
	setTranslation(translation);
}

void m2Transform::setRotation(float x, float y, float z)
{
	setRotation(glm::vec3(x, y, z));
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

void m2Transform::setDeltaTranslation(glm::vec3 translation)
{
	m_localTransformation[3].xyz += translation;
}

void m2Transform::setDeltaTranslation(float x, float y, float z)
{
	m_localTransformation[3].x += x;
	m_localTransformation[3].y += y;
	m_localTransformation[3].z += z;
}

void m2Transform::setDeltaRotation(glm::vec3 rotation)
{
	rotation = glm::radians(rotation);
	m_localTransformation *= glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);;
}

void m2Transform::setDeltaRotation(float x, float y, float z)
{
	setDeltaRotation(glm::vec3(x, y, z));
}

void m2Transform::setDeltaRotationX(float x)
{
	m_localTransformation *= glm::eulerAngleX(glm::radians(x));
}

void m2Transform::setDeltaRotationY(float y)
{
	m_localTransformation *= glm::eulerAngleY(glm::radians(y));
}

void m2Transform::setDeltaRotationZ(float z)
{
	m_localTransformation *= glm::eulerAngleZ(glm::radians(z));
}

void m2Transform::setScale(glm::vec3 scale)
{
	_scaleSafe(scale);
}

void m2Transform::setScale(float scale)
{
	_scaleSafe(glm::vec3(scale));
}

void m2Transform::setScaleX(float sx)
{
	m_localTransformation[0][0] = sx * _extractRotation00(m_localTransformation);
}

void m2Transform::setScaleY(float sy)
{
	m_localTransformation[1][1] = sy * _extractRotation11(m_localTransformation);
}

void m2Transform::setScaleZ(float sz)
{
	m_localTransformation[2][2] = sz * _extractRotation22(m_localTransformation);
}

inline void m2Transform::_scaleSafe(glm::vec3 scale)
{
	glm::vec3 rotation = _extractRotations(m_localTransformation);
	m_localTransformation[0][0] = scale.x * rotation.x;
	m_localTransformation[1][1] = scale.y * rotation.y;
	m_localTransformation[2][2] = scale.z * rotation.z;
}

inline void m2Transform::_scaleUnsafe(glm::vec3 scale)
{
	m_localTransformation[0][0] *= scale.x;
	m_localTransformation[1][1] *= scale.y;
	m_localTransformation[2][2] *= scale.z;
}

inline glm::vec3 m2Transform::_extractRotations(const glm::mat4& matrix)
{
	return glm::vec3(_extractRotation00(matrix), _extractRotation11(matrix), _extractRotation22(matrix));
}

inline float m2Transform::_extractRotation00(const glm::mat4& matrix)
{
	return 1.0f - glm::sqrt(matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2]);
}

inline float m2Transform::_extractRotation11(const glm::mat4& matrix)
{
	return 1.0f - glm::sqrt(matrix[1][0] * matrix[1][0] + matrix[1][2] * matrix[1][2]);
}

inline float m2Transform::_extractRotation22(const glm::mat4& matrix)
{
	return 1.0f - glm::sqrt(matrix[2][0] * matrix[2][0] + matrix[2][1] * matrix[2][1]);
}

inline void m2Transform::_setDirections(glm::vec3 front, glm::vec3 right, glm::vec3 up)
{
	glm::vec3 scale = getScale();
	m_localTransformation[0].xyz = right;
	m_localTransformation[1].xyz = up;
	m_localTransformation[2].xyz = front;
	_scaleUnsafe(scale);//Safe in this case because there is currently a uniform scale of 1.
}