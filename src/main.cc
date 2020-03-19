#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL library includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <debuggl.h>
#include "menger.h"
#include "camera.h"
#include "shaders.h"
#include "stb_image.h"
int window_width = 800, window_height = 600;

// VBO and VAO descriptors.
enum { kVertexBuffer, kIndexBuffer, kNumVbos };

// These are our VAOs.
enum { kGeometryVao, kFloorVao, kWireframeVao, kSkyboxVao, kNumVaos };

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

void
CreateFloor(std::vector<glm::vec4>& vertices, std::vector<glm::uvec4>& indices){
	float inf = 20.0f;
	float inc = 40.0f/16.0f;
	int dotCounter = 0;
	for(float j = -inf; j <= inf; j+=inc){
		for(float i = -inf; i <= inf; i+=inc){
			vertices.push_back(glm::vec4(i, -2.0f, j, 1.0f));
			dotCounter += 1;
			int lastDotPlacedIndex = dotCounter - 1;
			if(lastDotPlacedIndex > 16 && (lastDotPlacedIndex % 17) != 0){
				indices.push_back(glm::uvec4(lastDotPlacedIndex, lastDotPlacedIndex - 1, lastDotPlacedIndex - 17, lastDotPlacedIndex-18));
				// only if its not in the lowermost row and leftmost column, then you can make triangle
				//indices.push_back(glm::uvec3(lastDotPlacedIndex-18, lastDotPlacedIndex, lastDotPlacedIndex-1));
				//indices.push_back(glm::uvec3(lastDotPlacedIndex-18, lastDotPlacedIndex-17, lastDotPlacedIndex));
			}

		}
		std::cout << "dotCounter: " << dotCounter << std::endl;
	}
}

void generate_waves(glm::uvec4* waves){
    waves[0] = glm::uvec4(2, 4, 1, 7);
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
int outerTess = 5;
int innerTess = 5;
bool showOcean = false;
bool showWireframe = true;
bool showFloor = true;
glm::uvec4* waves;

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
  else if(key == GLFW_KEY_MINUS && action != GLFW_RELEASE){
		outerTess--;
		if (outerTess < 0) outerTess = 1;
  }
  else if(key == GLFW_KEY_EQUAL && action != GLFW_RELEASE){
    outerTess++;
  }
  else if(key == GLFW_KEY_COMMA && action != GLFW_RELEASE){
      innerTess--;
			if (innerTess < 0) innerTess = 1;
  }
  else if(key == GLFW_KEY_PERIOD && action != GLFW_RELEASE){
      innerTess++;
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

//--------------SKYBOX INIT-----------------------------------------------------
/*
	std::vector<glm::vec4> skybox_vertices;
	std::vector<glm::uvec3> skybox_faces;

	generate_skybox(skybox_vertices, skybox_faces);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	std::vector<std::string> faces
	{
	    "../../src/skybox/right.jpg",
	    "../../src/skybox/left.jpg",
	    "../../src/skybox/top.jpg",
	    "../../src/bottom.jpg",
	    "../../src/front.jpg",
	    "../../back.jpg"
	};
unsigned int cubemapTexture = loadCubemap(faces);

	int width, height, nrChannels;
unsigned char *data;
for(GLuint i = 0; i < faces.size(); i++)
{
    data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
    );
}

glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

*/
//--------FLOOR INIT----------------------------------------------------------------------

	CreateFloor(floor_quad_vertices, floor_quad_faces);
  CreateFloorTriangles(floor_triangle_vertices, floor_triangle_faces);

//-------WAVE INIT-----------------------------------------------------------------------
    waves = new glm::uvec4[1];
    generate_waves(waves);
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
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * obj_vertices.size() * 4, obj_vertices.data(),
				GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * obj_faces.size() * 3, obj_faces.data(), GL_STATIC_DRAW));

	//-----------setup floor VAO shit------------------
	// Switch to the VAO for Geometry.
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
    CHECK_GL_ERROR(glShaderSource(tesselation_control_shader_id, 1,
                &tesselation_control_source_pointer, nullptr));
    glCompileShader(tesselation_control_shader_id);
    CHECK_GL_SHADER_ERROR(tesselation_control_shader_id);

    // set up wireframe tesselation evaluation shader
    GLuint floor_wireframe_tesselation_evaluation_shader_id = 0;
    const char* floor_wireframe_tesselation_evaluation_source_pointer = floor_wireframe_tesselation_evaluation_shader;
    CHECK_GL_ERROR(floor_wireframe_tesselation_evaluation_shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER));
    CHECK_GL_ERROR(glShaderSource(floor_wireframe_tesselation_evaluation_shader_id, 1,
                &floor_wireframe_tesselation_evaluation_source_pointer, nullptr));
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

		// set up ocean tesselation evaluation shader
		GLuint ocean_tesselation_evaluation_shader_id = 0;
		const char* ocean_tesselation_evaluation_source_pointer = ocean_tesselation_evaluation_shader;
		CHECK_GL_ERROR(ocean_tesselation_evaluation_shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER));
		CHECK_GL_ERROR(glShaderSource(ocean_tesselation_evaluation_shader_id, 1,
								&ocean_tesselation_evaluation_source_pointer, nullptr));
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
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, tesselation_control_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, floor_wireframe_tesselation_evaluation_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, floor_wireframe_geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(floor_wireframe_program_id, floor_wireframe_fragment_shader_id));

	//create ocean program
	GLuint ocean_program_id = 0;
	CHECK_GL_ERROR(ocean_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, tesselation_control_shader_id));
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, ocean_tesselation_evaluation_shader_id));
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, ocean_geometry_shader_id));
	CHECK_GL_ERROR(glAttachShader(ocean_program_id, ocean_fragment_shader_id));

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

	//----init some vars we need--------------------------------------------------------------------------

	//relocated light position
	glm::vec4 light_position = glm::vec4(-10.0f, 10.0f, 0.0f, 1.0f);
	float aspect = 0.0f;
	float theta = 0.0f;


	//LOOP DE DOOP----------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);

		//UPDATE MATRICES-----------------------------------------------------------

		// Compute the projection matrix.
		aspect = static_cast<float>(window_width) / window_height;
		glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), aspect, 0.0001f, 1000.0f);

		// Compute the view matrix
		glm::mat4 view_matrix = g_camera.get_view_matrix();


		//----------------RENDER THE CUBE------------------------------------------
		// Switch to the Geometry VAO.
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kGeometryVao]));

		//update geometry if it's wrong
		if (g_menger && g_menger->is_dirty()) {
			g_menger->generate_geometry(obj_vertices, obj_faces);
			g_menger->set_clean();

			CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kVertexBuffer]));
			CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * obj_vertices.size() * 4, obj_vertices.data(), GL_STATIC_DRAW));

			CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kIndexBuffer]));
			CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * obj_faces.size() * 3, obj_faces.data(), GL_STATIC_DRAW));

		}

		//Use cube program
		CHECK_GL_ERROR(glUseProgram(cube_program_id));

		//Update cube uniforms
		CHECK_GL_ERROR(glUniformMatrix4fv(cube_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(cube_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(cube_light_position_location, 1, &light_position[0]));

		//draw the CUBE
		CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0));

		//----------------RENDER THE FLOOR (CHECKERS)------------------------------------------
		// Poll and swap.
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kFloorVao]));

		CHECK_GL_ERROR(glUseProgram(floor_program_id));

		//Update floor uniforms
		CHECK_GL_ERROR(glUniformMatrix4fv(floor_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(floor_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(floor_light_position_location, 1, &light_position[0]));

		// draw the FLOOR (checkers)
		if (showFloor)
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, floor_triangle_faces.size() * 3, GL_UNSIGNED_INT, 0));

		//----------------RENDER THE WIREFRAME------------------------------------------
		CHECK_GL_ERROR(glUseProgram(floor_wireframe_program_id));
		CHECK_GL_ERROR(glUniformMatrix4fv(floor_wireframe_projection_matrix_location, 1, GL_FALSE, &projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(floor_wireframe_view_matrix_location, 1, GL_FALSE, &view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(floor_wireframe_light_position_location, 1, &light_position[0]));
		CHECK_GL_ERROR(glUniform1i(outerTess_location, outerTess));
		CHECK_GL_ERROR(glUniform1i(innerTess_location, innerTess));

		if(showWireframe){
			std::cout << "used wireframe" << std::endl;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//draw wireframe
			CHECK_GL_ERROR(glDrawElements(GL_PATCHES, floor_quad_faces.size() * 4, GL_UNSIGNED_INT, 0));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		}

		else{
			std::cout << "didn't use wireframe" << std::endl;

		}
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
//----------SKYBOX SHADER SETUP--------------------------------------------------------------------
/*
		// Switch to the VAO for Skybox.
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kSkyboxVao]));

	// Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kSkyboxVao][0]));

	// Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kSkyboxVao][kVertexBuffer]));
	// NOTE: We do not send anything right now, we just describe it to OpenGL.
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * skybox_vertices.size() * 4, skybox_vertices.data(),
				GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kSkyboxVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(uint32_t) * skybox_faces.size() * 3,
				skybox_faces.data(), GL_STATIC_DRAW));

				// setup skybox shader

				GLuint skybox_vertex_shader_id = 0;
				const char* skybox_vertex_source_pointer = skybox_vertex_shader;
				CHECK_GL_ERROR(vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
				CHECK_GL_ERROR(glShaderSource(skybox_vertex_shader_id, 1, &skybox_vertex_source_pointer, nullptr));
				glCompileShader(skybox_vertex_shader_id);
				CHECK_GL_SHADER_ERROR(skybox_vertex_shader_id);

				GLuint skybox_fragment_shader_id = 0;
				const char* skybox_fragment_shader_source_pointer = skybox_fragment_shader;
				CHECK_GL_ERROR(skybox_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
				CHECK_GL_ERROR(glShaderSource(skybox_fragment_shader_id, 1,
							&skybox_fragment_shader_source_pointer, nullptr));
				glCompileShader(skybox_fragment_shader_id);
				CHECK_GL_SHADER_ERROR(skybox_fragment_shader_id);

				GLuint skybox_program_id = 0;
				CHECK_GL_ERROR(skybox_program_id = glCreateProgram());
				CHECK_GL_ERROR(glAttachShader(skybox_program_id, skybox_vertex_shader_id));
				CHECK_GL_ERROR(glAttachShader(skybox_program_id, skybox_fragment_shader_id));

				CHECK_GL_ERROR(glBindAttribLocation(skybox_program_id, 0, "vertex_position"));
				CHECK_GL_ERROR(glBindFragDataLocation(skybox_program_id, 0, "fragment_color"));
				glLinkProgram(skybox_program_id);
				CHECK_GL_PROGRAM_ERROR(skybox_program_id);

				GLint skybox_projection_matrix_location = 0;
				CHECK_GL_ERROR(skybox_projection_matrix_location =
					glGetUniformLocation(skybox_program_id, "projection"));
				GLint skybox_view_matrix_location = 0;
				CHECK_GL_ERROR(skybox_view_matrix_location =
					glGetUniformLocation(skybox_program_id, "view"));


		// render skybox
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kSkyboxVao]));
		// Setup vertex data in a VBO.
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kSkyboxVao][kVertexBuffer]));
		// NOTE: We do not send anything right now, we just describe it to OpenGL.
		CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
					sizeof(float) * skybox_vertices.size() * 4, skybox_vertices.data(),
					GL_STATIC_DRAW));
		CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
		CHECK_GL_ERROR(glEnableVertexAttribArray(0));

		// Setup element array buffer.
		CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kSkyboxVao][kIndexBuffer]));
		CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					sizeof(uint32_t) * skybox_faces.size() * 4,
					skybox_faces.data(), GL_STATIC_DRAW));
		CHECK_GL_ERROR(glUseProgram(skybox_program_id));			// Setup vertex data in a VBO.
		CHECK_GL_ERROR(glDrawElements(GL_PATCHES, skybox__faces.size() * 4, GL_UNSIGNED_INT, 0));

	//copied from https://learnopengl.com/Advanced-OpenGL/Cubemaps
	//used to load in cubemap into gl (Stbi_load)

	unsigned int loadCubemap(std::vector<std::string> faces)
	{
	    unsigned int textureID;
	    glGenTextures(1, &textureID);
	    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	    int width, height, nrChannels;
	    for (unsigned int i = 0; i < faces.size(); i++)
	    {
	        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
	        if (data)
	        {
	            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
	                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	            );
	            stbi_image_free(data);
	        }
	        else
	        {
	            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
	            stbi_image_free(data);
	        }
	    }
	    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	    CHECK_GL_ERROR(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

	    return textureID;
	}

	// FIXME generate skybox
	void generate_skybox(std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces){
		float s = 40.0f;
		glm::vec3 min(-20.0f, -20.0f, -20.0f);
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



		obj_faces.push_back(glm::uvec3(pastSize + 2, pastSize + 1, pastSize + 0));
		obj_faces.push_back(glm::uvec3(pastSize + 2, pastSize + 3, pastSize + 1));

		obj_faces.push_back(glm::uvec3(pastSize + 6, pastSize + 7, pastSize + 3));
		obj_faces.push_back(glm::uvec3(pastSize + 3, pastSize + 2, pastSize + 6));

		obj_faces.push_back(glm::uvec3(pastSize + 4, pastSize + 6, pastSize + 2));
		obj_faces.push_back(glm::uvec3(pastSize + 2, pastSize + 0, pastSize + 4));

		obj_faces.push_back(glm::uvec3(pastSize + 4, pastSize + 0, pastSize + 1));
		obj_faces.push_back(glm::uvec3(pastSize + 1, pastSize + 5, pastSize + 4));

		obj_faces.push_back(glm::uvec3(pastSize + 7, pastSize + 6, pastSize + 4));
		obj_faces.push_back(glm::uvec3(pastSize + 4, pastSize + 5, pastSize + 7));

		obj_faces.push_back(glm::uvec3(pastSize + 7, pastSize + 5, pastSize + 1));
		obj_faces.push_back(glm::uvec3(pastSize + 1, pastSize + 3, pastSize + 7));

	}
	*/
