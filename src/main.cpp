#include <iostream>
#include <sstream> // for std::stringstream

#include "mge/core/AbstractGame.hpp"
#include "mge/MGEDemo.hpp"

/**
 * Main entry point for the Micro Engine.

 * Design goals:
 * - correct division of OpenGL into appropriate wrapper classes
 * - simple design
 * - each class should know as little as possible about other classes and non related parts of OpenGL
 * - the engine must allow different objects with different transforms, meshes, materials (shaders) etc
 * - consistent coding conventions
 * - reusable and extendable core set of classes which should require no modification
 *   (in other words it is possible to have a working "empty" example)
 *
 * All documentation is contained within the HEADER files, not the CPP files if possible.
 *
 */
int main(int argc, char *argv[])
{
	int rows = 0; 
	int collums = 0;
	if (argc > 1) {
		for (int count{ 1 }; count < argc; ++count) {
			std::stringstream convert{ argv[count] };
			int myint{};
			if (!(convert >> myint)) 
				myint = 0;
			if (count == 1)
				rows = myint; 
			if (count == 2)
				collums = myint;

		}
		std::cout << "Well what number: " << rows << "\t" << collums << '\n';

	}
    std::cout << "Starting Game" << std::endl;

    AbstractGame* game = new MGEDemo();
	game->setLightAmount(rows, collums);
	game->initialize();
    game->run();

	delete game;

    return 0;
}



