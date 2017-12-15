#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect diffuseTexture;
uniform vec2 u_mousePos;

varying vec2 texCoord;

void main() {
    
	vec2 mousePos = u_mousePos;
	vec2 pos = texCoord;

	//mousePos.x *= (1920 / 1080);
	//mousePos.x *= 1920 / 1080;

	float dist = distance(pos, mousePos);

	float prob = smoothstep(600, 0.0, dist);

    gl_FragColor = vec4(prob, 0.0, 0.0, 1.0);
}
