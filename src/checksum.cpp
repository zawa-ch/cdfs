//	zawa-ch/cdfs:/src/checksum
//	Copyright 2020 zawa-ch.
//
#include "cdfs/checksum.hpp"
using namespace zawa_ch::CDFS;

// TODO: 実装上手くいっていない可能性があるため、実装の再確認と修正

CRC32::Dataset CRC32::data;

CRC32::Dataset::Dataset()
{
	// CRC-32 (反転)
	// x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
	const uint32_t generator = 0xEDB88320;
	for(size_t i = 0; i < 256; i++)
	{
		auto c = uint32_t(i);
		for(size_t j = 0; j < 8; j++)
		{
			c = (c & 1) ? (generator ^ (c >> 1)) : (c >> 1);
		}
		table[i] = c;
	}
}

CRC32::CRC32() noexcept : curr(0xFFFFFFFF) {}
CRC32::CRC32(const uint32_t& init) noexcept : curr(init) {}
void CRC32::Push(const uint8_t& value) noexcept { curr = data.table[(curr ^ value) & 0xFF] ^ (curr >> 8); }
void CRC32::Push(const std::initializer_list<uint8_t>& list) noexcept { for (const auto& item: list) { Push(item); } }
void CRC32::Push(const uint8_t* begin, const uint8_t* end) noexcept { auto current = begin; while(current != end) { Push(*current); ++current; } }
uint32_t CRC32::GetValue() const noexcept { return curr ^ 0xFFFFFFFF; }
