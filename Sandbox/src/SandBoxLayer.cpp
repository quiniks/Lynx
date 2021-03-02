#include "SandBoxLayer.h"
#include "Lynx/Detail/glm.h"
#include "Lynx/Detail/imgui.h"
#include "Lynx/Voxel/Voxel.h"
#include <glad/glad.h>

SandBoxLayer::SandBoxLayer()
{
	m_VoxelShader.Load("assets/shaders/voxelShaderAlt.glsl");
	m_VoxelShader.Bind();
	m_VoxelShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());
	m_VoxelShader.SetFloat("u_VoxelSize", Lynx::Voxel::SIZE);
	m_VoxelShader.SetFloat3("u_CameraPosition", m_FreeCamera.GetPosition());
	m_VoxelShader.SetFloat3("u_LightPosition", { 8.0f, 20.0f, 8.0f });
	m_VoxelShader.SetFloat3("u_AmbientColor", { 0.3f, 0.3f, 0.3f });
	m_VoxelShader.SetFloat3("u_DiffuseColor", { 0.8f, 0.8f, 0.8f });

	m_ColorShader.Load("assets/shaders/flatColorShader.glsl");
	m_ColorShader.Bind();
	m_ColorShader.SetFloat4("u_Tint", { 0.5f, 0.5f, 0.5f, 1.0f });
	m_ColorShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());

	m_Grid.Init(0.2f, 5, {0.0f, -0.01f, 0.0f});

	Lynx::App::Get().GetWindow().SetVSync(false);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
	

	m_ColorShader.Bind();
	m_ColorShader.SetMat4("u_MVP", vp);
	m_Grid.Render();
}

void SandBoxLayer::OnImGuiRender()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Frame time step: %.6fs", m_TimeStepStat);
	ImGui::Text("FPS: %.0f", 1.0f / m_TimeStepStat);
	ImGui::Text("Camera speed: %.2f", m_FreeCamera.m_PanSpeed);
	ImGui::Text("Camera pos: [%.2f, %.2f, %.2f]", m_FreeCamera.GetPosition().x, m_FreeCamera.GetPosition().y, m_FreeCamera.GetPosition().z);
	glm::uvec3 cameraPosVoxelSpace = glm::round(m_FreeCamera.GetPosition() * 2.0f);
	ImGui::Text("Camera voxel pos: [%i, %i, %i]", cameraPosVoxelSpace.x, cameraPosVoxelSpace.y, cameraPosVoxelSpace.z);
	//ImGui::Text("Clicked voxel pos: [%i, %i, %i]", m_LookVoxel.x, m_LookVoxel.y, m_LookVoxel.z);
	ImGui::End();
}

void SandBoxLayer::OnEvent(Lynx::Event& event)
{
	Lynx::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Lynx::WindowFrameResizeEvent>(BIND_EVENT_FN(SandBoxLayer::OnFrameResize));
	dispatcher.Dispatch<Lynx::MouseButtonPressedEvent>(BIND_EVENT_FN(SandBoxLayer::OnMousePressedButton));
	dispatcher.Dispatch<Lynx::KeyPressedEvent>(BIND_EVENT_FN(SandBoxLayer::OnKeyPressed));
}

bool SandBoxLayer::OnFrameResize(Lynx::WindowFrameResizeEvent& event)
{
	glViewport(0, 0, event.GetWidth(), event.GetHeight());
	m_FreeCamera.SetViewportSize((float)event.GetWidth(), (float)event.GetHeight());
	return true;
}

bool SandBoxLayer::OnMousePressedButton(Lynx::MouseButtonPressedEvent& event)
{
	
	if (event.GetMouseButton() == Lynx::Mouse::ButtonLeft) {
		glm::ivec3 voxelPos;
		if (m_World.VoxelPick(m_FreeCamera, voxelPos)) {
			LX_INFO("Click: {0}, {1}, {2}", voxelPos.x, voxelPos.y, voxelPos.z);
			m_World.VoxelSet(voxelPos, Lynx::Voxel::Type::Empty);
		}
	}
	return true;
}

bool SandBoxLayer::OnKeyPressed(Lynx::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Lynx::Key::T) {
		m_FreeCamera.SetPosition({ -1.0f, 2.0f, -1.0f });
	}
	return true;
}

SandBoxLayer::~SandBoxLayer()
{

}