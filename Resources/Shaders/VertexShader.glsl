#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time_elapsed;
uniform int vertex_flag;

// Output values to fragment shader
out vec3 world_position;
out vec3 world_normal;
out vec2 texcoord;
out float time;

const float amplitude = 0.125;
const float frequency = 4;
const float PI = 3.14159;

void main()
{
	world_position = vec3(Model * vec4(v_position,1));
	world_normal = normalize( mat3(Model) * v_normal );
	texcoord = v_texture_coord;
	time = time_elapsed;

	if (vertex_flag == 1) {
		float distance = length(v_position);
		float z = max(amplitude * sin( -PI * distance * frequency + time_elapsed ), v_position.z);
		gl_Position = Projection * View * Model * vec4( v_position.x, v_position.y, z, 1 );
	} else {
		gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	}
}