#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_LIGHTS 2
#define NUM_MATERIALS 3

GLFWwindow *window;
int PROG_SEL = 0;
#define NUM_PROGS 3
Program program[NUM_PROGS];
std::vector<float> posBuff;
std::vector<float> norBuff;
std::vector<float> texBuff;

glm::vec3 eye(0.0f, 0.0f, 4.0f); //in world space

struct materialStruct {
	glm::vec3 ka, kd, ks;
	float s;
} materials[NUM_MATERIALS];

int MATERIAL_SEL=0;

struct lightStruct {
	glm::vec3 position; //in world space
	glm::vec3 color;
} lights[NUM_LIGHTS];

int LIGHT_SEL = 0;

void Display()
{		
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	program[PROG_SEL].Bind();
	program[PROG_SEL].SendUniformData(modelMatrix, "model");
	program[PROG_SEL].SendUniformData(viewMatrix, "view");
	program[PROG_SEL].SendUniformData(projectionMatrix, "projection");

	program[PROG_SEL].SendUniformData(lights[0].position, "lights[0].position");
	program[PROG_SEL].SendUniformData(lights[0].color, "lights[0].color");
	program[PROG_SEL].SendUniformData(lights[1].position, "lights[1].position");
	program[PROG_SEL].SendUniformData(lights[1].color, "lights[1].color");

	program[PROG_SEL].SendUniformData(materials[MATERIAL_SEL].ka, "ka");
	program[PROG_SEL].SendUniformData(materials[MATERIAL_SEL].kd, "kd");
	program[PROG_SEL].SendUniformData(materials[MATERIAL_SEL].ks, "ks");
	program[PROG_SEL].SendUniformData(materials[MATERIAL_SEL].s, "s");

	glDrawArrays(GL_TRIANGLES, 0, posBuff.size() / 3);
	program[PROG_SEL].Unbind();

}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) 
	{
	case 'm':
		if (MATERIAL_SEL == 2) {
			MATERIAL_SEL = 0;
		}
		else {
			MATERIAL_SEL++;
		}
		break;
	case 'M':
		if (MATERIAL_SEL == 0) {
			MATERIAL_SEL = 2;
		}
		else {
			MATERIAL_SEL--;
		}
		break;
	case 'l':
		if (LIGHT_SEL == 1) {
			LIGHT_SEL = 0;
		}
		else {
			LIGHT_SEL++;
		}
		break;
	case 'L':
		if (LIGHT_SEL == 0) {
			LIGHT_SEL = 1;
		}
		else {
			LIGHT_SEL--;
		}
		break;
	case 'x':
		lights[LIGHT_SEL].position.x += 0.1;
		break;
	case 'X':
		lights[LIGHT_SEL].position.x -= 0.1;
		break;
	case 'y':
		lights[LIGHT_SEL].position.y += 0.1;
		break;
	case 'Y':
		lights[LIGHT_SEL].position.y -= 0.1;
		break;
	case 'z':
		lights[LIGHT_SEL].position.z += 0.1;
		break;
	case 'Z':
		lights[LIGHT_SEL].position.z -= 0.1;
		break;
	case 'r':
		lights[0].position = { 0.0, 0.0, 3.0 };
		lights[1].position = { 0.0, 3.0, 0.0 };
	case '0':
		PROG_SEL = 0;
		break;
	case '1':
		PROG_SEL = 1;
		break;
	case '2':
		PROG_SEL = 2;
		break;
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	default:

		break;
	}
}


void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void LoadModel(char* name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

void Init()
{
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment4 - Nandini Janapati", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	LoadModel("../obj/bunny.obj");

	char* vertfiles[3] = { "../shaders/shader1.vert", "../shaders/shader2.vert", "../shaders/shader3.vert" };
	char* fragfiles[3] = { "../shaders/shader1.frag", "../shaders/shader2.frag", "../shaders/shader3.frag" };
	for (int i = 0; i < 3; i++) {
		program[i].SetShadersFileName(vertfiles[i], fragfiles[i]);
		program[i].Init();
		program[i].SendAttributeData(posBuff, "vPositionModel");
		program[i].SendAttributeData(norBuff, "vNormalModel");
	}
	/*program.SetShadersFileName("../shaders/shader3.vert", "../shaders/shader3.frag");
	program.Init();
	program.SendAttributeData(posBuff, "vPositionModel");
	program.SendAttributeData(norBuff, "vNormalModel");*/
}


int main()
{	
	materials[0].ka = { 0.2, 0.2, 0.2 };
	materials[0].kd = { 0.8, 0.7, 0.7 };
	materials[0].ks = { 1.0, 1.0, 1.0 };
	materials[0].s = 10.0;

	materials[1].ka = { 0.0, 0.2, 0.2 };
	materials[1].kd = { 0.5, 0.7, 0.2 };
	materials[1].ks = { 0.1, 1.0, 0.1 };
	materials[1].s = 100.0;

	materials[2].ka = { 0.2, 0.2, 0.2 };
	materials[2].kd = { 0.1, 0.3, 0.9 };
	materials[2].ks = { 0.1, 0.1, 0.1 };
	materials[2].s = 1.0;

	lights[0].position = { 0.0, 0.0, 3.0 };
	lights[0].color = { 0.5, 0.5, 0.5 };

	lights[1].position = { 0.0, 3.0, 0.0 };
	lights[1].color = { 0.2, 0.2, 0.2 };
	

	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}