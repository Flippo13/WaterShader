#include <iostream>
#include <algorithm>

#include "mge/core/World.hpp"
#include "mge/core/Light.hpp"

World::World():GameObject("root"), _mainCamera(0)
{
	_world = this;
}

void World::setMainCamera (Camera* pCamera) {
    if (pCamera != NULL) _mainCamera = pCamera;
}

Camera* World::getMainCamera () {
    return _mainCamera;
}


void World::registerLight (Light* pLight) {
    std::cout << "Registering light " << pLight->getName() << std::endl;
    _lights.push_back(pLight);
}

void World::unregisterLight (Light* pLight) {
    std::cout << "Unregistering light " << pLight->getName() << std::endl;
	if (_lights.size() == 0) return;
    _lights.erase(std::remove(_lights.begin(), _lights.end(), pLight), _lights.end());
}

Light* World::getLightAt (int pIndex) {
	return _lights[pIndex];
}

int World::getLightCount() {
    return _lights.size();
}

void World::registerWater(GameObject* pWater)
{
	std::cout << "Registering water" << pWater->getName() << std::endl;
	_waters.push_back(pWater);
}

void World::unregisterWater(GameObject* pWater)
{
	std::cout << "Unregistering water" << pWater->getName() << std::endl;
	_waters.erase(std::remove(_waters.begin(), _waters.end(), pWater), _waters.end());
}

GameObject* World::getWaterAt(int pIndex)
{
	return _waters[pIndex];
}

int World::getWaterCount()
{
	return _waters.size();
}
