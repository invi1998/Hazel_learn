#include "hzpch.h"

#include "Scene.h"

#include "Components.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Scene/Entity.h"

namespace Hazel
{
	Scene::Scene()
	{
#if ENIT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		if (m_Registry.any_of<TransformComponent>(entity))
		{
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity);
		}

		auto view = m_Registry.view<TransformComponent>();
		for (auto eti : view)
		{
			
			TransformComponent& transform = view.get<TransformComponent>(eti);;
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto eti : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(eti);

			// Renderer::Submit(mesh, transform);
		}
#endif
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{

		// Render 2D sprites
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;

		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity: group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.Color);
			}

			Renderer2D::EndScene();
		}

		
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}
}
