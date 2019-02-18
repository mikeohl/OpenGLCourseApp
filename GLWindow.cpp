#include "GLWindow.h"



GLWindow::GLWindow()
{
	width = 800;
	height = 600;

	lastX = 0;
	lastY = 0;
	xChange = 0;
	yChange = 0;
	mouseFirstMoved = true;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	lastX = 0;
	lastY = 0;
	xChange = 0;
	yChange = 0;
	mouseFirstMoved = true;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int GLWindow::initialize()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Handle Key + Mouse Input
	createCallbacks();

	// Lock input to window
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Create and setup Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}

void GLWindow::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat GLWindow::getXChange()
{
	GLfloat change = xChange;
	xChange = 0;
	return change;
}

GLfloat GLWindow::getYChange()
{
	GLfloat change = yChange;
	yChange = 0;
	return change;
}

void GLWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("Released: %d\n", key);
		}
	}
}

void GLWindow::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	// Get change in position
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	// Set position for next tick
	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

	//printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
