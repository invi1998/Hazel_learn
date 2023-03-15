#pragma once

namespace Hazel
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSource, const std::string& fragmentSource);

		~Shader();

		void Bind() const;
		void UnBind() const;

	private:
		uint32_t m_RenderID;
	};

}
