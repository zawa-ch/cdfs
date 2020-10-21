#ifndef __cdfs_builder__
#define __cdfs_builder__
#include <array>
#include <iostream>
#include "cdfs.hpp"
namespace zawa_ch::CDFS
{
	class CDFSBuilder
	{
	public:
		typedef std::array<uint8_t, 240> ContainsType;
	private:
		std::string label;
		UInt128 frameindex;
		UInt128 datasize;
	public:
		CDFSBuilder();
		CDFSBuilder(const std::string& label);

		void WriteHEADFrame(std::ostream& stream);
		void WriteHEADFrame(std::ostream& stream, const UInt128& framecount, const UInt128& datasize);
		void WriteFINFFrame(std::ostream& stream);
		void WriteDATAFrame(std::ostream& stream, const ContainsType& data, const size_t& size = 240U);
		void WriteCONTFrame(std::ostream& stream);

		static void WriteToStream(std::ostream& stream, const CDFSFrame& frame);
		static void SetCRC(CDFSFrame& frame);
	};
}
#endif // __cdfs_builder__
