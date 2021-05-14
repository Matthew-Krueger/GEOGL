#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_TransformMatrix;

void main()
{
    gl_Position = u_ProjectionViewMatrix * u_TransformMatrix * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

uniform vec3 u_Color;

layout(location = 0) out vec4 color;

void main()
{
    color = vec4(u_Color,1.0f);
}