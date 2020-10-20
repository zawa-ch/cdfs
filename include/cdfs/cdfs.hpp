#ifndef __cdfs_cdfs__
#define __cdfs_cdfs__
#include <cstdint>
#include <array>
#include "datatype.hpp"
namespace zawa_ch::CDFS
{
	struct CDFSFrame;
	struct CDFSHEADFrame;
	struct CDFSFINFFrame;
	struct CDFSCONTFrame;

	struct CDFSFrame final
	{
		uint64_t seq;
		CDFSFrameTypes frametype;
		std::array<uint8_t, 240> data;
		uint32_t checksum;
	};

	struct CDFSHEADFrame final
	{
		uint64_t seq;
		CDFSFrameTypes frametype;
		uint32_t formatver;
		UInt128 count;
		std::array<char, 32> label;
		UInt128 size;
		std::array<uint8_t, 172> _res;
		uint32_t checksum;
	};

	struct CDFSFINFFrame final
	{
		uint64_t seq;
		CDFSFrameTypes frametype;
		std::array<uint8_t, 4> _res1;
		UInt128 count;
		std::array<uint8_t, 32> hash;
		UInt128 size;
		std::array<uint8_t, 172> _res2;
		uint32_t checksum;
	};

	struct CDFSCONTFrame final
	{
		uint64_t seq;
		CDFSFrameTypes frametype;
		std::array<uint8_t, 4> _res1;
		UInt128 current;
		std::array<uint8_t, 32> hash;
		std::array<uint8_t, 188> _res2;
		uint32_t checksum;
	};

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
#endif // __cdfs_cdfs__