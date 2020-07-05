#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/materials/ColorMaterial.hpp"

Light::Light(LightType pLightType, glm::vec3 pLightColor, glm::vec3 pLightDiffuseColor, glm::vec3 pLightAmbientColor, const std::string& pName, const glm::vec3& pPosition, float pIntensity) : GameObject(pName, pPosition)
, _lightType(pLightType), _intensity(pIntensity), _lightColor(pLightColor), _ambient(pLightAmbientColor), _diffuse(pLightDiffuseColor)
{
	//light object has the same color as the color it will transmit
	AbstractMaterial* lightMaterial = new ColorMaterial(pLightColor);
	setMaterial(lightMaterial);

	setDefaultColorProperties(); 
	setDefaultPointLightProperytValues();
}

Light::~Light() {
}

//Override setWorldRecursively to make sure we are registered
//to the world we are a part of.
void Light::_setWorldRecursively(World* pWorld) {

	//store the previous and new world before and after storing the given world
	World* previousWorld = _world;
	GameObject::_setWorldRecursively(pWorld);
	World* newWorld = _world;

	//check whether we need to register or unregister
	if (previousWorld != nullptr) previousWorld->unregisterLight(this);
	if (newWorld != nullptr) newWorld->registerLight(this);

}

void Light::setLightType(LightType pLightType)
{
	_lightType = pLightType;
}

LightType Light::getLightType()
{
	return _lightType;
}

float Light::getInstensity()
{
	return _intensity;
}

void Light::setupColorProperties(const glm::vec3& pDiffuseColor, const glm::vec3& pAmbientColor, const glm::vec3& pSpecularColor)
{
	_diffuse = pDiffuseColor; 
	_ambient = pAmbientColor; 
	_specular = pSpecularColor;
}

void Light::setupPointLightProperties(float pLinear, float pQuadratic)
{
	_linear = pLinear; 
	_quadratic = pQuadratic;
}

void Light::setSpecularColor(const glm::vec3& pSpecularColor)
{
	_specular = pSpecularColor;
}

glm::vec3 Light::getLightColor()
{
	return _lightColor;
}

glm::vec3 Light::getLightDiffuseColor()
{
	return _diffuse;
}

glm::vec3 Light::getLightAmbientColor()
{
	return _ambient;
}

glm::vec3 Light::getLightSpecularColor()
{
	return _specular;
}

float Light::getLightConstant()
{
	return _constant;
}

float Light::getLightLinear()
{
	return _linear;
}

float Light::getLightQuadratic()
{
	return _quadratic;
}

void Light::setDefaultPointLightProperytValues()
{
	_linear = 0.74f; 
	_quadratic = 1.800f;
}

void Light::setDefaultColorProperties()
{
	_ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	_diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	_specular = glm::vec3(0.0f, 1.0f, 0.5f);
}
