#pragma once
#include <Lynx/Core/Layer.h>
#include <Lynx/Events/Event.h>
#include <Lynx/Render/Shader.h>
#include <Lynx/Render/Texture.h>
#include <Lynx/Core/TimeStep.h>

class SandBoxLayer : public Lynx::Layer {
public:
	SandBoxLayer();
	~SandBoxLayer();
	void OnUpdate(Lynx::TimeStep timeStep) override;
	void OnEvent(Lynx::Event& event) override;
private:
	Lynx::Shader m_TextureShader;
	Lynx::Shader m_FlatShader;
	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_VAO2;
	uint32_t m_VBO2;
	glm::mat4 m_Transform;
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	//uint32_t m_EBO;
	Lynx::Texture m_CatTexture;
	Lynx::Texture m_CatTexture2;
};