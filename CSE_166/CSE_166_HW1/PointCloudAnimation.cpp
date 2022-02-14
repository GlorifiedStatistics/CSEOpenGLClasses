#include "PointCloudAnimation.h"

// Colors for each point cloud: red, green, and blue
std::vector<glm::vec3> colors = { glm::vec3(1, 0, 1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) };
GLfloat startingPointSize = 5;
unsigned int transitionSteps = 100;



PointCloudAnimation::PointCloudAnimation(std::vector<std::string> filePaths, int windowWidth, int windowHeight) {

	// Generate an initial set of random points/colors
	clouds.push_back(randomPoints(10000));

	// Read in each point cloud from OBJ
	for (unsigned int i = 0; i < filePaths.size(); i++) 
		clouds.push_back(loadObj(filePaths[i], windowWidth, windowHeight));

	pointSize = startingPointSize;
	currentCloud = clouds[0];
	currentColor = colors[0];
	currentStep = transitionSteps + 1;

	transitionTo(1);
	
	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	setBufferData();
}

PointCloudAnimation::~PointCloudAnimation(){
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void PointCloudAnimation::draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader)
{
	// Actiavte the shader program 
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(currentColor));

	// Bind the VAO
	glBindVertexArray(VAO);

	// Set point size
	glPointSize(pointSize);

	// Draw the points 
	glDrawArrays(GL_POINTS, 0, currentCloud.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void PointCloudAnimation::update() {
	if (currentStep > transitionSteps) {
		// Spin the cube by a small amount
		spin(0.005f);
	}
	else if (currentStep == transitionSteps) {
		currentCloud = clouds[nextCloud];
		currentColor = colors[nextCloud];
		setBufferData();
	}
	else {
		interpolateNext();
		setBufferData();
	}
	currentStep++;
}

void PointCloudAnimation::updatePointSize(GLfloat size) {
	pointSize = pointSize + size < 1 ? 1 : pointSize + size;
}

void PointCloudAnimation::spin(float deg) {
	// Update the model matrix by multiplying a rotation matrix
	model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void PointCloudAnimation::setBufferData() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * currentCloud.size(), currentCloud.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PointCloudAnimation::transitionTo(unsigned int idx) {
	if (nextCloud == idx) return;

	nextCloud = idx;
	currentStep = 0;

	int num = clouds[nextCloud].size() - currentCloud.size();
	if (num > 0) {
		for (glm::vec3 p : zeroPoints(num)) currentCloud.push_back(p);
	}
	else if (num < 0) {
		extraPoints = zeroPoints(-num);
	}
}

glm::vec3 interpolateStep(glm::vec3 start, glm::vec3 end, int curr) {
	return (end - start) * (1.0f / (transitionSteps - curr));
}

// Interpolate color and points to their next existance
void PointCloudAnimation::interpolateNext() {
	unsigned int i;
	for (i = 0; i < clouds[nextCloud].size(); i++) {
		currentCloud[i] += interpolateStep(currentCloud[i], clouds[nextCloud][i], currentStep);
	}

	// If we have too many points, and some need to be sent to space
	if (i < currentCloud.size()) {
		unsigned int s = i;
		for (; i < currentCloud.size(); i++) {
			currentCloud[i] += interpolateStep(currentCloud[i], extraPoints[i - s], currentStep);
		}
	}

	// Lastly, update the color
	currentColor += interpolateStep(currentColor, colors[nextCloud], currentStep);
}

