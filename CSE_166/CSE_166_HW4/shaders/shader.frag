#version 330 core
in vec3 fragNormal;
in vec3 fragVert;

uniform struct MaterialProperties{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
} matProps;

uniform struct Light{
    vec3 pos;
    vec3 intensities;
    float attenuation;
    float ambientCoefficient;
    float exponent;
} light;

uniform int mode;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 color;
uniform mat4 model;
uniform mat4 view;

uniform int DRAW_NORMALS;
uniform int DRAW_COLOR;
uniform int DRAW_PHONG;
uniform int DRAW_TOON;

uniform float TOON_OUTLINE;

out vec4 fragColor;

void main() {

    // Normal shading
    if (mode == DRAW_NORMALS) {
	fragColor = vec4((fragNormal + 1) / 2, 1.0f);
    }

    // Absolute color shading
    else if (mode == DRAW_COLOR) {
	fragColor = vec4(color, 1.0f);
    }

    // Phong shading
    else if (mode == DRAW_PHONG || mode == DRAW_TOON) {
	mat3 normalMat = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMat * fragNormal);

	vec3 fragPosition = vec3(model * vec4(fragVert, 1));
	vec3 surfaceToLight = light.pos - fragPosition;
	vec3 surfaceToView = fragPosition - cameraPos;

	// The light attenuation
	float atten = 1 / (1 + light.attenuation * pow(length(surfaceToLight), light.exponent));

	// Normalize the light and view vectors now
	surfaceToLight = normalize(surfaceToLight);
	surfaceToView = normalize(surfaceToView);

	// The ambient light
	vec3 ambient = matProps.ambient * light.ambientCoefficient;

	// The diffuse light
	vec3 diffuse = matProps.diffuse * dot(normal, surfaceToLight);

	// The specular light
	vec3 perfectReflection = -normalize(2 * dot(surfaceToLight, normal) * normal - surfaceToLight);
	vec3 specular = matProps.specular * pow(clamp(dot(perfectReflection, surfaceToView), 0, 1), matProps.shininess);

	fragColor = vec4((ambient + diffuse + specular) * light.intensities * atten, 1);

	if (mode == DRAW_TOON){

	    float intensity = (fragColor[0] + fragColor[1] + fragColor[2]) / 3.0f;
	    vec4 c = vec4(color, 1);

	    if (abs(dot(surfaceToView, normal)) < TOON_OUTLINE){
	        fragColor = vec4(0, 0, 0, 1);
	    }else if (intensity > 0.8){
	        fragColor = c;
	    }else if (intensity > 0.5){
	        fragColor = vec4(0.7, 0.7, 0.7, 1.0) * c;
	    }else if (intensity > 0.05){
	        fragColor = vec4(0.35, 0.35, 0.35, 1.0) * c;
	    }else {
	        fragColor = vec4(0.1, 0.1, 0.1, 1.0) * c;
	    }

	}
    }

    else {
	fragColor = vec4(0, 0, 1, 1);
    }
}