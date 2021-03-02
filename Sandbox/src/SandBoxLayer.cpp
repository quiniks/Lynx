#include "SandBoxLayer.h"
#include "Lynx/Detail/glm.h"
#include "Lynx/Detail/imgui.h"
#include "Lynx/Voxel/Voxel.h"
#include <glad/glad.h>

SandBoxLayer::SandBoxLayer()
{
	std::vector<glm::vec3> quad{
		{0.0f, 0.0f, 0.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{1.0f, 1.0f, 0.0f},

		{0.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
	};
	std::vector<int> quadI{
		//front
		0, 1, 3,
		0, 3, 2,
		//left
		4, 0, 2,
		4, 2, 6,
		//right
		1, 5, 7,
		1, 7, 3,
		//top
		3, 7, 6,
		3, 6, 2,
		//bottom
		0, 4, 5,
		0, 5, 1,
		//back
		5, 4, 6,
		5, 6, 7,
	};
	
	m_QuadVA = Lynx::VertexArray::Create();
	Lynx::Ref<Lynx::VertexBuffer> vb = Lynx::VertexBuffer::Create(&quad[0], quad.size() * sizeof(glm::vec3));
	vb->SetLayout({ { Lynx::ShaderDataType::Float3, "a_Position" } });
	Lynx::Ref<Lynx::IndexBuffer> ib = Lynx::IndexBuffer::Create(&quadI[0], quadI.size());
	m_QuadVA->AddVertexBuffer(vb);
	m_QuadVA->SetIndexBuffer(ib);

	m_VoxelShader.Load("assets/shaders/voxelShader.glsl");
	m_VoxelShader.Bind();
	m_VoxelShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());
	m_VoxelShader.SetFloat("u_VoxelSize", Lynx::Voxel::SIZE);
	m_VoxelShader.SetFloat3("u_CameraPosition", m_FreeCamera.GetPosition());
	m_VoxelShader.SetFloat3("u_LightPosition", { 10.0f, 100.0f, 500.0f });
	m_VoxelShader.SetFloat3("u_AmbientColor", { 0.3f, 0.3f, 0.3f });
	m_VoxelShader.SetFloat3("u_DiffuseColor", { 0.8f, 0.8f, 0.8f });

	m_ColorShader.Load("assets/shaders/flatColorShader.glsl");
	m_ColorShader.Bind();
	m_ColorShader.SetFloat4("u_Tint", { 0.5f, 0.5f, 0.5f, 1.0f });
	m_ColorShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());

	m_Grid.Init(0.2f, 5, { -0.1f, -0.2f, -0.1f });

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
	//m_World.Render();
	

	m_ColorShader.Bind();
	m_ColorShader.SetMat4("u_MVP", vp);
	m_Grid.Render();

	m_QuadVA->Bind();
	glDrawElements(GL_TRIANGLES, (GLsizei)m_QuadVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
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
	//ImGui::Text("Clicked voxel pos: [%i, %i, %i]", m_LookVoxel.x, m_LookVoxel.y, m_LookVoxel.z);
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
	if (event.GetMouseButton() == Lynx::Mouse::ButtonLeft) {
		glm::ivec3 voxelPos;
		if (m_World.VoxelPick(m_FreeCamera, voxelPos)) {
			m_World.VoxelSet(voxelPos, Lynx::Voxel::Type::Empty);
		}
	}

	return true;
}

SandBoxLayer::~SandBoxLayer()
{

}