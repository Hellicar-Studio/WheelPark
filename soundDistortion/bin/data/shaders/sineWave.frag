#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect diffuseTexture;
uniform vec2 u_mousePos;
uniform float u_amp;
uniform float u_offsetX;
uniform float u_offsetY;

varying vec2 texCoord;

void main() {
    
	vec2 mousePos = u_mousePos;
	vec2 pos = texCoord;

	pos.x /= 1920;

	float x = mousePos.x / 1920;

	float a = 1.0 * sin(pos.x + 0.0) + 0.0;

	//mousePos.x *= (1920 / 1080);
	//mousePos.x *= 1920 / 1080;

	//float dist = distance(pos, mousePos);

	float prob = a;//smoothstep(800, 0.0, a);

    gl_FragColor = vec4(prob, 0.0, 0.0, 1.0);
}
