#include <iostream>
#include "cdfs/cdfs.hpp"
using namespace zawa_ch::CDFS;

static_assert(sizeof(UInt128) == 16, "UInt128の大きさが16バイトではありません。");

int main(int argc, char const *argv[])
{
	auto max = UInt128(std::numeric_limits<uintmax_t>::max());
	auto p64 = UInt128(1) << 64;

	if (UInt128(100) < UInt128(200)) { std::cout << "100 < 200 OK" << std::endl; }
	else { throw std::exception(); }

	if (p64 > max) { std::cout << "2^64 > uintmax_t.max OK" << std::endl; }
	else { throw std::exception(); }

	if (UInt128(100) + UInt128(100) == UInt128(200)) { std::cout << "100 + 100 = 200 OK" << std::endl; }
	else { throw std::exception(); }

	if (UInt128(100) - UInt128(50) == UInt128(50)) { std::cout << "100 - 50 = 50 OK" << std::endl; }
	else { throw std::exception(); }

	if (UInt128(100) * UInt128(50) == UInt128(5000)) { std::cout << "100 * 50 = 5000 OK" << std::endl; }
	else { throw std::exception(); }

	if ((max + UInt128(1)) == (UInt128(1) << (sizeof(uintmax_t) * 8))) { std::cout << "uintmax_t.max + 1 = 1 << (sizeof(uintmax_t)*8) OK" << std::endl; }
	else { throw std::exception(); }

	if ((max + max) == (max * 2)) { std::cout << "uintmax_t.max * uintmax_t.max = uintmax_t.max * 2 OK" << std::endl; }
	else { throw std::exception(); }

	if ((p64 / 3) == 6148914691236517205U) { std::cout << "(2^64) / 3 = 6148914691236517205 OK" << std::endl; }
	else { throw std::exception(); }

	if ((p64 % 3) == 1U) { std::cout << "(2^64) % 3 = 1 OK" << std::endl; }
	else { throw std::exception(); }

	return 0;
}

