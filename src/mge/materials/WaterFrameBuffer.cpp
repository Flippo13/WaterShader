#include "WaterFrameBuffer.hpp"

constexpr auto REFLECTION_WIDTH = 320;
constexpr auto REFLECTION_HEIGHT = 180; 

constexpr auto REFRACTION_WIDTH = 1280; 
constexpr auto REFRACTION_HEIGHT = 720;



WaterFrameBuffer::WaterFrameBuffer() {

}

WaterFrameBuffer::~WaterFrameBuffer() {
}
	
void WaterFrameBuffer::Clear()
{

}

void WaterFrameBuffer::BindReflectionFrameBuffer()
{

}

void WaterFrameBuffer::BindRefractionFrameBuffer()
{

}

void WaterFrameBuffer::UnbindCurrentFrameBuffer()
{

}

int WaterFrameBuffer::GetReflectionTexture()
{

}

int WaterFrameBuffer::GetRefractionTexture(){
}
int WaterFrameBuffer::GetRefractionDepthTexture() {
}

void WaterFrameBuffer::InitializeReflectionFrameBuffer() {
}
void WaterFrameBuffer::InitializeRefractionFrameBuffer() {
	refractionFrameBuffer = CreateFrameBuffer();
	refractionTexture = CreateTextureAttachment()

}


int WaterFrameBuffer::BindFrameBuffer(int framebuffer, int width, int height) {
	glBindTexture(GL_TEXTURE, 0);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
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