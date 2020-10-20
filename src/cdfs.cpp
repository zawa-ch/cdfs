#include "cdfs/cdfs.hpp"
using namespace zawa_ch::CDFS;

static_assert(sizeof(CDFSFrame) == 256, "CDFSFrameの大きさが256バイトではありません。");
static_assert(sizeof(CDFSHEADFrame) == 256, "CDFSHEADFrameの大きさが256バイトではありません。");
static_assert(sizeof(CDFSFINFFrame) == 256, "CDFSFINFFrameの大きさが256バイトではありません。");
static_assert(sizeof(CDFSCONTFrame) == 256, "CDFSCONTFrameの大きさが256バイトではありません。");

uint32_t CDFS::GetLibraryVersion() noexcept
{
	return 0x00000100;
}
