#include "SandBoxLayer.h"
#include "Lynx/Detail/glm.h"
#include "Lynx/Detail/imgui.h"
#include "Lynx/Voxel/Voxel.h"
#include <glad/glad.h>

SandBoxLayer::SandBoxLayer()
{

	m_VoxelShader.Load("assets/shaders/voxelShader.glsl");
	m_VoxelShader.Bind();
	m_VoxelShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());
	m_VoxelShader.SetFloat("u_VoxelSize", Lynx::Voxel::SIZE);
	m_VoxelShader.SetFloat3("u_CameraPosition", m_FreeCamera.GetPosition());
	m_VoxelShader.SetFloat3("u_LightPosition", { 10.0f, 100.0f, 10.0f });
	m_VoxelShader.SetFloat3("u_AmbientColor", { 0.3f, 0.3f, 0.3f });
	m_VoxelShader.SetFloat3("u_DiffuseColor", { 1.0f, 1.0f, 1.0f });

	Lynx::App::Get().GetWindow().SetVSync(false);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//m_Chunk.CreateMesh(-5.0f, -5.0f, -5.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void SandBoxLayer::OnUpdate(Lynx::TimeStep timeStep)
{
	
	m_TimeStepStat = timeStep;
	m_FreeCamera.OnUpdate(timeStep);

	static float rot = 0;
	rot += timeStep.GetSeconds();

	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::rotate(m_Transform, rot, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 vp = m_FreeCamera.GetViewProjection();
	glm::mat4 mvp = m_FreeCamera.GetViewProjection() * m_Transform;

	glClearColor(1.0f, 0.9f, 0.85f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_VoxelShader.Bind();
	m_VoxelShader.SetMat4("u_MVP", vp);
	m_VoxelShader.SetFloat3("u_CameraPosition", m_FreeCamera.GetPosition());
	m_World.Render();
}

void SandBoxLayer::OnImGuiRender()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Frame time step: %.6fs", m_TimeStepStat);
	ImGui::Text("FPS: %.0f", 1.0f / m_TimeStepStat);
	ImGui::Text("Camera speed: %.2f", m_FreeCamera.m_PanSpeed);
	ImGui::Text("Camera pos: [%.1f, %.1f, %.1f]", m_FreeCamera.GetPosition().x, m_FreeCamera.GetPosition().y, m_FreeCamera.GetPosition().z);
	glm::uvec3 cameraPosVoxelSpace = glm::round(m_FreeCamera.GetPosition() * 2.0f);
	ImGui::Text("Camera voxel pos: [%i, %i, %i]", cameraPosVoxelSpace.x, cameraPosVoxelSpace.y, cameraPosVoxelSpace.z);
	ImGui::Text("Clicked voxel pos: [%i, %i, %i]", m_LookVoxel.x, m_LookVoxel.y, m_LookVoxel.z);
	ImGui::End();
}

void SandBoxLayer::OnEvent(Lynx::Event& event)
{
	Lynx::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Lynx::WindowFrameResizeEvent>(BIND_EVENT_FN(SandBoxLayer::OnFrameResize));
	dispatcher.Dispatch<Lynx::MouseButtonPressedEvent>(BIND_EVENT_FN(SandBoxLayer::OnMousePressedButton));
}

bool SandBoxLayer::OnFrameResize(Lynx::WindowFrameResizeEvent& event)
{
	glViewport(0, 0, event.GetWidth(), event.GetHeight());
	m_FreeCamera.SetViewportSize((float)event.GetWidth(), (float)event.GetHeight());
	return true;
}

bool SandBoxLayer::OnMousePressedButton(Lynx::MouseButtonPressedEvent& event)
{
	/*
	bool state = false;
	bool clicked = false;
	if (event.GetMouseButton() == Lynx::Mouse::ButtonLeft) {
		state = false;
		clicked = true;
	}
	else if (event.GetMouseButton() == Lynx::Mouse::ButtonRight) {
		state = true;
		clicked = true;
	}

	if (clicked) {
		GLint viewportSize[4];
		glGetIntegerv(GL_VIEWPORT, viewportSize);
		float x = (2.0f * Lynx::Input::GetMouseX()) / viewportSize[2] - 1.0f;
		float y = 1.0f - (2.0f * Lynx::Input::GetMouseY()) / viewportSize[3];
		glm::vec4 rayEye = glm::inverse(m_FreeCamera.GetProjection()) * glm::vec4(x, y, -1.0f, 1.0);
		glm::vec3 rayWorld = (glm::inverse(m_FreeCamera.GetView()) * glm::vec4(rayEye.x, rayEye.y, -1.0, 0.0f));
		rayWorld = glm::normalize(rayWorld);

		std::vector<glm::ivec3> VoxelsAlongRay;
		Lynx::VoxelRay::PosFromRay(m_FreeCamera.GetPosition(), rayWorld, 50.0f, VoxelsAlongRay);

		for (auto i = VoxelsAlongRay.begin(); i != VoxelsAlongRay.end(); ++i) {
			glm::ivec3 pos = *i;
			if (Lynx::Chunk::Inside(pos.x, pos.y, pos.z) && m_Chunk.GetVoxel(pos.x, pos.y, pos.z) != Lynx::Voxel::Type::Empty) {
				m_LookVoxel = pos;
				if (state && VoxelsAlongRay.size() > 1 && i != VoxelsAlongRay.begin()) {
					glm::ivec3 prevPos = *(i - 1);
					if (Lynx::Chunk::Inside(prevPos.x, prevPos.y, prevPos.z))
						m_Chunk.GetVoxel(prevPos.x, prevPos.y, prevPos.z) = Lynx::Voxel::Type::Solid;
				}
				else if (!state) {
					m_Chunk.GetVoxel(pos.x, pos.y, pos.z) = Lynx::Voxel::Type::Empty;
				}
				m_Chunk.Update();
				break;
			}
		}
	}
	*/
	return true;
}

SandBoxLayer::~SandBoxLayer()
{
}
