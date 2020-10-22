#include "cdfs/loader.hpp"
using namespace zawa_ch::CDFS;

CDFSLoader::CDFSLoader()
	: buffer(), label(), framecount(), frameindex(), datasize(), dataindex(), readhead(), readfinf(), fault()
{}

bool CDFSLoader::HasHEAD() const { return readhead; }
bool CDFSLoader::HasFINF() const { return readfinf; }
const UInt128& CDFSLoader::FrameIndex() const { return frameindex; }
const UInt128& CDFSLoader::FrameCount() const { return framecount; }
const UInt128& CDFSLoader::DataIndex() const { return dataindex; }
const UInt128& CDFSLoader::DataSize() const { return datasize; }
bool CDFSLoader::ReadNext(std::istream& stream)
{
	if (readfinf) { return false; }
	if (buffer.has_value())
	{
		++frameindex;
	}
	buffer = ReadFrameFromStream(stream);
	if (!buffer.has_value()) { return false; }
	if (!IsValidData())
	{
		fault = true;
		return buffer.has_value();
	}
	if ((!readhead)&&(CDFSHEADFrame::IsHEADFrame(*buffer)))
	{
		auto header = CDFSHEADFrame(*buffer);
		if (IsVersionCompatible(header))
		{
			label = std::string(header.data_label().data());
			framecount = header.data_count();
			datasize = header.data_size();
			readhead = true;
		}
	}
	if ((readhead)&&(!readfinf)&&(CDFSFINFFrame::IsFINFFrame(*buffer)))
	{
		auto finf = CDFSFINFFrame(*buffer);
		if (framecount == 0) { framecount = finf.data_count(); }
		if (datasize == 0) { datasize = finf.data_size(); }
		readfinf = true;
	}
	if ((readhead)&&(!readfinf)&&(CDFSDATAFrame::IsDATAFrame(*buffer)))
	{
		dataindex += 240U;
	}
	return true;
}
bool CDFSLoader::HasValue() const noexcept { return buffer.has_value(); }
bool CDFSLoader::IsValidData() const noexcept
{
	if(!HasValue()) { return false; }
	return buffer->IsValid()&&VerifySequence(*buffer, uint64_t(frameindex));
}
std::vector<uint8_t> CDFSLoader::GetData(const size_t& size) const
{
	if (!HasValue()) { return std::vector<uint8_t>(); }
	if (CDFSDATAFrame::IsDATAFrame(*buffer))
	{
		auto result = CDFSDATAFrame(*buffer);
		if ((result.data().cbegin() + size) <= (result.data().cend()))
		{
			return std::vector<uint8_t>(result.data().cbegin(), result.data().cbegin() + size);
		}
		else
		{
			return std::vector<uint8_t>(result.data().cbegin(), result.data().cend());
		}
	}
	return std::vector<uint8_t>();
}
const std::optional<CDFSFrame>& CDFSLoader::GetFrame() const { return buffer; }
std::optional<bool> CDFSLoader::CheckIntegrity() const
{
	if (!readfinf) { return std::nullopt; }
	return (!fault)&&((frameindex+1) == framecount);
}

std::optional<CDFSFrame> CDFSLoader::ReadFrameFromStream(std::istream& stream)
{
	if (stream.good())
	{
		auto result = CDFSFrame();
		stream.read((std::istream::char_type*)&result, sizeof(CDFSFrame));
		if (stream.gcount() == sizeof(CDFSFrame))
		{ return result; }
		else
		{ return std::nullopt; }
	}
	return std::nullopt;
}
bool CDFSLoader::IsVersionCompatible(const CDFSHEADFrame& frame)
{
	return frame.data_version() <= CDFS::FormatVersion;
}
bool CDFSLoader::VerifySequence(const CDFSFrame& frame, const uint64_t& seq)
{
	return frame.sequence == seq;
}
