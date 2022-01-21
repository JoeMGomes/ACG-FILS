#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Graphics\window.h"
#include "Camera\camera.h"
#include "Shaders\shader.h"
#include "Model Loading\mesh.h"
#include "Model Loading\texture.h"
#include "Model Loading\meshLoaderObj.h"
#include "Model Loading\skybox.h"
#include "Statistics/counter.h"
#include "Collisions/collision.h"
#include "TerrainOcean/Terrain.h"
#include "TerrainOcean/OceanTile.h"
#include "main.h"
#include "GameObjects/GameObject.h"

void processKeyboardInput();
void processMouseInput();
void drawGUI();

//count initialization with 0
int Counter::count = 0;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
unsigned int frameCounter = 0;
unsigned int currentFPS = 0;
float currentFrameTime = 0;
unsigned int seed = 29345843;

Window window("Game Engine", 1200, 800);
Camera camera(glm::vec3(0, 20, 20));
BoundingBox cameraBB;
OceanTile oceanTile(300, 300);
std::vector<BoundingBox> worldObjects;

double lastMousePosX = 600, lastMousePosY = 400;
bool firstMouseInput = true;
glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec3 lightPos = glm::vec3(-180.0f, 100.0f, -200.0f);

bool flyMode = true;
bool gLines = false;
bool canMoveCamera = true;

glm::vec3 oceanTranslate = glm::vec3(-321, -3, -150);
glm::vec3 mountainTranslate = glm::vec3(-26, 0, -150);

int main()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 400");

	glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.2f, 0.8f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);

	//building and compiling shader program
	Shader phongShader("Shaders/vertex_shader.glsl", "Shaders/fragment_shader.glsl");
	Shader sunShader("Shaders/sun_vertex_shader.glsl", "Shaders/sun_fragment_shader.glsl");
	Shader skyboxShader("Shaders/skybox_vert.glsl", "Shaders/skybox_frag.glsl");
	Shader mountainShader("Shaders/mountain_vertex_shader.glsl", "Shaders/mountain_fragment_shader.glsl");
	Shader oceanShader("Shaders/ocean_vertex_shader.glsl", "Shaders/ocean_fragment_shader.glsl");
	Shader BBShader("Shaders/bounding_box_vertex.glsl", "Shaders/bounding_box_fragment.glsl");

	//Setup skybox
	skyboxShader.use();
	glUniform1i(glGetUniformLocation(skyboxShader.getId(), "skyTexture"), 0);
	const char* skytexts[] = {
		"Resources/Skyboxes/LearnOGL/right.bmp",
		"Resources/Skyboxes/LearnOGL/left.bmp",
		"Resources/Skyboxes/LearnOGL/bottom.bmp",
		"Resources/Skyboxes/LearnOGL/top.bmp",
		"Resources/Skyboxes/LearnOGL/front.bmp",
		"Resources/Skyboxes/LearnOGL/back.bmp",
	};
	GLuint skytext = loadSkybox(skytexts);
	Skybox skybox = Skybox(skytext);

	TerrainChunk mountain = TerrainChunk(300, 300);

	//Textures
	std::vector<Texture> chestTextureVector;
	chestTextureVector.push_back(Texture());
	chestTextureVector[0].id = loadBMP("Resources/Textures/chest_wood.bmp");
	chestTextureVector[0].type = "texture_diffuse";


	std::vector<Texture> orangeTextureVector;
	orangeTextureVector.push_back(Texture());
	orangeTextureVector[0].id = loadBMP("Resources/Textures/orange.bmp");
	orangeTextureVector[0].type = "texture_diffuse";

	std::vector<Texture> crabTextureVector;
	crabTextureVector.push_back(Texture());
	crabTextureVector[0].id = loadBMP("Resources/Textures/mrcrabs.bmp");
	crabTextureVector[0].type = "texture_diffuse";

	std::vector<Texture> beachBallTextureVector;
	beachBallTextureVector.push_back(Texture());
	beachBallTextureVector[0].id = loadBMP("Resources/Textures/beachball.bmp");
	beachBallTextureVector[0].type = "texture_diffuse";

	std::vector<Texture> chairTextureVector;
	chairTextureVector.push_back(Texture());
	chairTextureVector[0].id = loadBMP("Resources/Textures/bwood.bmp");
	chairTextureVector[0].type = "texture_diffuse";


	MeshLoaderObj loader;
	Mesh sun = loader.loadObj("Resources/Models/sphere.obj");
	Mesh plane = loader.loadObj("Resources/Models/plane.obj", orangeTextureVector);
	Mesh wall = loader.loadObj("Resources/Models/wall.obj", orangeTextureVector);


	Mesh crab = loader.loadObj("Resources/Models/crab.obj", crabTextureVector);
	GameObject crabGO = GameObject(&crab);
	crabGO.setPosition(glm::vec3(-10, 0.1f, 0.0f));
	crabGO.setScale(glm::vec3(.1f));

	GameObject crabGO1 = GameObject(&crab);
	crabGO1.setPosition(glm::vec3(-10, 0.1f, 0.0f));
	crabGO1.setScale(glm::vec3(.1f));
	//worldObjects.push_back(crabGO.boundingbox);


	Mesh chest = loader.loadObj("Resources/Models/chest.obj", chestTextureVector);
	GameObject chestGO = GameObject(&chest);
	chestGO.setPosition(glm::vec3(-2.0, 1.0f, 12.0f));
	chestGO.setScale(glm::vec3(.7f));
	worldObjects.push_back(chestGO.boundingbox);

	Mesh beachBall = loader.loadObj("Resources/Models/beachBall.obj", beachBallTextureVector);
	GameObject beachBallGO = GameObject(&beachBall);
	beachBallGO.setPosition(glm::vec3(2.0, 1.0f, 20.0f));
	beachBallGO.setScale(glm::vec3(0.03f));
	worldObjects.push_back(beachBallGO.boundingbox);

	Mesh chair = loader.loadObj("Resources/Models/chair.obj", chairTextureVector);
	GameObject chairGO = GameObject(&chair);
	chairGO.setPosition(glm::vec3(-6.0, 1.0f, -2.0f));
	worldObjects.push_back(chairGO.boundingbox);

	GameObject chairGO1 = GameObject(&chair);
	chairGO1.setPosition(glm::vec3(-9.0, 1.0f, -4.0f));
	worldObjects.push_back(chairGO1.boundingbox);

	//The floor is a special case of the bounding box checking so it is not a world object
	GameObject floorGO = GameObject(&plane);
	floorGO.setPosition(glm::vec3(-30.0f, 0.0f, 0.0f));
	floorGO.setScale(glm::vec3(1.0f, 1.0f, 10.0f));

	GameObject mountainWallGO = GameObject(&wall);
	mountainWallGO.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	mountainWallGO.setScale(glm::vec3(1.0f, 1.0f, 10.0f));
	worldObjects.push_back(mountainWallGO.boundingbox);

	GameObject waterWallGO = GameObject(&wall);
	waterWallGO.setPosition(glm::vec3(-60.0f, 0.0f, 0.0f));
	waterWallGO.setScale(glm::vec3(1.0f, 1.0f, 10.0f));
	worldObjects.push_back(waterWallGO.boundingbox);


	cameraBB.center = camera.getCameraPosition();
	cameraBB.halfDepth = cameraBB.halfWidth = 0.5f;
	cameraBB.halfHeight = 3.0f;

	//check if we close the window or press the escape button
	while (!window.isPressed(GLFW_KEY_Q) &&
		glfwWindowShouldClose(window.getWindow()) == 0)
	{
		window.clear();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		frameCounter++;
		if (deltaTime >= 1.0 / 60) {
			currentFPS = (1.0 / deltaTime) * frameCounter;
			currentFrameTime = (deltaTime / frameCounter) * 1000;
			lastFrame = currentFrame;
			frameCounter = 0;
			//Proccess input at a fixed rate. Polling could be done here but we don't want to alter the Window class.
			processKeyboardInput();
			if (canMoveCamera)
				processMouseInput();


			//Add gravity to player
			camera.globalMoveDown(9.8 * deltaTime);
			// Update camera bounding box after gravity. I dont like that we do this two times but it works
			cameraBB.center = camera.getCameraPosition();
			//If touched the floor pull back up
			if (BoundingBox::checkCollision(cameraBB, floorGO.boundingbox) || flyMode) {
				camera.globalMoveDown(-9.8 * deltaTime);
			}
		}

		//// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		glm::mat4 ProjectionMatrix = glm::perspective(90.0f, window.getWidth() * 1.0f / window.getHeight() * 1.0f, 0.1f, 10000.0f);
		glm::mat4 ViewMatrix = glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp());


		//// Code for the light ////
		{
			sunShader.use();
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, lightPos);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(sunShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			sun.draw(sunShader);
		}


		phongShader.use();

		///// Test Obj files for box ////
		{
			glm::mat4  ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, chestGO.position);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniform3f(glGetUniformLocation(phongShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(phongShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(phongShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

			chestGO.mesh->draw(phongShader);
		}
		//// Crabs////
		{
			float x = std::round(glm::sin(currentFrame*0.5) + 0.5) * 2 - 1;
			float posX = glm::cos(2*currentFrame) * 4;
			crabGO.setPosition(glm::vec3(posX, crabGO.position.y, crabGO.position.z+ deltaTime * x));

			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, crabGO.position);
			ModelMatrix = glm::scale(ModelMatrix, crabGO.scale);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			crabGO.mesh->draw(phongShader);
		}
		{

			crabGO1.setPosition(glm::vec3(crabGO1.position.x, crabGO1.position.y, crabGO1.position.z));
			crabGO1.setRotation(glm::vec3(crabGO1.rotation.x , crabGO1.rotation.y + 3* deltaTime, crabGO1.rotation.z));


			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::rotate(ModelMatrix, crabGO1.rotation.y ,glm::vec3(0.0f,1.0f,0.0f));
			ModelMatrix = glm::translate(ModelMatrix, crabGO1.position);
			ModelMatrix = glm::scale(ModelMatrix, crabGO1.scale);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			crabGO1.mesh->draw(phongShader);
		}
		//// BeachBall////
		{
			float x = std::fmod(currentFrame, 1.0f);

			float height = -16 * std::pow(x - 0.5f, 2) + 4;

			float scale = -1 * std::pow(x - 0.5f, 2) + 1;

			beachBallGO.setPosition(glm::vec3(beachBallGO.position.x, height, beachBallGO.position.z));
			beachBallGO.setScaleNoUpdate(glm::vec3(.03f, scale * .03f, .03f));

			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, beachBallGO.position);
			ModelMatrix = glm::scale(ModelMatrix, beachBallGO.scale);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			beachBallGO.mesh->draw(phongShader);
		}

		///// Chairs /////
		{
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, chairGO.position);
			ModelMatrix = glm::scale(ModelMatrix, chairGO.scale);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			chairGO.mesh->draw(phongShader);

			ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, chairGO1.position);
			ModelMatrix = glm::scale(ModelMatrix, chairGO1.scale);
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			chairGO1.mesh->draw(phongShader);
		}
		/*
		///// Uncomment to see invisible world boundaries
		{
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::scale(ModelMatrix, floorGO.scale);
			ModelMatrix = glm::translate(ModelMatrix, floorGO.position);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			floorGO.mesh->draw(phongShader);
		}

		{
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::scale(ModelMatrix, waterWallGO.scale);
			ModelMatrix = glm::translate(ModelMatrix, waterWallGO.position);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phongShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);

			waterWallGO.mesh->draw(phongShader);
		}*/

		///// Mountain plain test /////
		{
			mountainShader.use();

			glm::mat4 ModelMatrix = glm::mat4(1.0);
			ModelMatrix = glm::translate(ModelMatrix, mountainTranslate);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(mountainShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(mountainShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniform3f(glGetUniformLocation(mountainShader.getId(), "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(mountainShader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(mountainShader.getId(), "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			glUniform1f(glGetUniformLocation(mountainShader.getId(), "maxHeight"), mountain.maxHeight);
			glUniform1f(glGetUniformLocation(mountainShader.getId(), "uTime"), glfwGetTime());

			mountain.draw(mountainShader);

		}

		//// Ocean Tile plain test ////
		{
			oceanShader.use();
			GLuint oceanShaderID = oceanShader.getId();

			glm::mat4 ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, oceanTranslate);

			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			glUniformMatrix4fv(glGetUniformLocation(oceanShaderID, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(oceanShaderID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniform3f(glGetUniformLocation(oceanShaderID, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
			glUniform3f(glGetUniformLocation(oceanShaderID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(glGetUniformLocation(oceanShaderID, "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			glUniform1f(glGetUniformLocation(oceanShaderID, "uTime"), glfwGetTime());
			oceanTile.draw(oceanShader);
		}

		///// Bounding Boxes /////

		{
			//BBShader.use();
			//glm::mat4 ModelMatrix = glm::mat4(1.0f);
			////ModelMatrix = glm::translate(ModelMatrix, boxGO.position);
			////ModelMatrix = glm::scale(ModelMatrix, glm::vec3(5.56f));
			//glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			////glUniformMatrix4fv(glGetUniformLocation(BBShader.getId(), "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			//glUniformMatrix4fv(glGetUniformLocation(BBShader.getId(), "MVP"), 1, GL_FALSE, &MVP[0][0]);
			//boxGO.boundingbox.draw();
		}

		///// Skybox //////
		{
			glDepthFunc(GL_LEQUAL);
			skyboxShader.use();

			glm::mat4 view = glm::mat4(1.0f);
			// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
			// The last row and column affect the translation of the skybox (which we don't want to affect)
			view = glm::mat4(glm::mat3(glm::lookAt(camera.getCameraPosition(), camera.getCameraPosition() + camera.getCameraViewDirection(), camera.getCameraUp())));

			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "view"), 1, GL_FALSE, &view[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.getId(), "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

			skybox.draw();
		}

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);

		//ImGui setup and render
		drawGUI();

		window.update();

		Counter::resetCount();
	}


	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();

}

void drawGUI() {
	// ImGui Setup
	ImGui::Begin("ACG Homework 2");
	if (ImGui::CollapsingHeader("-------  Statistics  -------")) {
		ImGui::Text("Frames per second: %d", currentFPS);
		ImGui::Text("Frame time: %f miliseconds", currentFrameTime);
		ImGui::Text("Draw Calls: %d ", Counter::getCount());
	}
	if (ImGui::CollapsingHeader("-------  Light Properties  -------")) {
		ImGui::Text("Light Position"); ImGui::SameLine(); ImGui::SliderFloat3("", glm::value_ptr(lightPos), -200, 200);
		ImGui::Text("Light Position"); ImGui::SameLine(); ImGui::ColorEdit3("", glm::value_ptr(lightColor));
	}
	if (ImGui::CollapsingHeader("-------  Camera Properties  -------")) {
		auto camPos = camera.getCameraPosition();
		char str[300];
		sprintf_s(str, 300, "Camera Position x : %.02f y : %.02f z : %.02f", camPos.x, camPos.y, camPos.z);
		ImGui::Text(str);
		auto camLookAt = camPos + camera.getCameraViewDirection();
		char str2[300];
		sprintf_s(str2, 300, "Camera View Direction x : %.02f y : %.02f z : %.02f", camLookAt.x, camLookAt.y, camLookAt.z);
		ImGui::Text(str2);
	}
	if (ImGui::CollapsingHeader("-------  Wave Properties  -------")) {
		char str[300];
		ImGui::Text("Amplitude"); ImGui::SameLine(); ImGui::SliderFloat("##WaveAmplitude", &oceanTile.amplitude, 1, 21);
		ImGui::Text("Wave Length"); ImGui::SameLine(); ImGui::SliderFloat("##WaveLength", &oceanTile.waveLength, 1, 20);
		ImGui::Text("No. Waves"); ImGui::SameLine(); ImGui::SliderFloat("##WaveNumber", &oceanTile.nWaves, 2, 20);
		ImGui::Text("Speed"); ImGui::SameLine(); ImGui::SliderFloat("##WaveSpeed", &oceanTile.speed, 1, 20);
		//ImGui::Text("Wave Steepness"); ImGui::SameLine(); ImGui::SliderFloat("##WaveQ", &oceanTile.q, 0, 1.0f);

		oceanTile.calculateConstants();
	}


	ImGui::Text("Fly Mode"); ImGui::SameLine();
	if (ImGui::Checkbox("##flymode", &flyMode)) {
	}
	ImGui::Text("Wireframe"); ImGui::SameLine();
	if (ImGui::Checkbox("##wireframe", &gLines)) {
		if (gLines)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	ImGui::Text("Terrain Position"); ImGui::SameLine(); ImGui::SliderFloat3("##mountain", glm::value_ptr(mountainTranslate), -300, 300);
	ImGui::Text("Water Position"); ImGui::SameLine(); ImGui::SliderFloat3("##water", glm::value_ptr(oceanTranslate), -600, 10);

	ImGui::End();

	//Draw ImGUI
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void processMouseInput() {

	double xpos, ypos;
	window.getMousePos(ypos, xpos);

	if (firstMouseInput) // initially set to true
	{
		lastMousePosX = xpos;
		lastMousePosY = ypos;
		firstMouseInput = false;
	}

	double xoffset = xpos - lastMousePosX;
	double yoffset = lastMousePosY - ypos;
	lastMousePosX = xpos;
	lastMousePosY = ypos;

	double sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.rotationOx -= xoffset;
	camera.rotationOy -= yoffset;

	if (camera.rotationOx > 89.0f)
		camera.rotationOx = 89.0f;
	if (camera.rotationOx < -89.0f)
		camera.rotationOx = -89.0f;

	camera.rotate();
}

void moveCamera(float cameraSpeed) {
	if (flyMode) {
		if (window.isPressed(GLFW_KEY_W))
			camera.keyboardMoveFront(cameraSpeed);
		if (window.isPressed(GLFW_KEY_S))
			camera.keyboardMoveBack(cameraSpeed);
		if (window.isPressed(GLFW_KEY_A))
			camera.keyboardMoveLeft(cameraSpeed);
		if (window.isPressed(GLFW_KEY_D))
			camera.keyboardMoveRight(cameraSpeed);
		if (window.isPressed(GLFW_KEY_R))
			camera.keyboardMoveUp(cameraSpeed);
		if (window.isPressed(GLFW_KEY_F))
			camera.keyboardMoveDown(cameraSpeed);
	}
	//FPS Mode
	else {
		if (window.isPressed(GLFW_KEY_W))
			camera.FPSForward(cameraSpeed);
		if (window.isPressed(GLFW_KEY_S))
			camera.FPSForward(-cameraSpeed);
		if (window.isPressed(GLFW_KEY_A))
			camera.FPSRight(-cameraSpeed);
		if (window.isPressed(GLFW_KEY_D))
			camera.FPSRight(cameraSpeed);
	}
}
void processKeyboardInput()
{
	//Show mouse cursor
	if (window.isPressed(GLFW_KEY_Z)) {
		canMoveCamera = true;
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (window.isPressed(GLFW_KEY_X)) {
		canMoveCamera = false;
		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	if (window.isPressed(GLFW_KEY_V)) {
		flyMode = false;
	}
	if (window.isPressed(GLFW_KEY_C)) {
		flyMode = true;
	}

	if (window.isPressed(GLFW_KEY_B)) {
		gLines = false;
	}
	if (window.isPressed(GLFW_KEY_N)) {
		gLines = true;
	}

	float cameraSpeed = 30 * deltaTime;
	moveCamera(cameraSpeed);
	// Update camera bounding box after input
	cameraBB.center = camera.getCameraPosition();

	//Collision checking code
	//If there is a collision reverse the last applied movements
	// Fairly ineficiente since this has to check all objects in the world
	for (auto& b : worldObjects) {
		if (BoundingBox::checkCollision(cameraBB, b)) {
			moveCamera(-cameraSpeed);
			// Update camera bounding box after reverting input
			cameraBB.center = camera.getCameraPosition();
		}
	}

	//rotation
	/*if (window.isPressed(GLFW_KEY_LEFT))
		camera.rotateOy(cameraSpeed);
	if (window.isPressed(GLFW_KEY_RIGHT))
		camera.rotateOy(-cameraSpeed);
	if (window.isPressed(GLFW_KEY_UP))
		camera.rotateOx(cameraSpeed);
	if (window.isPressed(GLFW_KEY_DOWN))
		camera.rotateOx(-cameraSpeed);*/
}


