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

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/OrbitBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo():AbstractGame (),_hud(0)
{
}

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
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
    Texture* grass = Texture::load(config::MGE_TEXTURE_PATH + "diffuse1.jpg");
    Texture* sand = Texture::load(config::MGE_TEXTURE_PATH + "diffuse2.jpg");
    Texture* rocks = Texture::load(config::MGE_TEXTURE_PATH + "bricks.jpg");
    Texture* stones = Texture::load(config::MGE_TEXTURE_PATH + "diffuse4.jpg");
    Texture* heightMap = Texture::load(config::MGE_TEXTURE_PATH + "heightmaptest.png");
    Texture* splatMap = Texture::load(config::MGE_TEXTURE_PATH + "splatmaptest.png");

	//MATERIALS

    //create some materials to display the cube, the plane and the light
    AbstractMaterial* lightMaterial = new ColorMaterial (glm::vec3(1,1,0));
    AbstractMaterial* runicStoneMaterial = new TextureMaterial (Texture::load (config::MGE_TEXTURE_PATH+"runicfloor.png"));
    AbstractMaterial* terrainMaterial = new TerrainMaterial(Texture::load(config::MGE_TEXTURE_PATH + "heightmaptest.png"), Texture::load(config::MGE_TEXTURE_PATH + "splatmap.png"), Texture::load(config::MGE_TEXTURE_PATH + "water.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse1.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse2.jpg"), Texture::load(config::MGE_TEXTURE_PATH + "diffuse3.jpg"));
	AbstractMaterial* waterMaterial = new WaterMaterial(Texture::load(config::MGE_TEXTURE_PATH + "water.jpg"));
	//SCENE SETUP


    //add the floor
    GameObject* water = new GameObject ("plane", glm::vec3(0,-20,0));
    water->scale(glm::vec3(10,10,10));
    water->setMesh(planeMeshDefault);
    water->setMaterial(waterMaterial);
    _world->add(water);

	/* 
	
    //add a spinning sphere
    GameObject* sphere = new GameObject ("sphere", glm::vec3(0,0,0));
    sphere->scale(glm::vec3(2.5,2.5,2.5));
    sphere->setMesh (cubeMeshF);
    sphere->setMaterial(runicStoneMaterial);
    sphere->setBehaviour (new KeysBehaviour());
   // _world->add(sphere);

   
	
    //add a light. Note that the light does ABSOLUTELY ZIP! NADA ! NOTHING !
    //It's here as a place holder to get you started.
    //Note how the texture material is able to detect the number of lights in the scene
    //even though it doesn't implement any lighting yet!
	*/

	//add camera first (it will be updated last)
	Camera* camera = new Camera("camera", glm::vec3(0, 6, 7));
	camera->rotate(glm::radians(-40.0f), glm::vec3(1, 0, 0));
	_world->add(camera);
	_world->setMainCamera(camera);
	camera->setBehaviour(new OrbitBehaviour(200, -110, 110, 20, water));

    Light* light = new Light("light", glm::vec3(0,4,0));
    light->scale(glm::vec3(0.1f, 0.1f, 0.1f));
    light->setMesh(cubeMeshF);
    light->setMaterial(lightMaterial);
    light->setBehaviour(new KeysBehaviour(25));
    _world->add(light);

}

void MGEDemo::_render() {
    AbstractGame::_render();
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
