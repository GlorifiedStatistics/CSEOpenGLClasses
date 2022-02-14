#version 330 core
in vec3 fragNormal;
in vec3 fragVert;
in vec3 texCoords;
in vec2 modelTexCoords;

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
uniform int DRAW_REFLECTION;
uniform int DRAW_CUBEMAP;
uniform int DRAW_PHONG;
uniform int DRAW_FLOOR;

uniform samplerCube skybox;

// You can output many things. The first vec4 type output determines the color of the fragment
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

    // Reflect skybox
    else if (mode == DRAW_REFLECTION) {
  	vec3 pos_eye = vec3(view * model * vec4(fragVert, 1.0));
  	vec3 n_eye = vec3(view * model * vec4(fragNormal, 0.0));
	vec3 incident_eye = normalize(pos_eye);
  	vec3 normal = normalize(n_eye);

  	vec3 reflected = reflect(incident_eye, normal);
  	// convert from eye to world space
  	reflected = vec3(inverse(view) * vec4(reflected, 0.0));

  	fragColor = texture(skybox, reflected);
    }

    // Draw actual skybox
    else if (mode == DRAW_CUBEMAP) {
	fragColor = texture(skybox, texCoords);
    }

    else if (mode == DRAW_FLOOR) {
	fragColor = vec4(fragVert + vec3(0, 0.5, 0), 1);
    }

    // Phong shading
    else if (mode == DRAW_PHONG) {
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
    }

    else {
	fragColor = vec4(0, 0, 1, 1);
    }
}