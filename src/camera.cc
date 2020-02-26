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
void print_vec3(glm::vec3 vec){
	for (int j = 0; j < 3; j ++){
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
	eye += cam_right * pan_speed;
	update();
	//update camera private variables 
} // a
void Camera::right(){
	eye += -cam_right * pan_speed;
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
	eye+= -cam_up * pan_speed;
	update();

} //up arrow
void Camera::down(){
	eye+= cam_up * pan_speed;
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

void Camera::swivel(glm::vec2 dir){
	//vector perpendicular to this and look
	glm::vec3 turn = cam_right*dir[0] - cam_up*dir[1];
	glm::vec3 axis = glm::cross(turn, look); //maybe this axis should go about center?
	//print_vec3(axis);

	look = glm::rotate(look, rotation_speed, axis);
	//cam_up = glm::rotate(cam_up, rotation_speed, axis);
	std::cout<< glm::length(glm::dot(look, cam_up)) <<std::endl;

	update();

}

glm::mat4 Camera::get_view_matrix() const
{

	glm::mat4 viewMatrix;

	glm::mat4 transformMatrix = glm::mat4();

	glm::vec3 forward = glm::normalize(look); //z
	glm::vec3 right_temp = glm::cross(forward, glm::normalize(cam_up)); //x
	glm::vec3 up_temp = glm::cross(right_temp, forward); //y


    viewMatrix[0][0] = right_temp[0];
    viewMatrix[1][0] = right_temp[1];
    viewMatrix[2][0] = right_temp[2];
    viewMatrix[0][1] = up_temp[0];
    viewMatrix[1][1] = up_temp[1];
    viewMatrix[2][1] = up_temp[2];
    viewMatrix[0][2] = -forward[0];
    viewMatrix[1][2] =-forward[1];
    viewMatrix[2][2] =-forward[2];
    viewMatrix[3][0] =-dot(right_temp, eye);
    viewMatrix[3][1] =-dot(up_temp, eye);
    viewMatrix[3][2] = dot(forward, eye);

    viewMatrix[0][3] = 0.0f;
    viewMatrix[1][3] = 0.0f;
    viewMatrix[2][3] = 0.0f;
    viewMatrix[3][3] = 1.0f;

    return viewMatrix;

/*
	glm::mat4 temp = glm::lookAt(
    eye, // the position of your camera, in world space
    center,   // where you want to look at, in world space
    cam_up        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
); 

	return temp;
	*/
	
	
}


