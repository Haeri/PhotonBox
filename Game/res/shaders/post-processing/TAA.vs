#version 420

layout (location = 0) in vec3 position;

out vec2 texCoordsFrag;

void main(){
	gl_Position = vec4(position.xy, 0, 1.0);
	texCoordsFrag = (position.xy + vec2(1)) * 0.5;
}
