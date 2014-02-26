#version 410

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;
uniform mat4 modelviewprojection;

in vec3 position;
in vec3 normal;
in vec2 tex_coord;

out vec3 fnormal;
out vec2 texCoord;


void main()
{
    // gl_Position = projection*view*model*vec4(position,1.0);
    gl_Position = modelviewprojection*vec4(position,1.0);
    fnormal = normal;
    texCoord = tex_coord;
}