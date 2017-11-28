#version 430

layout(location = 0) in vec3 g_Position;
layout(location = 2) in vec2 g_Texture;

out vec2 g_TexturePos;

uniform mat4 g_M;
uniform mat4 g_V;
uniform mat4 g_P;


void main() {
	gl_Position = g_P * g_V * g_M * vec4(g_Position, 1);


	g_TexturePos = g_Texture;
}
