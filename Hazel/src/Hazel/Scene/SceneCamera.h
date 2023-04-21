#pragma once
#include "Hazel/Renderer/Camera.h"

namespace Hazel
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,	// 透视
			Orthographic = 1	// 正交
		};
	public:
		SceneCamera();
		~SceneCamera() override = default;

		void SetViewportSize(uint32_t width, uint32_t height);

		[[nodiscard]] auto GetProjectionType() const -> ProjectionType;
		void SetProjectionType(const ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		// 正交
		void SetOrthographic(float size, float nearClip, float farClip);

		[[nodiscard]] float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }

		[[nodiscard]] float GetOrthographicNearClip() const { return m_OrthographicNear; }
		void SetOrthographicNearClip(float clip) { m_OrthographicNear = clip; RecalculateProjection(); }

		[[nodiscard]] float GetOrthographicFarClip() const { return m_OrthographicFar; }
		void SetOrthographicFarClip(float clip) { m_OrthographicFar = clip; RecalculateProjection(); }

		// 透视
		void SetPerspective(float vertivlFOV, float nearClip, float farClip);

		[[nodiscard]] float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float fov) { m_PerspectiveFOV = fov; RecalculateProjection(); }

		[[nodiscard]] float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
		void SetPerspectiveNearClip(float clip) { m_PerspectiveNear = clip; RecalculateProjection(); }

		[[nodiscard]] float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
		void SetPerspectiveFarClip(float clip) { m_PerspectiveFar = clip; RecalculateProjection(); }

	private:
		void RecalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f;
		float m_OrthographicFar = 1.0f;

		float m_PerspectiveFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 1000.0f;

		float m_AspectRadio = 0.0f;
	};
}
