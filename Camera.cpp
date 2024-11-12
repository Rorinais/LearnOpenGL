#include"Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


static float StepScale = 0.5f;


Camera::Camera(float width, float height, GLFWwindow* window)
{
	m_pos = Vector3f(0.0f, 0.0f, 0.0f);
	m_target = Vector3f(0.0f, 0.0f,-1.0f);
	m_up = Vector3f(0.0f, 1.0f, 0.0f);
	m_WindowHeight = height;
	m_WindowWidth = width;
	this->window = window;
}
void Camera::SetFOV(float fov) 
{
	m_fov = fov;
}

Camera::~Camera()
{

}

Camera::Camera(const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up, float width, float height, GLFWwindow* window)
{
	m_pos = Pos;
	m_target = Target;
	m_target.Normalize();
	m_up = Up;
	m_up = m_up.Normalize();

	m_WindowHeight = height;
	m_WindowWidth = width;
	this->window = window;
}

bool Camera::OnKeyboard(GLFWwindow* window,int Key)
{
	bool Ret = false;
	switch (Key)
	{
	case GLFW_KEY_W:
		m_pos -= (m_target * StepScale);
		Ret = true;
		cout << m_pos.x << "--" << m_pos.y << "--" << m_pos.z << endl;
		break;

	case GLFW_KEY_S:
		m_pos += (m_target * StepScale);
		Ret = true;
		cout << m_pos.x << "--" << m_pos.y << "--" << m_pos.z << endl;
		break;

	case GLFW_KEY_A:
		Left = m_target.Cross(m_up);
		Left.Normalize();
		m_pos += (Left * StepScale);
		cout << m_pos.x << "--" << m_pos.y << "--" << m_pos.z << endl;
		break;

	case GLFW_KEY_D:
		Right = m_target.Cross(m_up);
		Right.Normalize();
		m_pos -= (Right * StepScale);
		cout << m_pos.x <<"--" << m_pos.y <<"--" << m_pos.z << endl;
		break;
	}
	return Ret;
}


void Camera::init()
{
	Vector3f Htarget(m_target.x, 0, m_target.z);
	Htarget.Normalize();

	if (Htarget.z >= .0f)
	{
		if (Htarget.x >= .0f)
		{
			m_AngleH = 360 - ToDegree(asin(Htarget.z));
		}
		else
		{
			m_AngleH = 180 + ToDegree(asin(Htarget.z));
		}
	}
	else
	{
		if (Htarget.x >= .0f)
		{
			m_AngleH = ToDegree(asin(-Htarget.z));
		}
		else
		{
			m_AngleH = 90 + ToDegree(asin(-Htarget.z));
		}
	}
	m_AngleV = -ToDegree(asin(m_target.y));

 
	m_mousePosX = m_WindowWidth / 2.f;
	m_mousePosY = m_WindowHeight / 2.f;
	glfwSetCursorPos(this->window, m_mousePosX, m_mousePosY);
}

void Camera::OnMouse(GLFWwindow* window, double x, double y)
{
	m_mousePosX = m_WindowWidth / 2.f;
	m_mousePosY = m_WindowHeight / 2.f;
	const int DeltaX = x - m_mousePosX;
	const int DeltaY = y - m_mousePosY;

	if (abs(DeltaX) > 3 || abs(DeltaY) > 3)
	{
		m_AngleH += DeltaX / 50.f;
		m_AngleV += DeltaY / 50.f;
		Update();
	}
}

void Camera::Update()
{
	Vector3f Vaxis(0, 1, 0);
	Vector3f View(0, 0, -1);

	View.Rotate(m_AngleH, Vaxis);
	View.Normalize();
	Vector3f Haxis = Vaxis.Cross(View);
	Haxis.Normalize();
	View.Rotate(m_AngleV, Haxis);
	View.Normalize();

	m_target = View;
	m_target.Normalize();

	m_up = m_target.Cross(Haxis);
	m_up.Normalize();

	m_mousePosX = m_WindowWidth / 2.f;
	m_mousePosY = m_WindowHeight / 2.f;
	glfwSetCursorPos(this->window, m_mousePosX, m_mousePosY);
}