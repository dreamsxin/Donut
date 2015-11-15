#version 410 

// This is a gbuffer light vertex pipeline

// out color (just the light contribution)
out vec4 frag_color;

// View and projection matrices
uniform mat4 view; 
uniform mat4 projection;

// Input Gbuffer Data
uniform sampler2D albedo;
uniform sampler2D normal;
uniform sampler2D specular;
uniform sampler2D position;
uniform sampler2D depth;

// Additional data
uniform int width;
uniform int lenght;

// input data
in vec2 texCoord;

// The light source data
struct TLight
{
	vec3 position;
	vec4 diffuse;
	vec4 specular;
	float ray;
};
// Delcaring the light source
uniform TLight lightSource;


void main()
{
	// Fetching normal (view space)
	vec3 normal = texture(normal, texCoord).xyz;
	// Fetching specularity
	float specCoeff = 1.0 - texture(specular,texCoord).r;

	// Getting detph
	float profondeur = texture(depth,texCoord).r;
	if(profondeur == 1.0)
	{
		// Far away => discard
		discard;
	}

	// Computing light source position (view space)
	vec3 lightPos = (view*vec4(lightSource.position,1.0)).xyz;
	// Fetching xyz position (view space)
	vec3 pixelPos = texture(position,texCoord).xyz;

	// Computing the light direction
	vec3 l = lightPos - pixelPos;
	// Computing the attenuation
 	float att = clamp(1.0-length(l)/lightSource.ray,0.0,1.0)*clamp(dot(l,normal),0.0,1.0);
	// Noramlizing it
	l = normalize(l);
	// Computing the view vector
	vec3 v = normalize(-pixelPos);
	// Computing the half vector
	vec3 h = normalize(v + l);
	// Illumination coeffs
    float Idiff = 2.0*clamp(dot(l, normal),0.0,1.0);	
    float Ispec = pow(clamp(dot(h,normal),0.0,1.0),10);
    frag_color = att*(Idiff*lightSource.diffuse + Ispec*lightSource.specular);
}