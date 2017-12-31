// Local Headers
#include "glitter.h"
#include "Debug.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Config.h"
#include "Timer.h"

void setLight(Shader shader);
GLuint createTriangleVAO();
GLuint createLightVAO();
GLuint loadTexture(char const * path);
unsigned int loadCubemap(vector<string> faces);

// shaders
const GLchar *startVertexPath = "C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/start.vert";
const GLchar *startFragmentPath = "C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/start.frag";
const GLchar *lightingVertexPath = "C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/lighting.vert";
const GLchar *lightingFragmentPath = "C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/lighting.frag";
const GLchar *lampVertexPath = "C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/lamp.vert";
const GLchar *lampFragmentPath = "C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/lamp.frag";

GLuint VBO;
GLuint EBO;


// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Config& cfg = Config::getInstance();
Camera& camera = Camera::getInstance();

#include "Window.hpp"

static void displayGL()
{
	//cout << Timer::getInstance().getFPSCounter() << " FPS";
}

static void reshapeGL(int newwidth, int newheight)
{
	//VarManager& var = VarManager::getInstance();
	//var.set("win_width", newwidth);
	//var.set("win_height", newheight);

	//float ratio = (float)newwidth / (float)newheight;
	//glViewport(0, 0, (GLint)newwidth, (GLint)newheight);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective((GLdouble)var.getf("cam_fovy"), (GLdouble)ratio, (GLdouble)var.getf("cam_znear"), (GLdouble)var.getf("cam_zfar"));


	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//// Recréation des FBO
	//Effects::getInstance().reshapeFBO(newwidth, newheight);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	//glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#if _DEBUG
	debugCallback();
#endif

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	//glDisable(GL_LIGHTING);
	//glShadeModel(GL_SMOOTH);


	//SceneManager::getInstance().Init();

	//Sky::getInstance().Init();

	//Camera::getInstance().setEye(vec3(2.0f, 0.0f, 0.0f));

	//Effects::getInstance().init();

	//Camera::getInstance().setType(Camera::DRIVEN);

	//glInfo::getInstance().CheckErrors();
}

int main(int argc, char * argv[]) {
	Config& cfg = Config::getInstance();
	timerStart(glfwGetTime());

	camera.init(glm::vec3(0.0f, 2.0f, 4.0f));

	GLFWwindow* mWindow = initWindow();

	initGlad();
	initGL();

	// build and compile our shader zprogram
	// ------------------------------------
	Shader startShader = Shader(startVertexPath, startFragmentPath);
	Shader lightingShader = Shader(lightingVertexPath, lightingFragmentPath);
	Shader lampShader = Shader(lampVertexPath, lampFragmentPath);
	Shader modelShader = Shader("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/model.vert", "C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/model.frag");
	Shader blendingShader("shaders/blending.vert", "shaders/blending.frag");
	Shader skyboxShader = Shader("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/skybox.vert", "C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/shaders/skybox.frag");
	Shader screenShader("shaders/screen.vert", "shaders/screen.frag");

	// load models
	// -----------
	Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/nanosuit/nanosuit.obj");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/IronMan/IronMan.obj");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/bugatti/bugatti.obj");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/Avent/Avent.obj");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/luxuryhouse/luxury house interior.obj");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/wolf/Wolf_obj.obj");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/Black Dragon/Dragon 2.5_stl.stl");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/Princess_Hecuba/princess.obj");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/street/Street environment_V01.obj");
	//Model ourModel("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/objects/Habitat/Habitat.obj");

	float planeVertices[] = {
		// positions          // texture Coords
		50.0f, -0.5f,  50.0f,  2.0f, 0.0f,
		-50.0f, -0.5f,  50.0f,  0.0f, 0.0f,
		-50.0f, -0.5f, -50.0f,  0.0f, 2.0f,

		50.0f, -0.5f,  50.0f,  2.0f, 0.0f,
		-50.0f, -0.5f, -50.0f,  0.0f, 2.0f,
		50.0f, -0.5f, -50.0f,  2.0f, 2.0f
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

	float skyboxVertices[] = {
		// positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

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
	GLuint diffuseMap = loadTexture("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/container2.png");
	GLuint specularMap = loadTexture("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/container2_specular.png");
	GLuint emissionMap = loadTexture("C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/matrix.jpg");
	unsigned int floorTexture = loadTexture("resources/textures/metal.png");
	//unsigned int transparentTexture = loadTexture("resources/textures/grass.png");
	unsigned int transparentTexture = loadTexture("resources/textures/window.png");

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
	//GLuint texture1 = createTexture("resources/textures/container.jpg", GL_RGB);
	//GLuint texture2 = createTexture("resources/textures/awesomeface.png", GL_RGBA);
	vector<string> faces
	{
		"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/tuto/right.jpg",
		"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/tuto/left.jpg",
		"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/tuto/top.jpg",
		"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/tuto/bottom.jpg",
		"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/tuto/back.jpg",
		"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/tuto/front.jpg"
	};
	//vector<string> faces
	//{
	//	"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/mnight/mnight_rt.tga",
	//	"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/mnight/mnight_lt.tga",
	//	"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/mnight/mnight_up.tga",
	//	"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/mnight/mnight_dn.tga",
	//	"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/mnight/mnight_bk.tga",
	//	"C:/Users/miyuk/Documents/OpenGL/Projects/Glitter/Glitter/resources/textures/skybox/mnight/mnight_ft.tga"
	//};

	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	//lightingShader.use();
	//lightingShader.setInt("material.texture_diffuse", 0);
	//lightingShader.setInt("material.texture_specular", 1);
	//lightingShader.setInt("material.emission", 5);
	lightingShader.use();
	lightingShader.setInt("skybox", 0);

	blendingShader.use();
	blendingShader.setInt("texture1", 0);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	screenShader.use();
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
	while (!glfwWindowShouldClose(mWindow)) {
		// per-frame time logic
		// --------------------
		idleGL();
		displayGL();

		// input
		// -----
		processInput(mWindow, elapsedTime);

		// sort the transparent windows before rendering
		// ---------------------------------------------
		map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length2(camera.Position - windows[i]);
			sorted[distance] = windows[i];
		}

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
		blendingShader.use();
		// floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		model = glm::mat4();
		//model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		blendingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// windows
		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, transparentTexture);
		for (map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4();
			model = glm::translate(model, it->second);
			blendingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// don't forget to enable shader before setting uniforms
		modelShader.use();

		// render the loaded model
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		modelShader.setMat4("model", model);
		ourModel.Draw(modelShader);

		setLight(modelShader);

		lightingShader.use();

		// bind diffuse map
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, diffuseMap);
		//// bind specular map
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, specularMap);
		//// bind emission map
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionMap);

		//reflection
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		setLight(lightingShader);

		// world transformation
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 1; i < 10; i++) {
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle) * (float)glfwGetTime(), glm::vec3(1.0f, 0.3f, 0.5f));
			modelShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// also draw the lamp object
		lampShader.use();

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
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &skyboxVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteBuffers(1, &quadVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return EXIT_SUCCESS;
}

unsigned int loadCubemap(vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			cout << "Cubemap texture failed to load at path: " << faces[i] << endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void setLight(Shader shader) {
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// activate shader
	//lightingShader.use();
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
	shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	for (GLuint i = 0; i < 4; i++) {
		string number = to_string(i);

		shader.setVec3(("pointLights[" + number + "].position").c_str(), pointLightPositions[i]);
		shader.setVec3(("pointLights[" + number + "].ambient").c_str(), glm::vec3(0.1f));
		shader.setVec3(("pointLights[" + number + "].diffuse").c_str(), glm::vec3(0.8f));
		shader.setVec3(("pointLights[" + number + "].specular").c_str(), glm::vec3(1.0f));
		shader.setFloat(("pointLights[" + number + "].constant").c_str(), 1.0f);
		shader.setFloat(("pointLights[" + number + "].linear").c_str(), 0.09f);
		shader.setFloat(("pointLights[" + number + "].quadratic").c_str(), 0.032f);
	}
	// spotLight
	shader.setVec3("spotLight.position", camera.Position);
	shader.setVec3("spotLight.direction", camera.Front);
	shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	shader.setFloat("spotLight.constant", 1.0f);
	shader.setFloat("spotLight.linear", 0.09);
	shader.setFloat("spotLight.quadratic", 0.032);
	shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
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

