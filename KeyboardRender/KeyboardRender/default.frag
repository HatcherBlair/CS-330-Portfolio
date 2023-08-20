#version 440 core

// Output color
out vec4 fragmentColor;

// Current position
in vec3 crntPos;
// Normal position
in vec3 Normal;
// Color
in vec4 vertexColor;
// Texture coord
in vec2 texCoord;

// Get the texture units from main
uniform sampler2D tex0;
// Get light color from main
uniform vec4 pointLightColor;
uniform vec4 directLightColor;
// Get light position from main
uniform vec3 lightPos;
// Get the camera position from main
uniform vec3 camPos;

vec4 pointLight() {

	vec3 lightVec = lightPos - crntPos;

	// Intensity of light
	float inten = 1.0f;

	// ambient lighting
	vec3 ambient = 0.01f * pointLightColor.xyz;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float impact = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuse = impact * pointLightColor.xyz;

	// Specular lighting
	float specularIntensity = 0.8f;
	float highlightSize = 16.0f;
	vec3 viewDir = normalize(camPos - crntPos);
	vec3 reflectDir = reflect(-lightDirection, normal);
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComponent * pointLightColor.xyz;

	vec4 textureColor = texture(tex0, texCoord);
	
	vec3 phong = (ambient + diffuse + specular) * textureColor.xyz * inten;

	return vec4(phong, 1.0);
}

vec4 directLight() {
	// Intensity of light
	float inten = 0.3f;

	// ambient lighting
	vec3 ambient = 0.01f * directLightColor.xyz;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(-1.0f, 1.0f, 0.0f));
	float impact = max(dot(normal, lightDirection), 0.0f);
	vec3 diffuse = impact * directLightColor.xyz;

	// Specular lighting
	float specularIntensity = 0.8f;
	float highlightSize = 16.0f;
	vec3 viewDir = normalize(camPos - crntPos);
	vec3 reflectDir = reflect(-lightDirection, normal);
	float specularComponent = pow(max(dot(viewDir, reflectDir), 0.0), highlightSize);
	vec3 specular = specularIntensity * specularComponent * directLightColor.xyz;

	vec4 textureColor = texture(tex0, texCoord);

	vec3 phong = (ambient + diffuse + specular) * inten * textureColor.xyz;

	return vec4(phong, 1.0);
}

void main() {


	fragmentColor = (directLight() + pointLight());
}