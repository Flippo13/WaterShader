#include <SFML/Graphics.hpp>

#include "mge/materials/AbstractMaterial.hpp"
#include "GL/glew.h"

class ShaderProgram;
class Texture;

class WaterFrameBuffer {
public:
	 WaterFrameBuffer(sf::RenderWindow* aWindow);
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
	void BindFrameBuffer(int framebuffer, int width, int height);
	int CreateFrameBuffer(); 
	int CreateTextureAttachment(int width, int height); 
	int CreateDepthTextureAttachment(int width, int height); 

	unsigned int _refractionFrameBuffer;
	unsigned int _refractionTexture; 
	unsigned int _refractionDepthBuffer; 
	
	
	unsigned int _reflectionFrameBuffer;
	unsigned int _reflectionTexture;
	unsigned int _reflectionDepthBuffer;

	sf::RenderWindow* _window;

	WaterFrameBuffer(const WaterFrameBuffer&);
	WaterFrameBuffer& operator=(const WaterFrameBuffer&);

};