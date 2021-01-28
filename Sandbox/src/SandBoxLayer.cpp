#include "SandBoxLayer.h"
#include <glad/glad.h>

SandBoxLayer::SandBoxLayer()
{
	float vertices[] = {
		 0.0f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f 
	};

	//Mesh renderer
	//-Mesh
	//-Material
	//--Shader

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);


	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//m_Shader.Bind();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

}

void SandBoxLayer::OnUpdate()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//m_Shader.SetFloat3("triangleColor", glm::vec3(1.0f, 0.0f, 0.0f));
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void SandBoxLayer::OnEvent(Lynx::Event& event)
{
}

SandBoxLayer::~SandBoxLayer()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}
