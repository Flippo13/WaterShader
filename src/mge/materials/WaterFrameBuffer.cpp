#include <SFML/Graphics.hpp>

#include "WaterFrameBuffer.hpp"

constexpr auto reflection_width = 320;
constexpr auto reflection_height = 180; 

constexpr auto refraction_width = 1280; 
constexpr auto refraction_height = 720;


WaterFrameBuffer::WaterFrameBuffer(sf::RenderWindow * aWindow): _window(aWindow) {
	assert(_window != NULL);
}

WaterFrameBuffer::~WaterFrameBuffer() {
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
}
void WaterFrameBuffer::InitializeRefractionFrameBuffer() {
	_refractionFrameBuffer = CreateFrameBuffer();
	_refractionTexture = CreateTextureAttachment(reflection_width,reflection_height);
	_refractionDepthBuffer = CreateDepthTextureAttachment(refraction_width, refraction_height);
	UnbindCurrentFrameBuffer();
}


void WaterFrameBuffer::BindFrameBuffer(int framebuffer, int width, int height) {
	glBindTexture(GL_TEXTURE, 0);
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glViewport(0, 0, width, height);
}
int	WaterFrameBuffer::CreateFrameBuffer() {
	unsigned int fbo; 
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return fbo;
}
int	WaterFrameBuffer::CreateTextureAttachment(int width, int height) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	return texture;
}
int	WaterFrameBuffer::CreateDepthTextureAttachment(int width, int height)
{
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, rbo);
	return rbo;
}