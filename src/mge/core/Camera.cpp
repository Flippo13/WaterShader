#include "glm.hpp"
#include "mge/core/Camera.hpp"

Camera::Camera( std::string pName, glm::vec3 pPosition, glm::mat4 pProjectionMatrix )
:	GameObject(pName, pPosition), _projection(pProjectionMatrix), _position(pPosition)
{
}

Camera::~Camera()
{
	//dtor
}

glm::vec3& Camera::getPosition() {
	return _position;
}

glm::mat4& Camera::inversePitch()
{
	glm::mat4 reflectionMatrix = glm::inverse(glm::scale(glm::vec3(1, -1, 1)) * glm::inverse(getWorldTransform()) * glm::scale(glm::vec3(1, -1, 1)));

	return reflectionMatrix;
	//setTransform(reflectionMatrix);
}

void Camera::reflectCamera()
{
	glm::mat4 reflectionMatrix = glm::inverse(glm::scale(glm::vec3(1, -1, 1)) * glm::inverse(getWorldTransform()) * glm::scale(glm::vec3(1, -1, 1)));// * glm::scale(glm::vec3(1, -1, 1));
	setTransform(reflectionMatrix);
}

glm::mat4& Camera::getProjection() {
    return _projection;
}

