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

	//mat[2][2] / scale_Z
	glm::vec3 getFront();
	//mat[0][0] / scale_X
	glm::vec3 getRight();
	//mat[1][1] / scale_Y
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
	glm::mat4 m_localTransformation;
	m2Transform* m_parent;

	static glm::mat4 s_mIdentity;
	static glm::vec4 s_vIdentity;
	static glm::vec3 s_up;

	//Extracts rotation (overhead of 3 square roots), then multiplies by desired scale. Necessary cause we don't want to "delta-scale".
	inline void _scaleSafe(glm::vec3);
	//Applies the desired scale. No square root overhead, but there must be a uniform scale must be 1 for this to work correctly.
	inline void _scaleUnsafe(glm::vec3);

	inline glm::vec3 _extractRotations(const glm::mat4&);
	//inline glm::vec3 _removeRotations(glm::mat4&);
	//inline glm::vec3 _removeScale(glm::mat4&);
	inline float _extractRotation00(const glm::mat4&);
	inline float _extractRotation11(const glm::mat4&);
	inline float _extractRotation22(const glm::mat4&);
	inline void _setDirections(glm::vec3 front, glm::vec3 right, glm::vec3 up);

	inline const m2Transform& getParent();
	inline void setParent(const m2Transform&);
};

