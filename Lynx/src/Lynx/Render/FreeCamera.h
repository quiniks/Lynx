#pragma once
#include "Lynx/Core/TimeStep.h"
#pragma warning( push )
#pragma warning ( disable : 5033 )
#include <glm/glm.hpp>
#pragma warning( pop )

namespace Lynx {
	class FreeCamera {
	public:
		FreeCamera() = default;
		void Init(float fov, int width, int height, float nearClip, float farClip);
		glm::mat4 GetView() const { return m_ViewMatrix; }
		glm::mat4 GetProjection() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewProjection() const { return m_ProjectionMatrix * m_ViewMatrix; }
		glm::vec3 GetPosition() const;
		float GetSpeed() { return m_PanSpeed; };

		void SetViewportSize(float width, float height);
		void SetPosition(const glm::vec3& pos);

		void OnUpdate(TimeStep ts);
	private:
		void UpdateView();
		void UpdateProjection();
		void UpdateLook();
		void Pan(TimeStep ts);
		void Rotate();

		bool m_Moving = false;
		float m_FOV = 45.0f;
		float m_AspectRatio = 1.778f;
		float m_NearClip = 0.1f;
		float m_FarClip = 1000.0f;
		float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
		float m_ViewportWidth = 0.0f;
		float m_ViewportHeight = 0.0f;
		float m_Sensitivity = 0.003f;
		float m_PanSpeed = 0.0f;
		float m_PanMaxSpeed = 25.0f;
		float m_PanAccel = 5.0f;
		float m_RotationSpeed = 0.5f;
		float m_Zoom = 0.02f;
		glm::vec2 m_PreviousMouse = { 0.0f, 0.0f };

		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Right{0.0f};
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };

		glm::mat4 m_ProjectionMatrix = glm::mat4{ 0.0f };
		glm::mat4 m_ViewMatrix = glm::mat4{ 0.0f };
		glm::vec3 m_Position = {-2.0f, 4.0f, -2.0f};
	};
}