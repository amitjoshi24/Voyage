#include "menger.h"
#include <iostream>
#include <string>


namespace {
	const int kMinLevel = 0;
	const int kMaxLevel = 4;
};

Menger::Menger()
{
	// Add additional initialization if you like
}

Menger::~Menger()
{
}

void
Menger::set_nesting_level(int level)
{
	nesting_level_ = level;
	dirty_ = true;
}

bool
Menger::is_dirty() const
{
	return dirty_;
}

void
Menger::set_clean()
{
	dirty_ = false;
}

// FIXME generate Menger sponge geometry
void generate_cube(float m, float M, std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces){
	vertices.push_back(glm::vec4(m, M, m, 1.0f));
	vertices.push_back(glm::vec4(m, m, m, 1.0f));
	vertices.push_back(glm::vec4(M, M, m, 1.0f));
	vertices.push_back(glm::vec4(M, m, m, 1.0f));

	vertices.push_back(glm::vec4(m, M, M, 1.0f));
	vertices.push_back(glm::vec4(m, m, M, 1.0f));
	vertices.push_back(glm::vec4(M, M, M, 1.0f));
	vertices.push_back(glm::vec4(M, m, M, 1.0f));


	indices.push_back(glm::uvec3(0, 1, 2));
}
Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices,
                          std::vector<glm::uvec3>& obj_faces) const
{
	// generate basic cube for now
	// diametrically oppposite corners should be (m,m,m) and (M,M,M) where m = -0.5 and M = 0.5

	generate_cube(-0.5f, 0.5f, obj_vertices, obj_faces);


}






