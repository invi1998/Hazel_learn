#pragma once
#include <glm/fwd.hpp>

namespace Hazel
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);

	};

}
