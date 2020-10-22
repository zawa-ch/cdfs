#include "cdfs/builder.hpp"
using namespace zawa_ch::CDFS;

CDFSBuilder::CDFSBuilder() : label(), frameindex(), datasize(), wrotehead(), wrotefinf() {}
CDFSBuilder::CDFSBuilder(const std::string& label) : label(label), frameindex(), datasize(), wrotehead(), wrotefinf() {}

const std::string& CDFSBuilder::Label() const { return label; }
const UInt128& CDFSBuilder::FrameIndex() const { return frameindex; }
const UInt128& CDFSBuilder::DataSize() const { return datasize; }
void CDFSBuilder::WriteHEADFrame(std::ostream& stream) { WriteHEADFrame(stream, frameindex + 1, datasize); }
void CDFSBuilder::WriteHEADFrame(std::ostream& stream, const UInt128& framecount, const UInt128& datasize)
{
	CDFSHEADFrame frame = CDFSHEADFrame();
	frame.sequence() = 0U;
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
	wrotehead = true;
	if ((wrotehead)&&(!wrotefinf)) { ++frameindex; }
}
void CDFSBuilder::WriteFINFFrame(std::ostream& stream)
{
	if ((!wrotehead)||(wrotefinf)) { return; }
	CDFSFINFFrame frame = CDFSFINFFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data_count() = frameindex + 1;
	// TODO: チェックサムの仕様策定と実装
	// frame.data_hash;
	frame.data_size() = datasize;
	frame.Validate();
	WriteToStream(stream, frame.Frame());
	wrotefinf = true;
}
void CDFSBuilder::WriteDATAFrame(std::ostream& stream, const ContainsType& data, const size_t& size)
{
	if ((!wrotehead)||(wrotefinf)) { return; }
	CDFSDATAFrame frame = CDFSDATAFrame();
	frame.sequence() = uint64_t(frameindex);
	frame.data() = data;
	frame.Validate();
	WriteToStream(stream, frame.Frame());
	if ((wrotehead)&&(!wrotefinf))
	{
		++frameindex;
		datasize += size;
	}
}
void CDFSBuilder::WriteCONTFrame(std::ostream& stream)
{
	if ((!wrotehead)||(wrotefinf)) { return; }
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
	if ((wrotehead)&&(!wrotefinf)) { ++frameindex; }
}

void CDFSBuilder::WriteToStream(std::ostream& stream, const CDFSFrame& frame)
{
	auto sentry = std::ostream::sentry(stream);
	if (bool(sentry))
	{
		stream.write((const std::ostream::char_type*)&frame, sizeof(CDFSFrame));
	}
}
