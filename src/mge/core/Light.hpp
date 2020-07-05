#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"

/**
 * Exercise for the student: implement the Light class...
 * Imagine things like setLightType, setLightIntensity, setFalloffAngle etc, see the corresponding lectures
 */
enum LightType { DIRECTIONAL, POINT };

class Light : public GameObject
{
public:
	Light(LightType pLightType, glm::vec3 pLightColor, glm::vec3 pLightDiffuseColor, glm::vec3 pLightAmbientColor, const std::string& pName = "light", const glm::vec3& pPosition = glm::vec3(2.0f, 10.0f, 5.0f), float pIntensity = 0.1f);
	virtual ~Light();

	//override set parent to register/deregister light...
	virtual void _setWorldRecursively(World* pWorld) override;

	void setLightType(LightType pLightType);

	LightType getLightType();

	float getInstensity();

	void setupColorProperties(const glm::vec3& pDiffuseColor, const glm::vec3& pAmbientColor, const glm::vec3& pSpecularColor);

	void setupPointLightProperties(float pLinear, float pQuadratic);

	void setSpecularColor(const glm::vec3& pSpecularColor); 

	glm::vec3 getLightColor();

	glm::vec3 getLightDiffuseColor();

	glm::vec3 getLightAmbientColor();

	glm::vec3 getLightSpecularColor();

	float getLightConstant(); 
	
	float getLightLinear();
	
	float getLightQuadratic();



private:

	void setDefaultPointLightProperytValues(); 
	void setDefaultColorProperties();


	LightType _lightType;
	glm::vec3 _lightColor;
	float _intensity;
	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular; 
	const float _constant = 1.0f; 
	float _linear; 
	float _quadratic; 

};

#endif // LIGHT_HPP