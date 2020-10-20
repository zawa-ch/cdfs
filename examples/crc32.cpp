#include <iostream>
#include <array>
#include "cdfs/cdfs.hpp"
using namespace zawa_ch::CDFS;

const std::array<uint8_t, 256> data =
{
	0x00, 0x18, 0x24, 0x3F, 0xFF, 0xC3, 0x21, 0xFF
};

int main(int argc, char const *argv[])
{
	auto crc32gen = CRC32();
	crc32gen.Push(data);
	auto crc = crc32gen.GetValue();
	std::cout << std::hex << crc << std::endl;
	return 0;
}
