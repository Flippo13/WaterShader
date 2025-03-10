#ifndef ABSTRACTMATERIAL_HPP
#define ABSTRACTMATERIAL_HPP

#include "glm.hpp"
class GameObject;
class Mesh;
class World;

/**
 * AbstractMaterial should be subclassed for all materials, for example ColorMaterial, TextureMaterial.
 *
 * Basically a material is responsible for setting up the correct shader,
 * filling all the correct parameters and actually rendering a mesh by streaming all mesh data through a
 * specific shader. Subclasses often use a static shader variable which is initialized once upon first use,
 * but that is fully optional (!).
 *
 * This works like the Unity Material, where you have to select a shader for a material and the shader chosen
 * determines which properties actually become available for the material itself.
 */
class AbstractMaterial
{
    public:
        AbstractMaterial();
        virtual ~AbstractMaterial();

        /**
         * Render the given mesh in the given world using the given mvp matrices. Implement in subclass.
         */
        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pPerspectiveMatrix, glm::vec4& pClipPlanePosition = glm::vec4(0,0,0,0)) = 0;
};

#endif // ABSTRACTMATERIAL_HPP
