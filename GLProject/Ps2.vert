#version 460
layout(location=0) in vec4 vertPos;
layout(location=1) in vec4 vertCol;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 ourCol;
void main(){
	gl_Position = projection * view * model *vertPos;

	ourCol = vertCol;

	ourCol += (((vertPos) +0.75f)/1.5f);
}

