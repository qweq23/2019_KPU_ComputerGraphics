#version 330

in vec2 vPos;
in vec3 vColor;
out vec3 passColor;

uniform mat4 modelTransform;

void main(){
	gl_Position = modelTransform * vec4(vPos, 0.0f, 1.0f);
	passColor = vColor;
}