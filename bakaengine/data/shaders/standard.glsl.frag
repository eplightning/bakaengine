#version 330

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
in vec2 vs_texcoord;
in vec3 vs_normal;
in vec3 vs_pos;

// wyjście
out vec4 output_color;

void main()
{
	vec3 normal = normalize(vs_normal);

	// z tekstury
	vec4 diff = texture2D(diffuse_texture, vs_texcoord);
	vec4 spec = texture2D(diffuse_texture, vs_texcoord);
	vec4 ambient = texture2D(ambient_texture, vs_texcoord);

	// dodatkowo barwimy
	diff = diff * blend_color;
	spec = spec * blend_color;
	ambient = ambient * blend_color;
	
	// komponenty
	vec3 ambient_component = ambient.rgb * ambient_color;
	vec3 diff_component = vec3(0, 0, 0);
	vec3 spec_component = vec3(0, 0, 0);
	
	// światło punktowe
	for (int i = 0; i < pointlight_count; i++) {
		// obliczamy Lm, dodatkowo zachowując odległość
		vec3 Lm = vec3(view_matrix * vec4(pointlight[i], 1.0)) - vs_pos;
		float distance = length(Lm);
		Lm = Lm / distance;
		distance = distance * distance / pointlight_intensity[i];
		
		// N*Lm
		float NdotLm = max(dot(Lm, vs_normal), 0.0);
		
		// komponent diffuse gotowy
		diff_component += diff.rgb * pointlight_diffuse[i] * NdotLm / distance;
		
		// vs_pos jest już po przekształceniu V*M
		/*vec3 half_dir = normalize(Lm + V);
		float spec_power = max(dot(half_dir, normal), 0.0);
		spec_power = pow(spec_power, shininess);*/
		vec3 V = normalize(-vs_pos);
		vec3 refl = reflect(-Lm, normal);
		float spec_power = max(dot(refl, V), 0.0);
		spec_power = pow(spec_power, shininess / 4.0);
		
		// komponent spec gotowy
		spec_component += spec.rgb * pointlight_specular[i] * spec_power / distance;
	}
	
	// światło kierunkowe
	for (int i = 0; i < dirlight_count; i++) {
		// obliczamy Lm, dodatkowo zachowując odległość
		vec3 Lm = vec3(view_matrix * vec4(dirlight[i], 0.0)) - vs_pos;
		Lm = normalize(Lm);
		
		// N*Lm
		float NdotLm = max(dot(Lm, vs_normal), 0.0);
		
		// komponent diffuse gotowy
		diff_component += diff.rgb * dirlight_diffuse[i] * NdotLm;
		
		// vs_pos jest już po przekształceniu V*M
		/*vec3 half_dir = normalize(Lm + V);
		float spec_power = max(dot(half_dir, normal), 0.0);
		spec_power = pow(spec_power, shininess);*/
		vec3 V = normalize(-vs_pos);
		vec3 refl = reflect(-Lm, normal);
		float spec_power = max(dot(refl, V), 0.0);
		spec_power = pow(spec_power, shininess / 4.0);
		
		// komponent spec gotowy
		spec_component += spec.rgb * dirlight_specular[i] * spec_power;
	}
	
	// wyjście
	output_color.rgb = clamp(ambient_component + diff_component + spec_component, 0.0, 1.0);
	output_color.a = diff.a;
}