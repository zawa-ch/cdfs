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
		static const CDFSFrame& CastToFrame(const CDFSHEADFrame& frame);
		static CDFSFrame& CastToFrame(CDFSHEADFrame& frame);
		static const CDFSFrame& CastToFrame(const CDFSFINFFrame& frame);
		static CDFSFrame& CastToFrame(CDFSFINFFrame& frame);
		static const CDFSFrame& CastToFrame(const CDFSCONTFrame& frame);
		static CDFSFrame& CastToFrame(CDFSCONTFrame& frame);
	};
}
#endif // __cdfs_cdfs__
