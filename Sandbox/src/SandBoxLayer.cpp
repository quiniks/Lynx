#include "SandBoxLayer.h"
#include "Lynx/Detail/glm.h"
#include "Lynx/Detail/imgui.h"
#include "Lynx/Voxel/Voxel.h"
#include <glad/glad.h>

SandBoxLayer::SandBoxLayer()
{
	Lynx::Voxel2 test;
	test.SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	uint32_t color = test.GetColor();

	m_VertexData.emplace_back(VertexData{ { 0.0f, 0.0f, 0.0f }, color });
	m_VertexData.emplace_back(VertexData{ { 1.0f, 0.0f, 0.0f }, color });
	m_VertexData.emplace_back(VertexData{ { 0.0f, 1.0f, 0.0f }, color });

	m_VA = Lynx::VertexArray::Create();
	m_VB = Lynx::VertexBuffer::Create(m_VertexData.data(), m_VertexData.size() * sizeof(VertexData));
	m_VB->SetLayout({
		{ Lynx::ShaderDataType::Float3, "a_Position" },
		{ Lynx::ShaderDataType::PackedInt, "a_Color" },
		});
	m_VA->AddVertexBuffer(m_VB);
	m_PackedShader.Load("assets/shaders/packedColorTest.glsl");
	m_PackedShader.Bind();
	m_PackedShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());

	Lynx::Window& window = Lynx::App::Get().GetWindow();
	m_FreeCamera.Init(45.0f, window.GetFrameWidth(), window.GetFrameHeight(), 0.1f, 1000.0f);
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

	m_World.Init();
	m_Grid.Init(0.2f, 5, { 0.0f, 0.0f, 0.0f });

	//Lynx::App::Get().GetWindow().SetVSync(false);

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

	m_PackedShader.Bind();
	m_PackedShader.SetMat4("u_MVP", vp);
	m_VA->Bind();
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_VertexData.size());
}

void SandBoxLayer::OnImGuiRender()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Frame time step: %.6fs", m_TimeStepStat);
	ImGui::Text("FPS: %.0f", 1.0f / m_TimeStepStat);
	ImGui::Text("Camera speed: %.2f", m_FreeCamera.GetSpeed());
	ImGui::Text("Camera pos: [%.2f, %.2f, %.2f]", m_FreeCamera.GetPosition().x, m_FreeCamera.GetPosition().y, m_FreeCamera.GetPosition().z);
	glm::uvec3 cameraPosVoxelSpace = glm::round(m_FreeCamera.GetPosition() * 2.0f);
	ImGui::Text("Camera voxel pos: [%i, %i, %i]", cameraPosVoxelSpace.x, cameraPosVoxelSpace.y, cameraPosVoxelSpace.z);
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
			m_World.VoxelSet(voxelPos, Lynx::Voxel::Type::Empty);
		}
	}
	return true;
}

bool SandBoxLayer::OnKeyPressed(Lynx::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == Lynx::Key::T) {
		m_RenderMode = !m_RenderMode;
		if (m_RenderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	return true;
}

SandBoxLayer::~SandBoxLayer()
{

}