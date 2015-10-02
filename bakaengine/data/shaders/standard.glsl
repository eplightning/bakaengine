#version 330

// atrybuty
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;

// podstawy
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;

// oświetlenie
uniform float shininess;
uniform vec3 ambient_color;
uniform int dirlight_count;
uniform int pointlight_count;
uniform vec3 dirlight[3];
uniform vec3 pointlight[3];
uniform vec3 dirlight_diffuse[3];
uniform vec3 pointlight_diffuse[3];
uniform vec3 dirlight_specular[3];
uniform vec3 pointlight_specular[3];
uniform float pointlight_intensity[3];

// tekstury/kolor
uniform vec4 blend_color;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D ambient_texture;

// przekazywane do fragment shadera
out vec2 vs_texcoord;
out vec3 vs_normal;
out vec3 vs_pos;

void main()
{
	vs_texcoord = texcoord;
	vs_normal = vec3(normal_matrix * vec4(normal, 0.0));

	vec4 pos = view_matrix * model_matrix * vec4(position, 1.0);
	vs_pos = vec3(pos);
	
	gl_Position = projection_matrix * pos;
}