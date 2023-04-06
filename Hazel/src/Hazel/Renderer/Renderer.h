#pragma once

#include "OrthographicCamera.h"
#include "RenderCommand.h"

namespace Hazel
{
	class Shader;

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4{1.0f});

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void Init();

		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Shutdown();

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;

	};
}
