#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
	glm::mat4 get_view_matrix() const;
	void init();
	// FIXME: add functions to manipulate camera objects.
private:
	float camera_distance_ = 3.0;
	glm::vec3 look;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 eye; //pos of camera in space

	// Note: you may need additional member variables
};

#endif
