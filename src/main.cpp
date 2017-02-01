/*
CPE 471 Project 2A w/Lab 3 base code - includes use of glee
https://github.com/nshkurkin/glee
*/

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* to use glee */
#define GLEE_OVERWRITE_GL_FUNCTIONS
#include "glee.hpp"

using namespace std;
using namespace glm;

int pixW, pixH;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from
shared_ptr<Program> prog; //our shader program

/* Global data associated with triangle geometry - this will likely vary
   in later programs - so is left explicit for now  */
GLuint VertexArrayID;
static const GLfloat g_vertex_buffer_data[] = {
-1.94f, 1.36f, 0.0f,
1.1f, 1.65f, 0.0f,
-1.4f, -1.21f, 0.0f,
1.08f, -1.56f, 0.0f,
-1.09f, -1.26f, 0.0f,
-1.18f, 1.39f, 0.0f,
1.31f, 1.29f, 0.0f,
1.86f, 1.31f, 0.0f,
1.73f, 1.86f, 0.0f,
-1.05f, 1.65f, 0.0f,
1.37f, -1.91f, 0.0f,
-1.29f, 1.38f, 0.0f,
1.08f, 1.44f, 0.0f,
1.64f, 1.87f, 0.0f,
-1.23f, -1.35f, 0.0f,
1.45f, 1.46f, 0.0f,
-1.62f, 1.91f, 0.0f,
-1.18f, 1.84f, 0.0f,
-1.35f, -1.65f, 0.0f,
1.75f, 1.44f, 0.0f,
1.1f, -1.01f, 0.0f,
-1.9f, 1.3f, 0.0f,
1.04f, -1.6f, 0.0f,
-1.17f, -1.64f, 0.0f,
1.74f, -1.63f, 0.0f,
1.01f, -1.06f, 0.0f,
1.16f, -1.55f, 0.0f,
-1.54f, -1.97f, 0.0f,
1.27f, 1.27f, 0.0f,
1.93f, -1.78f, 0.0f,
1.21f, 1.05f, 0.0f,
1.23f, 1.57f, 0.0f,
-1.34f, 1.45f, 0.0f,
-1.25f, -1.44f, 0.0f,
1.75f, 1.11f, 0.0f,
1.63f, 1.49f, 0.0f,
-1.57f, 1.69f, 0.0f,
-1.29f, 1.8f, 0.0f,
1.89f, 1.9f, 0.0f,
1.16f, -1.38f, 0.0f,

};

//data necessary to give our triangle data to OGL
GLuint vertexbuffer; 

static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

//callback for the mouse when clicked move the triangle when helper functions
//written
static void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
	double posX, posY;
	float newPt[2];
	if (action == GLFW_PRESS) {
		glfwGetCursorPos(window, &posX, &posY);	
		cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		//change this to be the points converted to WORLD
		//THIS IS BROKEN< YOU GET TO FIX IT - yay!
		newPt[0] = 0;
		newPt[1] = 0;
		cout << "converted:" << newPt[0] << " " << newPt[1] << endl;
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		//update the vertex array with the updated points
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(float)*6, sizeof(float)*2, newPt);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

//if the window is resized, capture the new size and reset the viewport
static void resize_callback(GLFWwindow *window, int in_width, int in_height) {
	//get the window size - may be different then pixels for retina	
   int width, height;
   glfwGetFramebufferSize(window, &width, &height);
   glViewport(0, 0, width, height);
}

/*Note that any gl calls must always happen after a GL state is initialized */
static void initGeom() {

	//generate the VAO
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//generate vertex buffer to hand off to OGL
	glGenBuffers(1, &vertexbuffer);
	//set the current state to focus on our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//actually memcopy the data - only do this once
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

}

//General OGL initialization - set OGL state here
static void init()
{
	GLSL::checkVersion();

	// Set background color.
	glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
	// Enable z-buffer test.
	glEnable(GL_DEPTH_TEST);

	// Initialize the GLSL program.
	prog = make_shared<Program>();
	prog->setVerbose(true);
	prog->setShaderNames(RESOURCE_DIR + "simple_vert33.glsl", RESOURCE_DIR + "simple_frag33.glsl");
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addAttribute("vertPos");
	prog->addUniform("T");
}


/****DRAW
This is the most important function in your program - this is where you
will actually issue the commands to draw any geometry you have set up to
draw
********/
static void render()
{
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspect = width/(float)height;
	glViewport(0, 0, width, height);

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create the matrix stacks - please leave these alone for now
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	float T = glfwGetTime();
	// Apply orthographic projection.
	P->pushMatrix();
	if (width > height) {
		P->ortho(-2*aspect, 2*aspect, -2, 2, -2, 100.0f);
	} else {
		P->ortho(-2, 2, -2*1/aspect, 2*1/aspect, -2, 100.0f);
	}
	MV->pushMatrix();

	// Draw the triangle using GLSL.
	prog->bind();

	//send the matrices to the shaders
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform1f(prog->getUniform("T"), T);

	//we need to set up the vertex array
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//key function to get up how many elements to pull out at a time (3)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	//actually draw from vertex 0, 3 vertices
	glPointSize(34.0f);
	glDrawArrays(GL_POINTS, 0, 40);
	glDisableVertexAttribArray(0);
	
	prog->unbind();

	// Pop matrix stacks.
	MV->popMatrix();
	P->popMatrix();
}

int main(int argc, char **argv)
{
   if(argc < 2) {
      cout << "Please specify the resource directory." << endl;
      return 0;
   }
   RESOURCE_DIR = argv[1] + string("/");

	/* your main will always include a similar set up to establish your window
      and GL context, etc. */

	// Set error callback as openGL will report errors but they need a call back
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	//request the highest possible version of OGL - important for mac
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	pixW = 640;
	pixH = 480;
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(pixW, pixH, "Project 2A", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError();
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	//set the mouse call back
	glfwSetMouseButtonCallback(window, mouse_callback);
	//set the window resize call back
	glfwSetFramebufferSizeCallback(window, resize_callback);

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	init();
	initGeom();

	// Loop until the user closes the window.
	while(!glfwWindowShouldClose(window)) {
		// Render scene.
		render();
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
