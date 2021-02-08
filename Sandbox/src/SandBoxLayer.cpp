#include "SandBoxLayer.h"
#include <glad/glad.h>
#pragma warning( push )
#pragma warning ( disable : 26812 )
#include <glm/gtc/matrix_transform.hpp>
#pragma warning( pop )

SandBoxLayer::SandBoxLayer()
{
	float cube[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
										
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
										
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
										
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
										
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
										
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	};

	float plane[] = {
		-1.0f, -0.5f, -1.0f,
		 1.0f, -0.5f, -1.0f,
		 1.0f, -0.5f,  1.0f,
		 1.0f, -0.5f,  1.0f,
		-1.0f, -0.5f,  1.0f,
		-1.0f, -0.5f, -1.0f
	};

	float quad[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	struct PointVertex {
		glm::vec2 Pos;
		int a;
	};

	PointVertex points[] = {
		{{-0.10f,  0.45f}, 1 },
		{{-0.5f,  0.45f}, 2 },
		{{0.5f, -0.45f}, 4 },
		{{0.10f, -0.45f}, 8}
	};

	struct VoxelVertex {
		glm::vec3 Pos;
		glm::vec3 Color;
		int Sides;
	};

	VoxelVertex Voxels[] = {
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.4f, 0.7f}, 63},
		{{2.0f, 0.0f, 0.0f}, {0.9f, 0.9f, 0.6f}, 63},
		{{3.0f, 0.0f, 0.0f}, {0.4f, 0.8f, 0.8f}, 63}
	};

	auto voxelVB = std::make_shared<Lynx::VertexBuffer>(Voxels, sizeof(Voxels));
	voxelVB->SetLayout({
		{ Lynx::ShaderDataType::Float3, "a_Position" },
		{ Lynx::ShaderDataType::Float3, "a_Color" },
		{ Lynx::ShaderDataType::Int, "a_EnabledSides" }
	});
	m_VoxelVA.AddVertexBuffer(voxelVB);

	///////////
	auto cubeVB = std::make_shared<Lynx::VertexBuffer>(cube, sizeof(cube));
	cubeVB->SetLayout({
		{ Lynx::ShaderDataType::Float3, "a_Position" },
		{ Lynx::ShaderDataType::Float2, "a_Texel" }
	});
	m_CubeVA.AddVertexBuffer(cubeVB);

	auto mirrorVB = std::make_shared<Lynx::VertexBuffer>(plane, sizeof(plane));
	mirrorVB->SetLayout({ { Lynx::ShaderDataType::Float3, "a_Position" } });
	m_MirrorVA.AddVertexBuffer(mirrorVB);

	auto pointVB = std::make_shared<Lynx::VertexBuffer>(points, sizeof(points));
	pointVB->SetLayout({
		{ Lynx::ShaderDataType::Float2, "a_Position" },
		{ Lynx::ShaderDataType::Int, "a_Test" }
	});
	m_PointVA.AddVertexBuffer(pointVB);

	///////////

	m_CatTexture.Load("assets/textures/HUHhmm.png");
	m_CatTexture2.Load("assets/textures/cat.png");
	m_CatTexture.Bind(0);
	m_CatTexture2.Bind(1);

	m_FlatShader.Load("assets/shaders/flatColorShader.glsl");
	m_FlatShader.Bind();
	m_FlatShader.SetFloat4("u_Tint", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_FlatShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());
	m_TextureShader.Load("assets/shaders/testShader.glsl");
	m_TextureShader.Bind();
	m_TextureShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());
	m_TextureShader.SetInt("u_Texture", 0);
	m_TextureShader.SetInt("u_Texture2", 1);
	m_TextureShader.SetFloat4("u_Tint", glm::vec4(0.4f, 0.8f, 0.8f, 1.0f));
	m_PostShader.Load("assets/shaders/postShader.glsl");
	m_GeoShader.Load("assets/shaders/geoShader.glsl");
	m_VoxelShader.Load("assets/shaders/voxelShader.glsl");
	m_VoxelShader.Bind();
	m_VoxelShader.SetMat4("u_MVP", m_FreeCamera.GetViewProjection());
	m_VoxelShader.UploadUniformFloat("u_VoxelSize", 0.5f);
	m_VoxelShader.SetFloat3("u_CameraPosition", m_FreeCamera.GetPosition());

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//Lynx::App::Get().GetWindow().SetVSync(false);
}

void SandBoxLayer::OnUpdate(Lynx::TimeStep timeStep)
{
	m_FreeCamera.OnUpdate(timeStep);

	static float rot = 0;
	rot += timeStep.GetSeconds();

	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::rotate(m_Transform, rot, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 vp = m_FreeCamera.GetViewProjection();
	glm::mat4 mvp = m_FreeCamera.GetViewProjection() * m_Transform;

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	///////
	m_VoxelVA.Bind();
	m_VoxelShader.Bind();
	m_VoxelShader.SetMat4("u_MVP", vp);
	m_VoxelShader.SetFloat3("u_CameraPosition", m_FreeCamera.GetPosition());
	glDrawArrays(GL_POINTS, 0, 3);

	LX_CORE_INFO("Elapsed Time: {0}s", timeStep);
	//LX_CORE_INFO("cam pos: {0}, {1}, {2}", m_FreeCamera.GetPosition().x, m_FreeCamera.GetPosition().y, m_FreeCamera.GetPosition().z);

	/*
	//Draw cube
	m_CubeVA.Bind();
	glEnable(GL_DEPTH_TEST);
	m_TextureShader.Bind();
	m_TextureShader.SetMat4("u_MVP", mvp);
	m_CatTexture.Bind(0);
	m_CatTexture2.Bind(1);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Draw floor
	m_MirrorVA.Bind();
	m_FlatShader.Bind();
	m_FlatShader.SetMat4("u_MVP", mvp);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xff);
	glDepthMask(GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDepthMask(GL_TRUE);

	//Draw reflection
	m_CubeVA.Bind();
	m_TextureShader.Bind();
	glStencilFunc(GL_EQUAL, 1, 0xff);
	glStencilMask(0x00);
	m_Transform = glm::scale(glm::translate(m_Transform, glm::vec3(0, -1, 0)), glm::vec3(1, -1, 1));
	mvp = m_FreeCamera.GetViewProjection() * m_Transform;
	m_TextureShader.SetMat4("u_MVP", mvp);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisable(GL_STENCIL_TEST);

	/////////
	m_PointVA.Bind();
	m_GeoShader.Bind();
	glDrawArrays(GL_POINTS, 0, 4);
	*/
}

void SandBoxLayer::OnEvent(Lynx::Event& event)
{
}

SandBoxLayer::~SandBoxLayer()
{
}
