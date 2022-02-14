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
uniform mat4 model;
uniform mat4 view;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    if (mode == 0) {
	fragColor = vec4((fragNormal + 1) / 2, 1.0f);
    }

    else{
	mat3 normalMat = transpose(inverse(mat3(model)));
	vec3 normal = normalize(normalMat * fragNormal);

	vec3 fragPosition = vec3(model * vec4(fragVert, 1));
	vec3 surfaceToLight = light.pos - fragPosition;
	vec3 surfaceToView = fragPosition - vec3(view[0][3], view[1][3], view[2][3]);

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
	vec3 perfectReflection = normalize(2 * dot(surfaceToLight, normal) * normal - surfaceToLight);
	vec3 specular = matProps.specular * pow(clamp(dot(perfectReflection, surfaceToView), 0, 1), matProps.shininess);

	//fragColor = vec4((ambient + diffuse + specular) * light.intensities * atten, 1);
	fragColor = vec4((ambient + diffuse + specular) * light.intensities * atten, 1);
    }
}