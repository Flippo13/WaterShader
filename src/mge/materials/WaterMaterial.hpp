#ifndef WATERMATERIAL_HPP
#define WATERMATERIAL_HPP
#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"


class ShaderProgram;
class Texture; 

class WaterMaterial : public AbstractMaterial
{
public:
	WaterMaterial(Texture* pDiffuseTexture);
	virtual ~WaterMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;


	void setHeightMap(Texture* pHeightTexture); 
	void setDiffuseTexture(Texture* pDiffuseTexture);


private:

	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	static GLint _uMVPMatrix; 
	static GLint _uDiffuseTexture; 

	static GLint _aVertex; 
	static GLint _aNormal; 
	static GLint _aUV; 

	Texture* _diffuseTexture; 

	WaterMaterial(const WaterMaterial &);
	WaterMaterial& operator=(const WaterMaterial&);

};

#endif
