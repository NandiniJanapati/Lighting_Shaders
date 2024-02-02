#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct lightStruct
{
	vec3 position;
	vec3 color;
};

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS];

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec3 color;

void main()
{
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);

	vec4 normal_H = model * vec4(vNormalModel, 0.0); //normal vector in homogenous coords
	vec3 N = normal_H.xyz;
	N = normalize(N);

	vec4 position_H = model * vec4(vPositionModel, 1.0); //position vector in homogenous coords
	vec3 Pos = {position_H.x/position_H.w, position_H.y/position_H.w, position_H.z/position_H.w};

	vec3 eye = {0.0, 0.0, 4.0};
	vec3 E = eye - Pos;
	E = normalize(E);
	
	
	vec3 I = ka;
	for(int i = 0; i < NUM_LIGHTS; i++){
		vec3 L = lights[i].position - Pos;
		L = normalize(L);
		vec3 R = 2 * (dot(L,N))*N - L;
		R = normalize(R);
		I = I + lights[i].color * (kd * max(0, dot(L, N)) + ks * pow(max(0, dot(R,E)), s));
	}
	
	
	color = I;
}