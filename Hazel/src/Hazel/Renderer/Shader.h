#pragma once
#include <glm/fwd.hpp>

namespace Hazel
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);

		~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RenderID;
	};

}
