/*
vertex:
- took out world_vertex_position as an out
- took out view as a uniform
geometry:
- took out world_coordinates as an in
- removed wifeframe as uniform in
- removed vertex_id as out
*/
const char* vertex_shader =
R"zzz(#version 330 core
in vec4 vertex_position;
uniform vec4 light_position;
out vec4 vs_light_direction;
uniform uvec4 waves[1];
void main()
{
	gl_Position = vertex_position;
	vs_light_direction = -gl_Position + light_position;

}
)zzz";

const char* geometry_shader =
R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 view;
in vec4 vs_light_direction[];
flat out vec4 normal; //if interpolate normal, do it here by taking out flat
out vec4 light_direction;
out vec4 world_coordinates;
void main()
{

	vec3 nhn = normalize(cross( gl_Position[2].xyz - gl_Position[0].xyz, gl_Position[1].xyz - gl_Position[0].xyz));
	normal = vec4(nhn[0], nhn[1], nhn[2], 0.0);

	for (int n = 0; n < gl_in.length(); n++) {
		light_direction = vs_light_direction[n];
		world_coordinates = gl_in[n].gl_Position;
		gl_Position = projection * view * gl_in[n].gl_Position;

		EmitVertex();
	}
	EndPrimitive();
}
)zzz";



const char* cube_fragment_shader =
R"zzz(#version 330 core
flat in vec4 normal;
in vec4 light_direction;
out vec4 fragment_color;
void main()
{
	vec4 color;
	int maxIndex = 0;
	float maxVal = -9999999;
	for(int i = 0; i < 3; i++){
		if(abs(normal[i]) > maxVal){
			maxIndex = i;
			maxVal = abs(normal[i]);
		}
	}
	if(maxIndex == 0){
		color = vec4(1.0, 0.0, 0.0, 0.0);
	}
	else if(maxIndex == 1){
		color = vec4(0.0, 1.0, 0.0, 1.0);
	}
	else{
		color = vec4(0.0, 0.0, 1.0, 1.0);
	}

	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	fragment_color = clamp(dot_nl * color, 0.0, 1.0);
}
)zzz";

//-----------------------------------------------------------------------------
/*
glDrawElements(GL_TRIANGLES)
use with normal vertex shader and geometry shader
must be run using the normal TRIANGLE FLOOR faces
*/


// FIXME: Implement shader effects with an alternative shader.
const char* floor_fragment_shader =
R"zzz(#version 330 core
flat in vec4 normal;
in vec4 light_direction;
in vec4 world_coordinates;
in vec3 vertex_id;
out vec4 fragment_color;
void main()
{
	vec4 color;
	float thres = 0.1;
	if(mod((floor(world_coordinates.x) + floor(world_coordinates.z)), 2) != 0 ){
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	} else{
		color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	fragment_color = clamp(dot_nl * color, 0.0, 1.0);
}
)zzz";
//-----------------------------------------------------------------------------
/*
glDrawElements(GL_PATCHES), run on quad floor
can be used with normal vertex shader
-took out world_coordinates
fragment shader:
-changed normal to be interpolated (i.e. not flat)
*/
//sets up the constants for tesselation for each patch
const char* tesselation_control_shader =
R"zzz(#version 410 core

layout (vertices = 4) out;
uniform int outerTess;
uniform int innerTess;
in vec4 vs_light_direction[];
out vec4 vs_light_direction4[];
void main()
{


        gl_TessLevelInner[0] = innerTess;
        gl_TessLevelInner[1] = innerTess;
        gl_TessLevelOuter[0] = outerTess;
        gl_TessLevelOuter[1] = outerTess;
        gl_TessLevelOuter[2] = outerTess;
        gl_TessLevelOuter[3] = outerTess;


    gl_out[gl_InvocationID].gl_Position =
        gl_in[gl_InvocationID].gl_Position;
		vs_light_direction4[gl_InvocationID] = vs_light_direction[gl_InvocationID];
}
)zzz";

//run on tesselated patch
const char* tesselation_evaluation_shader =
R"zzz(#version 410 core
layout (quads) in;
in vec4 vs_light_direction4[];
out vec4 vs_light_direction;

void main(void)
{
		//world coordinates
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
    gl_Position = mix(p1, p2, gl_TessCoord.y);

		vec4 light1 = mix(vs_light_direction4[0], vs_light_direction4[1], gl_TessCoord.x);
		vec4 light2 = mix(vs_light_direction4[2], vs_light_direction4[3], gl_TessCoord.x);
		vs_light_direction = mix(light1, light2, gl_TessCoord.y);

}

)zzz";
const char* floor_wireframe_geometry_shader =
R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 view;
in vec4 vs_light_direction[];
flat out vec4 normal;
out vec4 light_direction;
out vec3 vertex_id;
void main()
{
	//normal is in global coordinates
	vec3 nhn = normalize(cross( gl_Position[2].xyz - gl_Position[0].xyz, gl_Position[1].xyz - gl_Position[0].xyz));
	normal = vec4(nhn[0], nhn[1], nhn[2], 0.0);

	for (int n = 0; n < gl_in.length(); n++) {
		light_direction = vs_light_direction[n];

		//shift the wireframe, gl_Position is in world cords
		gl_Position = projection * view * (gl_in[n].gl_Position + vec4(0.0f, 0.1f, 0.0f, 0.0f)); // homogoenous coord

		if (n == 0){
			vertex_id = vec3(1, 0, 0);
		} else if (n == 1){
			vertex_id = vec3(0,1, 0);
		} else {
			vertex_id = vec3(0, 0, 1);
		}
		EmitVertex();
	}
	EndPrimitive();
}
)zzz";
const char* floor_wireframe_fragment_shader =
R"zzz(#version 330 core
in vec4 normal;
in vec4 light_direction;
in vec3 vertex_id;
out vec4 fragment_color;
void main()
{
	vec4 color;
	float thres = 0.1;
	if (vertex_id [0] < thres || vertex_id [ 1 ] < thres || vertex_id[2] < thres){
		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	fragment_color = clamp(dot_nl * color, 0.0, 1.0);
}
)zzz";
//-----------------------------------------------------------------------------
/*
uses normal vertex, normal tcs
glDrawElements(GL_PATCHES), run on quad floor
geometry shader:
- changed normal to interpolate (i.e. not be flat)
- removed normal calculation, assumed passed in by tes
fragment shader:
- changed normal to interpolate (i.e. not be flat)
*/
//run on tesselated patch
const char* ocean_tesselation_evaluation_shader =
R"zzz(#version 410 core
layout (quads) in;
in vec4 vs_light_direction4[];
out vec4 vs_light_direction;
out vec4 ocean_normal;
void main(void)
{

		//TODO switch with whatever calculations you need for the normal
		//NOTE gl_Position is still in world coords
		ocean_normal = vec4(0.0, 0.0, 0.0, 0.0);

		//world coordinates
		vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
		vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
		gl_Position = mix(p1, p2, gl_TessCoord.y);

		vec4 light1 = mix(vs_light_direction4[0], vs_light_direction4[1], gl_TessCoord.x);
		vec4 light2 = mix(vs_light_direction4[2], vs_light_direction4[3], gl_TessCoord.x);
		vs_light_direction = mix(light1, light2, gl_TessCoord.y);
}

)zzz";

const char* ocean_geometry_shader =
R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 view;
in vec4 vs_light_direction[];
in vec4 ocean_normal[];
out vec4 normal;
out vec4 light_direction;
out vec4 world_coordinates;

void main()
{


	for (int n = 0; n < gl_in.length(); n++) {
		light_direction = vs_light_direction[n];
		normal = ocean_normal[n];
		world_coordinates = gl_Position;
		gl_Position = projection * view * gl_in[n].gl_Position;

		EmitVertex();
	}
	EndPrimitive();
}
)zzz";
const char* ocean_fragment_shader =
R"zzz(#version 330 core
in vec4 normal;
in vec4 light_direction;
in vec4 world_coordinates;
in vec3 vertex_id;
out vec4 fragment_color;
void main(){
	//TODO replace with proper lighting model
	vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	fragment_color = clamp(dot_nl * color, 0.0, 1.0);
}
)zzz";

//-----------------------------------------------------------------------------
const char* skybox_vertex_shader =
R"zzz(#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 TexCoords;
uniform mat4 projection;
uniform mat4 view;
void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
)zzz";

const char* skybox_fragment_shader =
R"zzz(#version 330in vec3 textureDir;
#version 330 core
out vec4 FragColor;
in vec3 TexCoords;
uniform samplerCube skybox;
void main()
{
    FragColor = texture(skybox, TexCoords);
}
)zzz";
