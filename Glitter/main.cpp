// Local Headers
#include "glitter.h"
#include "Debug.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Config.h"
#include "Timer.h"
#include "ResourceManager.h"
#include "Scenes/SceneManager.h"
#include "TextureCubemap.h"

bool blinn = false;
bool blinnKeyPressed = false;

#include "Window.hpp"

void setLight(Shader shader, float ambient);
GLuint createTriangleVAO();
GLuint createLightVAO();
GLuint loadTexture(char const * path);
void loadCubemap();
void setTexture(GLuint type, GLuint slot, GLuint texture);

// shaders
const string startPath = "start";
const string lightingPath = "lighting";
const string lampPath = "lamp";

GLuint VBO;
GLuint EBO;


// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);



static void displayGL()
{
	//ResourceManager& res = ResourceManager::getInstance();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


static void idleGL()
{
	//Inputs& inputs = Inputs::getInstance();
	//VarManager& var = VarManager::getInstance();
	//Camera& cam = Camera::getInstance();

	timerIdle(glfwGetTime());

	///*
	//stringstream strTitle;
	//strTitle << "Projet - " << timer.getFPSCounter() << " FPS" << "  - "
	//<< setprecision(4) << "EYE : " << cam.getEye().x << ", " << cam.getEye().y << ", " << cam.getEye().z
	//<< "  -  " << "LOOKAT : " << cam.getCenter().x << ", " << cam.getCenter().y << ", " << cam.getCenter().z;
	//glutSetWindowTitle(strTitle.str().c_str());
	//*/

	//ivec2 pos = Inputs::getInstance().MousePos();
	//static ivec2 ppos = pos;

	//if (var.getb("enable_move_control")) {
	//	if (inputs.MouseButton(GLUT_LEFT_BUTTON)) {
	//		float sensivity = var.getf("mouse_sensivity");
	//		Camera::getInstance().Rotate((float)(pos.x - ppos.x) * sensivity,
	//			(float)(pos.y - ppos.y) * sensivity);
	//	}
	//}

	//ppos = pos;

	//float fElapsedTime = var.getf("time_speed") * elapsedTime;

	//SceneManager::getInstance().Idle(fElapsedTime);

	//if (inputs.SKey(GLUT_KEY_UP))		Camera::getInstance().PlayerMoveForward(20.0f*fElapsedTime);
	//if (inputs.SKey(GLUT_KEY_DOWN))	Camera::getInstance().PlayerMoveForward(-20.0f*fElapsedTime);
	//if (inputs.SKey(GLUT_KEY_LEFT))	Camera::getInstance().PlayerMoveStrafe(20.0f*fElapsedTime);
	//if (inputs.SKey(GLUT_KEY_RIGHT))	Camera::getInstance().PlayerMoveStrafe(-20.0f*fElapsedTime);

	//Effects::getInstance().idle();

	//glutPostRedisplay();
}

// glad: load all OpenGL function pointers
// ---------------------------------------
static void initGlad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		//return -1;
	}
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
}

// configure global opengl state
// -----------------------------
static void initGL()
{
	initWindow();
	initGlad();

	timerStart(glfwGetTime());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#if _DEBUG
	debugCallback();
#endif

	//Shader::Init();

	SceneManager::getInstance().Init();


	Camera& camera = Camera::getInstance();

	camera.init(glm::vec3(0.0f, 3.0f, 4.0f));

	loadCubemap();

}

int main(int argc, char * argv[]) {
	Config& cfg = Config::getInstance();

	initGL();

	Camera& camera = Camera::getInstance();
	SceneManager &sc = SceneManager::getInstance();
	// build and compile our shader zprogram
	// ------------------------------------
	Shader startShader = Shader("start");
	Shader lightingShader = Shader("lighting");
	Shader lampShader = Shader("lamp");
	//Shader modelShader = Shader("model");
	Shader streetShader = Shader("model");
	Shader carShader = Shader("model");
	Shader floorShader("blinn");
	Shader skyboxShader = Shader("skybox");
	Shader screenShader("screen");
	//Shader carShader = Shader("model");

	// load models
	// -----------
	//Model carModel("./resources/objects/nanosuit/nanosuit.obj");
	//Model carModel("./resources/meshs/plane.obj");
	//Model carModel("./resources/objects/IronMan/IronMan.obj");
	Model carModel("./resources/objects/Avent/Avent.obj");
	//Model carModel("./resources/objects/audi/3D Models/Audi_R8_2017.obj");
	//Model carModel("./resources/objects/bmw_x5/BMW X5 4.obj");
	//Model carModel("./resources/objects/PeugeotOnyx/3D models/PeugeotOnyxConcept.obj");
	//Model carModel("./resources/objects/wolf/Wolf_obj.obj");
	//Model carModel("./resources/objects/Black Dragon/Dragon 2.5_stl.stl");
	Model streetModel("./resources/objects/street/Street environment_V01.obj");
	//Model streetModel("./resources/objects/Rockwall/Rockwall.obj");

	//float planeVertices[] = {
	//	// positions          // texture Coords
	//	500.0f, -0.43f,  500.0f,  50.0f, 0.0f,
	//	-500.0f, -0.43f,  500.0f,  0.0f, 0.0f,
	//	-500.0f, -0.43f, -500.0f,  0.0f, 50.0f,

	//	500.0f, -0.43f,  500.0f,  50.0f, 0.0f,
	//	-500.0f, -0.43f, -500.0f,  0.0f, 50.0f,
	//	500.0f, -0.43f, -500.0f,  50.0f, 50.0f
	//};
	float planeVertices[] = {
		// positions            // normals         // texcoords
		100.0f, -0.43f,  100.0f,  0.0f, 1.0f, 0.0f,  50.0f,  0.0f,
		-100.0f, -0.43f,  100.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-100.0f, -0.43f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,

		100.0f, -0.43f,  100.0f,  0.0f, 1.0f, 0.0f,  50.0f,  0.0f,
		-100.0f, -0.43f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,
		100.0f, -0.43f, -100.0f,  0.0f, 1.0f, 0.0f,  50.0f, 50.0f
	};
	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	GLuint cubeVAO = createTriangleVAO();
	GLuint lightVAO = createLightVAO();

	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
	// transparent VAO
	unsigned int transparentVAO, transparentVBO;
	glGenVertexArrays(1, &transparentVAO);
	glGenBuffers(1, &transparentVBO);
	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	// screen quad VAO
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	

	// configure a uniform buffer object
	// ---------------------------------
	// From OpenGL version 4.2, use layout(std140, binding = ...) for each block
	// Now actually create the buffer
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

	// store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
	glm::mat4 projection = glm::perspective(45.0f, (float)cfg.SCR_WIDTH / (float)cfg.SCR_HEIGHT, 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	//GLuint diffuseMap = loadTexture("./resources/textures/container2.png");
	GLuint specularMap = loadTexture("./resources/textures/container2_specular.png");
	GLuint emissionMap = loadTexture("./resources/textures/matrix.jpg");
	//unsigned int floorTexture = loadTexture("./resources/textures/stones_diffuse.jpg");
	unsigned int floorTexture = loadTexture("./resources/textures/terrain_rocky_map_1024.png");
	//unsigned int floorTexture = loadTexture("./resources/textures/sand_bm.jpg");
	//unsigned int floorTexture = loadTexture("./resources/textures/grass.png");
	unsigned int transparentTexture = loadTexture("./resources/textures/window.png");

	// transparent windows locations
	// --------------------------------
	vector<glm::vec3> windows
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

	//stbi_set_flip_vertically_on_load(true);
	//GLuint texture1 = createTexture("./resources/textures/container.jpg", GL_RGB);
	//GLuint texture2 = createTexture("./resources/textures/awesomeface.png", GL_RGBA);
	

	// lighting info
	// -------------
	glm::vec3 lightPos(0.0f, 1.0f, 0.0f);
	


	// shader configuration
	// --------------------
	lightingShader.Activate();
	lightingShader.setInt("material.texture_diffuse1", 0);
	lightingShader.setInt("material.texture_specular1", 1);
	//lightingShader.setInt("material.emission", 5);
	//lightingShader.Activate();
	//lightingShader.setInt("skybox", 0);

	floorShader.Activate();
	floorShader.setInt("texture1", 0);
	//floorShader.setInt("material.texture_diffuse1", 0);
	//floorShader.setInt("material.texture_specular1", 1);


	skyboxShader.Activate();
	skyboxShader.setInt("skybox", 0);

	screenShader.Activate();
	screenShader.setInt("screenTexture", 0);

	// framebuffer configuration
	// -------------------------
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cfg.SCR_WIDTH, cfg.SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, cfg.SCR_WIDTH, cfg.SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
																								  // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		// --------------------
		idleGL();
		displayGL();

		// input
		// -----
		processInput(window, elapsedTime);

		camera.Idle(elapsedTime);

		// sort the transparent windows before rendering
		// ---------------------------------------------
		//map<float, glm::vec3> sorted;
		//for (unsigned int i = 0; i < windows.size(); i++)
		//{
		//	float distance = glm::length2(camera.Position - windows[i]);
		//	sorted[distance] = windows[i];
		//}

		// render
		// ------
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		// make sure we clear the framebuffer's content
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// projection - view - model
		// set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// draw objects
		floorShader.Activate();
		floorShader.setMat4("view", view);
		floorShader.setMat4("projection", projection);

		//model = glm::mat4();
		////model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		//floorShader.setMat4("model", model);
		// set light uniforms
		floorShader.setVec3("viewPos", camera.Position);
		floorShader.setVec3("lightPos", lightPos);
		floorShader.setInt("blinn", blinn);

		// floor
		glBindVertexArray(planeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		

		//setTexture(0, GL_TEXTURE_2D, floorTexture);


		setLight(floorShader, 0.10f);


		// windows
		//glBindVertexArray(transparentVAO);
		//glBindTexture(GL_TEXTURE_2D, transparentTexture);
		//for (map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		//{
		//	model = glm::mat4();
		//	model = glm::translate(model, it->second);
		//	blendingShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 6);
		//}

		// don't forget to enable shader before setting uniforms
		carShader.Activate();

		setTexture(0, GL_TEXTURE_2D, transparentTexture);

		// render the loaded model
		model = glm::mat4();
		model = glm::translate(model, camera.carPosition); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-camera.carYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		carShader.setMat4("model", model);
		carModel.Draw(carShader);

		setLight(carShader, 0.05f);


		streetShader.Activate();
		// render the loaded model
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
																	 //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		streetShader.setMat4("model", model);
		streetModel.Draw(streetShader);

		setLight(streetShader, 0.05f);


		lightingShader.Activate();

		// bind diffuse map
		//setTexture(0, GL_TEXTURE_2D, diffuseMap);
		//// bind specular map
		//setTexture(1, GL_TEXTURE_2D, specularMap);
		//// bind emission map
		//setTexture(2, GL_TEXTURE_2D, emissionMap);

		//ResourceManager &res = ResourceManager::getInstance();
		////reflection
		//res.getTextureCubemap("skybox")->Bind();

		setLight(lightingShader, 0.05f);

		// world transformation
		//glBindVertexArray(cubeVAO);
		//for (unsigned int i = 1; i < 10; i++) {
		//	// calculate the model matrix for each object and pass it to shader before drawing
		//	model = glm::mat4();
		//	model = glm::translate(model, cubePositions[i]);
		//	float angle = 20.0f * i;
		//	model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
		//	//carShader.setMat4("model", model);

		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		// also draw the lamp object
		lampShader.Activate();

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Activate();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		
		//sceneManager render
		sc.Render();

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.Activate();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	//glDeleteBuffers(1, &skyboxVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &quadVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return EXIT_SUCCESS;
}

void loadCubemap()
{
	Config& cfg = Config::getInstance();
	ResourceManager &res = ResourceManager::getInstance();


	TextureCubemap* cubemap = res.getTextureCubemap("skybox");
	cubemap->Bind();

	int width, height, nrChannels;
	for (unsigned int i = 0; i < cfg.cubemapFaces.size(); i++)
	{
		cout << cfg.cubemapFaces[i].c_str() << endl;
		unsigned char *data = stbi_load(cfg.cubemapFaces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			cout << "elao";
			cubemap->LoadData(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, data, width, height, 3);
		}
		else
		{
			cout << "Cubemap texture failed to load at path: " << cfg.cubemapFaces[i] << endl;
		}
		stbi_image_free(data);
	}
}

void setLight(Shader shader, float ambient) {
	Camera& camera = Camera::getInstance();

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// activate shader
	//lightingShader.Activate();
	shader.setVec3("viewPos", camera.Position);
	shader.setFloat("material.shininess", 32.0f);

	/*
	Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/
	// directional light
	shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shader.setVec3("dirLight.ambient", ambient, ambient, ambient);
	shader.setVec3("dirLight.diffuse", ambient, ambient, ambient);
	shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

	for (GLuint i = 0; i < 4; i++) {
		string number = to_string(i);

		shader.setVec3(("pointLights[" + number + "].position").c_str(), pointLightPositions[i]);
		shader.setVec3(("pointLights[" + number + "].ambient").c_str(), glm::vec3(0.1f));
		shader.setVec3(("pointLights[" + number + "].diffuse").c_str(), glm::vec3(0.8f));
		shader.setVec3(("pointLights[" + number + "].specular").c_str(), glm::vec3(5.0f));
		shader.setFloat(("pointLights[" + number + "].constant").c_str(), 1.0f);
		shader.setFloat(("pointLights[" + number + "].linear").c_str(), 0.09f);
		shader.setFloat(("pointLights[" + number + "].quadratic").c_str(), 0.032f);
	}
	// spotLight
	shader.setVec3("spotLight.position", camera.carPosition);
	shader.setVec3("spotLight.direction", camera.carFront);
	cout << camera.Front.x << camera.Front.y << camera.Front.z << endl;
	shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	//shader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
	//shader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
	shader.setVec3("spotLight.diffuse", 3.0f, 3.0f, 3.0f);
	shader.setVec3("spotLight.specular", 4.0f, 4.0f, 4.0f);
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.09);
	shader.setFloat("spotLight.quadratic", 0.032);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(35.0f)));
}

GLuint createTriangleVAO() {
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	//unsigned int indices[] = {
	//	0, 1, 3, // first triangle
	//	1, 2, 3  // second triangle
	//};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	return VAO;
}

GLuint createLightVAO() {
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return lightVAO;
}

GLuint loadTexture(char const * path) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		cout << "Texture failed to load at path: " << path << endl;
		stbi_image_free(data);
	}

	return textureID;
}


void setTexture(GLuint slot, GLuint type, GLuint texture)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(type, texture);
}
