#ifndef WATERMATERIAL_HPP
#define WATERMATERIAL_HPP
#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"


class ShaderProgram;
class Texture; 
class WaterFrameBuffer;

class WaterMaterial : public AbstractMaterial
{
public:
	WaterMaterial(WaterFrameBuffer* pFrameBuffer);
	virtual ~WaterMaterial();

	virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, glm::vec4& pClipPlanePosition = glm::vec4(0, 0, 0, 0)) override;


	void setHeightMap(Texture* pHeightTexture); 
	void setDiffuseTexture(Texture* pDiffuseTexture);


private:

	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	static GLint _uMVPMatrix; 
	static GLint _uDiffuseTexture; 
	static GLint _uReflectionTexture; 
	static GLint _uRefractionTexture; 
	static GLint _uRefractiveDepthTexture; 

	static GLint _uDuDvTexture;
	static GLint _uNormalMap; 

	static GLint _aVertex; 
	static GLint _aNormal; 
	static GLint _aUV; 

	WaterFrameBuffer* _fbo; 
	Texture* _dudvTexture; 
	Texture* _diffuseTexture; 
	Texture* _normalMap; 

	WaterMaterial(const WaterMaterial &);
	WaterMaterial& operator=(const WaterMaterial&);

};

#endif
