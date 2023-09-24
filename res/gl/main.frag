#version 410 core
#extension GL_ARB_explicit_uniform_location : enable
layout(location = 0) out vec4 fragColor;
layout(location = 6) uniform sampler2D text;

in vec2 uvOut;

void main() {
	fragColor = texture(text,uvOut);
}
