#ifndef ABSTRACTGAME_HPP
#define ABSTRACTGAME_HPP

#include "glm.hpp"

#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include <string>

class World;
class Renderer;

/**
 * Defines the basic structure for a game, with defaults already filled in.
 * The only item that MUST be overridden/implemented is _initializeScene()
 * where you build up your world, the rest is optional to override (with care/caution).
 */
class AbstractGame
{
    public:

        AbstractGame();
        virtual ~AbstractGame();

        //creates a window, initializes glew, a renderer and a world instance
        virtual void initialize();
        //run the actual process of updating all objects, rendering them and processing events
        virtual void run();

		virtual void setLightAmount(int pRows, int pCollumns);
    protected:

        //methods above delegate behaviour to the methods below so that you can override it in a subclass

        //initialize sfml rendering context
        virtual void _initializeWindow();
        //print info about the current driver version etc
        virtual void _printVersionInfo();
        //initialize the extension wrangler
        virtual void _initializeGlew();
        //create our own custom renderer instance
        virtual void _initializeRenderer();
        //initialize a scene root to which we can attach/add objects
        virtual void _initializeWorld();

        //initialize the actual scene, HAS to be done by a subclass
        virtual void _initializeScene() = 0;

        //call update on all game objects in the display root
        virtual void _update(float pStep);
        //render all game objects in the display root
        virtual void _render();
		//render all water game objects
		virtual void _renderWater();
		//render all game objects with clipPlane
		virtual void _render(glm::vec4&);
        //process any sfml window events (see SystemEventDispatcher/Listener)
        virtual void _processEvents();

		sf::RenderWindow* _window;  //sfml window to render into
		Renderer* _renderer;        //the renderer class to render the world
		World* _world;              //the root game object that represents our scene
		float _fps;                 //stores the real fps

		unsigned int _fpsStart; 
		unsigned int _fpsFrames; 
		unsigned int _frameTime; 
    private:
        AbstractGame(const AbstractGame&);
        AbstractGame& operator=(const AbstractGame&);


};

#endif // ABSTRACTGAME_HPP
