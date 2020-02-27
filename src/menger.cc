#include "menger.h"
#include <iostream>
#include <string>
#include <cmath>        // std::abs
using namespace std;


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
void generate_cube(float s, glm::vec3 min, std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces){
	glm::vec3 sVec(s, s, s);

	glm::vec3 max = min + sVec;
	int pastSize = obj_vertices.size();

	obj_vertices.push_back(glm::vec4(min[0], max[1], min[2], 1.0f));
	obj_vertices.push_back(glm::vec4(min[0], min[1], min[2], 1.0f));

	obj_vertices.push_back(glm::vec4(max[0], max[1], min[2], 1.0f));
	obj_vertices.push_back(glm::vec4(max[0], min[1], min[2], 1.0f));

	obj_vertices.push_back(glm::vec4(min[0], max[1], max[2], 1.0f));
	obj_vertices.push_back(glm::vec4(min[0], min[1], max[2], 1.0f));

	obj_vertices.push_back(glm::vec4(max[0], max[1], max[2], 1.0f));
	obj_vertices.push_back(glm::vec4(max[0], min[1], max[2], 1.0f));


	/*obj_vertices.push_back(glm::vec4(m, M, m, 1.0f)); // O 0
	obj_vertices.push_back(glm::vec4(m, m, m, 1.0f)); // A 1
	obj_vertices.push_back(glm::vec4(M, M, m, 1.0f)); // B 2
	obj_vertices.push_back(glm::vec4(M, m, m, 1.0f)); // C 3

	obj_vertices.push_back(glm::vec4(m, M, M, 1.0f)); // backO 4
	obj_vertices.push_back(glm::vec4(m, m, M, 1.0f)); //backA 5
	obj_vertices.push_back(glm::vec4(M, M, M, 1.0f)); //backB 6
	obj_vertices.push_back(glm::vec4(M, m, M, 1.0f)); //backC 7
	*/

	

	obj_faces.push_back(glm::uvec3(pastSize + 0, pastSize + 1, pastSize + 2));
	obj_faces.push_back(glm::uvec3(pastSize + 1, pastSize + 3, pastSize + 2));

	obj_faces.push_back(glm::uvec3(pastSize + 3, pastSize + 7, pastSize + 6));
	obj_faces.push_back(glm::uvec3(pastSize + 6, pastSize + 2, pastSize + 3));
	
	obj_faces.push_back(glm::uvec3(pastSize + 2, pastSize + 6, pastSize + 4));
	obj_faces.push_back(glm::uvec3(pastSize + 4, pastSize + 0, pastSize + 2));  
	
	obj_faces.push_back(glm::uvec3(pastSize + 1, pastSize + 0, pastSize + 4));
	obj_faces.push_back(glm::uvec3(pastSize + 4, pastSize + 5, pastSize + 1));  
	
	obj_faces.push_back(glm::uvec3(pastSize + 4, pastSize + 6, pastSize + 7));
	obj_faces.push_back(glm::uvec3(pastSize + 7, pastSize + 5, pastSize + 4));  
	
	obj_faces.push_back(glm::uvec3(pastSize + 1, pastSize + 5, pastSize + 7));
	obj_faces.push_back(glm::uvec3(pastSize + 7, pastSize + 3, pastSize + 1));  
	
}
void check(std::vector<glm::vec4>& obj_vertices){
	for(unsigned int i = 0; i < obj_vertices.size(); i++){
		glm::vec4 curVertex = obj_vertices.at(i);
		for(unsigned int j = 0; j < 3; j++){
			if(abs(curVertex[j]) > 0.5){
				cout << "WHAT THE FUCK\n";
			}
		}
	}

}
void Menger::generate_geometry_helper(float s, glm::dvec3 min, std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces, int curDepth) const{
	//std::cout << "nestinglevel: " << nesting_level_ << std::endl;
	if(curDepth == nesting_level_){
		generate_cube(s, min, obj_vertices, obj_faces);
		//std::cout << "obj_faces.size(): " << obj_faces.size() << std::endl;
	}
	else{
		float newS = (float)(s/3);
		for(int i = 0; i < 3; i++){
			int forI = (int)(i==1);
			for(int j = 0; j < 3; j++){
				int forJ = (int)(j==1);
				for(int k = 0; k < 3; k++){
					int forK = (int)(k==1);
					int sumStrikes = forI + forJ + forK;
					if(sumStrikes < 2){
						//only if this subcube is in the center of less than 2 axes, then recur
						glm::dvec3 deltaFromMin(i*newS, j*newS, k*newS);
						glm::dvec3 newMin = min + deltaFromMin;
						generate_geometry_helper(newS, newMin, obj_vertices, obj_faces, curDepth + 1);
					}
				}
			}
		}
	}
}
void Menger::generate_geometry(std::vector<glm::vec4>& obj_vertices,
                          std::vector<glm::uvec3>& obj_faces) 
{
	obj_vertices.clear();
	obj_faces.clear();
	// generate basic cube for now
	// diametrically oppposite corners should be (m,m,m) and (M,M,M) where m = -0.5 and M = 0.5
	//std::cout << "litty" << endl;
	//generate_cube(1.0f, glm::dvec3(-0.5f, -0.5f, -0.5f), obj_vertices, obj_faces);
	this->obj_vertices = obj_vertices;
	this->obj_faces = obj_faces;
	generate_geometry_helper(1.0f, glm::dvec3(-0.5f, -0.5f, -0.5f), obj_vertices, obj_faces, 0);
	check(obj_vertices);


}






