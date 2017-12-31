#pragma once

float lastX;
float lastY;
bool firstMouse;
bool keys[1024]; // is a key pressed or not ?
				 // External static callback
				 // Is called whenever a key is pressed/released via GLFW

GLFWwindow* initWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, GLfloat deltaTime);
void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/);
void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset);

GLFWwindow* initWindow()
{
	firstMouse = true;
	lastX = cfg.SCR_WIDTH / 2.0;
	lastY = cfg.SCR_HEIGHT / 2.0;

	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// glfw window creation
	// --------------------
	GLFWwindow* mWindow = glfwCreateWindow(cfg.SCR_WIDTH, cfg.SCR_HEIGHT, "INFO-H-502 - Team ZH", nullptr, nullptr);
	if (mWindow == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	// Set the required callback functions
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
	glfwSetScrollCallback(mWindow, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(1);

	return mWindow;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, GLfloat deltaTime)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	// V-SYNC
	if (keys[GLFW_KEY_U]) {
		static bool vsync = false;
		if (vsync) {
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		vsync = !vsync;
	}

	if ((keys[GLFW_KEY_0] || keys[GLFW_KEY_KP_0])) {
		std::cout << "You have pressed 0" << std::endl;
	}
}

void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		keys[GLFW_MOUSE_BUTTON_RIGHT] = true;
	else
		keys[GLFW_MOUSE_BUTTON_RIGHT] = false;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		keys[GLFW_MOUSE_BUTTON_LEFT] = true;
	else
		keys[GLFW_MOUSE_BUTTON_LEFT] = false;

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		keys[GLFW_MOUSE_BUTTON_MIDDLE] = true;
	else
		keys[GLFW_MOUSE_BUTTON_MIDDLE] = false;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

	if (keys[GLFW_MOUSE_BUTTON_RIGHT]) {
		std::cout << "Mouse Position : (" << xpos << ", " << ypos << ")" << std::endl;
	}
}
void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);

	if (keys[GLFW_MOUSE_BUTTON_LEFT]) {
		std::cout << "Mouse Offset : " << yoffset << std::endl;
	}
}