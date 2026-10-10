#version 450 core

#define WORK_GROUP_DIM 16

layout(local_size_x = WORK_GROUP_DIM, local_size_y = WORK_GROUP_DIM) in;

layout(binding = 0, rgba32f) writeonly uniform image2D u_initial_spectrum;
layout(binding = 1)  uniform sampler2D noise;

uniform int resolution;
uniform int oceanSize;
uniform float windspeed;
uniform float fetch;
uniform float peak_enhancement;
uniform float wind_angle; // rad
uniform float spread;
uniform float spread_norm;

const float g = 9.81;
const float PI = 3.14159265358979323846264;
const float alpha = 0.0081;

float sigma = f <= fp ? 0.07 : 0.09;
float fetch_dash = g * fetch / (windspeed*windspeed);
float fp = 3.5 * g / windspeed / pow(fetch_dash,0.33);
float r = exp(-(pow(f-fp),2) / (2 * sigma*sigma * fp*fp));


float jonswap(w, wp, alpha){
	return ((alpha * g*g / pow(w, 5)) * exp(-5/4 * pow(wp/w, 4))) * pow(peak_enhancement, r);
}
void main() {
	ivec2 id = ivec2(gl_GlobalInvocationID.xy);
	if(any(greaterThanEqual(id, ivec2(resolution))) return;
	ivec2 n = ivec2(id.x < resolution / 2 ? id.x : id.x - resolution,
					id.y < resolution / 2 ? id.y : id.y - resolution);
	float dk = 2.0 * PI / oceanSize;
	vec2 k = dk * vec2(n);
	float kl = length(k);

	vec2 h0(0.0,0.0);

	float w = sqrt(g/kl);
	float wp = 22 * (g*g / windspeed / fetch);
	float X_t = windspeed * windspeed / fetch / g;
	float alpha = 0.076 * pow(X_t, 0.22);
	float d = w-wp;
	float sigma = w <= wp ? 0.07 : 0.09;
	float r = exp(-d*d / (2 * sigma *  sigma * wp * wp));
	float S = jonswap(w, wp, alpha);
	// convert the S to P
	float cosD = dot(k/kl, vec2(cos(windspeed), sin(windspeed)));
	float D = spread_norm * pow(max(0.5 * (1.0 + cosD),0.0), spread);
	float dwdk = g / (2 * w);
	float P = S * D * dwdk / kl;


	vec2 xi = texelFetch(noise, id, 0).rg;
	h0 = xi * inversesqrt(2.0) * dk * sqrt(P * 0.5);
	imageStore(u_initial_spectrum, id, vec4(h0, 0.0, 0.0));

}