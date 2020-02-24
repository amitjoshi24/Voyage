#include "camera.h"

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};


void Camera::init(){
	eye = glm::vec3(0.0f, 0.0f, camera_distance_);

	look = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0, 0.0f);
	
	right = glm::normalize(glm::cross(up, look));
	up = glm::cross(look, right);

}
// FIXME: Calculate the view matrix
glm::mat4 Camera::get_view_matrix() const
{


	return glm::mat4(1.0);
}

