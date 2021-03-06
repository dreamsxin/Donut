#version 330 
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in vec3 fnormalTE[];
in vec2 txCoordTE[];

out vec2 txCoord;
out vec3 fnormal;

void main()
{ 
  for(int i=0; i<3; i++)
  {
    gl_Position = gl_in[i].gl_Position;
    txCoord = txCoordTE[i];
    fnormal = fnormalTE[i];
    EmitVertex();
  }
  EndPrimitive();
}  