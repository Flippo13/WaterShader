#ifndef WORLD_HPP
#define WORLD_HPP

#include "mge/core/GameObject.hpp"

class Camera;
class Light;

class World : public GameObject
{
	public:
        World();

		void setMainCamera (Camera* pCamera);
		Camera* getMainCamera();

        //only used internally, do not use from outside
        void registerLight (Light* pLight);
        void unregisterLight (Light* pLight);

        Light* getLightAt (int pIndex);
        int getLightCount();

		void registerWater(GameObject* pWater); 
		void unregisterWater(GameObject* pWater);

		GameObject* getWaterAt(int pIndex);
		int getWaterCount();

	private:
	    Camera* _mainCamera;
	    std::vector<Light*> _lights;
		std::vector<GameObject*> _waters;

        World(const World&);
        World& operator=(const World&);
};


#endif // WORLD_HPP
