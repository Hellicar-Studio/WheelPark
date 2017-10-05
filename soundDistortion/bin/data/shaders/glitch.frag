#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect diffuseTexture;
uniform sampler2DRect u_glitchMask;
uniform float u_time;

uniform float u_aberrationStrength;
uniform float u_glitchScale = .5;
uniform vec4 u_groupSize;
uniform vec4 u_subGrid;
uniform vec4 u_blockSize;

uniform float u_badTVDistort;
uniform float u_badTVDistort2;
uniform float u_badTVSpeed;
uniform float u_badTVRollSpeed;
uniform float u_badTVAmount;

varying vec2 texCoord;

// --------------------------------------------------------
// Bad TV
// --------------------------------------------------------


vec3 mod289(vec3 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
	return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
	return mod289(((x*34.0) + 1.0)*x);
}

float snoise(vec2 v) {
	const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
		0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
		-0.577350269189626,  // -1.0 + 2.0 * C.x
		0.024390243902439); // 1.0 / 41.0
	vec2 i = floor(v + dot(v, C.yy));
	vec2 x0 = v - i + dot(i, C.xx);
	vec2 i1;
	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
	vec4 x12 = x0.xyxy + C.xxzz;
	x12.xy -= i1;
	i = mod289(i); // Avoid truncation effects in permutation
	vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0))
		+ i.x + vec3(0.0, i1.x, 1.0));
	vec3 m = max(0.5 - vec3(dot(x0, x0), dot(x12.xy, x12.xy), dot(x12.zw, x12.zw)), 0.0);
	m = m*m;
	m = m*m;
	vec3 x = 2.0 * fract(p * C.www) - 1.0;
	vec3 h = abs(x) - 0.5;
	vec3 ox = floor(x + 0.5);
	vec3 a0 = x - ox;
	m *= 1.79284291400159 - 0.85373472095314 * (a0*a0 + h*h);
	vec3 g;
	g.x = a0.x  * x0.x + h.x  * x0.y;
	g.yz = a0.yz * x12.xz + h.yz * x12.yw;
	return 130.0 * dot(m, g);
}

// --------------------------------------------------------
// Glitch core
// --------------------------------------------------------

float rand(vec2 co) {
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec2 glitchCoord(vec2 p, vec2 gridSize) {
	vec2 coord = floor(p / gridSize) * gridSize;;
	coord += (gridSize / 2.);
	return coord;
}

struct GlitchSeed {
	vec2 seed;
	float prob;
};

GlitchSeed glitchSeed(vec2 p, float speed, float prob) {
	float seedTime = floor(u_time * speed);
	vec2 seed = vec2(
		1. + mod(seedTime / 100., 100.),
		1. + mod(seedTime, 100.)
	) / 100.;
	seed += p;

	//float prob = texture2DRect(u_glitchMask, p * vec2(1920, 1080)).r;
	//Hit hit = raymarchPixel(p, true);
	//if (!hit.isBackground) {
		//prob = hit.model.material.albedo.x;
	//}

	return GlitchSeed(seed, prob);
}

float shouldApply(GlitchSeed seed) {
	return round(
		mix(
			mix(rand(seed.seed), 1., seed.prob - .5),
			0.,
			(1. - seed.prob) * .5
		)
	);
}

float isInBlock(vec2 pos, vec4 block) {
	vec2 a = sign(pos - block.xy);
	vec2 b = sign(block.zw - pos);
	return min(sign(a.x + a.y + b.x + b.y - 3.), 0.);
}

vec2 moveDiff(vec2 pos, vec4 swapA, vec4 swapB) {
	vec2 diff = swapB.xy - swapA.xy;
	return diff * isInBlock(pos, swapA);
}

// --------------------------------------------------------
// Glitch effects
// --------------------------------------------------------

// Swap

vec4 swapCoords(vec2 seed, vec2 groupSize, vec2 subGrid, vec2 blockSize) {
	vec2 rand2 = vec2(rand(seed), rand(seed + .1));
	vec2 range = subGrid - (blockSize - 1.);
	vec2 coord = floor(rand2 * range) / subGrid;
	vec2 bottomLeft = coord * groupSize;
	vec2 realBlockSize = (groupSize / subGrid) * blockSize;
	vec2 topRight = bottomLeft + realBlockSize;
	topRight -= groupSize / 2.;
	bottomLeft -= groupSize / 2.;
	return vec4(bottomLeft, topRight);
}

void swapBlocks(inout vec2 xy, vec2 groupSize, vec2 subGrid, vec2 blockSize, vec2 seed, float apply) {

	vec2 groupOffset = glitchCoord(xy, groupSize);
	vec2 pos = xy - groupOffset;

	vec2 seedA = seed * groupOffset;
	vec2 seedB = seed * (groupOffset + .1);

	vec4 swapA = swapCoords(seedA, groupSize, subGrid, blockSize);
	vec4 swapB = swapCoords(seedB, groupSize, subGrid, blockSize);

	vec2 newPos = pos;
	newPos += moveDiff(pos, swapA, swapB) * apply;
	newPos += moveDiff(pos, swapB, swapA) * apply;
	pos = newPos;

	xy = pos + groupOffset;
}


// Static

void staticNoise(inout vec2 p, vec2 groupSize, float grainSize, float contrast, float prob) {
	GlitchSeed seedA = glitchSeed(glitchCoord(p, groupSize), 5., prob);
	seedA.prob *= .5;
	if (shouldApply(seedA) == 1.) {
		GlitchSeed seedB = glitchSeed(glitchCoord(p, vec2(grainSize)), 5., prob);
		vec2 offset = vec2(rand(seedB.seed), rand(seedB.seed + .1));
		offset = round(offset * 2. - 1.);
		offset *= contrast;
		p += offset;
	}
}

// Freeze time

void freezeTime(vec2 p, inout float time, vec2 groupSize, float speed, float prob) {
	GlitchSeed seed = glitchSeed(glitchCoord(p, groupSize), speed, prob);
	//seed.prob *= .5;
	if (shouldApply(seed) == 1.) {
		float frozenTime = floor(time * speed) / speed;
		time = frozenTime;
	}
}


// --------------------------------------------------------
// Glitch compositions
// --------------------------------------------------------

void glitchSwap(inout vec2 p, float prob) {

	vec2 pp = p;

	float scale = u_glitchScale;
	float speed = 10.;

	vec2 groupSize;
	vec2 subGrid;
	vec2 blockSize;
	GlitchSeed seed;
	float apply;

	groupSize = vec2(u_groupSize[0]) * scale; //0.6
	subGrid = vec2(u_subGrid[0]); //2
	blockSize = vec2(u_blockSize[0]); //0.2

	seed = glitchSeed(glitchCoord(p, groupSize), speed, prob);
	apply = shouldApply(seed);
	swapBlocks(p, groupSize, subGrid, blockSize, seed.seed, apply);

	groupSize = vec2(u_groupSize[1]) * scale; //0.8
	subGrid = vec2(u_subGrid[1]); //3
	blockSize = vec2(u_blockSize[1]); //1

	seed = glitchSeed(glitchCoord(p, groupSize), speed, prob);
	apply = shouldApply(seed);
	swapBlocks(p, groupSize, subGrid, blockSize, seed.seed, apply);

	groupSize = vec2(u_groupSize[2]) * scale; //0.2
	subGrid = vec2(u_subGrid[2]); //6
	blockSize = vec2(u_blockSize[2]); //1

	seed = glitchSeed(glitchCoord(p, groupSize), speed, prob);
	float apply2 = shouldApply(seed);
	swapBlocks(p, groupSize, subGrid, blockSize, (seed.seed + 1.), apply * apply2);
	swapBlocks(p, groupSize, subGrid, blockSize, (seed.seed + 2.), apply * apply2);
	swapBlocks(p, groupSize, subGrid, blockSize, (seed.seed + 3.), apply * apply2);
	swapBlocks(p, groupSize, subGrid, blockSize, (seed.seed + 4.), apply * apply2);
	swapBlocks(p, groupSize, subGrid, blockSize, (seed.seed + 5.), apply * apply2);

	groupSize = vec2(u_groupSize[3]) * scale; //1.2, .2
	subGrid = vec2(u_subGrid[3]); //9, 2
	blockSize = vec2(u_blockSize[3]); //6, 1

	seed = glitchSeed(glitchCoord(p, groupSize), speed, prob);
	apply = shouldApply(seed);
	swapBlocks(p, groupSize, subGrid, blockSize, seed.seed, apply);
}

void glitchStatic(inout vec2 p, float prob) {

	// Static
	//staticNoise(p, vec2(.25, .25/2.) * u_glitchScale, .005, 5.);

	// 8-bit
	staticNoise(p, vec2(.5, .25 / 2.) * u_glitchScale, .2 * u_glitchScale, 2., prob);
}

void glitchTime(vec2 p, inout float time, float prob) {
	freezeTime(p, time, vec2(.5) * u_glitchScale, 2., prob);
}

void glitchColor(vec2 p, inout vec3 color, float prob) {
	vec2 groupSize = vec2(.75, .125) * u_glitchScale;
	vec2 subGrid = vec2(0, 6);
	float speed = 5.;
	GlitchSeed seed = glitchSeed(glitchCoord(p, groupSize), speed, prob);
	seed.prob *= .3;
	if (shouldApply(seed) == 1.) {
		vec2 co = mod(p, groupSize) / groupSize;
		co *= subGrid;
		float a = max(co.x, co.y);
		//color.rgb *= vec3(
		//  min(floor(mod(a - 0., 3.)), 1.),
		//    min(floor(mod(a - 1., 3.)), 1.),
		//    min(floor(mod(a - 2., 3.)), 1.)
		//);

		color *= min(floor(mod(a, 2.)), 1.) * 10.;
	}
}

void main() {
	float time = u_time;
    //Getting coordinates of the current pixel in texture
    vec2 pos = texCoord / vec2(1920, 1080);
	float prob = texture2DRect(u_glitchMask, pos * vec2(1920, 1080)).r;

	glitchSwap(pos, prob);
	glitchTime(pos, time, prob);
	glitchStatic(pos, prob);

	float timeBTV = time*u_badTVSpeed;
	float yt = pos.y - timeBTV;
	float offset = snoise(vec2(yt*3.0, 0.0))*0.2;
	//offset = pow(offset*u_badTVDistort, 3.0) / u_badTVDistort;
	offset += snoise(vec2(yt*50.0, 0.0))*u_badTVDistort2*0.001;
	vec2 normalizedCoords = vec2(fract(pos.x + offset), fract(pos.y - time*u_badTVRollSpeed));

	pos = mix(pos, normalizedCoords, u_badTVAmount*prob);
        
	vec3 color = vec3(0.0, 0.0, 0.0);
	//color.rg = pos;
	float cr = texture2DRect(diffuseTexture, pos * vec2(1920, 1080) + vec2(prob * u_aberrationStrength, 0.0)).r;
	float cg = texture2DRect(diffuseTexture, pos * vec2(1920, 1080)).g;
	float cb = texture2DRect(diffuseTexture, pos * vec2(1920, 1080) - vec2(prob * u_aberrationStrength, 0.0)).b;

	color = vec3(cr, cg, cb);

	//glitchColor(pos, color);
	//color.rg = vec3(1.0) - color.rgb;

    gl_FragColor = vec4(color, 1.0);
}
