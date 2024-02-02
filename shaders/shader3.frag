#version 120

varying vec3 N;
varying vec3 Pos;

void main()
{
	vec3 eye = {0.0, 0.0, 4.0};
	vec3 E = eye - Pos;
	E = normalize(E);

	float angle = dot(N, E);
	if(angle < 0.3){
		gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else{
		gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
}
