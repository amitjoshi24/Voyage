#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};


void Camera::init(){
	eye = glm::vec3(0.0f, 0.0f, camera_distance_);

	look = glm::vec3(0.0f, 0.0f, -1.0f);
	cam_up = glm::vec3(0.0f, 1.0, 0.0f);

	update();
}

void Camera::update(){
	cam_right = glm::normalize(glm::cross(cam_up, look));
	cam_up = glm::cross(look, cam_right);

	center = eye + camera_distance_*look; //TODO elementwise multiply might be weird
}

void Camera::left(){
	eye += -cam_right * pan_speed;

	update();
	//update camera private variables 
} // a
void Camera::right(){
	eye += cam_right * pan_speed;
	update();
} //d
void Camera::forward(){
	eye += look * zoom_speed;
	update();

} //w
void Camera::backward(){
	eye+= -look * zoom_speed;
	update();

} // s
void Camera::up(){
	eye+= cam_up * pan_speed;
	update();

} //up arrow
void Camera::down(){
	eye+= -cam_up * pan_speed;
	update();

} //down arrow

// FIXME: Calculate the view matrix
glm::mat4 Camera::get_view_matrix() const
{

	glm::mat4 viewMatrix;

	glm::mat4 transformMatrix = glm::mat4();

	for (int i = 0; i < 4; i ++){
		for (int j = 0; j < 4; j ++){
			if (j < 3){
				if ( i == 0) // right vector
					transformMatrix[i][j] = cam_right[j];
				else if ( i == 1)
					transformMatrix[i][j] = cam_up[j];
				else if (i == 2)
					transformMatrix[i][j] = look[j];

			}
			else
				transformMatrix[i][j] = 0;

		}

	}

	transformMatrix[3][3] = 1.0f;

	glm::mat4 translationMatrix = glm::translate(glm::mat4(), -eye);

	return transformMatrix * translationMatrix;
}

