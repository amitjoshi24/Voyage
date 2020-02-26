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
	vertices.push_back(glm::vec4(m, M, m, 1.0f)); // O 0
	vertices.push_back(glm::vec4(m, m, m, 1.0f)); // A 1
	vertices.push_back(glm::vec4(M, M, m, 1.0f)); // B 2
	vertices.push_back(glm::vec4(M, m, m, 1.0f)); // C 3

	vertices.push_back(glm::vec4(m, M, M, 1.0f)); // backO 4
	vertices.push_back(glm::vec4(m, m, M, 1.0f)); //backA 5
	vertices.push_back(glm::vec4(M, M, M, 1.0f)); //backB 6
	vertices.push_back(glm::vec4(M, m, M, 1.0f)); //backC 7

	indices.push_back(glm::uvec3(0, 1, 2));
	indices.push_back(glm::uvec3(1, 3, 2));

	indices.push_back(glm::uvec3(3, 7, 6));
	indices.push_back(glm::uvec3(6, 2, 3));
	
	indices.push_back(glm::uvec3(2, 6, 4));
	indices.push_back(glm::uvec3(4, 0, 2));  
	
	indices.push_back(glm::uvec3(1, 0, 4));
	indices.push_back(glm::uvec3(4, 5, 0));  
	
	indices.push_back(glm::uvec3(4, 6, 7));
	indices.push_back(glm::uvec3(7, 5, 4));  
	
	indices.push_back(glm::uvec3(1, 5, 7));
	indices.push_back(glm::uvec3(7, 3, 1));  
	


}
Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices,
                          std::vector<glm::uvec3>& obj_faces) const
{
	// generate basic cube for now
	// diametrically oppposite corners should be (m,m,m) and (M,M,M) where m = -0.5 and M = 0.5

	generate_cube(-0.5f, 0.5f, obj_vertices, obj_faces);


}






