#pragma once
#include <Lynx/Core/Layer.h>
#include <Lynx/Events/Event.h>
#include <Lynx/Render/Shader.h>

class SandBoxLayer : public Lynx::Layer {
public:
	SandBoxLayer();
	~SandBoxLayer();
	void OnUpdate() override;
	void OnEvent(Lynx::Event& event) override;
private:
	//Lynx::Shader m_Shader = Lynx::Shader("assets/shaders/testShader.glsl");
	uint32_t m_VAO;
	uint32_t m_VBO;
};