#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextureCoord;
layout(location = 3) in float a_TilingFactor;
layout(location = 4) in float a_TextureIndex;

uniform mat4 u_ProjectionViewMatrix;

out vec4 v_Color;
out vec2 v_TextureCoord;
out float v_TilingFactor;
out float v_TextureIndex;

void main(){
    v_Color = a_Color;
    v_TextureCoord = a_TextureCoord;
    v_TextureIndex = a_TextureIndex;
    v_TilingFactor = a_TilingFactor;
    gl_Position = u_ProjectionViewMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

in vec4 v_Color;
in vec2 v_TextureCoord;
in float v_TilingFactor;
in float v_TextureIndex;

uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 color;

void main(){
    color = texture(u_Textures[(int(v_TextureIndex))], v_TextureCoord * v_TilingFactor) * v_Color;
}