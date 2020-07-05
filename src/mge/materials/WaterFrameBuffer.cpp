#include <SFML/Graphics.hpp>
#include "mge/core/Texture.hpp"

#include "WaterFrameBuffer.hpp"

constexpr auto reflection_width = 256;
constexpr auto reflection_height = 256; 

constexpr auto refraction_width = 800;
constexpr auto refraction_height = 600;


WaterFrameBuffer::WaterFrameBuffer(sf::RenderWindow * aWindow): _window(aWindow), 
_reflectionDepthBuffer(0), _reflectionFrameBuffer(0), _reflectionTexture(0), _refractionDepthBuffer(0), _refractionFrameBuffer(0), _refractionTexture(0) {
	assert(_window != NULL);
	InitializeReflectionFrameBuffer();
	InitializeRefractionFrameBuffer();
}

WaterFrameBuffer::~WaterFrameBuffer() {
	Clear();
}
	
void WaterFrameBuffer::Clear()
{
	glDeleteFramebuffers(1, &_reflectionFrameBuffer);
	glDeleteTextures(1, &_reflectionTexture);
	glDeleteRenderbuffers(1, &_reflectionDepthBuffer);
	glDeleteFramebuffers(1, &_refractionFrameBuffer);
	glDeleteTextures(1, &_refractionTexture);
	glDeleteTextures(1, &_refractionDepthBuffer);
}

void WaterFrameBuffer::BindReflectionFrameBuffer()
{
	BindFrameBuffer(_reflectionFrameBuffer, reflection_width, reflection_height);
}

void WaterFrameBuffer::BindRefractionFrameBuffer()
{
	BindFrameBuffer(_refractionFrameBuffer, refraction_width, refraction_height);
}

void WaterFrameBuffer::UnbindCurrentFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _window->getSize().x, _window->getSize().y);
}

int WaterFrameBuffer::GetReflectionTexture()
{
	return _reflectionTexture;
}

int WaterFrameBuffer::GetRefractionTexture(){
	return _refractionTexture;
}
int WaterFrameBuffer::GetRefractionDepthTexture() {
	return _refractionDepthBuffer;
}

void WaterFrameBuffer::InitializeReflectionFrameBuffer() {
	_reflectionFrameBuffer = CreateFrameBuffer();
	_reflectionTexture = CreateTextureAttachment(reflection_width, reflection_height);
	_reflectionDepthBuffer = CreateDepthBufferAttachment(reflection_width, reflection_height);
	UnbindCurrentFrameBuffer();
}
void WaterFrameBuffer::InitializeRefractionFrameBuffer() {
	_refractionFrameBuffer = CreateFrameBuffer();
	_refractionTexture = CreateTextureAttachment(refraction_width, refraction_height);
	_refractionDepthBuffer = CreateDepthTextureAttachment(refraction_width, refraction_height);
	UnbindCurrentFrameBuffer();
}


void WaterFrameBuffer::BindFrameBuffer(int framebuffer, int width, int height) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glViewport(0, 0, width, height);
}
GLuint	WaterFrameBuffer::CreateFrameBuffer() {
	GLuint fbo; 
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return fbo;
}

GLuint	WaterFrameBuffer::CreateTextureAttachment(int width, int height) {
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	return texture;
}

Texture* WaterFrameBuffer::CreateTextureAttachmentTex(int width, int height) {
	Texture* texture = Texture::createTexture();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->getId(), 0);
	return texture;
}

GLuint	WaterFrameBuffer::CreateDepthTextureAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	glDrawBuffer(GL_NONE); 
	glReadBuffer(GL_NONE);
	return texture;
}

GLuint WaterFrameBuffer::CreateDepthBufferAttachment(int width, int height) {
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	return depthBuffer;
}