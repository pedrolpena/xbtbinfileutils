//============================================================================
// Name        : bin2ascii.cpp
// Author      : Pedro Pena
// Version     :
// Copyright   :
// Description : bin2ascii in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <sys/stat.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif

#include <BinDecoder.h>
#include <FormatConverter.h>
#include <XBTProfile.h>
#include <bitvector.h>

// ===================================================================
//  Read all bytes from stdin into memory.
// ===================================================================
static std::vector<char> readStdinBytes() {
#ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
#endif
    std::vector<char> result;
    std::vector<char> buf(4096);
    while (std::cin.good()) {
        std::cin.read(buf.data(), static_cast<std::streamsize>(buf.size()));
        std::streamsize n = std::cin.gcount();
        if (n > 0)
            result.insert(result.end(), buf.data(), buf.data() + n);
    }
    return result;
}

// ===================================================================
//  Populate a BinDecoder's public `bits` from raw bytes in memory.
//  Replicates the constructor's byte -> bitvector -> changeEndian
//  logic without touching the filesystem.
// ===================================================================
static void decoderFromBytes(BinDecoder& decoder, const std::vector<char>& bytes) {
    size_t size = bytes.size();
    decoder.bits = stdex::bitvector(8 * size);
    int pos = 0;
    for (size_t i = 0; i < size; ++i) {
        for (int j = 0; j < 8; ++j) {
            bool val = ((bytes[i] >> j) & 0x01) == 1;
            decoder.bits.set(pos, val);
            ++pos;
        }
    }
    decoder.bits = decoder.changeEndian(decoder.bits);
}

// ===================================================================
//  Check if a path exists on the filesystem.
// ===================================================================
static bool fileExists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0;
}

// ===================================================================
//  Check if a string is a small non-negative integer (a type code).
// ===================================================================
static bool isTypeCode(const char* s) {
    if (!s || !*s) return false;
    for (const char* p = s; *p; ++p) {
        if (*p < '0' || *p > '9') return false;
    }
    return true;
}

int main(int argc, char *argv[]) {

	std::string fileName = "";
	int type = 0;
	double salinity = 35.0;
	bool fromStdin = false;

	if (argc > 1 && std::string(argv[1]) == "-") {
		// Explicit stdin marker: bin2ascii - [type] [salinity]
		fromStdin = true;
		fileName = "stdin";
		if (argc > 2) type = std::stoi(argv[2]);
		if (argc > 3) salinity = std::atof(argv[3]);

	} else if (argc > 1 && fileExists(argv[1])) {
		// File mode (original): bin2ascii file.bin [type] [salinity]
		fileName = argv[1];
		if (argc > 2) type = std::stoi(argv[2]);
		if (argc > 3) salinity = std::atof(argv[3]);

	} else if (argc > 1 && isTypeCode(argv[1])) {
		// Pipe mode with args: cat file | bin2ascii [type] [salinity]
		fromStdin = true;
		fileName = "stdin";
		type = std::stoi(argv[1]);
		if (argc > 2) salinity = std::atof(argv[2]);

	} else if (argc == 1) {
		// No arguments at all
#ifdef _WIN32
		bool isTTY = _isatty(_fileno(stdin));
#else
		bool isTTY = isatty(fileno(stdin));
#endif
		if (isTTY) {
			std::cout << "you must provide an amverseas bin file" << std::endl;
			return 1;
		}
		// Stdin is a pipe with no args — defaults
		fromStdin = true;
		fileName = "stdin";

	} else {
		// argv[1] is not "-", not a file, not a type code —
		// treat it as a filename and let BinDecoder report the error.
		fileName = argv[1];
		if (argc > 2) type = std::stoi(argv[2]);
		if (argc > 3) salinity = std::atof(argv[3]);
	}

	BinDecoder *decodedXBTProfile;
	XBTProfile *xBTProfile = new XBTProfile();

	if (fromStdin) {
		auto bytes = readStdinBytes();
		if (bytes.empty()) {
			std::cerr << "no data received on stdin" << std::endl;
			delete xBTProfile;
			return 1;
		}
		decodedXBTProfile = new BinDecoder("");
		decoderFromBytes(*decodedXBTProfile, bytes);
	} else {
		decodedXBTProfile = new BinDecoder(fileName);
	}

	decodedXBTProfile->decodeXBTProfile(*xBTProfile);

	FormatConverter *fc = new FormatConverter(*xBTProfile);
	if (type == 0)
		std::cout << fc->getASCII() << std::endl;
	if (type == 1)
		std::cout << fc->getASCIIEDF(fileName, salinity) << std::endl;
	if (type == 2)
		std::cout << fc->getASCIINDC() << std::endl;

	delete fc;
	delete decodedXBTProfile;
	delete xBTProfile;

	return 0;
}
