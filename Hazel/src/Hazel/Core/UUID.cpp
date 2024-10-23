#include "hzpch.h"
#include "UUID.h"

#include <random>
#include <unordered_map>

namespace Hazel
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID()
		: m_UUID(s_UniformDistribution(s_Engine))
	{
		HZ_CORE_WARN("UUID: {0}", m_UUID);
	}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{
	}

}
