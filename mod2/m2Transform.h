#pragma once
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

class m2Transform
{
public:
	m2Transform();
	~m2Transform();
	
	glm::mat4 getWorldTransformation();
	glm::vec3 getWorldPosition();
	glm::vec3 getWorldTranslation();
	glm::vec3 getWorldRotation();
	
	const glm::mat4& getLocalTransformation();
	glm::vec3 getLocalPosition();
	glm::vec3 getLocalTranslation();
	glm::vec3 getLocalRotation();
	float getLocalRotationX();
	float getLocalRotationY();
	float getLocalRotationZ();

	glm::vec3 getScale();
	float getScaleX();
	float getScaleY();
	float getScaleZ();

	glm::mat3 getDirections();
	glm::vec3 getFront();
	glm::vec3 getRight();
	glm::vec3 getAbove();

	//Right and above will correct automatically.
	void setFront(glm::vec3);
	//Front and above will correct automatically.
	void setRight(glm::vec3);
	//Front and right will correct automatically.
	void setAbove(glm::vec3);

	void setTranslation(glm::vec3);
	void setRotation(glm::vec3);
	void setTranslation(float, float, float);
	void setRotation(float, float, float);
	void setRotationX(float);
	void setRotationY(float);
	void setRotationZ(float);

	void setDeltaTranslation(glm::vec3);
	void setDeltaRotation(glm::vec3);
	void setDeltaTranslation(float, float, float);
	void setDeltaRotation(float, float, float);
	void setDeltaRotationX(float);
	void setDeltaRotationY(float);
	void setDeltaRotationZ(float);

	void setScale(glm::vec3);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void setScaleZ(float);

private:
	glm::mat4 m_transformation;
	glm::quat m_orientation;
	glm::vec3 m_scale;
	m2Transform* m_parent;
	//Encode the pointer across xy and the dirty flag in z! 

	static const glm::mat4 s_mIdentity;
	static const glm::vec4 s_vIdentity;
	static const glm::vec3 s_up;

	inline glm::quat _getWorldOrientation();
	inline void _setDirections(glm::vec3 front, glm::vec3 right, glm::vec3 up);

	inline const m2Transform& getParent();
	inline void setParent(const m2Transform&);
};

