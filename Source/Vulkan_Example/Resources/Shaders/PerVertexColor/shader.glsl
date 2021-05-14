#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_TransformMatrix;

out vec4 v_Position;
void main(){
    v_Position = a_Color;
    gl_Position = u_ProjectionViewMatrix * u_TransformMatrix * vec4(a_Position, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec4 v_Position;

void main(){
    color = v_Position;
}