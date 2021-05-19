#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoord;

uniform mat4 u_ProjectionViewMatrix;
uniform mat4 u_TransformMatrix;

out vec2 v_TextureCoord;

void main(){
    gl_Position = u_ProjectionViewMatrix * u_TransformMatrix * vec4(a_Position, 1.0);
    v_TextureCoord = a_TextureCoord;
}

#type fragment
#version 330 core

in vec2 v_TextureCoord;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 color;

void main(){
    vec4 textureColor = texture(u_Texture,v_TextureCoord);
    color = textureColor;
}