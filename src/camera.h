#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>

class Camera {
public:
	glm::mat4 get_view_matrix() const;
	void init();
	//move position of camera
	void left(); // a
	void right(); //d
	void forward(); //w
	void backward(); // s
	void up(); //up arrow
	void down(); //down arrow
	void update(); //updates things that arent look or eye or cam_up

	//rotate camera
	void clockwise();
	void counterclockwise();
	void swivel(glm::vec2); //with mouse delta ROTATE
	glm::vec3 eye; //pos of camera in space
private:
	float camera_distance_ = 3.0;
	glm::vec3 look;
	glm::vec3 cam_up;
	glm::vec3 cam_right;
	glm::vec3 center;
	glm::vec3 abs_up;
	glm::vec3 abs_right;

	// Note: you may need additional member variables
};

#endif
