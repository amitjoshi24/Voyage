#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

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
	void swivel(); //with mouse delta ROTATE
	// FIXME: add functions to manipulate camera objects.
private:
	float camera_distance_ = 3.0;
	glm::vec3 look;
	glm::vec3 cam_up;
	glm::vec3 cam_right;
	glm::vec3 eye; //pos of camera in space
	glm::vec3 center;

	// Note: you may need additional member variables
};

#endif
