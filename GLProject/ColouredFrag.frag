#version 460
out vec4 FragColor;

in vec3 FragPos;
in vec3 ourCol;
in vec3 ourNormal;
uniform vec3 cameraPos;
void main(){
	//make these uniform
	float glossVal = 20.0f;
	vec3 lightPos = vec3(0.0f,10.0f,15.0f);
	//vec3 cameraPos = vec3(0.0f,0.0f,0.0f);

	vec3 finalCol = vec3(0.0f,0.0f,0.0f);
	vec3 viewDir = normalize(cameraPos - FragPos);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 normalDir = normalize(ourNormal);//normals are not guaranteed to be normalised thanks to interpolation


	vec3 halfVec = normalize(lightDir + viewDir);
	float specular = clamp(dot(halfVec,normalDir),0.0f,1.0f);
	specular = pow(specular, glossVal);
	//vec3 lightDir = vec3(0.0f,0.0f,1.0f);
	finalCol *= clamp(dot(normalDir,lightDir),0.0f,1.0f);
	finalCol += specular.xxx;
	FragColor = vec4(finalCol,1.0f);

}