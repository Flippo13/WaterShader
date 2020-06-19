#include "glm.hpp"

#include "WaterMaterial.hpp"

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

GLint WaterMaterial::_aVertex = 0; 
GLint WaterMaterial::_aNormal = 0;
GLint WaterMaterial::_aUV = 0;

WaterMaterial::WaterMaterial(Texture* pDiffuseTexture) :_diffuseTexture(pDiffuseTexture) {
	_lazyInitializeShader();
}

WaterMaterial::~WaterMaterial() {}

void WaterMaterial::_lazyInitializeShader()
{
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "water2.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "water2.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseTexture = _shader->getUniformLocation("diffuseTexture");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
	}
}

void WaterMaterial::render(World * pWorld, Mesh * pMesh, const glm::mat4 & pModelMatrix, const glm::mat4 & pViewMatrix, const glm::mat4 & pProjectionMatrix) 
{
	if (!_diffuseTexture) return;

	_shader->use();

	//Print the number of lights in the scene and the position of the first light.
	//It is not used, but this demo is just meant to show you THAT materials can access the lights in a world
	//if (pWorld->getLightCount() > 0) {
	//    std::cout << "TextureMaterial has discovered light is at position:" << pWorld->getLightAt(0)->getLocalPosition() << std::endl;
	//}

	//set the material color
	glUniform3fv(_shader->getUniformLocation("diffuseColor"), 1, glm::value_ptr(glm::vec4(0,0,1,1)));

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(
		_shader->getAttribLocation("vertex"),
		_shader->getAttribLocation("normal"),
		_shader->getAttribLocation("uv")
	);



	/*
	 * 
	 
	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	//bind the texture to the current active slot
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	//tell the shader the texture slot for the diffuse texture is slot 0
	glUniform1i(_uDiffuseTexture, 0);

	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	glUniform1f(_shader->getUniformLocation("time"), glm::float1(std::clock()/50));
	glUniform1i(_shader->getUniformLocation("numWaves"), 4);
	for (int i = 0; i < 4; ++i)
	{
		float amplitude = 0.5f / (i + 1);
		glUniform1f(_shader->getUniformLocation("amplitude[" + std::to_string(i) + "]"), glm::float1(amplitude));

		float wavelength = 10 * glm::pi<float>() / (i + 1);
		glUniform1f(_shader->getUniformLocation("waveLength[" + std::to_string(i) + "]"), glm::float1(wavelength));

		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(-glm::pi<float>()/3, glm::pi<float>() / 3);
		float angle = distribution(generator);  // generates number in the range 1..6 

		glUniform2f (_shader->getUniformLocation("direction[" + std::to_string(i) + "]"), glm::float1(cos(angle)), glm::float1(sin(angle)));
		
		float speed = 1.0f + 1 * i;
		glUniform1f(_shader->getUniformLocation("speed[" + std::to_string(i) + "]"), glm::float1(speed));
	}
	glm::vec3 eyePos = pWorld->getMainCamera()->getPosition();
	glUniform3f(_shader->getUniformLocation("eyePos"), glm::float1(eyePos.x), glm::float1(eyePos.y), glm::float1(eyePos.z));
	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
	*/
}

	void WaterMaterial::setDiffuseTexture(Texture* pDiffuseTexture)
	{
	}


