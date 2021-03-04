#version 330

in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 eye_position;

// Main light info
uniform vec3 light_position;
uniform vec3 intensity_ambient;
uniform vec3 intensity_light;

// Mux lights info
uniform int aux_count;
uniform vec3[20] aux_lights_positions;
uniform vec3[20] aux_lights_intensity;
uniform int[20] aux_lights_isSpotlight;
uniform vec3[20] aux_lights_directions;
uniform float[20] aux_lights_angles;

uniform vec3 material_ke;
uniform vec3 material_ka;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;


uniform int fragment_flags[3];
uniform sampler2D texture;

in vec2 texcoord;
in float time;

layout(location = 0) out vec4 out_color;

void main()
{
	/*
		Main Light Calculations
	*/
	vec3 L = normalize( light_position - world_position );
	vec3 V = normalize( eye_position - world_position );
	vec3 H = normalize( L + V );
	
	vec3 em = material_ke;
	vec3 amb = intensity_ambient * material_ka;
	vec3 diff = material_kd * intensity_light * max(dot(world_normal, L), 0);
	vec3 spec = (length(diff) > 0)? material_ks * intensity_light * pow(max(dot(world_normal, H), 0), material_shininess) : vec3(0);
	
	float att = 1 / distance(light_position, world_position) * distance(light_position, world_position);

	out_color = vec4(att * (diff + spec), 1);

	/*
		Aux Light illumination
	*/
	int totalLights = 1;
	for (int i = 0; i < aux_count; i++) {
		L = normalize( aux_lights_positions[i] - world_position );
		H = normalize( L + V );
		
		diff = material_kd * aux_lights_intensity[i] * max(dot(world_normal, L), 0);
		spec = ((length(diff) > 0)? material_ks * aux_lights_intensity[i] * pow(max(dot(world_normal, H), 0), material_shininess) : vec3(0));
		
		if (aux_lights_isSpotlight[i] == 1) {
			float cut_off = radians(aux_lights_angles[i]);
			float spot_light = dot(-L, aux_lights_directions[i]);
			float spot_light_limit = cos(cut_off);

			if (spot_light > spot_light_limit) {
				att = pow((spot_light - spot_light_limit) / (1 - spot_light_limit), 2);
				totalLights++;
			} else {
				att = 0;
			}
		} else {
			att = 1 / distance(aux_lights_positions[i], world_position) * distance(aux_lights_positions[i], world_position);
			totalLights++;
		}

		out_color += vec4((diff + spec) * att, 1);
	}

	// Color without any textures
	out_color = vec4(em + amb, 1) + out_color / totalLights;
	
	vec2 use_texcoord = texcoord;
	if (fragment_flags[0] == 1) {
		if (fragment_flags[1] == 1) {
			use_texcoord += vec2(time / 5, 0);
		}
		if (fragment_flags[2] == 1) {
			use_texcoord += vec2(0, time / 5);
		}
		vec4 tex_color = texture2D(texture, use_texcoord);
		if (tex_color.a < 0.7) discard;
		out_color = mix (tex_color, out_color, 0.2);
	}
}