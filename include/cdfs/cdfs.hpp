//	cdfs/cdfs
//	Copyright 2020 zawa-ch.
//
#ifndef __cdfs_cdfs__
#define __cdfs_cdfs__
#include "datatype.hpp"
#include "checksum.hpp"
namespace zawa_ch::CDFS
{
	///	CDFSライブラリの情報を提供します。
	class CDFS final
	{
		CDFS() = delete;
		~CDFS() = delete;
	public:
		///	対応しているCDFSのバージョン。
		static constexpr uint32_t FormatVersion = 0x00000100;
		///	CDFSライブラリのバージョンを取得します。
		static uint32_t GetLibraryVersion() noexcept;
	};
}
#endif // __cdfs_cdfs__
