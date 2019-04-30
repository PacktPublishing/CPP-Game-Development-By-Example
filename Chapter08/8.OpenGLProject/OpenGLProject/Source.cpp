
// GLEW needs to be included first
#include <GL/glew.h>

// GLFW is included next
#include <GLFW/glfw3.h>

#include "ShaderLoader.h"
#include "Camera.h"
#include "LightRenderer.h"

#include "MeshRenderer.h"
#include "TextureLoader.h"

#include <btBulletDynamicsCommon.h>

#include<chrono>

#include "TextRenderer.h"

TextRenderer* label;

void initGame();
void renderScene(float dt);
void addRigidBodies();
void myTickCallback(btDynamicsWorld *dynamicsWorld, btScalar timeStep);

Camera* camera;
LightRenderer* light;

MeshRenderer* sphere;
MeshRenderer* ground;
MeshRenderer* enemy;

//physics
btDiscreteDynamicsWorld* dynamicsWorld;


GLuint flatShaderProgram, litTexturedShaderProgram, textProgram;

GLuint texturedShaderProgram;

GLuint sphereTexture, groundTexture;

bool grounded = false;

bool gameover = true;
int score = 0;


void initGame() {
	
	// Enable the depth testing
	glEnable(GL_DEPTH_TEST); 
	
	ShaderLoader shader;

	flatShaderProgram = shader.createProgram("Assets/Shaders/FlatModel.vs", "Assets/Shaders/FlatModel.fs");
	
	texturedShaderProgram = shader.createProgram("Assets/Shaders/TexturedModel.vs", "Assets/Shaders/TexturedModel.fs");

	litTexturedShaderProgram = shader.createProgram("Assets/Shaders/LitTexturedModel.vs", "Assets/Shaders/LitTexturedModel.fs");

	textProgram = shader.createProgram("Assets/Shaders/text.vs", "Assets/Shaders/text.fs");

	TextureLoader tLoader;

	sphereTexture = tLoader.getTextureID("Assets/Textures/globe.jpg");
	groundTexture = tLoader.getTextureID("Assets/Textures/ground.jpg");


	camera = new Camera(45.0f, 800, 600, 0.1f, 100.0f, glm::vec3(0.0f, 4.0f, 20.0f));

	light = new LightRenderer(MeshType::kSphere, camera);
	light->setProgram(flatShaderProgram);
	light->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	light->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	//text label
	label = new TextRenderer("Score: 0", "Assets/fonts/gooddog.ttf", 64, glm::vec3(1.0f, 0.0f, 0.0f), textProgram);
	label->setPosition(glm::vec2(320.0f, 500.0f));


	//init physics

	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

	dynamicsWorld->setInternalTickCallback(myTickCallback);
	

	addRigidBodies();

}


void myTickCallback(btDynamicsWorld *dynamicsWorld, btScalar timeStep) {

	if (!gameover) {

		// Get enemy transform
		btTransform t(enemy->rigidBody->getWorldTransform());


		// Set enemy position

		t.setOrigin(t.getOrigin() + btVector3(-15, 0, 0) * timeStep);

		// Check if offScreen

		if (t.getOrigin().x() <= -18.0f) {

			t.setOrigin(btVector3(18, 1, 0));
			score++;
			label->setText("Score: " + std::to_string(score));

		}

		enemy->rigidBody->setWorldTransform(t);
		enemy->rigidBody->getMotionState()->setWorldTransform(t);
	}

	// Check Collisions

	grounded = false;

	int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {

		btPersistentManifold *contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		int numContacts = contactManifold->getNumContacts();

		if (numContacts > 0) {


			const btCollisionObject *objA = contactManifold->getBody0();
			const btCollisionObject *objB = contactManifold->getBody1();

			MeshRenderer* gModA = (MeshRenderer*)objA->getUserPointer();
			MeshRenderer* gModB = (MeshRenderer*)objB->getUserPointer();

				if ((gModA->name == "hero" && gModB->name == "enemy") ||
					(gModA->name == "enemy" && gModB->name == "hero")) {

					//printf("collision: %s with %s \n", gModA->name, gModB->name);
					if (gModB->name == "enemy") {
						btTransform b(gModB->rigidBody->getWorldTransform());
						b.setOrigin(btVector3(18, 1, 0));
						gModB->rigidBody->setWorldTransform(b);
						gModB->rigidBody->getMotionState()->setWorldTransform(b);
					}else {
					
						btTransform a(gModA->rigidBody->getWorldTransform());
						a.setOrigin(btVector3(18, 1, 0));
						gModA->rigidBody->setWorldTransform(a);
						gModA->rigidBody->getMotionState()->setWorldTransform(a);
					}

					gameover = true;
					score = 0;
					label->setText("Score: " + std::to_string(score));

				}

				if ((gModA->name == "hero" && gModB->name == "ground") ||
					(gModA->name == "ground" && gModB->name == "hero")) {

					//printf("collision: %s with %s \n", gModA->name, gModB->name);

					grounded = true;

				}
		}
	}
}

void renderScene(float dt){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0, 1.0, 0.0, 1.0);


	// Draw Objects

	light->draw();

	sphere->draw();

	enemy->draw();

	ground->draw();

	//debugDraw->SetMatrices(bulletDebugProgram, camera->getViewMatrix(), camera->getprojectionMatrix());
	//dynamicsWorld->debugDrawWorld();

	label->draw();
}

void updateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods){

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
	
		glfwSetWindowShouldClose(window, true);
	
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {


		if (gameover) {
		
			gameover = false;
		
		} else {
			
			if (grounded == true) {

				grounded = false;
				sphere->rigidBody->applyImpulse(btVector3(0.0f, 100.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f));
				printf("pressed up key \n");
			}
		}
	}

}

int main(int argc, char **argv)
{

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, " Hello OpenGL ", NULL, NULL);
	
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, updateKeyboard);

	glewInit();

	initGame();

	auto previousTime = std::chrono::high_resolution_clock::now();

	while (!glfwWindowShouldClose(window)){

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();

		dynamicsWorld->stepSimulation(dt);

		renderScene(dt);

		glfwSwapBuffers(window);
		glfwPollEvents();

		previousTime = currentTime;
	}

	glfwTerminate();


	delete camera;
	delete light;

	return 0;
}

void addRigidBodies() {

	// Sphere Rigid Body

	btCollisionShape* sphereShape = new btSphereShape(1);
	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0.5, 0)));

	btScalar mass = 13.0f;
	btVector3 sphereInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(mass, sphereInertia);

	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(mass, sphereMotionState, sphereShape, sphereInertia);

	btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);

	sphereRigidBody->setFriction(1.0f);
	sphereRigidBody->setRestitution(0.0f);

	sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);

	dynamicsWorld->addRigidBody(sphereRigidBody);

	// Sphere Mesh

	sphere = new MeshRenderer(MeshType::kSphere, "hero", camera, sphereRigidBody, light, 0.1f, 0.5f);
	sphere->setProgram(litTexturedShaderProgram);
	sphere->setTexture(sphereTexture);
	sphere->setScale(glm::vec3(1.0f));

	sphereRigidBody->setUserPointer(sphere);

	// Ground Rigid body

	btCollisionShape* groundShape = new btBoxShape(btVector3(4.0f, 0.5f, 4.0f));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1.0f, 0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.0f, groundMotionState, groundShape, btVector3(0, 0, 0));

	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	groundRigidBody->setFriction(1.0);
	groundRigidBody->setRestitution(0.0);

	groundRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);

	dynamicsWorld->addRigidBody(groundRigidBody);

	// Ground Mesh
	ground = new MeshRenderer(MeshType::kCube, "ground", camera, groundRigidBody, light, 0.1f, 0.5f);
	ground->setProgram(litTexturedShaderProgram);
	ground->setTexture(groundTexture);
	ground->setScale(glm::vec3(4.0f, 0.5f, 4.0f));

	groundRigidBody->setUserPointer(ground);

	// Enemy Rigid body

	btCollisionShape* shape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
		btVector3(18.0, 1.0f, 0)));
	btRigidBody::btRigidBodyConstructionInfo rbCI(0.0f, motionState, shape, btVector3(0.0f, 0.0f, 0.0f));

	btRigidBody* rb = new btRigidBody(rbCI);

	rb->setFriction(1.0);
	rb->setRestitution(0.0);

	//rb->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);

	rb->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	dynamicsWorld->addRigidBody(rb);

	// Enemy Mesh
	enemy = new MeshRenderer(MeshType::kCube, "enemy", camera, rb, light, 0.1f, 0.5f);
	enemy->setProgram(litTexturedShaderProgram);
	enemy->setTexture(groundTexture);
	enemy->setScale(glm::vec3(1.0f, 1.0f, 1.0f));


	rb->setUserPointer(enemy);

}