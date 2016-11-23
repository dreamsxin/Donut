#version 410 

// Input data
in vec2 texCoord;

// Out data
out vec4 frag_color;

// Input uniforms
uniform sampler2D composed;
uniform sampler2D specular;
uniform sampler2D normal;
uniform sampler2D position;
uniform samplerCube skybox;
uniform mat3 view_inverse;
uniform mat4 projection;
uniform float near_plane;

// Reflection parameters
int max_steps = 300;
float maxDistance = 500.0; 
float pixel_stride = 1.0;
float stride_Z_cutoff = 1.0;
float jitter = 0.0;
float zThickness = 0.0002;

const mat4 toTextureSpace = mat4(0.5f, 0.0f, 0.0f, 0.0f,
								 0.0f, 0.5f, 0.0f, 0.0f, 
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.5f, 0.5f, 0.0f, 1.0f);

// Tells if a given depth is between two depts
bool intersectsDepthBuffer(float z, float minZ, float maxZ)
{
	float thickness = z > 0.995 ? 0.0 : zThickness;
	return (maxZ >= z) && (minZ - thickness<= z);
}

// Swaps two float values
void swap(inout float a, inout float b)
{
	float t = a;
	a = b;
	b = t;
}

// Normalizes an interger pixel data
vec2 normalizeTexCoord(vec2 _pix)
{
	return vec2(_pix.x/1280.0, _pix.y/720.0);
}

float linearDepthTexelFetch(vec2 hitPixel)
{
	vec3 pos = texture(position, hitPixel).xyz;
	vec4 posProj = projection * vec4(pos, 1.0);
	return (hitPixel.x <0 || hitPixel.x > 1.0 || hitPixel.y <0 || hitPixel.y > 1.0) ? 0.0 : posProj.z / posProj.w;
}
 
void main()
{ 
	// Fetch reflection percentage
    float reflection = texture(specular, texCoord).z;

    vec4 originalColor = texture(composed, texCoord.xy);

    // Fetch the world space position
    vec3 vs_origin = texture(position, texCoord).xyz;

    // Fetch the textel's normal
    vec3 vs_normal = texture(normal, texCoord).xyz;

    // This is the direction we will be looking through
    vec3 vs_direction = reflect(normalize(vs_origin), vs_normal);

	// Farest contribution point possible
	float maxZ = (vs_origin.z + vs_direction.z * maxDistance);

	// We clip the ray length to the near plane
	float rayLength = maxZ > -near_plane ? (-near_plane - vs_origin.z) / vs_direction.z : maxDistance;

	// Compute the real end point
	vec3 vs_endPoint = vs_origin + vs_direction * rayLength;

	// Project the star and end point into homogeneous space
	vec4 H0 = toTextureSpace * projection * vec4(vs_origin, 1.0f);
	vec4 H1 = toTextureSpace * projection * vec4(vs_endPoint, 1.0f);

	// Scale the X and Y coordinates in order to iterate over pixels
	H0 *= vec4(1280.0, 720.0, 1.0, 1.0);
	H1 *= vec4(1280.0, 720.0, 1.0, 1.0);

	// Compute the homogenous factor for each point
	float inv_w0 = 1.0f / H0.w;
	float inv_w1 = 1.0f / H1.w;

	// The interpolated homogeneous version of the camera-space points
	float z0_w = H0.z * inv_w0;
	float z1_w = H1.z * inv_w1;

	// Compute the homogenous pixel data
	vec2 P0 = H0.xy * inv_w0;
	vec2 P1 = H1.xy * inv_w1;

	// If the line is degenerate, make it cover at least one pixel
	// to avoid handling zero-pixel extent as a special case later
	P1 += (length(P0- P1) < 0.0001f) ? vec2(1.0f, 1.0f) : vec2(0.0f, 0.0f);

	// Compute the 2D delta to rasterize
	vec2 delta = P1 - P0;
	// Permute so that the primary iteration is in x to collapse
	// all quadrant-specific DDA cases later
	bool permute = false;

	if(abs(delta.x) < abs(delta.y))
	{
		// This is a more-vertical line
		permute = true;
		delta = delta.yx;
		P0 = P0.yx;
		P1 = P1.yx;
	}

	// Compute the step dir of the X coord
	float stepDir = sign(delta.x);
	float invdx = stepDir / delta.x;

	// Track the derivatives of Q and k
	float dz = (z1_w - z0_w) * invdx;
	vec2 dP = vec2(stepDir, delta.y * invdx);

	// Scale derivatives by the desired pixel stride and then
	// offset the starting values by the jitter fraction
	/*
	float strideScale = 1.0f - min(1.0f, vs_origin.z * stride_Z_cutoff);
	float stride = 1.0f + strideScale * pixel_stride;
	dP *= stride;
	dQ *= stride;
	dk *= stride;
	P0 += dP * 1.0;
	Q0 += dQ * 1.0;
	k0 += dk * 1.0;
	*/

	// Slide P from P0 to P1, (now-homogeneous) Q from Q0 to Q1, k from k0 to k1
	vec3 PQk = vec3(P0, z0_w);
	vec3 dPQk = vec3(dP, dz);

	// Adjust end condition for iteration direction
	float end = P1.x * stepDir;

	// Initla estimate is the starting point
	float prevZMaxEstimate = PQk.z;
	// THe initial min and max z are also the starting point
	float rayZMin = prevZMaxEstimate;
	float rayZMax = prevZMaxEstimate;

	// Value just to avoid intersection
	float candidateZ = rayZMax - 100.0f;

	// output variables
	vec2 hitPixel =  vec2(0.0,0.0);

	// We do not want to hit a reflective surface
	float reflect = 1.0;
	// we count the number of steps
	bool continueCondition = true;
	bool intersection = false;

	// Why we didn't reach the end and we didn't get to the last step and it didn't overlap
	for(int step = 0; continueCondition && (step < max_steps); ++step)
	 {
	 	// Compute the depth rage
		rayZMin = prevZMaxEstimate;
		rayZMax = (dPQk.z * 0.5 + PQk.z);

		// Fetch the current pixel
		hitPixel = permute ? PQk.yx : PQk.xy;

		// Fetch the candidate's depth
		candidateZ = linearDepthTexelFetch(normalizeTexCoord(hitPixel));

		// Is the target reflective?
		reflect = texture(specular, normalizeTexCoord(hitPixel)).z;
		
		// Compute the intersection
		intersection = intersectsDepthBuffer(candidateZ, rayZMin, rayZMax) && (reflect == 0.0);

		// Adjust the parameter for next step
		prevZMaxEstimate = rayZMax;
		PQk += dPQk;

		// Are we done with the rasterization? is the pixel matchting without being refective  and are we still processing a viable pixel (not oob)
		continueCondition = (PQk.x*stepDir <= end) && !intersection && (candidateZ != 0.0f);
	}
	
	// return the target color
	frag_color = (1-reflection)*originalColor + (intersection? texture(composed, normalizeTexCoord(hitPixel)) : texture(skybox, view_inverse *vs_direction)) * reflection;
}
