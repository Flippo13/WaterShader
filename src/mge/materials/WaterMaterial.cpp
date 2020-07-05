#include "glm.hpp"

#include "WaterMaterial.hpp"

#include "mge/materials/WaterFrameBuffer.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <random>

ShaderProgram* WaterMaterial::_shader = NULL; 

GLint WaterMaterial::_uMVPMatrix = 0;
GLint WaterMaterial::_uDiffuseTexture = 0; 
GLint WaterMaterial::_uReflectionTexture = 0;
GLint WaterMaterial::_uRefractionTexture = 0;
GLint WaterMaterial::_uRefractiveDepthTexture = 0;

GLint WaterMaterial::_uDuDvTexture = 0;
GLint WaterMaterial::_uNormalMap = 0;

GLint WaterMaterial::_aVertex = 0; 
GLint WaterMaterial::_aNormal = 0;
GLint WaterMaterial::_aUV = 0;

constexpr auto dudv_map = "dudvMap.jpg";
constexpr auto normalMap = "normalMap.jpg";
constexpr auto water = "water.jpg";


constexpr auto wave_speed = 0.3f; 

WaterMaterial::WaterMaterial(WaterFrameBuffer* pFrameBuffer) :_fbo(pFrameBuffer) {
	_lazyInitializeShader();
	_dudvTexture = Texture::load(config::MGE_TEXTURE_PATH + dudv_map);
	_normalMap = Texture::load(config::MGE_TEXTURE_PATH + normalMap);
	//_diffuseTexture = Texture::load(config::MGE_TEXTURE_PATH + water);
}


WaterMaterial::~WaterMaterial() {
	glDisable(GL_BLEND);
}

void WaterMaterial::_lazyInitializeShader()
{
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "water.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "water.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uReflectionTexture = _shader->getUniformLocation("reflectionTexture");
		_uRefractionTexture = _shader->getUniformLocation("refractionTexture");
		_uRefractiveDepthTexture = _shader->getUniformLocation("depthTexture"); 

		_uDuDvTexture = _shader->getUniformLocation("dudvMap");
		_uNormalMap = _shader->getUniformLocation("normalMap");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

void WaterMaterial::render(World * pWorld, Mesh * pMesh, const glm::mat4 & pModelMatrix, const glm::mat4 & pViewMatrix, const glm::mat4 & pProjectionMatrix, glm::vec4& pClipPlanePosition) 
{
	if (!_fbo) return;

	_shader->use();
	int numLights = pWorld->getLightCount();
	for (int i = 0; i < pWorld->getLightCount(); i++)
	{
		if (pWorld->getLightAt(i)->getLightType() == LightType::DIRECTIONAL)
		{
			Light* directLight = pWorld->getLightAt(i);
			glUniform3fv(_shader->getUniformLocation("directLight.direction"), 1, glm::value_ptr(directLight->getTransform()[2]));
			glUniform3fv(_shader->getUniformLocation("directLight.specular"), 1, glm::value_ptr(directLight->getLightSpecularColor()));
		}
		else {
			
			std::string indexS = std::to_string(i);
			Light* pointLight = pWorld->getLightAt(i);

			glUniform3fv(_shader->getUniformLocation("pointLights[" + indexS + "].position"), 1, glm::value_ptr(pointLight->getWorldPosition()));
			glUniform3fv(_shader->getUniformLocation("pointLights[" + indexS + "].specular"), 1, glm::value_ptr( pointLight->getLightSpecularColor()));
		
			glUniform1f(_shader->getUniformLocation("pointLights[" + indexS + "].constant"),  glm::float1( pointLight->getLightConstant()));
			glUniform1f(_shader->getUniformLocation("pointLights[" + indexS + "].linear"),  glm::float1(   pointLight->getLightLinear()));
			glUniform1f(_shader->getUniformLocation("pointLights[" + indexS + "].quadratic"), glm::float1(pointLight->getLightQuadratic()));
		}
	}
	glUniform1i(_shader->getUniformLocation("numLights"), glm::int1(numLights));
	//Light* currentLight = pWorld->getLightAt(0);

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _fbo->GetReflectionTexture());
	glUniform1i(_uReflectionTexture,0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _fbo->GetRefractionTexture());
	glUniform1i(_uRefractionTexture,1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _dudvTexture->getId());
	glUniform1i(_uDuDvTexture, 2);

	glActiveTexture(GL_TEXTURE3); 
	glBindTexture(GL_TEXTURE_2D, _normalMap->getId()); 
	glUniform1i(_uNormalMap, 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _fbo->GetRefractionDepthTexture());
	glUniform1i(_uRefractiveDepthTexture, 4);

	float waveFactor = wave_speed * std::clock() / 8000.0f;
	glUniform1f(_shader->getUniformLocation("waveFactor"), glm::float1(waveFactor));

	//glUniform3fv(_shader->getUniformLocation("lightPosition"),1, glm::value_ptr(currentLight->getWorldPosition()));
	//glUniform3fv(_shader->getUniformLocation("fromLightDirection"), 1, glm::value_ptr(currentLight->getTransform()[2]));
	//glUniform3fv(_shader->getUniformLocation("lightColor"), 1, glm::value_ptr(currentLight->getLightColor()));
	
	//set the material color
	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));  
	 
	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	glUniform1f(_shader->getUniformLocation("time"), glm::float1(std::clock() / 1500.0f));
	glUniform1i(_shader->getUniformLocation("numWaves"), 8);
	/* 
	for (int i = 0; i < 8; ++i)
	{
		float amplitude = 0.6f / (i + 1);
		glUniform1f(_shader->getUniformLocation("amplitude[" + std::to_string(i) + "]"), glm::float1(amplitude));

		float wavelength = 6 * glm::pi<float>() / (i + 1);
		glUniform1f(_shader->getUniformLocation("waveLength[" + std::to_string(i) + "]"), glm::float1(wavelength));

		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(-glm::pi<float>()/3, glm::pi<float>() / 3);
		float angle = distribution(generator);  // generates number in the range 1..6 

		glUniform2f (_shader->getUniformLocation("direction[" + std::to_string(i) + "]"), glm::float1(cos(angle)), glm::float1(sin(angle)));
		
		float speed = 1.0f + 0.5f * i;
		glUniform1f(_shader->getUniformLocation("speed[" + std::to_string(i) + "]"), glm::float1(speed));
	}
	*/
	glm::vec3 eyePos = pWorld->getMainCamera()->getWorldPosition();
	glUniform3f(_shader->getUniformLocation("eyePos"), glm::float1(eyePos.x), glm::float1(eyePos.y), glm::float1(eyePos.z));
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
	
}

	void WaterMaterial::setDiffuseTexture(Texture* pDiffuseTexture)
	{
	}


