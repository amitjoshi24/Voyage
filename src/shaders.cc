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
flat out vec4 normal;
out vec4 light_direction;
out vec4 world_coordinates;
void main()
{

	vec3 nhn = normalize(cross( gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz));
	normal = vec4(nhn[0], nhn[1], nhn[2], 0.0f);

	for (int n = 0; n < gl_in.length(); n++) {
		light_direction = vs_light_direction[n];
		world_coordinates = gl_in[n].gl_Position;
		gl_Position = projection * view * gl_in[n].gl_Position;

		EmitVertex();
	}
	EndPrimitive();
}
)zzz";

const char* fragment_shader =
R"zzz(#version 330 core
flat in vec4 normal;
in vec4 light_direction;
out vec4 fragment_color;
void main()
{
	fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
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

const char* floor_fragment_shader =
R"zzz(#version 330 core
flat in vec4 normal;
in vec4 light_direction;
in vec4 world_coordinates;
out vec4 fragment_color;
uniform float ocean_time;
uniform int showOcean;
uniform int tidalX;
uniform int tidal;
uniform vec4 light_position;

void main()
{
	vec4 color = vec4(1.0f, 1.0f, 0.811f, 1.0f);
	if (showOcean == 1){
			vec4 color = vec4(1.0, 1.0, 0.811f, 1.0);
			float e = 2.71828f;
			float A = 4;
			float c = 0.5;
			float x = world_coordinates.x;
			float z = world_coordinates.z;
			float tidal_height_increase = 0;
			vec4 tidalNormal = vec4(0,0,0,0);
		    float fTidalX = tidalX;
			fTidalX /= 10.0f;
			float thePower = (0 - c) * ( ((x - fTidalX)*(x-fTidalX)) + (z*z) );
			float theBase = e;
			if(tidal == 1){
				tidal_height_increase = A * pow(theBase, thePower);
			}
			float t = ocean_time;
	  		vec2 pos = vec2(x, z);

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

    		float h = 2;
			float dhdx = 0;
	  		float dhdz = 0;

			float wave [6];
			float w = 0;

    		wave = wave1;
			w = wave1[5];

	  		h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
	  		dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
      		dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));

			wave = wave2;
			w = wave2[5];

			h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
			dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
			dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));

			wave = wave3;
			w = wave3[5];

			h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
			dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
			dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));


	      	if(tidal == 1){
	      		dhdx += A*(-2*c)*(x-fTidalX)*pow(theBase, thePower);
	      		dhdz += A*(-2*c)*(z)*pow(theBase, thePower);
	      		h += tidal_height_increase;
	      	}
		  	vec4 ocean_normal = normalize(vec4(-dhdx, 1, -dhdz,0.0));






		  	vec4 new_light_direction = light_position - vec4(world_coordinates.x, world_coordinates.y + h, world_coordinates.z, 1);
		  	

		  	float dot_nl = dot(normalize(new_light_direction), normalize(ocean_normal));
		  	dot_nl = clamp(dot_nl, 0.0, 1.0);
			fragment_color = clamp(dot_nl * color, 0.0, 1.0);

	}
	else{
		color = vec4(1.0f, 1.0f, 0.811f, 1.0f);
		float dot_nl = dot(normalize(light_direction), normalize(normal));
		dot_nl = clamp(dot_nl, 0.0, 1.0);
		fragment_color = clamp(dot_nl * color, 0.0, 1.0);
	}



	
}
)zzz";
//-----------------------------------------------------------------------------
/*
glDrawElements(GL_PATCHES), run on quad floor
can be used with normal vertex shader
-took out world_coordinates
geometry shader:
- changed normal to be interpolated
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

	if (gl_InvocationID == 0){

	    gl_TessLevelInner[0] = innerTess;
	    gl_TessLevelInner[1] = innerTess;
	    gl_TessLevelOuter[0] = outerTess;
	    gl_TessLevelOuter[1] = outerTess;
			gl_TessLevelOuter[2] = outerTess;
	    gl_TessLevelOuter[3] = outerTess;

	}

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
		vs_light_direction4[gl_InvocationID] = vs_light_direction[gl_InvocationID];
}
)zzz";

//run on tesselated patch
const char* floor_wireframe_tesselation_evaluation_shader =
R"zzz(#version 410 core
layout (quads, equal_spacing) in;
in vec4 vs_light_direction4[];
out vec4 vs_light_direction;
void main(void)
{
		//world coordinates
    vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
    vec4 p2 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
    gl_Position = mix(p1, p2, gl_TessCoord.y);

		vec4 light1 = mix(vs_light_direction4[0], vs_light_direction4[1], gl_TessCoord.x);
		vec4 light2 = mix(vs_light_direction4[3], vs_light_direction4[2], gl_TessCoord.x);
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
in vec4 ocean_normal[];
out vec4 normal;
out vec4 light_direction;
out vec3 vertex_id;
void main()
{

	for (int n = 0; n < gl_in.length(); n++) {
		light_direction = vs_light_direction[n];
		normal = ocean_normal[n];
		//shift the wireframe, gl_Position is in world cords
		gl_Position = projection * view * (gl_in[n].gl_Position + vec4(0.0f, 0.25f, 0.0f, 0.0f));
		//gl_Position is in !!!!!!NDC!!!!!!

		if (n == 0){
			vertex_id = vec3(1.0f, 0.0f, 0.0f);
		} else if (n == 1){
			vertex_id = vec3(0.0f, 1.0f, 0.0f);
		} else {
			vertex_id = vec3(0.0f, 0.0f, 1.0f);
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
const char* ocean_tesselation_control_shader =
R"zzz(#version 410 core
layout (vertices = 4) out;
uniform int outerTess;
uniform int innerTess;
uniform int showOcean;
uniform int tidalX;
uniform int tidal;
in vec4 vs_light_direction[];
out vec4 vs_light_direction4[];

out vec4 tidal_normals[];

void main()
{
	if(gl_InvocationID == 0){
		float d = 0.0f;

		float fTidalX = tidalX;
		fTidalX /= 10.0f;

		vec3 meanOfTidal = vec3(fTidalX, 0.0f, 0.0f);
		int multiplier = 1;

		//if tidal wave
		if(tidal == 1 && showOcean == 1){
			for(int i = 0; i < 4; i++){
				d += (distance(meanOfTidal, gl_in[i].gl_Position.xyz));
			}
			d /= 16;
			multiplier = int(4.0 - d);
			if(multiplier < 1){
				multiplier = 1;
			}
			if(multiplier > 4){
				multiplier = 4;
			}
		}
		//multiplier = 1;

		//control tesselation levels based off of how close to mean
		int innerVal = int(innerTess * multiplier);
		int outerVal = int(outerTess * multiplier);

	  gl_TessLevelInner[0] = innerVal;
	  gl_TessLevelInner[1] = innerVal;

	  gl_TessLevelOuter[0] = outerVal;
	  gl_TessLevelOuter[1] = outerVal;
	  gl_TessLevelOuter[2] = outerVal;
      gl_TessLevelOuter[3] = outerVal;
	}


	//done: offset gl_Position by height of tidal wave

	//TODO potentially tweak normals here too, to account for the tidal wave
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	vs_light_direction4[gl_InvocationID] = vs_light_direction[gl_InvocationID];
}
)zzz";

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
uniform float ocean_time;
uniform int showOcean;
uniform int tidalX;
uniform int tidal;
out vec4 vs_light_direction;
out vec4 ocean_normal;
void main(void)
{


		float t = ocean_time;

		//NOTE gl_Position is still in world coords
		vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
		vec4 p2 = mix(gl_in[3].gl_Position, gl_in[2].gl_Position, gl_TessCoord.x);
		gl_Position = mix(p1, p2, gl_TessCoord.y);

		float e = 2.71828f;
		float A = 4;
		float c = 0.5;
		float x = gl_Position[0];
		float z = gl_Position[2];
		float tidal_height_increase = 0;
		vec4 tidalNormal = vec4(0,0,0,0);
	    float fTidalX = tidalX;
		fTidalX /= 10.0f;
		float thePower = (0 - c) * ( ((x - fTidalX)*(x-fTidalX)) + (z*z) );
		float theBase = e;
		if(tidal == 1){
			tidal_height_increase = A * pow(theBase, thePower);
		}

		if (showOcean == 1){
	  		vec2 pos = vec2(x, z);

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

    	float h = 2;
			float dhdx = 0;
	  	float dhdz = 0;

			float wave [6];
			float w = 0;

    	wave = wave1;
			w = wave1[5];

	  	h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
	  	dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
      dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));

			wave = wave2;
			w = wave2[5];

			h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
			dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
			dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));

			wave = wave3;
			w = wave3[5];

			h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
			dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
			dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));


	      	if(tidal == 1){
	      		dhdx += A*(-2*c)*(x-fTidalX)*pow(theBase, thePower);
	      		dhdz += A*(-2*c)*(z)*pow(theBase, thePower);
	      	}
		  	ocean_normal = normalize(vec4(-dhdx, 1, -dhdz,0.0));
				//offset gl_Position by height of normal waves

		  	gl_Position[1] += h + tidal_height_increase;
		} else{
			vec3 nhn = normalize(cross( gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz));
			ocean_normal = vec4(nhn[0], nhn[1], nhn[2], 0.0f);
		}

		vec4 light1 = mix(vs_light_direction4[0], vs_light_direction4[1], gl_TessCoord.x);
		vec4 light2 = mix(vs_light_direction4[3], vs_light_direction4[2], gl_TessCoord.x);
		vs_light_direction = mix(light1, light2, gl_TessCoord.y);
}

)zzz";

const char* ocean_geometry_shader =
R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 view;
uniform vec4 camera_pos; //homogenous point, (xyzw) w= 1
in vec4 vs_light_direction[];
in vec4 ocean_normal[];
out vec4 normal;
out vec4 global_coords;
out vec4 light_direction;
out vec4 camera_direction;
out vec4 world_coordinates;

void main()
{

	for (int n = 0; n < gl_in.length(); n++) {
		light_direction = vs_light_direction[n];
		normal = ocean_normal[n];
		global_coords = gl_in[n].gl_Position;
		world_coordinates = gl_in[n].gl_Position;
		camera_direction = -gl_in[n].gl_Position + camera_pos;
		gl_Position = projection * view * gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
)zzz";

const char* ocean_fragment_shader =
R"zzz(#version 330 core
in vec4 normal; //interpolated to ensure smooth water
in vec4 global_coords;
in vec4 light_direction;
in vec4 camera_direction;
in vec4 world_coordinates;
out vec4 fragment_color;
uniform vec4 light_position;
uniform float ocean_time;
uniform int tidalX;
uniform int tidal;
void main(){

	vec4 nlight_direction = normalize(light_direction);
	vec4 ncamera_direction = normalize(camera_direction);
	vec4 nnormal = normalize(normal);
	float shadowMultiplier = 1.0f;
	float x = global_coords[0] - 0.5;
	float z = global_coords[2];
	float k_a = 0.5f; //random number
	vec4 water_ambient = vec4(0.0f, 1.0f, 1.0f, 1.0f); //bg contribution

	vec4 ambient_component = k_a * water_ambient; //add this

	//diffuse lighting
	float k_d = 0.3f; //also random number
	vec4 light_color = vec4(0.5f, 1.0f, 1.0f, 1.0f);


	float normalLightDot = float(dot(nnormal, nlight_direction));
	if(normalLightDot < 0){
		normalLightDot = 0;
	}

	vec4 diffuse_component = k_d*normalLightDot*light_color;

	//specular lighting
	float k_s = 0.3f; //random num 3
	vec4 nreflected_light_direction = normalize(reflect(nlight_direction, nnormal));
	float alpha = 0.8; //random num 4
	float cameraReflectDot = float(dot(nreflected_light_direction, ncamera_direction));
	if(cameraReflectDot < 0){
		cameraReflectDot = 0;
	}

	float dot_nl = dot(nlight_direction, nnormal);
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	vec4 specular_component = dot_nl*(k_s*pow(cameraReflectDot, alpha)*light_color);

	vec4 color = ambient_component + diffuse_component + specular_component;
	color = shadowMultiplier * color;
	color[3] = 0.2;
	//made it so dot_nl only multiplies specular comp











	vec4 sandColor = vec4(1.0, 0.811, 1.0, 1.0);
	float e = 2.71828f;
	float A = 4;
	float c = 0.5;
	//float x = world_coordinates.x;
	//float z = world_coordinates.z;
	float tidal_height_increase = 0;
	vec4 tidalNormal = vec4(0,0,0,0);
    float fTidalX = tidalX;
	fTidalX /= 10.0f;
	float thePower = (0 - c) * ( ((x - fTidalX)*(x-fTidalX)) + (z*z) );
	float theBase = e;
	if(tidal == 1){
		tidal_height_increase = A * pow(theBase, thePower);
	}
	float t = ocean_time;
		vec2 pos = vec2(x, z);

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

	float h = 2;
	float dhdx = 0;
		float dhdz = 0;

	float wave [6];
	float w = 0;

	wave = wave1;
	w = wave1[5];

		h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
		dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
		dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));

	wave = wave2;
	w = wave2[5];

	h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
	dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
	dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));

	wave = wave3;
	w = wave3[5];

	h += (wave[1] * sin((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave1[2] * 2.0f/wave[0]))));
	dhdx += (w * wave[3] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));
	dhdz += (w * wave[4] * wave[1] * cos((dot( vec2(wave[3], wave[4]), pos)*w) + (t * (wave[2] * 2.0f/wave[0]))));


  	if(tidal == 1){
  		dhdx += A*(-2*c)*(x-fTidalX)*pow(theBase, thePower);
  		dhdz += A*(-2*c)*(z)*pow(theBase, thePower);
  		h += tidal_height_increase;
  	}
  	vec4 ocean_normal = normalize(vec4(-dhdx, 1, -dhdz,0.0));

  	vec4 new_light_direction = light_position - vec4(world_coordinates.x, world_coordinates.y + h, world_coordinates.z, 1);
  	

  	float lavanya_dot_nl = dot(normalize(new_light_direction), normalize(ocean_normal));
  	lavanya_dot_nl = clamp(lavanya_dot_nl, 0.0, 1.0);
  	vec4 floor_color = clamp(lavanya_dot_nl*sandColor, 0.0, 1.0);

	fragment_color = clamp(color*0.5 + floor_color*0.5, 0.0, 1.0);
}
)zzz";
//-----------------------------------------------------------------------------
const char* orb_vertex_shader =
R"zzz(#version 330 core
in vec4 vertex_position;
uniform vec4 light_position;
out vec4 vs_light_direction;
void main()
{
	//multiply by model matrix
	gl_Position[0] = vertex_position[0] + light_position[0];
	gl_Position[1] = vertex_position[1] + light_position[1];
	gl_Position[2] = vertex_position[2] + light_position[2];
	gl_Position[3] = vertex_position[3];
	vs_light_direction = -gl_Position + light_position;
}
)zzz";


const char* boat_vertex_shader =
R"zzz(#version 330 core
in vec4 vertex_position;
uniform vec4 light_position;
uniform vec4 translate_by;
uniform float boatTheta;
out vec4 vs_light_direction;
void main()
{
	//multiply by model matrix
	gl_Position[0] = (cos(boatTheta)*vertex_position[0]) - (sin(boatTheta)*vertex_position[2]) + translate_by[0];
	gl_Position[1] = vertex_position[1] + translate_by[1];
	gl_Position[2] = (sin(boatTheta)*vertex_position[0]) + (cos(boatTheta)*vertex_position[2]) + translate_by[2];
	gl_Position[3] = vertex_position[3];
	vs_light_direction = -gl_Position + light_position;
}
)zzz";

const char* boat_fragment_shader =
R"zzz(#version 330 core
flat in vec4 normal;
in vec4 light_direction;
out vec4 fragment_color;
void main()
{
	vec4 color = vec4(1.0f, 0.0, 0.0, 1.0);
	float dot_nl = dot(normalize(light_direction), normalize(normal));
	dot_nl = clamp(dot_nl, 0.0, 1.0);
	fragment_color = clamp(dot_nl * color, 0.0, 1.0);
}
)zzz";
