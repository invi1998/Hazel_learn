#pragma once

#include "Hazel/Scene/Components.h"

namespace Hazel
{
	namespace Utils
	{
		inline b2BodyType ConvertToBox2DBodyType(Rigidbody2DComponent::BodyType type)
		{
			switch (type)
			{
				case Rigidbody2DComponent::BodyType::Static: return b2BodyType::b2_staticBody;
				case Rigidbody2DComponent::BodyType::Dynamic: return b2BodyType::b2_dynamicBody;
				case Rigidbody2DComponent::BodyType::Kinematic: return b2BodyType::b2_kinematicBody;
			}

			HZ_CORE_ASSERT(false, "Unknown Rigidbody2DComponent::Type!");
			return b2BodyType::b2_staticBody;
		}

		inline Rigidbody2DComponent::BodyType ConvertToHazelBodyType(b2BodyType type)
		{
			switch (type)
			{
				case b2BodyType::b2_staticBody: return Rigidbody2DComponent::BodyType::Static;
				case b2BodyType::b2_dynamicBody: return Rigidbody2DComponent::BodyType::Dynamic;
				case b2BodyType::b2_kinematicBody: return Rigidbody2DComponent::BodyType::Kinematic;
			}

			HZ_CORE_ASSERT(false, "Unknown b2BodyType!");
			return Rigidbody2DComponent::BodyType::Static;
		}
	}

}
