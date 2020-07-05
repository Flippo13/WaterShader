#include <iostream>
#include <string>

#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/TerrainMaterial.hpp"
#include "mge/materials/WaterMaterial.hpp"
#include "mge/materials/WaterFrameBuffer.hpp"
#include "mge/materials/LitMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/OrbitBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame (),_hud(0),fbo(0)
{
	_lightRows = 0;
	_lightCollumns = 0;
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

void MGEDemo::setLightAmount(int pRows, int pCollumns)
{
	_lightRows = pRows;
	_lightCollumns = pCollumns;

	std::cout << "lights: " << _lightRows * _lightCollumns << '\n';
}

//build the game _world
void MGEDemo::_initializeScene()
{
    //MESHES

    //load a bunch of meshes we will be using throughout this demo
    //each mesh only has to be loaded once, but can be used multiple times:
    //F is flat shaded, S is smooth shaded (normals aligned or not), check the models folder!
    Mesh* planeMeshDefault = Mesh::load (config::MGE_MODEL_PATH+"plane20x20_2048tris_aligned_uvs.obj");
    Mesh* cubeMeshF = Mesh::load (config::MGE_MODEL_PATH+"cube_flat.obj");
    Mesh* sphereMeshS = Mesh::load (config::MGE_MODEL_PATH+"sphere_smooth.obj");
	Mesh* suzannaMeshS = Mesh::load(config::MGE_MODEL_PATH + "suzanna_smooth.obj");

    Texture* grass = Texture::load(config::MGE_TEXTURE_PATH + "diffuse1.jpg");
    Texture* sand = Texture::load(config::MGE_TEXTURE_PATH + "diffuse2.jpg");
    Texture* bricks = Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg");
    Texture* stones = Texture::load(config::MGE_TEXTURE_PATH + "diffuse4.jpg");
    Texture* heightMap = Texture::load(config::MGE_TEXTURE_PATH + "heightmap.png");
    Texture* splatMap = Texture::load(config::MGE_TEXTURE_PATH + "splatmap.png");
	WaterFrameBuffer* frameBuffer = new WaterFrameBuffer(_window);
	fbo = frameBuffer;
	//MATERIALS

    //create some materials to display the cube, the plane and the light
    
	AbstractMaterial* litMaterial = new LitMaterial (glm::vec3(1,1,0),glm::vec3(1,1,0));
	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(0.7f, 0.3f, 0.4f));
	AbstractMaterial* bricksMaterial = new TextureMaterial(bricks);
    AbstractMaterial* runicStoneMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"runicfloor.png"));
    AbstractMaterial* terrainMaterial = new TerrainMaterial(Texture::load(config::MGE_TEXTURE_PATH + "heightmaptest.png"), Texture::load(config::MGE_TEXTURE_PATH + "splatmap.png"), Texture::load(config::MGE_TEXTURE_PATH + "water.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse1.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse2.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse3.jpg"));
	AbstractMaterial* waterMaterial = new WaterMaterial(fbo);

	//SCENE SETUP
	Camera* camera = new Camera("camera", glm::vec3(0, 30, 0));
	camera->rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	//add the floor
	GameObject* water = new GameObject("plane", glm::vec3(0, 0, 0));
	water->scale(glm::vec3(2, 1, 2));
	water->setMesh(planeMeshDefault);
	water->setMaterial(waterMaterial);
	_world->registerWater(water);

	GameObject* suzanna = new GameObject("Suzanna", glm::vec3(0, 2, 0));
	suzanna->scale(glm::vec3(1, 1, 1)); 
	suzanna->setMesh(suzannaMeshS);
	suzanna->setMaterial(bricksMaterial);
	suzanna->setBehaviour(new RotatingBehaviour());
	//_world->add(suzanna);


	int index = 0;

	for (int i = 0; i < _lightRows; i++)
	{
		for (int j = 0; j < _lightCollumns; j++)
		{
			index++;
			Light* redLight = new Light(
				LightType::POINT,
				glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
				glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
				glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
				"light" + std::to_string(index),
				glm::vec3(-20 + i * 1.5f, 1, -18 + j * 2)
			);
			redLight->setSpecularColor(glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f));
			_world->add(redLight);
		}
	}
	
	/*

	Light* greenLight = new Light(
		LightType::POINT,
		glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
		glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
		glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
		"light",
		glm::vec3(0, 1, 0)
	);
	greenLight->setSpecularColor(glm::vec3(130.0f / 255.0f, 255.0f / 255.0f, 16.0f / 255.0f));
	_world->add(greenLight);

	Light* greenLight2 = new Light(
		LightType::POINT,
		glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
		glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
		glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
		"light",
		glm::vec3(10, 1, 0)
	);
	greenLight2->setSpecularColor(glm::vec3(130.0f / 255.0f, 255.0f / 255.0f, 16.0f / 255.0f));
	_world->add(greenLight2);
	*/
	/* 
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			Light* greenLight = new Light(
				LightType::POINT,
				glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
				glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
				glm::vec3(255.0f / 255.0f, 130.0f / 255.0f, 16.0f / 255.0f),
				"light",
				glm::vec3(-9 + i * 2, 1, -9 + j * 2)
			);
			greenLight->setSpecularColor(glm::vec3(130.0f / 255.0f, 255.0f / 255.0f, 16.0f / 255.0f));
			_world->add(greenLight);
		}
	}
	*/


	//camera->setBehaviour(new OrbitBehaviour(15, -30, 30, 5, water));

	

}

void MGEDemo::_render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CLIP_DISTANCE0);
	//glCullFace(GL_FRONT);
		fbo->BindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_world->getMainCamera()->reflectCamera();
		AbstractGame::_render(glm::vec4(0, 1, 0, 0));
		_world->getMainCamera()->reflectCamera();
		fbo->UnbindCurrentFrameBuffer();
		//glCullFace(GL_BACK);
		fbo->BindRefractionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		AbstractGame::_render(glm::vec4(0, -1, 0, 0));

		fbo->UnbindCurrentFrameBuffer();
		glDisable(GL_CLIP_DISTANCE0);
		AbstractGame::_render(glm::vec4(0, 0, 0, 0));

		AbstractGame::_renderWater();
    _updateHud();
}

void MGEDemo::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

MGEDemo::~MGEDemo()
{
	//dtor
}
