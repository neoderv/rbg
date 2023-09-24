#version 410 core
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 1) in vec3 vPos;
layout(location = 2) in vec2 UV;
layout(location = 3) uniform mat4 model;
layout(location = 4) uniform mat4 view;
layout(location = 5) uniform mat4 projection;

out vec2 uvOut;

void main() {
	gl_Position = projection * view * model * vec4(vPos,1.0);
	uvOut = UV;
}
