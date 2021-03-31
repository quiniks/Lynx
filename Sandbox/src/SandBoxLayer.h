#pragma once
#include <Lynx.h>
#include <Lynx/Voxel/World.h>
#include "Lynx/Render/Grid.h"

class SandBoxLayer : public Lynx::Layer {
public:
	SandBoxLayer();
	~SandBoxLayer();
	void OnUpdate(Lynx::TimeStep timeStep) override;
	void OnImGuiRender() override;
	void OnEvent(Lynx::Event& event) override;
	bool OnFrameResize(Lynx::WindowFrameResizeEvent& event);
	bool OnMousePressedButton(Lynx::MouseButtonPressedEvent& event);
	bool OnKeyPressed(Lynx::KeyPressedEvent& event);
private:
	Lynx::Shader m_VoxelShader;
	Lynx::Shader m_ColorShader;
	Lynx::World m_World;
	Lynx::Grid m_Grid;

	glm::mat4 m_Transform = glm::mat4(1.0f);

	Lynx::FreeCamera m_FreeCamera;
	float m_TimeStepStat = 0.0f;
	glm::ivec3 m_LookVoxel{ 0 };

	bool m_RenderMode = false;
};