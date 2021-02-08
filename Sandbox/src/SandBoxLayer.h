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
	Lynx::Shader m_GeoShader;
	Lynx::Shader m_VoxelShader;

	Lynx::VertexArray m_CubeVA;
	Lynx::VertexArray m_MirrorVA;
	Lynx::VertexArray m_QuadVA;
	Lynx::VertexArray m_PointVA;
	Lynx::VertexArray m_VoxelVA;

	glm::mat4 m_Transform = glm::mat4(1.0f);

	Lynx::Texture m_CatTexture;
	Lynx::Texture m_CatTexture2;

	Lynx::FreeCamera m_FreeCamera{ 45.0f, 1080.0f / 720.0f, 0.1f, 1000.0f };
};