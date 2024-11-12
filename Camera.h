#pragma once
#include"math3d.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(float width, float height, GLFWwindow* window);

	Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up,float width,float height, GLFWwindow* window);

	bool OnKeyboard(GLFWwindow* window,int Key);

	const Vector3f& GetPos()const
	{
		return m_pos;
	}

	const Vector3f& GetTarget()const
	{
		return m_target;
	}

	const Vector3f& GetUp()const
	{
		return m_up;
	}
	const float& GetFOV()const
	{
		return m_fov;
	}
	void SetFOV(float fov);

	~Camera();

	void OnMouse(GLFWwindow* window,double x, double y);
private:
	void init();
	void Update();

	float m_AngleH;
	float m_AngleV;

	float m_mousePosX;
	float m_mousePosY;

	float m_fov;

	float m_WindowWidth;
	float m_WindowHeight;

	GLFWwindow* window;
	
	Vector3f m_pos;
	Vector3f m_target;
	Vector3f m_up;

	Vector3f Right;
	Vector3f Left;
};
