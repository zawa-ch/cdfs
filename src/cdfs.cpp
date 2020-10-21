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
const CDFSFrame& CDFS::CastToFrame(const CDFSHEADFrame& frame) { return reinterpret_cast<const CDFSFrame&>(frame); }
CDFSFrame& CDFS::CastToFrame(CDFSHEADFrame& frame) { return reinterpret_cast<CDFSFrame&>(frame); }
const CDFSFrame& CDFS::CastToFrame(const CDFSFINFFrame& frame) { return reinterpret_cast<const CDFSFrame&>(frame); }
CDFSFrame& CDFS::CastToFrame(CDFSFINFFrame& frame) { return reinterpret_cast<CDFSFrame&>(frame); }
const CDFSFrame& CDFS::CastToFrame(const CDFSCONTFrame& frame) { return reinterpret_cast<const CDFSFrame&>(frame); }
CDFSFrame& CDFS::CastToFrame(CDFSCONTFrame& frame) { return reinterpret_cast<CDFSFrame&>(frame); }
