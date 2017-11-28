#version 430

layout(location = 0) in vec3 g_Position;

out vec3 g_TexturePos;

uniform mat4 g_V;
uniform mat4 g_P;

void main() {
	vec4 pos = g_P * g_V * vec4(g_Position, 1);
	gl_Position = pos.xyww;


	g_TexturePos = normalize(g_Position);
}
