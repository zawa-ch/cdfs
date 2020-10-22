#include "cdfs/builder.hpp"
using namespace zawa_ch::CDFS;

CDFSBuilder::CDFSBuilder() {}
CDFSBuilder::CDFSBuilder(const std::string& label) : label(label) {}

void CDFSBuilder::WriteHEADFrame(std::ostream& stream) { WriteHEADFrame(stream, frameindex + 1, datasize); }
void CDFSBuilder::WriteHEADFrame(std::ostream& stream, const UInt128& framecount, const UInt128& datasize)
{
	CDFSHEADFrame frame = CDFSHEADFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data_version() = CDFS::FormatVersion;
	frame.data_count() = framecount;
	{
		auto si = label.cbegin();
		auto di = frame.data_label().begin();
		auto se = label.cend();
		auto de = frame.data_label().end();
		while((si != se)&&(di != de)) { *(di++) = *(si++); }
	}
	frame.data_size() = datasize;
	frame.Validate();
	WriteToStream(stream, frame.Frame());
	++frameindex;
}
void CDFSBuilder::WriteFINFFrame(std::ostream& stream)
{
	CDFSFINFFrame frame = CDFSFINFFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data_count() = frameindex + 1;
	// TODO: チェックサムの仕様策定と実装
	// frame.data_hash;
	frame.data_size() = datasize;
	frame.Validate();
	WriteToStream(stream, frame.Frame());
	++frameindex;
}
void CDFSBuilder::WriteDATAFrame(std::ostream& stream, const ContainsType& data, const size_t& size)
{
	CDFSDATAFrame frame = CDFSDATAFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data() = data;
	frame.Validate();
	WriteToStream(stream, frame.Frame());
	++frameindex;
	datasize += size;
}
void CDFSBuilder::WriteCONTFrame(std::ostream& stream)
{
	CDFSCONTFrame frame = CDFSCONTFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data_current() = frameindex;
	{
		auto si = label.cbegin();
		auto di = frame.data_label().begin();
		auto se = label.cend();
		auto de = frame.data_label().end();
		while((si != se)&&(di != de)) { *(di++) = *(si++); }
	}
	frame.Validate();
	WriteToStream(stream, frame.Frame());
	++frameindex;
}

void CDFSBuilder::WriteToStream(std::ostream& stream, const CDFSFrame& frame)
{
	auto sentry = std::ostream::sentry(stream);
	if (bool(sentry))
	{
		stream.write((const std::ostream::char_type*)&frame, sizeof(CDFSFrame));
	}
}
