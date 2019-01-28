#pragma once
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>

class m2Transform
{
public:
	m2Transform();
	~m2Transform();

	const glm::mat4 getWorldTransformation();
	const glm::vec3 getWorldPosition();
	const glm::vec3 getWorldTranslation();
	const glm::vec3 getWorldRotation();
	
	const glm::mat4& getLocalTransformation();
	const glm::vec3 getLocalPosition();
	const glm::vec3 getLocalTranslation();
	const glm::vec3 getLocalRotation();
	float getLocalRotationX();
	float getLocalRotationY();
	float getLocalRotationZ();

	//{ length(mat[0]), length(mat[1]), length(mat[2]) }
	glm::vec3 getScale();
	//length(mat[0])
	float getScaleX();
	//length(mat[1])
	float getScaleY();
	//length(mat[2])
	float getScaleZ();

	//mat[2] / scale_Z
	const glm::vec3 getFront();
	//mat[0] / scale_X
	const glm::vec3 getRight();
	//mat[1] / scale_Y
	const glm::vec3 getAbove();

	//Right and above will correct automatically.
	void setFront(const glm::vec3);
	//Front and above will correct automatically.
	void setRight(const glm::vec3);
	//Front and right will correct automatically.
	void setAbove(const glm::vec3);

	void setTranslation(const glm::vec3);
	void setRotation(const glm::vec3);
	void setRotationX(float);
	void setRotationY(float);
	void setRotationZ(float);

	void setDeltaTranslation(const glm::vec3);
	void setDeltaRotation(const glm::vec3);
	void setDeltaRotationX(float);
	void setDeltaRotationY(float);
	void setDeltaRotationZ(float);

	void setScale(glm::vec3);
	void setScale(float);
	void setScaleX(float);
	void setScaleY(float);
	void setScaleZ(float);

private:
	glm::mat4 m_localTransformation;
	m2Transform* m_parent;

	static glm::mat4 s_mIdentity;
	static glm::vec4 s_vIdentity;
	static glm::vec3 s_up;

	inline void _scale(glm::vec3);
	inline void setDirections(glm::vec3 front, glm::vec3 right, glm::vec3 up);
};

