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

	float points[] = {
	-0.45f,  0.45f, //1.0f, 0.0f, 0.0f,
	 0.45f,  0.45f, //0.0f, 1.0f, 0.0f,
	 0.45f, -0.45f, //0.0f, 0.0f, 1.0f,
	-0.45f, -0.45f //1.0f, 1.0f, 0.0f
	};

	/*
	GLuint elements[] = {
		0, 1, 2,//front
		2, 3, 0,
	};
	*/

	m_Transform = glm::mat4(1.0f);

	m_View = glm::lookAt(
		glm::vec3(0.0f, 2.0f, 5.2f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	m_Projection = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);

	//Mesh renderer
	//-Mesh
	//-Material
	//--Shader

	///////////

	auto cubeVB = std::make_shared<Lynx::VertexBuffer>(cube, sizeof(cube));
	cubeVB->SetLayout({
		{ Lynx::ShaderDataType::Float3, "a_Position" },
		{ Lynx::ShaderDataType::Float2, "a_Texel" },
	});
	m_CubeVA.AddVertexBuffer(cubeVB);

	auto mirrorVB = std::make_shared<Lynx::VertexBuffer>(plane, sizeof(plane));
	mirrorVB->SetLayout({{ Lynx::ShaderDataType::Float3, "a_Position" }});
	m_MirrorVA.AddVertexBuffer(mirrorVB);

	auto pointVB = std::make_shared<Lynx::VertexBuffer>(points, sizeof(points));
	pointVB->SetLayout({{ Lynx::ShaderDataType::Float2, "a_Position" }});
	m_PointVA.AddVertexBuffer(pointVB);

	///////////

	m_CatTexture.Load("assets/textures/HUHhmm.png");
	m_CatTexture2.Load("assets/textures/cat.png");
	m_CatTexture.Bind(0);
	m_CatTexture2.Bind(1);

	m_FlatShader.Load("assets/shaders/flatColorShader.glsl");
	m_FlatShader.Bind();
	m_FlatShader.SetFloat4("u_Tint", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_FlatShader.SetMat4("u_Model", m_Transform);
	m_FlatShader.SetMat4("u_View", m_View);
	m_FlatShader.SetMat4("u_Projection", m_Projection);
	m_TextureShader.Load("assets/shaders/testShader.glsl");
	m_TextureShader.Bind();
	m_TextureShader.SetMat4("u_Model", m_Transform);
	m_TextureShader.SetMat4("u_View", m_View);
	m_TextureShader.SetMat4("u_Projection", m_Projection);
	m_TextureShader.SetInt("u_Texture", 0);
	m_TextureShader.SetInt("u_Texture2", 1);
	m_TextureShader.SetFloat4("u_Tint", glm::vec4(0.4f, 0.8f, 0.8f, 1.0f));
	m_PostShader.Load("assets/shaders/postShader.glsl");
	m_VoxelShader.Load("assets/shaders/voxelShader.glsl");
}

void SandBoxLayer::OnUpdate(Lynx::TimeStep timeStep)
{

	static float rot = 0;
	rot += timeStep.GetSeconds() * 25.0f;

	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::rotate(m_Transform, rot, glm::vec3(0.0f, 1.0f, 0.0f));
	m_TextureShader.SetMat4("u_Model", m_Transform);

	//glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	///////
	//Draw cube
	//glBindVertexArray(m_VAO);
	m_CubeVA.Bind();
	glEnable(GL_DEPTH_TEST);
	m_TextureShader.Bind();
	m_TextureShader.SetMat4("u_Model", m_Transform);
	m_CatTexture.Bind(0);
	m_CatTexture2.Bind(1);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Draw floor
	m_MirrorVA.Bind();
	m_FlatShader.Bind();
	m_FlatShader.SetMat4("u_Model", m_Transform);
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
	m_TextureShader.SetMat4("u_Model", m_Transform);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisable(GL_STENCIL_TEST);
	////////
	/*
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindVertexArray(m_VAO3);
	glDisable(GL_DEPTH_TEST);
	m_PostShader.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FrameBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	*/
	/////////
	//glBindVertexArray(m_VAO4);
	m_PointVA.Bind();
	m_VoxelShader.Bind();
	glDrawArrays(GL_POINTS, 0, 4);
}

void SandBoxLayer::OnEvent(Lynx::Event& event)
{
}

SandBoxLayer::~SandBoxLayer()
{
	//glDeleteBuffers(1, &m_VBO);
	//glDeleteBuffers(1, &m_VBO2);
	//glDeleteBuffers(1, &m_VBO3);
	//glDeleteBuffers(1, &m_VBO4);
	//glDeleteVertexArrays(1, &m_VAO);
	//glDeleteVertexArrays(1, &m_VAO2);
	//glDeleteVertexArrays(1, &m_VAO3);
	//glDeleteVertexArrays(1, &m_VAO4);
	//
	//glDeleteFramebuffers(1, &m_FrameBuffer);
	//glDeleteTextures(1, &m_ColorBuffer);
	//glDeleteRenderbuffers(1, &m_DepthStencilBuffer);
}
