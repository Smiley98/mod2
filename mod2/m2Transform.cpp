#include "m2Transform.h"
#include <glm/gtc/matrix_transform.hpp>

#include "m2Utilities.h"
#include <cstdio>

const glm::mat4 m2Transform::s_mIdentity = glm::mat4(1.0f);
const glm::vec4 m2Transform::s_vIdentity = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec3 m2Transform::s_up = glm::vec3(0.0f, 1.0f, 0.0f);

m2Transform::m2Transform() : m_transformation(s_mIdentity), m_orientation(glm::quat()), m_scale(glm::vec3(1.0f)), m_parent(nullptr)
{
}

m2Transform::~m2Transform()
{
}

glm::mat4 m2Transform::getWorldTransformation()
{	//Recursively combine transformations.
	if (m_parent)
		return m_parent->getWorldTransformation() * m_transformation;
	return m_transformation;
}

glm::vec3 m2Transform::getWorldPosition()
{
	return getWorldTransformation() * s_vIdentity;
}

glm::vec3 m2Transform::getWorldTranslation()
{	//Recursively combine translations.
	if (m_parent)
		return m_parent->getWorldTranslation() + m_transformation[3].xyz;
	return m_transformation[3];
}

glm::vec3 m2Transform::getWorldRotation()
{	//Recursively combine orientations.
	return glm::degrees(glm::eulerAngles(_getWorldOrientation()));
}

const glm::mat4 & m2Transform::getLocalTransformation()
{
	return m_transformation;
}

glm::vec3 m2Transform::getLocalPosition()
{	//Add update check here for most up to date info and to remove dirty flag from [0][3].
	return m_transformation * s_vIdentity;
}

glm::vec3 m2Transform::getLocalTranslation()
{
	return m_transformation[3];
}

glm::vec3 m2Transform::getLocalRotation()
{
	return glm::degrees(glm::eulerAngles(m_orientation));
}

float m2Transform::getLocalRotationX()
{
	return glm::degrees(glm::pitch(m_orientation));
}

float m2Transform::getLocalRotationY()
{
	return glm::degrees(glm::yaw(m_orientation));
}

float m2Transform::getLocalRotationZ()
{
	return glm::degrees(glm::roll(m_orientation));
}

glm::vec3 m2Transform::getScale()
{
	return m_scale;
}

float m2Transform::getScaleX()
{
	return m_scale.x;
}

float m2Transform::getScaleY()
{
	return m_scale.y;
}

float m2Transform::getScaleZ()
{
	return m_scale.z;
}

glm::mat3 m2Transform::getDirections()
{
	return glm::mat3_cast(m_orientation);
}

glm::vec3 m2Transform::getFront()
{	//See glm::mat3_cast(q);
	const glm::quat& q = m_orientation;
	return glm::vec3(2.0f * (q.x * q.z + q.w * q.y), 2.0f * (q.y * q.z  - q.w * q.x), 1.0f - 2.0f * (q.x * q.x + q.y * q.y));
}

glm::vec3 m2Transform::getRight()
{	//See glm::mat3_cast(q);
	const glm::quat& q = m_orientation;
	return glm::vec3(1.0f - 2.0f * (q.y * q.y + q.z * q.z), 2.0f * (q.x * q.y + q.w * q.z), 2.0f * (q.x * q.z - q.w * q.y));
}

glm::vec3 m2Transform::getAbove()
{	//See glm::mat3_cast(q);
	const glm::quat& q = m_orientation;
	return glm::vec3(2.0f * (q.x * q.y - q.w * q.z), 1.0f - 2.0f * (q.x * q.x + q.z * q.z), 2.0f * (q.y * q.z + q.x * q.x));
}

void m2Transform::setFront(glm::vec3 front)
{
	glm::vec3 right(glm::cross(s_up, front));
	glm::vec3 above(glm::cross(front, right));
	_setDirections(front, right, above);//Converts to radians.
}

void m2Transform::setRight(glm::vec3 right)
{
	glm::vec3 front(glm::cross(right, s_up));
	glm::vec3 above(glm::cross(front, right));
	_setDirections(front, right, above);//Converts to radians.
}

void m2Transform::setAbove(glm::vec3 above)
{
	glm::vec3 right(glm::cross(s_up, above));
	glm::vec3 front(glm::cross(right, above));
	_setDirections(front, right, above);//Converts to radians.
}

void m2Transform::setTranslation(glm::vec3 translation)
{
	m_transformation[3].xyz = translation;
}

void m2Transform::setTranslation(float x, float y, float z)
{
	m_transformation[3].x = x;
	m_transformation[3].y = y;
	m_transformation[3].z = z;
}

void m2Transform::setRotation(glm::vec3 rotation)
{	//Might want to compute deltas rather than a straight-up assignment. Internal rotation order + gl coordinate system + model orientation should suffice though.
	m_orientation = glm::quat(glm::radians(rotation));
}

void m2Transform::setRotation(float x, float y, float z)
{	//No function tunneling this time!!
	m_orientation = glm::quat(glm::radians(glm::vec3(x, y, z)));
}

void m2Transform::setRotationX(float x)
{
	glm::vec3 angles(glm::eulerAngles(m_orientation));
	angles.x = glm::radians(x);
	m_orientation = glm::quat(angles);
}

void m2Transform::setRotationY(float y)
{
	glm::vec3 angles(glm::eulerAngles(m_orientation));
	angles.y = glm::radians(y);
	m_orientation = glm::quat(angles);
}

void m2Transform::setRotationZ(float z)
{
	glm::vec3 angles(glm::eulerAngles(m_orientation));
	angles.z = glm::radians(z);
	m_orientation = glm::quat(angles);
}

void m2Transform::setDeltaTranslation(glm::vec3 translation)
{
	m_transformation[3].xyz += translation;
}

void m2Transform::setDeltaTranslation(float x, float y, float z)
{
	m_transformation[3].x += x;
	m_transformation[3].y += y;
	m_transformation[3].z += z;
}

void m2Transform::setDeltaRotation(glm::vec3 rotation)
{
	m_orientation = glm::quat(glm::radians(rotation)) * m_orientation;
}

void m2Transform::setDeltaRotation(float x, float y, float z)
{
	m_orientation = glm::quat(glm::radians(glm::vec3(x, y, z))) * m_orientation;
}

void m2Transform::setDeltaRotationX(float x)
{
	//m_orientation = glm::quat(glm::radians(glm::vec3(x, 0.0f, 0.0f))) * m_orientation;
}

void m2Transform::setDeltaRotationY(float y)
{
	//m_orientation = glm::quat(glm::radians(glm::vec3(0.0f, y, 0.0f))) * m_orientation;
}

void m2Transform::setDeltaRotationZ(float z)
{
	//m_orientation = glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, z))) * m_orientation;
}

void m2Transform::setScale(glm::vec3 scale)
{
	m_scale = scale;
}

void m2Transform::setScale(float scale)
{
	m_scale = glm::vec3(scale);
}

void m2Transform::setScaleX(float x)
{
	m_scale.x = x;
}

void m2Transform::setScaleY(float y)
{
	m_scale.y = y;
}

void m2Transform::setScaleZ(float z)
{
	m_scale.z = z;
}

inline glm::quat m2Transform::_getWorldOrientation()
{	//Order is root * A * B * C * etc. Despite being called from a potential leaf node, the multiplications doesn't start till root.
	if (m_parent)
		return m_parent->_getWorldOrientation() * m_orientation;
	return m_orientation;
}

inline void m2Transform::_setDirections(glm::vec3 front, glm::vec3 right, glm::vec3 above)
{	//Make a rotation matrix, then turn it into a quaternion. The matrix construction overhead is worth what glm is doing cause quat_cast() is difficult!
	m_orientation = glm::quat_cast(glm::mat3(glm::radians(right), glm::radians(above), glm::radians(front)));
}

inline const m2Transform & m2Transform::getParent()
{
	
}

inline void m2Transform::setParent(const m2Transform &)
{
}
