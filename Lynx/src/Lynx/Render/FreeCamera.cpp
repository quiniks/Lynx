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
		m_Yaw = glm::radians(135.0f);
		UpdateLook();
		UpdateView();
		UpdateProjection();
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

	glm::vec3 FreeCamera::GetPosition() const
	{
		return m_Position;
	}

	void FreeCamera::SetViewportSize(float width, float height)
	{
		m_ViewportWidth = width; m_ViewportHeight = height;
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		UpdateProjection();
	}

	void FreeCamera::UpdateView()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void FreeCamera::UpdateProjection()
	{
		m_ProjectionMatrix = glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void FreeCamera::UpdateLook()
	{
		glm::vec3 front;
		front.x = sin(m_Yaw) * cos(m_Pitch);
		front.y = sin(m_Pitch);
		front.z = -cos(m_Yaw) * cos(m_Pitch);
		m_Front = glm::normalize(front);
	}

	void FreeCamera::Pan(TimeStep ts)
	{
		m_PanSpeed += m_PanAccel * ts;
		if (m_PanSpeed > m_PanMaxSpeed)
			m_PanSpeed = m_PanMaxSpeed;

		float up = 0;
		float forward = 0;
		float right = 0;

		if (!Input::IsKeyPressed(Key::LeftControl)) {
			if (Input::IsKeyPressed(Key::W)) {
				forward = 1;
				m_Moving = true;
			}
			if (Input::IsKeyPressed(Key::S)) {
				forward = -1;
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

		if (m_Moving) {
			m_Right = glm::cross(m_Front, m_Up);
			m_Position += glm::normalize(m_Front * forward + m_Up * up + m_Right * right) * m_PanSpeed * glm::vec3(ts, ts, ts);
		}
		else
			m_PanSpeed = 0.0f;
	}

	void FreeCamera::Rotate()
	{
		const glm::vec2& mouse { Input::GetMouseX(), Input::GetMouseY() };
		glm::vec2 delta = (mouse - m_PreviousMouse) * m_Sensitivity;
		m_PreviousMouse = mouse;

		if (Input::IsMouseButtonPressed(Mouse::ButtonRight)) {
			m_Moving = true;
			m_Yaw += delta.x * m_RotationSpeed;
			m_Pitch += delta.y * -m_RotationSpeed;
			//LX_CORE_INFO("Pitch: {0}, Yaw: {1}", m_Pitch, m_Yaw);
		}
	}
}