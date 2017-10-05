#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect diffuseTexture;
uniform vec2 u_mousePos;

varying vec2 texCoord;

void main() {
    
	vec2 mousePos = u_mousePos / vec2(1920, 1080);
	vec2 pos = texCoord / vec2(1920, 1080);

	float distX = abs(pos.x - mousePos.x);

	float prob = smoothstep(0.2, 0.0, distX);

    gl_FragColor = vec4(prob, 0.0, 0.0, 1.0);
}
