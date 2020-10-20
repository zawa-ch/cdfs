#ifndef __cdfs_checksum__
#define __cdfs_checksum__
#include <cstdint>
#include <array>
namespace zawa_ch::CDFS
{
	class CRC32
	{
	private:
		struct Dataset final
		{
		public:
			std::array<uint32_t, 256> table;
			Dataset();
		};
		static Dataset data;

		uint32_t curr;

	public:
		CRC32() noexcept;
		CRC32(const uint32_t& init) noexcept;

		void Push(const uint8_t& value) noexcept;
		void Push(const std::initializer_list<uint8_t>& list) noexcept;
		void Push(const uint8_t* begin, const uint8_t* end) noexcept;
		template<size_t length>
		void Push(const std::array<uint8_t, length>& array) noexcept { for(const auto& item: array) { Push(item); } }
		uint32_t GetValue() const noexcept;
	};
}
#endif // __cdfs_checksum__