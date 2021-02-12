#include "SandBoxLayer.h"
#include "Lynx/Detail/glm.h"
#include "Lynx/Detail/imgui.h"
#include <glad/glad.h>

SandBoxLayer::SandBoxLayer()
{
	m_VoxelShader.Load("assets/shaders/voxelShader.glsl");
	m_VoxelShader.Bind();
	m_VoxelShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());
	m_VoxelShader.UploadUniformFloat("u_VoxelSize", 0.5f);
	m_VoxelShader.SetFloat3("u_CameraPosition", m_FreeCamera.GetPosition());

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//Lynx::App::Get().GetWindow().SetVSync(false);
	m_VoxelMachine.CreateBox({ 5, 5, 5 });

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_VoxelMachine.Bind();
	m_VoxelShader.Bind();
	m_VoxelShader.SetMat4("u_MVP", vp);
	m_VoxelShader.SetFloat3("u_CameraPosition", m_FreeCamera.GetPosition());
	m_VoxelMachine.Draw();

	//LX_CORE_INFO("Elapsed Time: {0}s", timeStep);
	//LX_CORE_INFO("cam pos: {0}, {1}, {2}", m_FreeCamera.GetPosition().x, m_FreeCamera.GetPosition().y, m_FreeCamera.GetPosition().z);
}

void SandBoxLayer::OnImGuiRender()
{
	ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Frame time step: %.4f", m_TimeStepStat);
	ImGui::Text("FPS: %.0f", 1.0f / m_TimeStepStat);
	ImGui::Text("Camera speed: %.2f", m_FreeCamera.m_PanSpeed);
	ImGui::Text("Camera pos: [%.1f, %.1f, %.1f]", m_FreeCamera.GetPosition().x, m_FreeCamera.GetPosition().y, m_FreeCamera.GetPosition().z);
	ImGui::End();
}

void SandBoxLayer::OnEvent(Lynx::Event& event)
{
	Lynx::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Lynx::WindowFrameResizeEvent>(BIND_EVENT_FN(SandBoxLayer::OnFrameResize));
}

bool SandBoxLayer::OnFrameResize(Lynx::WindowFrameResizeEvent& event)
{
	glViewport(0, 0, event.GetWidth(), event.GetHeight());
	m_FreeCamera.SetViewportSize((float)event.GetWidth(), (float)event.GetHeight());
	return true;
}

SandBoxLayer::~SandBoxLayer()
{
}
