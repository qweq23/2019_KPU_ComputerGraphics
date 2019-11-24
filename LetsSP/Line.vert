#version 330

in vec2 vPos;
out vec3 passColor;

void main(){
	gl_Position = vec4 (vPos, 0.0f, 1.0f);
	passColor = vec3 (1.0f, 1.0f, 1.0f);
}