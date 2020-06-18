#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

class WaterFrameBuffer {
public:
	 WaterFrameBuffer();
	~WaterFrameBuffer();
	
	void Clear(); 
	void BindReflectionFrameBuffer(); 
	void BindRefractionFrameBuffer(); 
	void UnbindCurrentFrameBuffer(); 
	int GetReflectionTexture(); 
	int GetRefractionTexture();
	int GetRefractionDepthTexture(); 
private:

	void InitializeReflectionFrameBuffer(); 
	void InitializeRefractionFrameBuffer(); 
	int BindFrameBuffer(int framebuffer, int width, int height);
	int CreateFrameBuffer(); 
	int CreateTextureAttachment(int width, int height); 
	int CreateDepthTextureAttachment(int width, int height); 

	int refractionFrameBuffer;
	int refractionTexture; 

	int reflectionFrameBuffer;
};