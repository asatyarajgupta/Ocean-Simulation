#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;

out vec2 TexCoords;
out vec3 normal;
out vec3 fragPos;

struct Wave{
	ivec2 m;
	float amplitude;
};
const float L = 50.0f;
const float PI = 3.14159265;
const int N = 4;

const Wave waves[N] = Wave[](
    Wave(ivec2( 5,  0), 0.5),
    Wave(ivec2( 4,  4), 0.4),
    Wave(ivec2(-2,  7), 0.3),
    Wave(ivec2( 3, -5), 0.2)
);

void main() {
	vec3 pos = aPos;
	
	vec3 T = vec3(1.0,0.0,0.0);
	vec3 B = vec3(0.0,0.0,1.0);

	for(int i = 0; i < N; i++){
		vec2 kvec = (2 * PI / L) * vec2(waves[i].m);
		float k = length(kvec);
		vec2 d = kvec / k;
		float omega = sqrt(9.8 * k);
		float A = waves[i].amplitude * 0.3;
		

		float phase = dot(kvec, aPos.xz) - omega * time;
		float c = cos(phase);
		float s = sin(phase);
		pos.xz -= d * A * s;
		pos.y -= A * c;
		
		T += vec3(-k*A*d.x*d.x*c, k*A*s*d.x, -k*A*d.x*d.y*c);
		B += vec3(-k*A*d.y*d.x*c, k*A*s*d.y, -k*A*d.y*d.y*c);
	}

	gl_Position = projection * view * model * vec4(pos, 1.0);
	TexCoords = aTexCoords;
	normal = normalize(mat3(model) * normalize(cross(B, T)));
	fragPos = vec3(model * vec4(pos, 1.0));
}

