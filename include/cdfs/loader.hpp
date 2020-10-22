#ifndef __cdfs_loader__
#define __cdfs_loader__
#include <array>
#include <vector>
#include <optional>
#include <iostream>
#include "cdfs.hpp"
namespace zawa_ch::CDFS
{
	class CDFSLoader
	{
	private:
		std::optional<CDFSFrame> buffer;
		std::string label;
		UInt128 framecount;
		UInt128 frameindex;
		UInt128 datasize;
		UInt128 dataindex;
		bool readhead;
		bool readfinf;
		bool fault;
	public:
		CDFSLoader();

		bool HasHEAD() const;
		bool HasFINF() const;
		bool ReadNext(std::istream& stream);
		bool HasValue() const noexcept;
		bool IsValidData() const noexcept;
		std::vector<uint8_t> GetData() const;
		const std::optional<CDFSFrame>& GetFrame() const;
		std::optional<bool> CheckIntegrity() const;

		static std::optional<CDFSFrame> ReadFrameFromStream(std::istream& stream);
		static bool IsVersionCompatible(const CDFSHEADFrame& frame);
		static bool VerifySequence(const CDFSFrame& frame, const uint64_t& seq);
	};
}
#endif // __cdfs_loader__
