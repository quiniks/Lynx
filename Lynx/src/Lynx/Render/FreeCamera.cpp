#include "Lynxpch.h"
#include "FreeCamera.h"
#include "Lynx/Events/Input.h"
#include "Lynx/Events/KeyCodes.h"
#include "Lynx/Events/MouseCodes.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Lynx {
	FreeCamera::FreeCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
	{
		UpdateLook();
		UpdateView();
	}

	void FreeCamera::OnUpdate(TimeStep ts)
	{
		UpdateLook();
		Pan(ts);
		Rotate();
		if (m_Moving)
			UpdateView();
		m_Moving = false;
	}

	void FreeCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width; m_ViewportHeight = height;
		UpdateProjection();
	}

	void FreeCamera::UpdateView()
	{
		m_ViewMatrix[0][0] = -m_Right.x;
		m_ViewMatrix[1][0] = -m_Right.y;
		m_ViewMatrix[2][0] = -m_Right.z;

		m_ViewMatrix[0][1] = -m_Up.x;
		m_ViewMatrix[1][1] = -m_Up.y;
		m_ViewMatrix[2][1] = -m_Up.z;

		m_ViewMatrix[0][2] = m_Forward.x;
		m_ViewMatrix[1][2] = m_Forward.y;
		m_ViewMatrix[2][2] = m_Forward.z;

		m_ViewMatrix[3][0] = glm::dot(m_Right, m_Position);
		m_ViewMatrix[3][1] = glm::dot(m_Up, m_Position);
		m_ViewMatrix[3][2] = -glm::dot(m_Forward, m_Position);

		m_ViewMatrix[0][3] = 0;
		m_ViewMatrix[1][3] = 0;
		m_ViewMatrix[2][3] = 0;
		m_ViewMatrix[3][3] = 1;

	}

	void FreeCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void FreeCamera::UpdateLook()
	{
		float cosPitch = cos(m_Pitch);
		float sinPitch = sin(m_Pitch);
		float cosYaw = cos(m_Yaw);
		float sinYaw = sin(m_Yaw);

		m_Right = glm::vec3{ cosYaw, 0, -sinYaw };
		m_Up = glm::vec3{ sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
		m_Forward = glm::vec3{ sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
	}

	void FreeCamera::Pan(TimeStep ts)
	{
		m_PanSpeed = m_PanSpeed + (m_PanAccel * ts);
		if (m_PanSpeed > m_PanMaxSpeed)
			m_PanSpeed = m_PanMaxSpeed;

		float up = 0;
		float forward = 0;
		float right = 0;

		if (!Input::IsKeyPressed(Key::LeftControl)) {
			if (Input::IsKeyPressed(Key::W)) {
				forward = -1;
				m_Moving = true;
			}
			if (Input::IsKeyPressed(Key::S)) {
				forward = 1;
				m_Moving = true;
			}
			if (Input::IsKeyPressed(Key::D)) {
				right = 1;
				m_Moving = true;
			}
			if (Input::IsKeyPressed(Key::A)) {
				right = -1;
				m_Moving = true;
			}
			if (Input::IsKeyPressed(Key::E)) {
				up = 1;
				m_Moving = true;
			}
			if (Input::IsKeyPressed(Key::Q)) {
				up = -1;
				m_Moving = true;
			}
		}

		if (m_Moving)
			m_Position += glm::normalize(m_Forward * forward + m_Up * up + m_Right * right) * m_PanSpeed;
		else
			m_PanSpeed = 0.0f;
	}

	void FreeCamera::Rotate()
	{
		const glm::vec2 mouse { Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_PreviousMouse) * m_Sensitivity;
		m_PreviousMouse = mouse;

		if (Input::IsMouseButtonPressed(Mouse::ButtonRight)) {
			m_Moving = true;
			m_Yaw += delta.x * -m_RotationSpeed;
			m_Pitch += delta.y * -m_RotationSpeed;
		}
	}
}