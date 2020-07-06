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
    Texture* heightMap = Texture::load(config::MGE_TEXTURE_PATH + "poolHeightmap.jpg");
    Texture* splatMap = Texture::load(config::MGE_TEXTURE_PATH + "splatmap.png");
	WaterFrameBuffer* frameBuffer = new WaterFrameBuffer(_window);
	fbo = frameBuffer;
	//MATERIALS

    //create some materials to display the cube, the plane and the light
    
	AbstractMaterial* litMaterial = new LitMaterial (glm::vec3(1,1,0),glm::vec3(1,1,0));
	AbstractMaterial* lightMaterial = new ColorMaterial(glm::vec3(0.7f, 0.3f, 0.4f));
	AbstractMaterial* bricksMaterial = new TextureMaterial(bricks);
	AbstractMaterial* grassMaterial = new TextureMaterial(grass);
	AbstractMaterial* sandMaterial = new TextureMaterial(sand);
	AbstractMaterial* stonesMaterial = new TextureMaterial(stones);



    AbstractMaterial* runicStoneMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"runicfloor.png"));
    AbstractMaterial* terrainMaterial = new TerrainMaterial(heightMap, Texture::load(config::MGE_TEXTURE_PATH + "splatmap.png"), Texture::load(config::MGE_TEXTURE_PATH + "water.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse1.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse2.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse3.jpg"));
	AbstractMaterial* waterMaterial = new WaterMaterial(fbo);

	//SCENE SETUP
	Camera* camera = new Camera("camera", glm::vec3(-60, 20, 0));
	camera->rotate(glm::radians(-90.0f), glm::vec3(0, 1, 0));
	camera->rotate(glm::radians(-30.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);

	GameObject* terrain = new GameObject("terrain", glm::vec3(0, -7, 0)); 
	terrain->scale(glm::vec3(4, 1, 4)); 
	terrain->setMesh(planeMeshDefault);
	terrain->setMaterial(terrainMaterial);
	_world->add(terrain);

	//add the floor
	GameObject* water = new GameObject("plane", glm::vec3(0, 0, 0));
	water->scale(glm::vec3(4, 1, 4));
	water->setMesh(planeMeshDefault);
	water->setMaterial(waterMaterial);
	_world->registerWater(water);
	for (int i = 0; i < _lightRows; i++)
	{
		for (int j = 0; j < _lightCollumns; j++)
		{
			GameObject* suzanna = new GameObject("Suzanna", glm::vec3(40 - i * 2.f, 2, 40 - j * 2.0f));
			suzanna->scale(glm::vec3(0.5f, 1, 0.5f));
			suzanna->setMesh(suzannaMeshS);
			suzanna->setMaterial(bricksMaterial);
			suzanna->setBehaviour(new RotatingBehaviour());
			_world->add(suzanna);
		}
	}
	
	//camera->setBehaviour(new OrbitBehaviour(20, -90, 0, 5, water));

	

}

void MGEDemo::_render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CLIP_DISTANCE0);
		fbo->BindReflectionFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		_world->getMainCamera()->reflectCamera();
		AbstractGame::_render(glm::vec4(0, 1, 0, 0));
		_world->getMainCamera()->reflectCamera();
		fbo->UnbindCurrentFrameBuffer();
		
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
