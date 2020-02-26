#include "camera.h"
#include <glm/glm.hpp>

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};

#include <iostream>


void print_vec(glm::vec4 vec){
	for (int j = 0; j < 4; j ++){
			std::cout<< vec[j] << ' ';
		}

		std::cout<< std::endl;
}

void print_matrix(glm::mat4 mat){
	for (int i = 0; i < 4; i ++){
		print_vec(mat[i]);
	}

}


void Camera::init(){
	eye = glm::vec3(0.0f, 0.0f, camera_distance_);

	look = glm::vec3(0.0f, 0.0f, -1.0f);
	cam_up = glm::vec3(0.0f, 1.0f, 0.0f);

	abs_up = glm::vec3(0.0f, 1.0, 0.0f);
	abs_right = glm::vec3(1.0f, 0.0f, 0.0f);

	update();


}

void Camera::update(){
	cam_right = glm::normalize(glm::cross(look, cam_up));

	center = eye + camera_distance_*look; //TODO elementwise multiply might be weird
}

void Camera::left(){
	eye += -abs_right * pan_speed;

	update();
	//update camera private variables 
} // a
void Camera::right(){
	eye += abs_right * pan_speed;
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
	eye+= abs_up * pan_speed;
	update();

} //up arrow
void Camera::down(){
	eye+= -abs_up * pan_speed;
	update();

} //down arrow

void Camera::clockwise(){
	cam_up = glm::rotate(cam_up, roll_speed, look);

	update();

} //rotate up around look

void Camera::counterclockwise(){
	cam_up = glm::rotate(cam_up, -roll_speed, look);

	update();


}

void Camera::swivel(){

}

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
					transformMatrix[i][j] = -look[j];
				else if (i == 3)
					transformMatrix[i][j] = -eye[j];

			}
			else
				transformMatrix[i][j] = 0.0f;

		}

	}


	transformMatrix[3][3] = 1.0f;

	return transformMatrix;

	/*

	glm::mat4 translationMatrix = glm::mat4();

	for (int j = 0; j < 4; j++){
		for (int i = 0; i < 4; i++){
			if ( j == 3 && i < 3)
				translationMatrix[j][i] = -eye[i];
			if (j == i)
				translationMatrix[j][i] = 1;
		}
	}

	glm::mat4 view_matrix = transformMatrix * translationMatrix;

	return  view_matrix;

	glm::mat4 temp = glm::lookAt(
    eye, // the position of your camera, in world space
    center,   // where you want to look at, in world space
    cam_up        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
); 

	print_matrix(temp);
	//SOLUTION
	return temp;
	*/
}

