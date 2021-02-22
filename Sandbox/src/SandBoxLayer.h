#pragma once
#include <Lynx.h>
#include <Lynx/Voxel/Chunk.h>

class SandBoxLayer : public Lynx::Layer {
public:
	SandBoxLayer();
	~SandBoxLayer();
	void OnUpdate(Lynx::TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnEvent(Lynx::Event& event) override;
	bool OnFrameResize(Lynx::WindowFrameResizeEvent& event);
	bool OnMousePressedButton(Lynx::MouseButtonPressedEvent& event);
private:
	Lynx::Shader m_VoxelShader;
	Lynx::Chunk m_Chunk;
	//Lynx::VoxelMachine m_VoxelMachine;

	glm::mat4 m_Transform = glm::mat4(1.0f);
	Lynx::FreeCamera m_FreeCamera{ 45.0f, 1080.0f / 720.0f, 0.1f, 1000.0f };

	float m_TimeStepStat = 0.0f;
	glm::ivec3 m_LookVoxel{ 0 };
};