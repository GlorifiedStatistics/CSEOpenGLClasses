#include "PointUtils.h"



// Code taken from the writeup
std::vector<glm::vec3> loadObj(std::string filename, int windowWidth, int windowSize) {
	std::ifstream objFile(filename); // The obj file we are reading.
	std::vector<glm::vec3> points;

	bool notDone = true;

	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.

		// Read lines from the file.
		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;

			// Read the first word of the line.
			std::string label;
			ss >> label;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				points.push_back(point);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << filename << std::endl;
	}

	objFile.close();
	return centerAndResize(points, windowWidth, windowSize);
}


// Gets the centers of the point cloud
glm::vec3 getCenters(std::vector<glm::vec3>& pc) {
	// Set the initial min and max values
	GLfloat minx = mx, miny = mx, minz = mx, maxx = mn, maxy = mn, maxz = mn;

	// Find min and max in each dim
	for (glm::vec3 p : pc) {
		if (p.x < minx) minx = p.x;
		if (p.x > maxx) maxx = p.x;

		if (p.y < miny) miny = p.y;
		if (p.y > maxy) maxy = p.y;

		if (p.z < minz) minz = p.z;
		if (p.z > maxz) maxz = p.z;
	}

	return glm::vec3((maxx + minx) / 2, (maxy + miny) / 2, (maxz + minz) / 2);
}



// Center the points, and resize them to the window
std::vector<glm::vec3> centerAndResize(std::vector<glm::vec3> pc, int width, int height) {

	// Center the points by finding the min and max values, and setting middle of each dimension
	glm::vec3 center = getCenters(pc);

	// Shift all the points by subtracting the center values
	for (unsigned int i = 0; i < pc.size(); i++) pc[i] -= center;

	// Get the largest absolute offset from the center
	GLfloat maxx = mn, maxy = mn, maxz = mn;
	for (glm::vec3 p : pc) {
		GLfloat x = abs(p.x), y = abs(p.y), z = abs(p.z);
		if (x > maxx) maxx = x;
		if (y > maxy) maxy = y;
		if (z > maxz) maxz = z;
	}

	// Find the ratio between the min window size and the max distance for each width/height
	GLfloat scalew = M_x / (maxx > maxz ? maxx : maxz);
	GLfloat scaleh = M_y / maxy;

	// Pick the smallest scale between the two because we want the whole object to fit into frame
	GLfloat scale = scalew < scaleh ? scalew : scaleh;

	// Scale all the points by multiplying by the scale in each direction
	for (unsigned int i = 0; i < pc.size(); i++) pc[i] *= scale;

	return pc;
}



// Generate a set of random points within the given range with random colors and sizes
std::vector<glm::vec3> randomPoints(unsigned int num) {
	std::vector<glm::vec3> points;

	for (unsigned int i = 0; i < num; i++)
		points.push_back(glm::vec3(randomFloat(m_x, M_x), randomFloat(m_y, M_y), randomFloat(m_z, M_z)));

	return std::vector<glm::vec3>(points);
}

// Generate a set of random points within the given range with random colors and sizes
std::vector<glm::vec3> randomPointsExplode(unsigned int num) {
	std::vector<glm::vec3> points;
	
	glm::vec3 p;
	for (unsigned int i = 0; i < num; i++) {
		while (true) {
			p = glm::vec3(randomFloat(m_x * E_x, M_x * E_x),
				randomFloat(m_y * E_y, M_y * E_y), randomFloat(m_z * E_x, M_z * E_x));

			// If the point is outside that inner cube, then exit
			if (p.x < m_x * e_x || p.y < m_y * e_y || p.z < m_z * e_x
				|| p.x > M_x * e_x || p.y > M_y * e_y || p.z > M_z * e_x) {
				break;
			}
		}
		points.push_back(p);
	}

	return std::vector<glm::vec3>(points);
}


// Return a random float between lo and hi (or between hi and lo if hi < lo)
float randomFloat(float lo, float hi) {
	if (lo > hi) {
		float t = lo;
		lo = hi;
		hi = t;
	}
	return lo + (float)(rand()) / ((float)(RAND_MAX / (hi - lo)));
}

// Return a random int between lo and hi
int randomInt(int lo, int hi) {
	return rand() % (hi - lo) + lo;
}


std::vector<glm::vec3> zeroPoints(unsigned int num) {
	std::vector<glm::vec3> ret;
	for (unsigned int i = 0; i < num; i++) ret.push_back(glm::vec3(0, 0, 0));
	return ret;
}