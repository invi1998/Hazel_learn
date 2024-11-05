#pragma once

namespace Hazel
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template <typename T> struct hash;

	template<>
	struct hash<Hazel::UUID>
	{
		// 这是一个哈希函数，它将 UUID 转换为 std::size_t
		std::size_t operator()(const Hazel::UUID& uuid) const noexcept
		{
			return (uint64_t)uuid;
		}
	};
}


