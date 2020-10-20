#ifndef __cdfs_cdfs__
#define __cdfs_cdfs__
#include "datatype.hpp"
#include "checksum.hpp"
namespace zawa_ch::CDFS
{
	class CDFS final
	{
		CDFS() = delete;
		~CDFS() = delete;
	public:
		static constexpr uint32_t FormatVersion = 0x00000100;
		static uint32_t GetLibraryVersion() noexcept;
	};
}
#endif // __cdfs_cdfs__
