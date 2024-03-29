#ifndef MENGER_H
#define MENGER_H

#include <glm/glm.hpp>
#include <vector>

class Menger {
public:
	Menger();
	~Menger();
	void set_nesting_level(int);
	bool is_dirty() const;
	void set_clean();
	void set_dirty();
	void generate_geometry(std::vector<glm::vec4>& obj_vertices,
	                       std::vector<glm::uvec3>& obj_faces) ;
	void generate_geometry_helper(float s, glm::dvec3 min, std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces, int curDepth) const;
	std::vector<glm::vec4> obj_vertices;
	std::vector<glm::uvec3> obj_faces;

public:
	int nesting_level_ = 0;
	bool dirty_ = false;
};

#endif
