#include "LoadSkel.h"

Joint* loadJoint(Tokenizer& token) {
	char name[256];
	token.GetToken(name);

	//std::cout << name << std::endl;

	token.FindToken("{");
	Joint* joint = NULL;

	glm::vec3 offset = DEFAULT_OFFSET, boxmin = DEFAULT_BOXMIN, boxmax = DEFAULT_BOXMAX, pose = DEFAULT_POSE;
	glm::vec2 rotxlimit = DEFAULT_ROTLIM, rotylimit = DEFAULT_ROTLIM, rotzlimit = DEFAULT_ROTLIM;
	std::vector<Joint*> subjoints;

	while (1) {
		char temp[256];
		token.GetToken(temp);

		// Check for tokens
		if (strcmp(temp, "offset") == 0) {
			offset.x = token.GetFloat();
			offset.y = token.GetFloat();
			offset.z = token.GetFloat();
		}
		
		else if (strcmp(temp, "boxmin") == 0) {
			boxmin.x = token.GetFloat();
			boxmin.y = token.GetFloat();
			boxmin.z = token.GetFloat();
		}

		else if (strcmp(temp, "boxmax") == 0) {
			boxmax.x = token.GetFloat();
			boxmax.y = token.GetFloat();
			boxmax.z = token.GetFloat();
		}

		else if (strcmp(temp, "rotxlimit") == 0) {
			rotxlimit.x = token.GetFloat();
			rotxlimit.y = token.GetFloat();
		}

		else if (strcmp(temp, "rotylimit") == 0) {
			rotylimit.x = token.GetFloat();
			rotylimit.y = token.GetFloat();
		}

		else if (strcmp(temp, "rotzlimit") == 0) {
			rotzlimit.x = token.GetFloat();
			rotzlimit.y = token.GetFloat();
		}

		else if (strcmp(temp, "pose") == 0) {
			pose.x = token.GetFloat();
			pose.y = token.GetFloat();
			pose.z = token.GetFloat();
		}

		else if (strcmp(temp, "balljoint") == 0) {
			subjoints.push_back(loadJoint(token));
		}

		// Ending/unknown token
		else if (strcmp(temp, "}") == 0) {
			joint = new Joint(offset, boxmin, boxmax, rotxlimit, rotylimit, rotzlimit, pose, subjoints);
			break;
		}
		else token.SkipLine();
	}

	return joint;
}

Skeleton* loadSkel(const char* path) {
	Tokenizer token;
	token.Open(path);

	token.FindToken("balljoint");
	Skeleton* skel = new Skeleton(loadJoint(token));

	token.Close();
	return skel;
}