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

glm::mat4& Camera::getProjection() {
    return _projection;
}

