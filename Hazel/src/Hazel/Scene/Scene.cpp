#include "hzpch.h"

#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include "ScriptableEntity.h"

#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"


namespace Hazel
{
	Scene::Scene()
	{
		// m_Registry.on_construct<CameraComponent>().connect<&Function>();
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		const auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (const auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			// Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, static_cast<int>(entity));
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// update scripts
		{
			m_Registry.view<NativeScriptComponent>().each(
				[=](auto entity, auto &nsc)
				{
					// TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateFunction();
						nsc.Instance->m_Entity = Entity{entity, this};
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
		}

		// Render 2D
		Camera *mainCamera = nullptr;
		glm::mat4 cameraTransform;

		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			const auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (const auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				// Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color, static_cast<int>(entity));
				Renderer2D::DrawSprite(transform.GetTransform(), sprite, static_cast<int>(entity));
			}

			Renderer2D::EndScene();
		}
	}

	Entity Scene::CreateEntity(const std::string &name)
	{
		return CreateEntityWithUUID(name, UUID());
	}

	Entity Scene::CreateEntityWithUUID(const std::string& name, UUID uuid)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto &tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

		return entity;
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio Cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto &cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::DestroyEntity(Entity entity)
	{
		// 我们这里传递进来的entity并不是一个entity实体，而只是一个Entity抽象,
		// destroy需要一个entity实体，这里当然可以在Entity里写一个get函数
		// 当然我们也可以重写 entity 运算符，如下
		// operator entt::entity() const { return m_EntityHandle; }
		m_Registry.destroy(entity);
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		auto view = m_Registry.view<IDComponent>();
		for (auto entity : view)
		{
			const auto& id = view.get<IDComponent>(entity);
			if (id.ID == uuid)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
			{
				return Entity{ entity, this };
			}
		}
		return {};
	}

	template <typename T>
	void Scene::OnComponentAdded(Entity entity, T &component)
	{
		static_assert(sizeof(T) == 0);
	}

	template <>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent &component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template <>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent &component)
	{
	}

	template <>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent &component)
	{
	}
}
