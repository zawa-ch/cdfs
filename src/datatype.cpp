#include "cdfs/datatype.hpp"
using namespace zawa_ch::CDFS;

void CDFSFrame::Validate()
{
	auto calculator = CRC32();
	calculator.Push((const uint8_t*)this, (const uint8_t*)&checksum);
	checksum = calculator.GetValue();
}
bool CDFSFrame::IsValid() const
{
	auto calculator = CRC32();
	calculator.Push((const uint8_t*)this, (const uint8_t*)&checksum);
	return checksum == calculator.GetValue();
}

CDFSHEADFrame::CDFSHEADFrame() : frame()
{
	frame.frametype = CDFSFrameTypes::HEAD;
}
CDFSHEADFrame::CDFSHEADFrame(const CDFSFrame& frame) : frame(frame)
{
	if (!IsHEADFrame(this->frame)) { throw std::exception(); }
}
CDFSHEADFrame::CDFSHEADFrame(CDFSFrame&& frame) : frame(frame)
{
	if (!IsHEADFrame(this->frame)) { throw std::exception(); }
}
const CDFSFrame& CDFSHEADFrame::Frame() const { return frame; }
uint64_t& CDFSHEADFrame::sequence() { return frame.sequence; }
const uint64_t& CDFSHEADFrame::sequence() const { return frame.sequence; }
uint32_t& CDFSHEADFrame::data_version() { return reinterpret_cast<uint32_t&>(frame.data[0]); }
const uint32_t& CDFSHEADFrame::data_version() const { return reinterpret_cast<const uint32_t&>(frame.data[0]); }
UInt128& CDFSHEADFrame::data_count() { return reinterpret_cast<UInt128&>(frame.data[4]); }
const UInt128& CDFSHEADFrame::data_count() const { return reinterpret_cast<const UInt128&>(frame.data[4]); }
std::array<char, 32>& CDFSHEADFrame::data_label() { return reinterpret_cast<std::array<char, 32>&>(frame.data[20]); }
const std::array<char, 32>& CDFSHEADFrame::data_label() const { return reinterpret_cast<const std::array<char, 32>&>(frame.data[20]); }
UInt128& CDFSHEADFrame::data_size() { return reinterpret_cast<UInt128&>(frame.data[52]); }
const UInt128& CDFSHEADFrame::data_size() const { return reinterpret_cast<const UInt128&>(frame.data[52]); }
void CDFSHEADFrame::Validate() { frame.Validate(); }
bool CDFSHEADFrame::IsValid() const { return frame.IsValid(); }
bool CDFSHEADFrame::IsHEADFrame(const CDFSFrame& frame) { return frame.frametype == CDFSFrameTypes::HEAD; }

CDFSFINFFrame::CDFSFINFFrame() : frame()
{
	frame.frametype = CDFSFrameTypes::FINF;
}
CDFSFINFFrame::CDFSFINFFrame(const CDFSFrame& frame) : frame(frame)
{
	if (!IsFINFFrame(this->frame)) { throw std::exception(); }
}
CDFSFINFFrame::CDFSFINFFrame(CDFSFrame&& frame) : frame(frame)
{
	if (!IsFINFFrame(this->frame)) { throw std::exception(); }
}
const CDFSFrame& CDFSFINFFrame::Frame() const { return frame; }
uint64_t& CDFSFINFFrame::sequence() { return frame.sequence; }
const uint64_t& CDFSFINFFrame::sequence() const { return frame.sequence; }
UInt128& CDFSFINFFrame::data_count() { return reinterpret_cast<UInt128&>(frame.data[4]); }
const UInt128& CDFSFINFFrame::data_count() const { return reinterpret_cast<const UInt128&>(frame.data[4]); }
std::array<uint8_t, 32>& CDFSFINFFrame::data_hash() { return reinterpret_cast<std::array<uint8_t, 32>&>(frame.data[20]); }
const std::array<uint8_t, 32>& CDFSFINFFrame::data_hash() const { return reinterpret_cast<const std::array<uint8_t, 32>&>(frame.data[20]); }
UInt128& CDFSFINFFrame::data_size() { return reinterpret_cast<UInt128&>(frame.data[52]); }
const UInt128& CDFSFINFFrame::data_size() const { return reinterpret_cast<const UInt128&>(frame.data[52]); }
void CDFSFINFFrame::Validate() { frame.Validate(); }
bool CDFSFINFFrame::IsValid() const { return frame.IsValid(); }
bool CDFSFINFFrame::IsFINFFrame(const CDFSFrame& frame) { return frame.frametype == CDFSFrameTypes::FINF; }

CDFSDATAFrame::CDFSDATAFrame() : frame()
{
	frame.frametype = CDFSFrameTypes::DATA;
}
CDFSDATAFrame::CDFSDATAFrame(const CDFSFrame& frame) : frame(frame)
{
	if (!IsDATAFrame(this->frame)) { throw std::exception(); }
}
CDFSDATAFrame::CDFSDATAFrame(CDFSFrame&& frame) : frame(frame)
{
	if (!IsDATAFrame(this->frame)) { throw std::exception(); }
}
const CDFSFrame& CDFSDATAFrame::Frame() const { return frame; }
uint64_t& CDFSDATAFrame::sequence() { return frame.sequence; }
const uint64_t& CDFSDATAFrame::sequence() const { return frame.sequence; }
std::array<uint8_t, 240>& CDFSDATAFrame::data() { return frame.data; }
const std::array<uint8_t, 240>& CDFSDATAFrame::data() const { return frame.data; }
void CDFSDATAFrame::Validate() { frame.Validate(); }
bool CDFSDATAFrame::IsValid() const { return frame.IsValid(); }
bool CDFSDATAFrame::IsDATAFrame(const CDFSFrame& frame) { return frame.frametype == CDFSFrameTypes::DATA; }

CDFSCONTFrame::CDFSCONTFrame() : frame()
{
	frame.frametype = CDFSFrameTypes::CONT;
}
CDFSCONTFrame::CDFSCONTFrame(const CDFSFrame& frame) : frame(frame)
{
	if (!IsCONTFrame(this->frame)) { throw std::exception(); }
}
CDFSCONTFrame::CDFSCONTFrame(CDFSFrame&& frame) : frame(frame)
{
	if (!IsCONTFrame(this->frame)) { throw std::exception(); }
}
const CDFSFrame& CDFSCONTFrame::Frame() const { return frame; }
uint64_t& CDFSCONTFrame::sequence() { return frame.sequence; }
const uint64_t& CDFSCONTFrame::sequence() const { return frame.sequence; }
UInt128& CDFSCONTFrame::data_current() { return reinterpret_cast<UInt128&>(frame.data[4]); }
const UInt128& CDFSCONTFrame::data_current() const { return reinterpret_cast<const UInt128&>(frame.data[4]); }
std::array<char, 32>& CDFSCONTFrame::data_label() { return reinterpret_cast<std::array<char, 32>&>(frame.data[20]); }
const std::array<char, 32>& CDFSCONTFrame::data_label() const { return reinterpret_cast<const std::array<char, 32>&>(frame.data[20]); }
void CDFSCONTFrame::Validate() { frame.Validate(); }
bool CDFSCONTFrame::IsValid() const { return frame.IsValid(); }
bool CDFSCONTFrame::IsCONTFrame(const CDFSFrame& frame) { return frame.frametype == CDFSFrameTypes::CONT; }
