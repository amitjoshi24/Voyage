#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <stdlib.h>
#include <math.h>       /* cos */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL library includes
#include <GL/glew.h>
#include <chrono>
using namespace std::chrono;
#include <GLFW/glfw3.h>
#include <debuggl.h>
#include "menger.h"
#include "camera.h"
#include "shaders.h"


//#include "stb_image.h"
int window_width = 800, window_height = 600;

// VBO and VAO descriptors.
enum { kVertexBuffer, kIndexBuffer, kNumVbos };

// These are our VAOs.
enum { kGeometryVao, kFloorVao, kWireframeVao, kOrbVao, KBoatVao, kSkyboxVao, kNumVaos };

GLuint g_array_objects[kNumVaos];  // This will store the VAO descriptors.
GLuint g_buffer_objects[kNumVaos][kNumVbos];  // These will store VBO descriptors.



void
CreateFloorTriangles(std::vector<glm::vec4>& vertices,
	std::vector<glm::uvec3>& indices){
	float inf = 20.0f;
	vertices.push_back(glm::vec4(0.0f, -2.0f, 0.0f, 1.0f));
	vertices.push_back(glm::vec4(-inf, -2.0f, -inf, 1.0f));
	vertices.push_back(glm::vec4(-inf, -2.0f, inf, 1.0f));
	vertices.push_back(glm::vec4(inf, -2.0f, -inf, 1.0f));
	vertices.push_back(glm::vec4(inf, -2.0f, inf, 1.0f));
	indices.push_back(glm::uvec3(0, 2, 1));
	indices.push_back(glm::uvec3(0, 4, 2));
	indices.push_back(glm::uvec3(0, 3, 4));
	indices.push_back(glm::uvec3(0, 1, 3));
}

void CreateSphere(std::vector<glm::vec4>& vertices,
	std::vector<glm::uvec3>& faces){

	float t = (1.0 + sqrt(5.0)) / 2.0;
	float k = 4;

	vertices.push_back((1/k)*glm::vec4(-1,  t, 0.0, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4( 1,  t, 0.0, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4(-1, -t, 0.0, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4( 1, -t, 0.0, k * 1.0f));

	vertices.push_back((1/k)*glm::vec4( 0, -1, t, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4( 0,  1, t, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4( 0, -1, -t, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4( 0,  1, -t, k * 1.0f));

	vertices.push_back((1/k)*glm::vec4( t,  0, -1, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4( t,  0, 1, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4(-t,  0, -1, k * 1.0f));
	vertices.push_back((1/k)*glm::vec4(-t,  0, 1, k * 1.0f));

//create faces
	faces.push_back(glm::uvec3(0, 11, 5));
	faces.push_back(glm::uvec3(0, 5, 1));
	faces.push_back(glm::uvec3(0, 1, 7));
	faces.push_back(glm::uvec3(0, 7, 10));
	faces.push_back(glm::uvec3(0, 10, 11));

	faces.push_back(glm::uvec3(1, 5, 9));
	faces.push_back(glm::uvec3(5, 11, 4));
	faces.push_back(glm::uvec3(11, 10, 2));
	faces.push_back(glm::uvec3(10, 7, 6));
	faces.push_back(glm::uvec3(7, 1, 8));

	faces.push_back(glm::uvec3(3, 9, 4));
	faces.push_back(glm::uvec3(3, 4, 2));
	faces.push_back(glm::uvec3(3, 2, 6));
	faces.push_back(glm::uvec3(3, 6, 8));
	faces.push_back(glm::uvec3(3, 8, 9));

	faces.push_back(glm::uvec3(4, 9, 5));
	faces.push_back(glm::uvec3(2, 4, 11));
	faces.push_back(glm::uvec3(6, 2, 10));
	faces.push_back(glm::uvec3(8, 6, 7));
	faces.push_back(glm::uvec3(9, 8, 1));
}
void
ReadObj(const std::string& file_name,
	std::vector<glm::vec4>& vertices,
	std::vector<glm::uvec4>& faces){

	std::string line;
	std::string v;
	std::string num;

	std::ifstream my_file(file_name);
	float k = 100;
	while(std::getline(my_file, line))
	{

		if (line.length() && line[0] == 'v')
		{

			std::istringstream get_indices(line);

			float temp[3];
			int i =0;
                std::getline(get_indices, num, ' '); //skip the v

                while (std::getline(get_indices, num, ' ')){
                	if ( i == 0)
                		i++;
                	else{
                		temp[i - 1] = std::atof(num.c_str());
                		i++;
                      //std::cout<<num<<std::endl;
                	}

                }
                vertices.push_back((1/k)*glm::vec4(temp[0], temp[2], temp[1],  k * 1.0f));

            }
            else if (line.length() && line[0] == 'f'){

            	std::istringstream get_indices(line);

            	int temp[4];
            	int i = 0;
            	int j = 0;

            	std::string thing;
                std::getline(get_indices, thing, ' '); //skip the f

                //get the v/v/v portions
                while (std::getline(get_indices, thing, ' ')){
                	std::istringstream get_num(thing);

                  //get first v
                	while (std::getline(get_num, num, '/')){
                    //skipping the texture and normals
                		if (j++ % 3 == 0){
                			temp[i++] = std::atoi(num.c_str()) - 1;
                      //std::cout<<num<<std::endl;
                		}

                	}
                }
                faces.push_back(glm::uvec4(temp[0], temp[1], temp[2], temp[3]));

            }
        }
    }
// FIXME: Save geometry to OBJ file
    void
    SaveObj(const std::string& file,
    	const std::vector<glm::vec4>& vertices,
    	const std::vector<glm::uvec3>& indices)
    {
    	std::ofstream outputFile(file);
    	for(unsigned int i = 0; i < vertices.size(); i++){
    		glm::vec4 curVertex = vertices.at(i);
    		outputFile << "v " << curVertex[0] << " " << curVertex[1] << " " << curVertex[2]<< std::endl;
    	}

    	for(unsigned int i = 0; i < indices.size(); i++){
    		glm::uvec3 curFace = indices.at(i);
    		outputFile << "f " << curFace[0] + 1<< " " << curFace[1] + 1<< " " << curFace[2] + 1 << std::endl;
		//outputFile << "f " << curFace[0] << " " << curFace[1] << " " << curFace[2]  << std::endl;
    	}
    	outputFile.close();
    }

    void
    SaveObj4(const std::string& file,
    	const std::vector<glm::vec4>& vertices,
    	const std::vector<glm::uvec4>& indices)
    {
    	std::ofstream outputFile(file);
    	for(unsigned int i = 0; i < vertices.size(); i++){
    		glm::vec4 curVertex = vertices.at(i);
    		outputFile << "v " << curVertex[0] << " " << curVertex[1] << " " << curVertex[2]<< std::endl;
    	}

    	for(unsigned int i = 0; i < indices.size(); i++){
    		glm::uvec4 curFace = indices.at(i);
    		outputFile << "f " << curFace[0] + 1<< " " << curFace[1] + 1<< " " << curFace[2] + 1 << " "<<curFace[3] + 1 <<std::endl;
    	}
    	outputFile.close();
    }


    void
    CreateFloor(std::vector<glm::vec4>& vertices, std::vector<glm::uvec4>& indices){
    	float inf = 20.0f;
    	float inc = 40.0f/16.0f;
    	int dotCounter = 0;
    	for(float j = -inf; j <= inf; j+=inc){
    		for(float i = -inf; i <= inf; i+=inc){
    			vertices.push_back(glm::vec4(i, -2.0f, j, 1.0f));
			dotCounter++; //one indexed
			if((dotCounter % 17) != 0 && j!= inf){ //not the right most dot, not the top most row
				auto temp = glm::uvec4(dotCounter - 1, dotCounter, dotCounter + 17, dotCounter + 16);
				indices.push_back(temp);
			}
		}
	}
	SaveObj4("../../thefloor.obj", vertices, indices);
}


void
ErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << "\n";
}
std::shared_ptr<Menger> g_menger;
void updateMengerStuff(int level){
	g_menger->set_nesting_level(level);
	g_menger->set_dirty();
}

Camera g_camera;
int outerTess = 3;
int innerTess = 2;
bool showOcean = true;
bool additiveBlending = false;
bool fps = false;
bool showWireframe = true;
bool showFloor = true;
float ocean_time;
std::chrono::steady_clock ocean_clock;
int tidal = 0;
int tidalX = 0;
glm::vec4 fixed_boat_position = glm::vec4(0.f, 1.0f, 0.0f, 1.0f);
float boatTheta = 0.0f;
glm::vec4 boat_direction = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
float boatSpeed = 0.1f;
float boatRotateSpeed = 0.1f;

void forward(){
	
}
void
KeyCallback(GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods)
{
	// Note:
	// This is only a list of functions to implement.
	// you may want to re-organize this piece of code.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_S && mods == GLFW_MOD_CONTROL && action == GLFW_RELEASE) {
		// FIXME: save geometry to OBJ
		SaveObj("geometry.obj", g_menger->obj_vertices, g_menger->obj_faces);
		std::cout<<"Saved it." <<std::endl;
	} else if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
		g_camera.forward();
	} else if (key == GLFW_KEY_S && action != GLFW_RELEASE) {
		g_camera.backward();
	} else if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
		g_camera.left();
	} else if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
		g_camera.right();
	} else if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {
		g_camera.counterclockwise();
	} else if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE) {
		g_camera.clockwise();
	} else if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE) {
		g_camera.down();
	} else if (key == GLFW_KEY_UP && action != GLFW_RELEASE) {
		g_camera.up();
	} else if (key == GLFW_KEY_C && action != GLFW_RELEASE) {
		// FIXME: FPS mode on/off
		fps = !fps;
		if(fps){
			g_camera.setPos(glm::vec3(fixed_boat_position), glm::vec3(boat_direction));
		}
	}
	else if (key == GLFW_KEY_Q && action != GLFW_RELEASE) {
		g_camera.defaultOrientation();
	}
	if (key == GLFW_KEY_J && action != GLFW_RELEASE) {
		boatTheta -= boatRotateSpeed;
		boat_direction = glm::vec4(cos(boatTheta)*1, boat_direction[1], sin(boatTheta), boat_direction[3]);
		boat_direction = glm::normalize(boat_direction);
		if(fps)
			g_camera.setPos(g_camera.eye, glm::vec3(boat_direction));
	}
	if (key == GLFW_KEY_L && action != GLFW_RELEASE) {
		boatTheta += boatRotateSpeed;
		boat_direction = glm::vec4(cos(boatTheta)*1, boat_direction[1], sin(boatTheta), boat_direction[3]);
		boat_direction = glm::normalize(boat_direction);
		if(fps)
			g_camera.setPos(g_camera.eye, glm::vec3(boat_direction));
	}
	else if (key == GLFW_KEY_I && action != GLFW_RELEASE) {
		fixed_boat_position += boatSpeed*boat_direction;
		if(fps)
			g_camera.setPos(glm::vec3(fixed_boat_position), glm::vec3(boat_direction));
	}
	else if (key == GLFW_KEY_K && action != GLFW_RELEASE) {
		fixed_boat_position -= boatSpeed*boat_direction;
		if(fps)
			g_camera.setPos(glm::vec3(fixed_boat_position), glm::vec3(boat_direction));
	}
	else if(key == GLFW_KEY_B && action != GLFW_RELEASE) {
		additiveBlending = !additiveBlending;
	}
	
	if (!g_menger)
		return ; // 0-4 only available in Menger mode.
	if (key == GLFW_KEY_0 && action != GLFW_RELEASE) {
		// FIXME: Change nesting level of g_menger
		// Note: GLFW_KEY_0 - 4 may not be continuous.
		updateMengerStuff(0);
	} else if (key == GLFW_KEY_1 && action != GLFW_RELEASE) {
		updateMengerStuff(1);
	} else if (key == GLFW_KEY_2 && action != GLFW_RELEASE) {
		updateMengerStuff(2);
	} else if (key == GLFW_KEY_3 && action != GLFW_RELEASE) {
		updateMengerStuff(3);
	} else if (key == GLFW_KEY_4 && action != GLFW_RELEASE) {
		updateMengerStuff(4);
	}
	else if (key == GLFW_KEY_F && mods == GLFW_MOD_CONTROL && action == GLFW_RELEASE){
		//show checkers or don't show checkers
		showFloor = !showFloor;
	}
	else if(key == GLFW_KEY_F && action == GLFW_RELEASE){
		//turn on wireframe
		showWireframe = !showWireframe;

	}
	else if (key == GLFW_KEY_O && mods == GLFW_MOD_CONTROL && action == GLFW_RELEASE){
		//turn on ocean
		showOcean = !showOcean;
	}
	else if (key == GLFW_KEY_T && mods == GLFW_MOD_CONTROL && action == GLFW_RELEASE){
		tidal = 1;
		tidalX = 0;
	}
	else if(key == GLFW_KEY_MINUS && action != GLFW_RELEASE){
		outerTess--;
		if (outerTess <= 0) outerTess = 1;
		std::cout<<outerTess<<std::endl;
	}
	else if(key == GLFW_KEY_EQUAL && action != GLFW_RELEASE){
		outerTess++;
		std::cout<<outerTess<<std::endl;
	}
	else if(key == GLFW_KEY_COMMA && action != GLFW_RELEASE){
		innerTess--;
		if (innerTess <= 0) innerTess = 1;
		std::cout<<innerTess<<std::endl;
	}
	else if(key == GLFW_KEY_PERIOD && action != GLFW_RELEASE){
		innerTess++;
		std::cout<<innerTess<<std::endl;
	}

}

int g_current_button;
bool g_mouse_pressed;
int prev_x;
int prev_y;
void
MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	if (!g_mouse_pressed){
		prev_x = -1;
		prev_y = -1;
		return;
	}
	if (g_current_button == GLFW_MOUSE_BUTTON_LEFT) {

		if (prev_x >= 0 && prev_y >= 0 && mouse_x > 0 && mouse_y > 0){
			g_camera.swivel(glm::vec2(mouse_x - prev_x, mouse_y - prev_y));

		}
		prev_x = mouse_x;
		prev_y = mouse_y;
		// FIXME: left drag
	} else if (g_current_button == GLFW_MOUSE_BUTTON_RIGHT) {
		// FIXME: middle drag, nothing for now
	} else if (g_current_button == GLFW_MOUSE_BUTTON_MIDDLE) {
		// FIXME: right drag, nothing for now
	}
}

void
MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	g_mouse_pressed = (action == GLFW_PRESS);
	g_current_button = button;
}

int main(int argc, char* argv[])
{
	//initializes camera
	g_camera.init();

	std::string window_title = "Menger";
	if (!glfwInit()) exit(EXIT_FAILURE);
	g_menger = std::make_shared<Menger>();
	glfwSetErrorCallback(ErrorCallback);

	// Ask an OpenGL 4.1 core profile context
	// It is required on OSX and non-NVIDIA Linux
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(window_width, window_height,
		&window_title[0], nullptr, nullptr);
	CHECK_SUCCESS(window != nullptr);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	CHECK_SUCCESS(glewInit() == GLEW_OK);
	glGetError();  // clear GLEW's error for it
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string

	std::cout << "OpenGL version supported:" << version << "\n";
//------------------------------------------------------------------------------
	std::vector<glm::vec4> obj_vertices;
	std::vector<glm::uvec3> obj_faces;

	std::vector<glm::vec4> floor_quad_vertices;
	std::vector<glm::uvec4> floor_quad_faces;

	std::vector<glm::vec4> floor_triangle_vertices;
	std::vector<glm::uvec3> floor_triangle_faces;

	std::vector<glm::vec4> sphere_vertices;
	std::vector<glm::uvec3> sphere_faces;

	std::vector<glm::vec4> boat_vertices;
	std::vector<glm::uvec4> boat_faces;

	auto start_time = ocean_clock.now();


//--------FLOOR INIT----------------------------------------------------------------------

	CreateFloor(floor_quad_vertices, floor_quad_faces);
	CreateFloorTriangles(floor_triangle_vertices, floor_triangle_faces);

//-------ORB INIT-------------------------------------------------------------------------
	CreateSphere(sphere_vertices, sphere_faces);

//-----------------------BOAT INIT----------------------------------------------
	ReadObj("../../wakeboard.obj", boat_vertices, boat_faces);
  //SaveObj4("../../myobj_save.obj", boat_vertices, boat_faces);
//-------------CUBE INIT-----------------------------------------------------------------
	g_menger->set_nesting_level(1);
	g_menger->generate_geometry(obj_vertices, obj_faces);

	glm::vec4 min_bounds = glm::vec4(std::numeric_limits<float>::max());
	glm::vec4 max_bounds = glm::vec4(-std::numeric_limits<float>::max());
	for (const auto& vert : obj_vertices) {
		min_bounds = glm::min(vert, min_bounds);
		max_bounds = glm::max(vert, max_bounds);
	}
	std::cout << "min_bounds = " << glm::to_string(min_bounds) << "\n";
	std::cout << "max_bounds = " << glm::to_string(max_bounds) << "\n";

	//-------SETUP THE VAO and like VBO SHIT-----------------------------------------------------------------------
	// Setup our VAO array.
	CHECK_GL_ERROR(glGenVertexArrays(kNumVaos, &g_array_objects[0]));

	//-----------setup cube VAO shit------------------

	// Switch to the VAO for Geometry.
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kGeometryVao]));

	// Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kGeometryVao][0]));

	// Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kVertexBuffer]));
	// NOTE: We do not send anything right now, we just describe it to OpenGL.
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * obj_vertices.size() * 4, obj_vertices.data(), GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * obj_faces.size() * 3, obj_faces.data(), GL_STATIC_DRAW));

	//-----------setup floor VAO shit------------------
	// Switch to the VAO for Floor
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kFloorVao]));

		// Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kFloorVao][0]));

	// Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kVertexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floor_triangle_vertices.size() * 4, floor_triangle_vertices.data(),GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * floor_triangle_faces.size() * 3, floor_triangle_faces.data(), GL_STATIC_DRAW));

	//-----------setup wireframe VAO shit------------------
	// Switch to the VAO for Geometry.
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kWireframeVao]));

		// Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kWireframeVao][0]));

	// Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kWireframeVao][kVertexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floor_quad_vertices.size() * 4, floor_quad_vertices.data(),GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kWireframeVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * floor_quad_faces.size() * 4, floor_quad_faces.data(), GL_STATIC_DRAW));

  //---------setup orb VAO-----------
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kOrbVao]));

		// Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kOrbVao][0]));

	// Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kOrbVao][kVertexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere_vertices.size() * 4, sphere_vertices.data(),GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kOrbVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * sphere_faces.size() * 3, sphere_faces.data(), GL_STATIC_DRAW));

  //---------setup boat VAO-----------
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[KBoatVao]));

    // Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[KBoatVao][0]));

  // Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[KBoatVao][kVertexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * boat_vertices.size() * 4, boat_vertices.data(),GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

  // Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[KBoatVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * boat_faces.size() * 4, boat_faces.data(), GL_STATIC_DRAW));

	//----set up all the shaders--------------------------------------------------------------------------
	// Setup vertex shader.
	GLuint vertex_shader_id = 0;
	const char* vertex_source_pointer = vertex_shader;
	CHECK_GL_ERROR(vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	CHECK_GL_ERROR(glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, nullptr));
	glCompileShader(vertex_shader_id);
	CHECK_GL_SHADER_ERROR(vertex_shader_id);

	// Setup geometry shader.
	GLuint geometry_shader_id = 0;
	const char* geometry_source_pointer = geometry_shader;
	CHECK_GL_ERROR(geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER));
	CHECK_GL_ERROR(glShaderSource(geometry_shader_id, 1, &geometry_source_pointer, nullptr));
	glCompileShader(geometry_shader_id);
	CHECK_GL_SHADER_ERROR(geometry_shader_id);

  // Setup fragment shader.
	GLuint fragment_shader_id = 0;
	const char* fragment_source_pointer = fragment_shader;
	CHECK_GL_ERROR(fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, nullptr));
	glCompileShader(fragment_shader_id);
	CHECK_GL_SHADER_ERROR(fragment_shader_id);

	// Setup cube_fragment shader.
	GLuint cube_fragment_shader_id = 0;
	const char* cube_fragment_source_pointer = cube_fragment_shader;
	CHECK_GL_ERROR(cube_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(cube_fragment_shader_id, 1, &cube_fragment_source_pointer, nullptr));
	glCompileShader(cube_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(cube_fragment_shader_id);

	// Setup floor_fragment shader.
	GLuint floor_fragment_shader_id = 0;
	const char* floor_fragment_source_pointer = floor_fragment_shader;
	CHECK_GL_ERROR(floor_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(floor_fragment_shader_id, 1, &floor_fragment_source_pointer, nullptr));
	glCompileShader(floor_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(floor_fragment_shader_id);

    // set up tesselation control shader
	GLuint tesselation_control_shader_id = 0;
	const char* tesselation_control_source_pointer = tesselation_control_shader;
	CHECK_GL_ERROR(tesselation_control_shader_id = glCreateShader(GL_TESS_CONTROL_SHADER));
	CHECK_GL_ERROR(glShaderSource(tesselation_control_shader_id, 1, &tesselation_control_source_pointer, nullptr));
	glCompileShader(tesselation_control_shader_id);
	CHECK_GL_SHADER_ERROR(tesselation_control_shader_id);

    // set up wireframe tesselation evaluation shader
	GLuint floor_wireframe_tesselation_evaluation_shader_id = 0;
	const char* floor_wireframe_tesselation_evaluation_source_pointer = floor_wireframe_tesselation_evaluation_shader;
	CHECK_GL_ERROR(floor_wireframe_tesselation_evaluation_shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER));
	CHECK_GL_ERROR(glShaderSource(floor_wireframe_tesselation_evaluation_shader_id, 1, &floor_wireframe_tesselation_evaluation_source_pointer, nullptr));
	glCompileShader(floor_wireframe_tesselation_evaluation_shader_id);
	CHECK_GL_SHADER_ERROR(floor_wireframe_tesselation_evaluation_shader_id);

		// Setup floor_wireframe geometry shader.
	GLuint floor_wireframe_geometry_shader_id = 0;
	const char* floor_wireframe_geometry_source_pointer = floor_wireframe_geometry_shader;
	CHECK_GL_ERROR(floor_wireframe_geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER));
	CHECK_GL_ERROR(glShaderSource(floor_wireframe_geometry_shader_id, 1, &floor_wireframe_geometry_source_pointer, nullptr));
	glCompileShader(floor_wireframe_geometry_shader_id);
	CHECK_GL_SHADER_ERROR(floor_wireframe_geometry_shader_id);

		// Setup floor_wireframe fragment shader.
	GLuint floor_wireframe_fragment_shader_id = 0;
	const char* floor_wireframe_fragment_source_pointer = floor_wireframe_fragment_shader;
	CHECK_GL_ERROR(floor_wireframe_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(floor_wireframe_fragment_shader_id, 1, &floor_wireframe_fragment_source_pointer, nullptr));
	glCompileShader(floor_wireframe_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(floor_wireframe_fragment_shader_id);

		// set up tesselation control shader
	GLuint ocean_tesselation_control_shader_id = 0;
	const char* ocean_tesselation_control_source_pointer = ocean_tesselation_control_shader;
	CHECK_GL_ERROR(ocean_tesselation_control_shader_id = glCreateShader(GL_TESS_CONTROL_SHADER));
	CHECK_GL_ERROR(glShaderSource(ocean_tesselation_control_shader_id, 1, &ocean_tesselation_control_source_pointer, nullptr));
	glCompileShader(ocean_tesselation_control_shader_id);
	CHECK_GL_SHADER_ERROR(ocean_tesselation_control_shader_id);

		// set up ocean tesselation evaluation shader
	GLuint ocean_tesselation_evaluation_shader_id = 0;
	const char* ocean_tesselation_evaluation_source_pointer = ocean_tesselation_evaluation_shader;
	CHECK_GL_ERROR(ocean_tesselation_evaluation_shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER));
	CHECK_GL_ERROR(glShaderSource(ocean_tesselation_evaluation_shader_id, 1, &ocean_tesselation_evaluation_source_pointer, nullptr));
	glCompileShader(ocean_tesselation_evaluation_shader_id);
	CHECK_GL_SHADER_ERROR(ocean_tesselation_evaluation_shader_id);

		// Setup ocean geometry shader.
	GLuint ocean_geometry_shader_id = 0;
	const char* ocean_geometry_source_pointer = ocean_geometry_shader;
	CHECK_GL_ERROR(ocean_geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER));
	CHECK_GL_ERROR(glShaderSource(ocean_geometry_shader_id, 1, &ocean_geometry_source_pointer, nullptr));
	glCompileShader(ocean_geometry_shader_id);
	CHECK_GL_SHADER_ERROR(ocean_geometry_shader_id);

		// Setup ocean fragment shader.
	GLuint ocean_fragment_shader_id = 0;
	const char* ocean_fragment_source_pointer = ocean_fragment_shader;
	CHECK_GL_ERROR(ocean_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(ocean_fragment_shader_id, 1, &ocean_fragment_source_pointer, nullptr));
	glCompileShader(ocean_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(ocean_fragment_shader_id);

    // Setup orb vertex shader.
	GLuint orb_vertex_shader_id = 0;
	const char* orb_vertex_source_pointer = orb_vertex_shader;
	CHECK_GL_ERROR(orb_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	CHECK_GL_ERROR(glShaderSource(orb_vertex_shader_id, 1, &orb_vertex_source_pointer, nullptr));
	glCompileShader(orb_vertex_shader_id);
	CHECK_GL_SHADER_ERROR(orb_vertex_shader_id);

    // Setup boat vertex shader.
	GLuint boat_vertex_shader_id = 0;
	const char* boat_vertex_source_pointer = boat_vertex_shader;
	CHECK_GL_ERROR(boat_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	CHECK_GL_ERROR(glShaderSource(boat_vertex_shader_id, 1, &boat_vertex_source_pointer, nullptr));
	glCompileShader(boat_vertex_shader_id);
	CHECK_GL_SHADER_ERROR(boat_vertex_shader_id);

    // Setup boat fragment shader.
	GLuint boat_fragment_shader_id = 0;
	const char* boat_fragment_source_pointer = boat_fragment_shader;
	CHECK_GL_ERROR(boat_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(boat_fragment_shader_id, 1, &boat_fragment_source_pointer, nullptr));
	glCompileShader(boat_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(boat_fragment_shader_id);

    // Setup depth boat vertex shader
	GLuint depth_boat_vertex_shader_id = 0;
	const char* depth_boat_vertex_source_pointer = depth_boat_vertex_shader;
	CHECK_GL_ERROR(depth_boat_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	CHECK_GL_ERROR(glShaderSource(depth_boat_vertex_shader_id, 1, &depth_boat_vertex_source_pointer, nullptr));
	glCompileShader(depth_boat_vertex_shader_id);
	CHECK_GL_SHADER_ERROR(depth_boat_vertex_shader_id);

	// Setup depth boat tesselation control shader
	GLuint depth_boat_tcs_shader_id = 0;
	const char* depth_boat_tcs_source_pointer = depth_boat_tesselation_control_shader;
	CHECK_GL_ERROR(depth_boat_tcs_shader_id = glCreateShader(GL_TESS_CONTROL_SHADER));
	CHECK_GL_ERROR(glShaderSource(depth_boat_tcs_shader_id, 1, &depth_boat_tcs_source_pointer, nullptr));
	glCompileShader(depth_boat_tcs_shader_id);
	CHECK_GL_SHADER_ERROR(depth_boat_tcs_shader_id);

	// Setup depth boat tesselation evaluation shader
	GLuint depth_boat_tes_shader_id = 0;
	const char* depth_boat_tes_source_pointer = depth_boat_tesselation_evaluation_shader;
	CHECK_GL_ERROR(depth_boat_tes_shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER));
	CHECK_GL_ERROR(glShaderSource(depth_boat_tes_shader_id, 1, &depth_boat_tes_source_pointer, nullptr));
	glCompileShader(depth_boat_tes_shader_id);
	CHECK_GL_SHADER_ERROR(depth_boat_tes_shader_id);

    // Setup depth boat geometry shader
	GLuint depth_boat_geometry_shader_id = 0;
	const char* depth_boat_geometry_source_pointer = depth_boat_geometry_shader;
	CHECK_GL_ERROR(depth_boat_geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER));
	CHECK_GL_ERROR(glShaderSource(depth_boat_geometry_shader_id, 1, &depth_boat_geometry_source_pointer, nullptr));
	glCompileShader(depth_boat_geometry_shader_id);
	CHECK_GL_SHADER_ERROR(depth_boat_geometry_shader_id);

    // Setup depth boat fragment shader.
	GLuint depth_boat_fragment_shader_id = 0;
	const char* depth_boat_fragment_source_pointer = depth_boat_fragment_shader;
	CHECK_GL_ERROR(depth_boat_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(depth_boat_fragment_shader_id, 1, &depth_boat_fragment_source_pointer, nullptr));
	glCompileShader(depth_boat_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(depth_boat_fragment_shader_id);




	//----create programs--------------------------------------------------------------------------
	// create cube program
	GLuint cube_program_id = 0;
	CHECK_GL_ERROR(cube_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(cube_program_id, vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(cube_program_id, geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(cube_program_id, cube_fragment_shader_id));

	//create floor program
	GLuint floor_program_id = 0;
	CHECK_GL_ERROR(floor_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(floor_program_id, vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_program_id, geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_program_id, floor_fragment_shader_id));

	//create wireframe program
	GLuint floor_wireframe_program_id = 0;
	CHECK_GL_ERROR(floor_wireframe_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, ocean_tesselation_control_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, ocean_tesselation_evaluation_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, floor_wireframe_geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, floor_wireframe_fragment_shader_id));

	//create ocean program
	GLuint ocean_program_id = 0;
	CHECK_GL_ERROR(ocean_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, ocean_tesselation_control_shader_id));
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, ocean_tesselation_evaluation_shader_id));
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, ocean_geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, ocean_fragment_shader_id));


  //create orb program
	GLuint orb_program_id = 0;
	CHECK_GL_ERROR(orb_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(orb_program_id, orb_vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(orb_program_id, geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(orb_program_id, fragment_shader_id));

  //create boat program
	GLuint boat_program_id= 0;
	CHECK_GL_ERROR(boat_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(boat_program_id, boat_vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(boat_program_id, ocean_tesselation_control_shader_id));
	CHECK_GL_ERROR(glAttachShader(boat_program_id, ocean_tesselation_evaluation_shader_id));
	CHECK_GL_ERROR(glAttachShader(boat_program_id, geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(boat_program_id, boat_fragment_shader_id));

  //create boat depth program
	GLuint depth_boat_program_id=0;
	CHECK_GL_ERROR(depth_boat_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(depth_boat_program_id, depth_boat_vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(depth_boat_program_id, depth_boat_tcs_shader_id));
	CHECK_GL_ERROR(glAttachShader(depth_boat_program_id, depth_boat_tes_shader_id));
	CHECK_GL_ERROR(glAttachShader(depth_boat_program_id, depth_boat_geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(depth_boat_program_id, depth_boat_fragment_shader_id));

	//----set up variables and link programs--------------------------------------------------------------------------

	//set up cube program variables-----------------
	// Bind attributes.
	CHECK_GL_ERROR(glBindAttribLocation(cube_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(cube_program_id, 0, "fragment_color"));
	glLinkProgram(cube_program_id);
	CHECK_GL_PROGRAM_ERROR(cube_program_id);

	// Get the uniform locations.
	GLint cube_projection_matrix_location = 0;
	CHECK_GL_ERROR(cube_projection_matrix_location =	glGetUniformLocation(cube_program_id, "projection"));
	GLint cube_view_matrix_location = 0;
	CHECK_GL_ERROR(cube_view_matrix_location = glGetUniformLocation(cube_program_id, "view"));
	GLint cube_light_position_location = 0;
	CHECK_GL_ERROR(cube_light_position_location = glGetUniformLocation(cube_program_id, "light_position"));

	//set up floor program variables------------
	// Bind attributes.
	CHECK_GL_ERROR(glBindAttribLocation(floor_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(floor_program_id, 0, "fragment_color"));
	glLinkProgram(floor_program_id);
	CHECK_GL_PROGRAM_ERROR(floor_program_id);

	// Get the uniform locations.
	GLint floor_projection_matrix_location = 0;
	CHECK_GL_ERROR(floor_projection_matrix_location = glGetUniformLocation(floor_program_id, "projection"));
	GLint floor_view_matrix_location = 0;
	CHECK_GL_ERROR(floor_view_matrix_location = glGetUniformLocation(floor_program_id, "view"));
	GLint floor_light_position_location = 0;
	CHECK_GL_ERROR(floor_light_position_location = glGetUniformLocation(floor_program_id, "light_position"));
	GLint floor_showOcean_location = 0;
	CHECK_GL_ERROR(floor_showOcean_location = glGetUniformLocation(floor_program_id, "showOcean"));
	GLint floor_time_location = 0;
	CHECK_GL_ERROR(floor_time_location = glGetUniformLocation(floor_program_id, "ocean_time"));
	GLint floor_tidal_location = 0;
	CHECK_GL_ERROR(floor_tidal_location = glGetUniformLocation(floor_program_id, "tidal"));
	GLint floor_tidalX_location = 0;
	CHECK_GL_ERROR(floor_tidalX_location = glGetUniformLocation(floor_program_id, "tidalX"));

	//set up wireframe program variables--------------
	CHECK_GL_ERROR(glBindAttribLocation(floor_wireframe_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(floor_wireframe_program_id, 0, "fragment_color"));
	glLinkProgram(floor_wireframe_program_id);
	CHECK_GL_PROGRAM_ERROR(floor_wireframe_program_id);

	GLint floor_wireframe_projection_matrix_location = 0;
	CHECK_GL_ERROR(floor_wireframe_projection_matrix_location =	glGetUniformLocation(floor_wireframe_program_id, "projection"));
	GLint floor_wireframe_view_matrix_location = 0;
	CHECK_GL_ERROR(floor_wireframe_view_matrix_location = glGetUniformLocation(floor_wireframe_program_id, "view"));
	GLint floor_wireframe_light_position_location = 0;
	CHECK_GL_ERROR(floor_wireframe_light_position_location =	glGetUniformLocation(floor_wireframe_program_id, "light_position"));
	GLint outerTess_location = 0;
	CHECK_GL_ERROR(outerTess_location = glGetUniformLocation(floor_wireframe_program_id, "outerTess"));
	GLint innerTess_location = 0;
	CHECK_GL_ERROR(innerTess_location = glGetUniformLocation(floor_wireframe_program_id, "innerTess"));
	GLint floor_wireframe_showOcean_location = 0;
	CHECK_GL_ERROR(floor_wireframe_showOcean_location = glGetUniformLocation(floor_wireframe_program_id, "showOcean"));
	GLint floor_wireframe_time_location = 0;
	CHECK_GL_ERROR(floor_wireframe_time_location = glGetUniformLocation(floor_wireframe_program_id, "ocean_time"));
	GLint floor_wireframe_tidal_location = 0;
	CHECK_GL_ERROR(floor_wireframe_tidal_location = glGetUniformLocation(floor_wireframe_program_id, "tidal"));
	GLint floor_wireframe_tidalX_location = 0;
	CHECK_GL_ERROR(floor_wireframe_tidalX_location = glGetUniformLocation(floor_wireframe_program_id, "tidalX"));
	GLint floor_wireframe_camera_pos_location = 0;
	CHECK_GL_ERROR(floor_wireframe_camera_pos_location = glGetUniformLocation(floor_wireframe_program_id, "camera_pos"));

	//set up ocean program variables-----------------
	// Bind attributes.
	CHECK_GL_ERROR(glBindAttribLocation(ocean_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(ocean_program_id, 0, "fragment_color"));
	glLinkProgram(ocean_program_id);
	CHECK_GL_PROGRAM_ERROR(ocean_program_id);

	// Get the uniform locations.
	GLint ocean_projection_matrix_location = 0;
	CHECK_GL_ERROR(ocean_projection_matrix_location =	glGetUniformLocation(ocean_program_id, "projection"));
	GLint ocean_view_matrix_location = 0;
	CHECK_GL_ERROR(ocean_view_matrix_location = glGetUniformLocation(ocean_program_id, "view"));
	GLint ocean_light_position_location = 0;
	CHECK_GL_ERROR(ocean_light_position_location = glGetUniformLocation(ocean_program_id, "light_position"));
	GLint ocean_outerTess_location = 0;
	CHECK_GL_ERROR(ocean_outerTess_location = glGetUniformLocation(ocean_program_id, "outerTess"));
	GLint ocean_innerTess_location = 0;
	CHECK_GL_ERROR(ocean_innerTess_location = glGetUniformLocation(ocean_program_id, "innerTess"));
	GLint time_location = 0;
	CHECK_GL_ERROR(time_location = glGetUniformLocation(ocean_program_id, "ocean_time"));
	GLint tidal_location = 0;
	CHECK_GL_ERROR(tidal_location = glGetUniformLocation(ocean_program_id, "tidal"));
	GLint tidalX_location = 0;
	CHECK_GL_ERROR(tidalX_location = glGetUniformLocation(ocean_program_id, "tidalX"));
	GLint camera_pos_location = 0;
	CHECK_GL_ERROR(camera_pos_location = glGetUniformLocation(ocean_program_id, "camera_pos"));
	GLint ocean_showOcean_location = 0;
	CHECK_GL_ERROR(ocean_showOcean_location = glGetUniformLocation(ocean_program_id, "showOcean"));
	GLint ocean_additiveBlending_location = 0;
	CHECK_GL_ERROR(ocean_additiveBlending_location = glGetUniformLocation(ocean_program_id, "additiveBlending"));
  //set up orb program variables------------
	// Bind attributes.
	CHECK_GL_ERROR(glBindAttribLocation(orb_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(orb_program_id, 0, "fragment_color"));
	glLinkProgram(orb_program_id);
	CHECK_GL_PROGRAM_ERROR(orb_program_id);

	// Get the uniform locations.
	GLint orb_projection_matrix_location = 0;
	CHECK_GL_ERROR(orb_projection_matrix_location = glGetUniformLocation(orb_program_id, "projection"));
	GLint orb_view_matrix_location = 0;
	CHECK_GL_ERROR(orb_view_matrix_location = glGetUniformLocation(orb_program_id, "view"));
	GLint orb_light_position_location = 0;
	CHECK_GL_ERROR(orb_light_position_location = glGetUniformLocation(orb_program_id, "light_position"));

  //------------set up boat
  //set up wireframe program variables--------------
	CHECK_GL_ERROR(glBindAttribLocation(boat_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(boat_program_id, 0, "fragment_color"));
	glLinkProgram(boat_program_id);
	CHECK_GL_PROGRAM_ERROR(boat_program_id);

	GLint boat_projection_matrix_location = 0;
	CHECK_GL_ERROR(boat_projection_matrix_location =	glGetUniformLocation(boat_program_id, "projection"));
	GLint boat_view_matrix_location = 0;
	CHECK_GL_ERROR(boat_view_matrix_location = glGetUniformLocation(boat_program_id, "view"));
	GLint boat_light_position_location = 0;
	CHECK_GL_ERROR(boat_light_position_location =	glGetUniformLocation(boat_program_id, "light_position"));
	GLint boat_translate_by_location = 0;
	CHECK_GL_ERROR(boat_translate_by_location =	glGetUniformLocation(boat_program_id, "translate_by"));
	GLint boat_outerTess_location = 0;
	CHECK_GL_ERROR(boat_outerTess_location = glGetUniformLocation(boat_program_id, "outerTess"));
	GLint boat_innerTess_location = 0;
	CHECK_GL_ERROR(boat_innerTess_location = glGetUniformLocation(boat_program_id, "innerTess"));
	GLint boat_showOcean_location = 0;
	CHECK_GL_ERROR(boat_showOcean_location = glGetUniformLocation(boat_program_id, "showOcean"));
	GLint boat_time_location = 0;
	CHECK_GL_ERROR(boat_time_location = glGetUniformLocation(boat_program_id, "ocean_time"));
	GLint boat_tidal_location = 0;
	CHECK_GL_ERROR(boat_tidal_location = glGetUniformLocation(boat_program_id, "tidal"));
	GLint boat_tidalX_location = 0;
	CHECK_GL_ERROR(boat_tidalX_location = glGetUniformLocation(boat_program_id, "tidalX"));
	GLint boat_camera_pos_location = 0;
	CHECK_GL_ERROR(boat_camera_pos_location = glGetUniformLocation(boat_program_id, "camera_pos"));
	GLint boat_theta_location = 0;
	CHECK_GL_ERROR(boat_theta_location = glGetUniformLocation(boat_program_id, "boatTheta"));

  //----------set up depth boat
  //set up wireframe program variables--------------
	CHECK_GL_ERROR(glBindAttribLocation(depth_boat_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(depth_boat_program_id, 0, "fragment_color"));
	glLinkProgram(depth_boat_program_id);
	CHECK_GL_PROGRAM_ERROR(depth_boat_program_id);

	GLint depth_boat_light_position_location = 0;
	CHECK_GL_ERROR(depth_boat_light_position_location =	glGetUniformLocation(depth_boat_program_id, "light_position"));
	GLint depth_boat_translate_by_location = 0;
	CHECK_GL_ERROR(depth_boat_translate_by_location =	glGetUniformLocation(depth_boat_program_id, "translate_by"));
	GLint depth_boat_theta_location = 0;
	CHECK_GL_ERROR(depth_boat_theta_location = glGetUniformLocation(depth_boat_program_id, "boatTheta"));
	GLint depth_boat_far_plane_location = 0;
	CHECK_GL_ERROR(depth_boat_far_plane_location = glGetUniformLocation(depth_boat_program_id, "far_plane"));

	GLint depth_boat_shadow_matrix1_location = 0;
	CHECK_GL_ERROR(depth_boat_shadow_matrix1_location =	glGetUniformLocation(depth_boat_program_id, "shadowMatrix1"));
	GLint depth_boat_shadow_matrix2_location = 0;
	CHECK_GL_ERROR(depth_boat_shadow_matrix2_location =	glGetUniformLocation(depth_boat_program_id, "shadowMatrix2"));
	GLint depth_boat_shadow_matrix3_location = 0;
	CHECK_GL_ERROR(depth_boat_shadow_matrix3_location =	glGetUniformLocation(depth_boat_program_id, "shadowMatrix3"));
	GLint depth_boat_shadow_matrix4_location = 0;
	CHECK_GL_ERROR(depth_boat_shadow_matrix4_location =	glGetUniformLocation(depth_boat_program_id, "shadowMatrix4"));
	GLint depth_boat_shadow_matrix5_location = 0;
	CHECK_GL_ERROR(depth_boat_shadow_matrix5_location =	glGetUniformLocation(depth_boat_program_id, "shadowMatrix5"));
	GLint depth_boat_shadow_matrix6_location = 0;
	CHECK_GL_ERROR(depth_boat_shadow_matrix6_location =	glGetUniformLocation(depth_boat_program_id, "shadowMatrix6"));
	




	//----init some vars we need--------------------------------------------------------------------------

	//relocated light position
	glm::vec4 light_position = glm::vec4(-6.0f, 10.0f, 0.0f, 1.0f);

	float aspect = 0.0f;
	float theta = 0.0f;

	//-----shadow mapping setup---------------------------------------------------------------------------

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);  

	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL); 
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  

	float shadowAspect = (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT;
	float shadowNear = 1.0f;
	float shadowFar = 25.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), shadowAspect, shadowNear, shadowFar); 
	std::vector<glm::mat4> shadowTransforms;
	glm::vec3 lightPos(light_position);
	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * 
		glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));


	//LOOP DE DOOP----------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		
		






		glm::vec4 boat_position = fixed_boat_position;
		//std::cout << "boat_position[0]: " << boat_position[0] << std::endl;
		//UPDATE MATRICES-----------------------------------------------------------
		    //Finding height
		float h = 0.0f;
		if (showOcean == 1){
			float t = ocean_time;
			float x = boat_position[0];
			float z = boat_position[2];
			glm::vec2 pos = glm::vec2(x, z);

			float wave1 [6];
			wave1[0] = 8.0f; //wavelength
 			wave1[1] = 0.1f; //amplitude
			wave1[2] =  0.5f; //speed
			wave1[3] =  5.0f; //x
			wave1[4] =  0.0f; //z
			wave1[5] = 0.25; //w


			float wave2 [6];
			wave2[0] = 25.0f; //wavelength
 			wave2[1] = 0.3f; //amplitude
			wave2[2] =  0.25f; //speed
			wave2[3] = 1.0f; //x
			wave2[4] =  1.0f; //z
			wave2[5] = 0.5; //w

			float wave3 [6];
			wave3[0] = 1.0f; //wavelength
			wave3[1] = 0.05f; //amplitude
			wave3[2] =  0.5f; //speed
			wave3[3] = 0.0f; //x
			wave3[4] =  5.0f; //z
			wave3[5] = 0.5; //w


			float* wave;
			float w = 0;

			wave = wave1;
			w = wave1[5];

			h += (wave[1] * glm::sin((glm::dot( glm::vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));

			wave = wave2;
			w = wave2[5];

			h += (wave[1] * glm::sin((glm::dot( glm::vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));

			wave = wave3;
			w = wave3[5];

			h += (wave[1] * glm::sin((glm::dot( glm::vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));

			float e = 2.71828f;
			float A = 4;
			float c = 0.5;
			float tidal_height_increase = 0;
			float fTidalX = tidalX;
			fTidalX /= 10.0f;
			float thePower = (0 - c) * ( ((x - fTidalX)*(x-fTidalX)) + (z*z) );
			float theBase = e;
			if(tidal == 1){
				tidal_height_increase = A * glm::pow(theBase, thePower);
			}
			h += tidal_height_increase;
		}
		boat_position[1] += h;
		if(fps){
			g_camera.setPos(boat_position, boat_direction);
		}
		// Compute the projection matrix.
		aspect = static_cast<float>(window_width) / window_height;
		glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 0.0001f, 1000.0f);

		// Compute the view matrix
		glm::mat4 view_matrix = g_camera.get_view_matrix();

		//TODO divide by some number
		auto current_time = ocean_clock.now() - start_time;
		ocean_time = (float) current_time.count() / 100000000;



		int ugh = showOcean? 1 : 0;
		glm::vec4 temp_eye = glm::vec4(g_camera.eye[0], g_camera.eye[1], g_camera.eye[2], 1.0f);

			//----------------Render the depth boat shadow thing--------------

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		

		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[KBoatVao]));
		CHECK_GL_ERROR(glUseProgram(depth_boat_program_id));

		CHECK_GL_ERROR(glUniform4fv(depth_boat_light_position_location, 1, &light_position[0]));
		CHECK_GL_ERROR(glUniform1f(depth_boat_theta_location, boatTheta));
		CHECK_GL_ERROR(glUniform4fv(depth_boat_translate_by_location, 1, &boat_position[0]));
		CHECK_GL_ERROR(glUniform1f(depth_boat_far_plane_location, shadowFar));

		CHECK_GL_ERROR(glUniformMatrix4fv(depth_boat_shadow_matrix1_location, 1, GL_FALSE, &shadowTransforms[0][0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(depth_boat_shadow_matrix2_location, 1, GL_FALSE, &shadowTransforms[1][0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(depth_boat_shadow_matrix3_location, 1, GL_FALSE, &shadowTransforms[2][0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(depth_boat_shadow_matrix4_location, 1, GL_FALSE, &shadowTransforms[3][0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(depth_boat_shadow_matrix5_location, 1, GL_FALSE, &shadowTransforms[4][0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(depth_boat_shadow_matrix6_location, 1, GL_FALSE, &shadowTransforms[5][0][0]));

		CHECK_GL_ERROR(glDrawElements(GL_PATCHES, boat_faces.size() * 4, GL_UNSIGNED_INT, 0));

		glBindFramebuffer(GL_FRAMEBUFFER, 0); 

		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);

		//bind the cubemap that we drew
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

		
		//back to regular stuff


    	/*
		//----------------RENDER THE CUBE------------------------------------------
		// Switch to the Geometry VAO.
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kGeometryVao]));
		CHECK_GL_ERROR(glUseProgram(cube_program_id));

		//update geometry if it's wrong
		if (g_menger && g_menger->is_dirty()) {
			g_menger->generate_geometry(obj_vertices, obj_faces);
			g_menger->set_clean();

			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kVertexBuffer]));
			CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * obj_vertices.size() * 4, obj_vertices.data(), GL_STATIC_DRAW));

			CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kIndexBuffer]));
			CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * obj_faces.size() * 3, obj_faces.data(), GL_STATIC_DRAW));

		}

		//Update cube uniforms
		CHECK_GL_ERROR(glUniformMatrix4fv(cube_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(cube_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(cube_light_position_location, 1, &light_position[0]));

		//draw the CUBE
		CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0));
		*/

		//----------------RENDER THE FLOOR (CHECKERS)------------------------------------------
		// Poll and swap.
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kFloorVao]));
		CHECK_GL_ERROR(glPatchParameteri(GL_PATCH_VERTICES, 4));
		CHECK_GL_ERROR(glUseProgram(floor_program_id));

		//Update floor uniforms
		CHECK_GL_ERROR(glUniformMatrix4fv(floor_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(floor_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(floor_light_position_location, 1, &light_position[0]));
		CHECK_GL_ERROR(glUniform1f(floor_time_location, ocean_time));
		CHECK_GL_ERROR(glUniform1i(floor_tidal_location, tidal));
		CHECK_GL_ERROR(glUniform1i(floor_tidalX_location, tidalX));
		CHECK_GL_ERROR(glUniform1i(floor_showOcean_location, ugh));

		// draw the FLOOR (checkers)
		if (showFloor)
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, floor_triangle_faces.size() * 3, GL_UNSIGNED_INT, 0));

      //----------------RENDER THE ORB------------------------------------------
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kOrbVao]));
		CHECK_GL_ERROR(glUseProgram(orb_program_id));

		CHECK_GL_ERROR(glUniformMatrix4fv(orb_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(orb_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(orb_light_position_location, 1, &light_position[0]));

		CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, sphere_faces.size() * 3, GL_UNSIGNED_INT, 0));
		//----------------RENDER THE WIREFRAME------------------------------------------
		CHECK_GL_ERROR(glUseProgram(floor_wireframe_program_id));
		CHECK_GL_ERROR(glPatchParameteri(GL_PATCH_VERTICES, 4));
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kWireframeVao]));


		CHECK_GL_ERROR(glUniformMatrix4fv(floor_wireframe_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(floor_wireframe_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(floor_wireframe_light_position_location, 1, &light_position[0]));
		CHECK_GL_ERROR(glUniform1i(outerTess_location, outerTess));
		CHECK_GL_ERROR(glUniform1i(innerTess_location, innerTess));
		CHECK_GL_ERROR(glUniform1f(floor_wireframe_time_location, ocean_time));
		CHECK_GL_ERROR(glUniform1i(floor_wireframe_tidal_location, tidal));
		CHECK_GL_ERROR(glUniform1i(floor_wireframe_tidalX_location, tidalX));
		CHECK_GL_ERROR(glUniform1i(floor_wireframe_showOcean_location, ugh));
		CHECK_GL_ERROR(glUniform4fv(floor_wireframe_camera_pos_location, 1, &temp_eye[0]));


		if(false && showWireframe){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			/*for(int i = 0; i < floor_quad_vertices.size(); i++){
				glm::vec4 vertex = floor_quad_vertices.at(i);
				std::cout << vertex[0] << "," << vertex[1] << "," << vertex[2] << std::endl;
			}*/
			CHECK_GL_ERROR(glDrawElements(GL_PATCHES, floor_quad_faces.size() * 4, GL_UNSIGNED_INT, 0));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		}



		//TODO if showing ocean have wireframe render with a pipeline that uses the ocean TES
		//----------------RENDER THE WIREFRAME------------------------------------------
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kWireframeVao]));
		CHECK_GL_ERROR(glUseProgram(ocean_program_id));

		CHECK_GL_ERROR(glUniformMatrix4fv(ocean_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(ocean_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(ocean_light_position_location, 1, &light_position[0]));
		CHECK_GL_ERROR(glUniform1i(ocean_outerTess_location, outerTess));
		CHECK_GL_ERROR(glUniform1i(ocean_innerTess_location, innerTess));
		CHECK_GL_ERROR(glUniform1f(time_location, ocean_time));
		CHECK_GL_ERROR(glUniform1i(tidal_location, tidal));
		CHECK_GL_ERROR(glUniform1i(tidalX_location, tidalX));
		CHECK_GL_ERROR(glUniform1i(ocean_showOcean_location, ugh));
		CHECK_GL_ERROR(glUniform4fv(camera_pos_location, 1, &temp_eye[0]));
		CHECK_GL_ERROR(glUniform1i(ocean_additiveBlending_location, (additiveBlending ? 1 : 0)));


		if (showOcean)
			CHECK_GL_ERROR(glDrawElements(GL_PATCHES, floor_quad_faces.size() * 4, GL_UNSIGNED_INT, 0));

		tidalX += 1;
		if(tidalX > 2000){
			tidalX = 0;
			tidal = 0;
		}







    //----------------RENDER THE BOAT------------------------------------------
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[KBoatVao]));
		CHECK_GL_ERROR(glUseProgram(boat_program_id));

		CHECK_GL_ERROR(glUniformMatrix4fv(boat_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(boat_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(boat_light_position_location, 1, &light_position[0]));

    //MODIFY BOAT POSITION TO HAVE THE HEIGHT FOR THIS GIVEN TIME



		CHECK_GL_ERROR(glUniform4fv(boat_translate_by_location, 1, &boat_position[0]));
		CHECK_GL_ERROR(glUniform1i(boat_outerTess_location, 1));
		CHECK_GL_ERROR(glUniform1i(boat_innerTess_location, 1));
		CHECK_GL_ERROR(glUniform1f(boat_time_location, ocean_time));
		CHECK_GL_ERROR(glUniform1i(boat_tidal_location, 0));
		CHECK_GL_ERROR(glUniform1i(boat_tidalX_location, tidalX));
		CHECK_GL_ERROR(glUniform1i(boat_showOcean_location, 0));
		CHECK_GL_ERROR(glUniform4fv(boat_camera_pos_location, 1, &temp_eye[0]));
		CHECK_GL_ERROR(glUniform1f(boat_theta_location, boatTheta));


		CHECK_GL_ERROR(glDrawElements(GL_PATCHES, boat_faces.size() * 4, GL_UNSIGNED_INT, 0));

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
