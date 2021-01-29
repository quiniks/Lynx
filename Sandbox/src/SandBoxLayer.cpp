#include "SandBoxLayer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

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

	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	/*
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	*/
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glGenVertexArrays(1, &m_VAO2);
	glBindVertexArray(m_VAO2);

	glGenBuffers(1, &m_VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindVertexArray(m_VAO);

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
}

void SandBoxLayer::OnUpdate(Lynx::TimeStep timeStep)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float rot = 0;
	rot += timeStep.GetSeconds() * 25.0f;

	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::rotate(m_Transform, rot, glm::vec3(0.0f, 1.0f, 0.0f));
	m_TextureShader.SetMat4("u_Model", m_Transform);
	//Draw cube
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Draw floor
	glBindVertexArray(m_VAO2);
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
	glBindVertexArray(m_VAO);
	m_TextureShader.Bind();
	glStencilFunc(GL_EQUAL, 1, 0xff);
	glStencilMask(0x00);
	m_Transform = glm::scale(glm::translate(m_Transform, glm::vec3(0, -1, 0)), glm::vec3(1, -1, 1));
	m_TextureShader.SetMat4("u_Model", m_Transform);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisable(GL_STENCIL_TEST);

	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void SandBoxLayer::OnEvent(Lynx::Event& event)
{
}

SandBoxLayer::~SandBoxLayer()
{
	glDeleteBuffers(1, &m_VBO);
	//glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}
