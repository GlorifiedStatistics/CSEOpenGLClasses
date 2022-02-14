#include "BuildScene.h"

OBJ makeSphere(float stretchX, float stretchY, float stretchZ);
OBJ makeSphere(glm::vec3 stretch);
OBJ makeSphere(float stretch);
OBJ makeCylinder(float stretchX, float stretchY, float stretchZ);
OBJ makeCylinder(glm::vec3 stretch);
Geometry* geom(OBJ obj);
Transform* empty();
Transform* empty(OBJ obj);
Transform* trans(OBJ obj, glm::vec3 translate);
Transform* trans(OBJ obj, float x, float y, float z);
Transform* rot(OBJ obj, RotateInfo rotateInfo);
Transform* rot(OBJ obj, float angle, glm::vec3 axis);
Transform* rot(OBJ obj, float angle, float x, float y, float z);
Transform* wrapY(Node* object, int num);

long CURR_RIDE_ARM_UPDATE_COUNT = 0;
int RIDE_ARM_ROTATE_DIR = 1;
bool rot1 = true, rot2 = true, rot3 = true;

Transform* buildScene() {

	// Ride base
	Transform* rideRideBit = empty()->addChild(geom(makeCylinder(RIDE_RIDE_ARM_STRETCH).rotate(-DEG_45 * 3.0f / 2.0f, X_AXIS))->set(BLUE_METAL))
		->addChild(empty()->translate(RIDE_RIDE_SPHERE_TRANSLATE)
			->addChild(geom(genDiscoSphere().stretch(DISCO_BALL_SIZE).rotate(DEG_90, X_AXIS))->set(DRAW_REFLECTION)));

	Transform* rideRide = empty()->translate(RIDE_ARM_JOINT_2_TRANSLATE)->addChild(geom(makeSphere(1.5))->set(BLACK_METAL))
		->addChild(empty()->translate(RIDE_RIDE_TRANSLATE)
			->addChild(rideRideBit)
			->addChild(empty()->rotateAbout(DEG_120, RIDE_RIDE_ROTATE_POINT, Y_AXIS)->addChild(rideRideBit))
			->addChild(empty()->rotateAbout(DEG_240, RIDE_RIDE_ROTATE_POINT, Y_AXIS)->addChild(rideRideBit))
		)
		->addAnimation([](glm::mat4 M)->glm::mat4 {
			return rot3 ? glm::rotate(M, -RIDE_RIDE_ROTATION_SPEED, Y_AXIS) : M;
		});

	Transform* rideArm = empty()->translate(RIDE_ARM_TRANSLATE)
		->addChild(geom(makeCylinder(RIDE_ARM_1_STRETCH).rotate(DEG_45, X_AXIS))->set(BLACK_METAL))
		->addChild(empty()->translate(RIDE_ARM_2_TRANSLATE)
			->addChild(geom(makeCylinder(RIDE_ARM_2_STRETCH).rotate(-DEG_45, X_AXIS))->set(BLACK_METAL))
		)
		->addChild(empty()->translate(RIDE_ARM_JOINT_1_TRANSLATE)->addChild(geom(makeSphere(1))->set(BLACK_METAL)))
		->addChild(rideRide)
		->addAnimation([](glm::mat4 M)->glm::mat4 {
			if (CURR_RIDE_ARM_UPDATE_COUNT > RIDE_ARM_ROTATION_SPEED * RIDE_ARM_ROTATE_DEG) {
				RIDE_ARM_ROTATE_DIR = -1;
			}else if (CURR_RIDE_ARM_UPDATE_COUNT < -RIDE_ARM_ROTATION_SPEED * RIDE_ARM_ROTATE_DEG) {
				RIDE_ARM_ROTATE_DIR = 1;
			}
			return rot2 ? glm::rotate(M, RIDE_ARM_ROTATE_DIR * RIDE_ARM_ROTATION_SPEED, Z_AXIS) : M;
		});


	Transform* rideBase = empty()->addChild(geom(makeCylinder(RIDE_BASE_STRETCH))->set(BLACK_METAL));

	// The main ride ball
	Transform* rideBall = empty()->addChild(geom(makeSphere(RIDE_BALL_STRETCH))->set(RED_METAL))->translate(RIDE_BALL_TRANSLATE)
		->addAnimation([](glm::mat4 M)->glm::mat4 {
			return rot1 ? glm::rotate(M, RIDE_BALL_ROTATE_SPEED, Y_AXIS) : M;
		})
		->addChild(rideArm)
		->addChild(empty()->rotateAbout(DEG_120, RIDE_ARM_ROTATE_POINT, Y_AXIS)->addChild(rideArm))
		->addChild(empty()->rotateAbout(DEG_240, RIDE_ARM_ROTATE_POINT, Y_AXIS)->addChild(rideArm));

	// Add the disco and ride and return
	return empty()->translate(RIDE_TRANSLATE)
		->addChild(rideBase)
		->addChild(rideBall);
}

OBJ makeSphere(glm::vec3 stretch) { return loadObj(SPHERE_PATH).stretch(stretch); }
OBJ makeSphere(float stretchX, float stretchY, float stretchZ) { return makeSphere(glm::vec3(stretchX, stretchY, stretchZ)); }
OBJ makeSphere(float stretch) { return makeSphere(glm::vec3(stretch, stretch, stretch)); }

OBJ makeCylinder(glm::vec3 stretch) { return loadObj(CYLINDER_PATH).stretch(stretch); }
OBJ makeCylinder(float stretchX, float stretchY, float stretchZ) { return makeCylinder(glm::vec3(stretchX, stretchY, stretchZ)); }

Geometry* geom(OBJ obj) { return new Geometry(obj); }

Transform* empty(OBJ obj) { return empty()->addObj(obj); }
Transform* empty() { return new Transform(); }

Transform* trans(OBJ obj, glm::vec3 translate) { return empty()->translate(translate)->addObj(obj); }

Transform* trans(OBJ obj, float x, float y, float z) { return empty()->translate(x, y, z)->addObj(obj); }

Transform* rot(OBJ obj, RotateInfo rotateInfo) { return empty()->rotate(rotateInfo)->addObj(obj); }
Transform* rot(OBJ obj, float angle, glm::vec3 axis) { return rot(obj, RotateInfo(angle, axis)); }
Transform* rot(OBJ obj, float angle, float x, float y, float z) { return rot(obj, RotateInfo(angle, glm::vec3(x, y, z))); }

Transform* wrapY(Node* object, int num) {
	Transform* ret = empty();
	for (unsigned int i = 0; i < num; i++) ret->addChild(empty()->rotate((i / (float)num) * DEG_360, Y_AXIS)->addChild(object));
	return ret;
}

void toggleRot(int rot) {
	if (rot == 1) {
		rot1 = !rot1;
	}
	else if (rot == 2) {
		rot2 = !rot2;
	}
	else if (rot == 3) {
		rot3 = !rot3;
	}
}