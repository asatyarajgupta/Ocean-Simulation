#version 450 core
out vec4 fragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 fragPos;
uniform vec3 viewPos;

struct Material {
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 direction;
	float constant;
	float linear;
	float quadratic;
	float cutoff;
	float outerCutoff;

};


uniform Material material;
uniform Light light;


vec3 PointLight(Light light, vec3 norm, vec3 viewPos){
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear* distance + light.quadratic * distance * distance);

	vec3 ambientLight = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

	vec3 lightDir = normalize(light.position - fragPos);
	float texture_diffuse1 = max(dot(lightDir, norm), 0.0);
	// vec3 texture_diffuse1Light = texture_diffuse1 * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoords)) * attenuation;
	vec3 texture_diffuse1Light = texture_diffuse1 * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoords)) * attenuation;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 texture_specular1Light = spec * light.specular * vec3(texture(material.texture_specular1, TexCoords)) *attenuation;

	vec3 result = ambientLight + texture_diffuse1Light + texture_specular1Light;
	return result;

}

vec3 DirLight(Light light, vec3 norm, vec3 viewPos){
	vec3 ambientLight = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));

	vec3 lightDir = normalize(-light.position);
	float texture_diffuse1 = max(dot(lightDir, norm), 0.0);
	vec3 texture_diffuse1Light = texture_diffuse1 * light.diffuse * vec3(texture(material.texture_diffuse1, TexCoords));

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 texture_specular1Light = spec * light.specular * vec3(texture(material.texture_specular1, TexCoords));

	vec3 result = ambientLight + texture_diffuse1Light + texture_specular1Light;
	return result;

}

void main() {
	vec3 color = DirLight(light, normal, viewPos);
	fragColor = vec4(color,1.0);
}