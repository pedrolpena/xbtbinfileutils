/*
 * EDFFormat.h
 *
 * Constants and helpers that describe the layout of a Sippican MK21 export
 * data file (EDF). This library writes EDF files with the layout of the
 * MK21 software under the AOML title.
 */

#ifndef INCLUDE_EDFFORMAT_H_
#define INCLUDE_EDFFORMAT_H_
#include <string>
#include <vector>

class EDFFormat {
public:
	// Layouts of EDF files that can be read
	static constexpr int NONE = 0;
	// "// This is a MK21 EXPORT DATA FILE  (EDF)", written by WinMK21
	static constexpr int WINMK21 = 1;
	// "// MK21 EXPORT DATA FILE  (EDF)", written by the MK21 software
	static constexpr int MK21 = 2;
	// "// AOML EXPORT DATA FILE  (EDF)", the MK21 layout written by this
	// library
	static constexpr int AOML = 3;

	// Columns found in the data section of an EDF file
	static constexpr int FIELD_NONE = 0;
	static constexpr int FIELD_TIME = 1;
	static constexpr int FIELD_RESISTANCE = 2;
	static constexpr int FIELD_DEPTH = 3;
	static constexpr int FIELD_TEMPERATURE = 4;
	static constexpr int FIELD_SOUND_VELOCITY = 5;

	// The MK21 limits temperatures to this range
	static constexpr double MIN_TEMPERATURE = -2.53;
	static constexpr double MAX_TEMPERATURE = 37.07;

	EDFFormat();
	virtual ~EDFFormat();
	static std::string getFieldLabel(int);
	static int getFieldCode(std::string);
	static std::string getTitle();
	static std::vector<int> getDataFields(bool);
	static std::vector<std::string> getProbeLines(int);
	static std::string getSerialNumber(int);
	static std::string formatCoefficient(double);
	static std::string formatNumber(double);
};

#endif /* INCLUDE_EDFFORMAT_H_ */
