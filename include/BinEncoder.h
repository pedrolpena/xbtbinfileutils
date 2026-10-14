/*
 * BinEncoder.h
 *
 *  Created on: Jul 27, 2019
 *      Author: pedro
 */
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <ostream>
#include <vector>
#include <array>
#include "bitvector.h"
#include "EDFFormat.h"
#include "MessageType.h"
#include "XBTProfile.h"
#include "XBTProfileDataRanges.h"

#ifndef BINENCODER_H_
#define BINENCODER_H_

class BinEncoder {
public:

	BinEncoder();
	BinEncoder(XBTProfile &xBTProfile);
	virtual ~BinEncoder();
	stdex::bitvector bits; //holds bits from file
	int newMessageType;
	stdex::bitvector changeEndian(stdex::bitvector);
	void setBitWithinBits(stdex::bitvector &b0, stdex::bitvector &b1,int);
	static constexpr int MISSING_VALUE = -999;
	int scaleValue(double value, double scale, double offset = 0);
	void integerToBits(stdex::bitvector &b0, int, std::array<int,2>);
	void longToBits(stdex::bitvector &b0, long long, std::array<int,2>);
	void stringToBits(stdex::bitvector &b0, std::string,std::array<int,2>);
	void encodeCommentBlocks(stdex::bitvector &b0,std::string,std::array<int,2>,int);
	void encodeProfile(XBTProfile &xBTProfile);
	void encodeEDFFields(XBTProfile &xBTProfile);
	int encodeTextBlocks(stdex::bitvector &b0, std::string, int, int);
	int getBitsSize(void);
	std::string getBinarySequence(int, int);
	void setMessageCRC(stdex::bitvector &b);
	void writeOutBinFile(std::string outputFile);
	void writeOutBin(std::ostream &out);
	std::vector<char> getBytes();
	uint32_t computeCRC32(char *buf, uint32_t len);
};

#endif /* BINENCODER_H_ */
