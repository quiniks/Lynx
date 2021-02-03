#pragma once
#include <Lynx.h>

class SandBoxLayer : public Lynx::Layer {
public:
	SandBoxLayer();
	~SandBoxLayer();
	void OnUpdate(Lynx::TimeStep timeStep) override;
	void OnEvent(Lynx::Event& event) override;
private:
	Lynx::Shader m_TextureShader;
	Lynx::Shader m_FlatShader;
	Lynx::Shader m_PostShader;
	Lynx::Shader m_VoxelShader;

	Lynx::VertexArray m_CubeVA;
	Lynx::VertexArray m_MirrorVA;
	Lynx::VertexArray m_QuadVA;
	Lynx::VertexArray m_PointVA;
	//uint32_t m_VAO;
	//uint32_t m_VAO2;
	//uint32_t m_VAO3;
	//uint32_t m_VAO4;
	//uint32_t m_VBO;
	//uint32_t m_VBO2;
	//uint32_t m_VBO3;
	//uint32_t m_VBO4;
	//uint32_t m_FrameBuffer;
	//uint32_t m_ColorBuffer;
	//uint32_t m_DepthStencilBuffer;
	glm::mat4 m_Transform;
	glm::mat4 m_View;
	glm::mat4 m_Projection;
	//uint32_t m_EBO;
	Lynx::Texture m_CatTexture;
	Lynx::Texture m_CatTexture2;
};