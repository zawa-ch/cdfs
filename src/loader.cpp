#include "cdfs/loader.hpp"
using namespace zawa_ch::CDFS;

CDFSLoader::CDFSLoader()
	: buffer(), label(), framecount(), frameindex(), datasize(), dataindex(), readhead(), readfinf(), fault()
{}

bool CDFSLoader::HasHEAD() const { return readhead; }
bool CDFSLoader::HasFINF() const { return readfinf; }
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
	return true;
}
bool CDFSLoader::HasValue() const noexcept { return buffer.has_value(); }
bool CDFSLoader::IsValidData() const noexcept
{
	if(!HasValue()) { return false; }
	return buffer->IsValid()&&VerifySequence(*buffer, uint64_t(frameindex));
}
std::vector<uint8_t> CDFSLoader::GetData() const
{
	if (!HasValue()) { return std::vector<uint8_t>(); }
	if (CDFSDATAFrame::IsDATAFrame(*buffer))
	{
		auto result = CDFSDATAFrame(*buffer);
		return std::vector<uint8_t>(result.data().begin(), result.data().end());
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
	auto sentry = std::istream::sentry(stream);
	if (bool(sentry))
	{
		CDFSFrame result;
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
