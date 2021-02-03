#pragma once
#pragma warning( push )
#pragma warning ( disable : 5033 )
#include <glm/glm.hpp>
#pragma warning( pop )
#include <unordered_map>

namespace Lynx {
	struct UniformInfo
	{
		int32_t location;
		int32_t size;
	};

	class Shader {
	public:
		Shader() = default;
		virtual ~Shader();
		void Load(const std::string& filePath);
		void Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		void Load(const std::string& name, const std::string& vertexSrc, const std::string& geometrySrc, const std::string& fragmentSrc);

		void Bind() const;
		void Unbind() const;

		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);

		uint32_t GetProg() const { return m_ShaderID; }
		const std::string& GetName() const { return m_Name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);
	private:
		uint32_t m_ShaderID = 0;
		std::string m_Name;
		std::unordered_map<std::string, UniformInfo> m_Uniforms;
	};
}