#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <cstring>
#include <cassert>
#include <iostream>
#include "Utils.h"
#include "math3d.h"
#include "Pipeline.h"
#include "Camera.h"
#include "mesh.h"
#include "Technique.h"
#include "LightingTechinque.h"
using namespace std;

Camera* pGameCamera = NULL;
Mesh* m_mesh = NULL;
LightingTechinque* m_pEffect;
DirectionalLight m_directionalLight;
float fov = 45.0f;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window,double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void KeyPressFunc(int Key);

void Render() 
{
	static float Scale = 0.0f;
	Scale += 0.5f;

	//启用着色器程序
	m_pEffect->Enable(m_pEffect->m_shaderProg);

	//获取shader的uniform变量
	if (!m_pEffect->Init()) {}

	//设置模型矩阵
	Pipeline P;
	P.worldPos(0, -1, 2);

	P.Rotate(90, 180, 0);

	//设置视图矩阵
	P.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());

	if (pGameCamera->GetFOV())
	{
		fov = pGameCamera->GetFOV();
	}
	//设置透视投影矩阵
	P.SetPerspectivePro(fov, 800, 600, 1.0f, 100.0f);

	//设置MVP矩阵
	m_pEffect->SetWVP(*P.GetTrans());

	//设置世界矩阵
	const Matrix4f& WorldTransform = *P.GetWorldTrans();
	m_pEffect->SetWorldMatrix(WorldTransform);

	//给uniform变量传值
	m_pEffect->SetTextureUnit(0, 1);//转递0，1号贴图
	m_pEffect->SetDirectionalLight(m_directionalLight);//传递定向光结构体
	m_pEffect->SetEyeWorldPos(pGameCamera->GetPos());//传递摄像机位置
	m_pEffect->SetMatSpecularIntensity(1.0f);//设置高光强度
	m_pEffect->SetMatSpecularPower(32);//设置高光范围

	m_mesh->Render(m_pEffect);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);


	pGameCamera = new Camera(800, 600, window);

	m_mesh = new Mesh;
	m_mesh->LoadMesh("g.fbx");

	m_pEffect = new LightingTechinque;
	if (!m_pEffect->CompileShader("vert.glsl","frag.glsl")) {}

	m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity =  0.5;
	m_directionalLight.DiffuseIntensity = 0.5f;
	m_directionalLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render();

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	pGameCamera->OnKeyboard(window, key);
	KeyPressFunc(key);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		glfwSetCursorPosCallback(window, mouse_callback);
	}
}

void mouse_callback(GLFWwindow* window, double x, double y) 
{
	pGameCamera->OnMouse(window, x,y);
}

void KeyPressFunc(int Key)
{
	switch (Key)
	{
	case GLFW_KEY_UP:
		m_directionalLight.AmbientIntensity += 0.05f;
		break;
	case GLFW_KEY_DOWN:
		m_directionalLight.AmbientIntensity -= 0.05f;
		break;
	case GLFW_KEY_LEFT:
		m_directionalLight.DiffuseIntensity += 0.05f;
		break;
	case GLFW_KEY_RIGHT:
		m_directionalLight.DiffuseIntensity -= 0.05f;
		break;
	default:
		break;
	}

}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 60.0f)
		fov = 60.0f;
	pGameCamera->SetFOV(fov);
}