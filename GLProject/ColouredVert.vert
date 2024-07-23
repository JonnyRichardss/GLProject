#version 460
layout(location=0) in vec3 vertPos;
layout(location=1) in vec3 vertCol;
layout(location=2) in vec3 vertNormal;
layout(location=3) in vec2 vertUV;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



out vec3 FragPos;
out vec3 ourCol;
out vec3 ourNormal;
void main(){
	gl_Position = projection * view * model * vec4(vertPos,1.0f);
	FragPos = vec3(model * vec4(vertPos,1.0f));
	ourCol = vertCol;
	ourNormal = mat3(transpose(inverse(model))) * vertNormal;  
}

