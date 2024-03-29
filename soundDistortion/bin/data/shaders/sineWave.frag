#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

#define PI 3.14159265358

uniform sampler2DRect diffuseTexture;
uniform vec2 u_mousePos;
uniform float u_amp;
uniform float u_blur;

varying vec2 texCoord;

void main() {
    
	vec2 mousePos = u_mousePos;
	vec2 pos = texCoord;

	float mouseX = 1.0 - (u_mousePos.x / 1920);

	pos.x /= 1920;
	pos.y /= 1080;

	float c = pos.x * 2 * PI;
	c += mouseX * 2 * PI;
	if (c < PI || c > 3*PI) {
		c = PI;
	}
	float amp = u_amp;
	float a = amp * cos(c) - (1.0 - amp);

	//mousePos.x *= (1920 / 1080);
	//mousePos.x *= 1920 / 1080;

	//float dist = distance(pos, mousePos);

	float prob = smoothstep(a - u_blur , a + u_blur, (1.0 - pos.y));

	prob = 1.0 - prob;

	prob *= 0.7;


    gl_FragColor = vec4(prob, 0.0, 0.0, 1.0);
}
