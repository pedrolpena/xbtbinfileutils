/*
 * EDFFormat.cpp
 *
 * Constants that describe the layout of a Sippican MK21 export data file
 * (EDF). Type 4 messages store these values so the EDF file can be
 * recreated from the binary.
 */

#include "EDFFormat.h"

#include <cstdio>
#include "XBTProbe.h"

// C++14 needs a definition for constants that are passed by reference
constexpr int EDFFormat::NONE;
constexpr int EDFFormat::WINMK21;
constexpr int EDFFormat::MK21;
constexpr int EDFFormat::AOML;
constexpr int EDFFormat::FIELD_NONE;
constexpr int EDFFormat::FIELD_TIME;
constexpr int EDFFormat::FIELD_RESISTANCE;
constexpr int EDFFormat::FIELD_DEPTH;
constexpr int EDFFormat::FIELD_TEMPERATURE;
constexpr int EDFFormat::FIELD_SOUND_VELOCITY;
constexpr double EDFFormat::MIN_TEMPERATURE;
constexpr double EDFFormat::MAX_TEMPERATURE;

EDFFormat::EDFFormat() {

}

EDFFormat::~EDFFormat() {

}

/**
 * This method returns the column label used by the MK21 for a data field.
 * The degree sign is written in ISO-8859-1 like the MK21 does.
 *
 * @param field the field code
 * @return the column label or an empty string when the code is unknown
 */
std::string EDFFormat::getFieldLabel(int field) {

    const char * label = "";
    switch (field) {
        case FIELD_TIME:
            label = "Time (sec)";
            break;
        case FIELD_RESISTANCE:
            label = "Resistance (ohms)";
            break;
        case FIELD_DEPTH:
            label = "Depth (m)";
            break;
        case FIELD_TEMPERATURE:
            label = "Temperature (\xB0" "C)";
            break;
        case FIELD_SOUND_VELOCITY:
            label = "Sound Velocity (m/s)";
            break;
    }// end switch

    return std::string(label);
}

/**
 * This method returns the first line of the EDF files written by this
 * library.
 *
 * @return the title line without the line break
 */
std::string EDFFormat::getTitle() {
    return "// AOML EXPORT DATA FILE  (EDF)";
}

/**
 * This method returns the columns of the data section of the EDF files
 * written by this library: time, the resistance when there are resistance
 * values, depth, temperature and sound velocity.
 *
 * @param resistance true when the profile has resistance values
 * @return the field codes in the order of the columns
 */
std::vector<int> EDFFormat::getDataFields(bool resistance) {
    std::vector<int> fields = {FIELD_TIME};
    if (resistance) {
        fields.push_back(FIELD_RESISTANCE);
    }
    fields.push_back(FIELD_DEPTH);
    fields.push_back(FIELD_TEMPERATURE);
    fields.push_back(FIELD_SOUND_VELOCITY);
    return fields;
}

/**
 * This method returns the probe lines of an EDF header made from the probe
 * table, e.g. "Probe Type       :  Deep Blue". Sippican is left out of the
 * probe name like the MK21 does.
 *
 * @param instrumentType the WMO code of the probe
 * @return the lines without line breaks
 */
std::vector<std::string> EDFFormat::getProbeLines(int instrumentType) {
    std::string probeType = XBTProbe::getProbeDescription(instrumentType);
    const std::string sippican = "Sippican ";
    if (probeType.compare(0, sippican.size(), sippican) == 0) {
        probeType = probeType.substr(sippican.size());
    }
    std::vector<std::string> lines;
    lines.push_back("Probe Type       :  " + probeType);
    lines.push_back("Terminal Depth   :  " + formatNumber(XBTProbe::getMaxDepth(instrumentType)) + " m");
    lines.push_back("Depth Equation   :  Standard");
    lines.push_back("Depth Coeff. 1   :  " + formatCoefficient(0.0));
    lines.push_back("Depth Coeff. 2   :  " + formatCoefficient(XBTProbe::getCoefficientA(instrumentType)));
    lines.push_back("Depth Coeff. 3   :  " + formatCoefficient(XBTProbe::getCoefficientB(instrumentType) * .001));
    lines.push_back("Depth Coeff. 4   :  " + formatCoefficient(0.0));
    return lines;
}

/**
 * This method returns the probe serial number the way the MK21 writes it,
 * with 8 digits.
 *
 * @param probeSerialNumber the serial number
 * @return the serial number or an empty string when it is missing
 */
std::string EDFFormat::getSerialNumber(int probeSerialNumber) {
    if (probeSerialNumber < 0) {
        return "";
    }
    char buf[32];
    snprintf(buf, sizeof(buf), "%08d", probeSerialNumber);
    return std::string(buf);
}

/**
 * Formats a number the way the MK21 writes the depth coefficients, without
 * trailing zeros but with at least one decimal, e.g. 0.0, 6.691, -0.00225.
 */
std::string EDFFormat::formatCoefficient(double value) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%.10f", value);
    std::string s = buf;
    while (s.back() == '0' && s[s.size() - 2] != '.') {
        s.pop_back();
    }
    if (s == "-0.0") {
        s = "0.0";
    }
    return s;
}

/**
 * Formats a number without trailing zeros, e.g. 760 or 760.5.
 */
std::string EDFFormat::formatNumber(double value) {
    std::string s = formatCoefficient(value);
    if (s.size() > 2 && s.compare(s.size() - 2, 2, ".0") == 0) {
        s.resize(s.size() - 2);
    }
    return s;
}

/**
 * This method returns the field code for a column label. Only the name in
 * front of the units is compared so the encoding of the degree sign does
 * not matter.
 *
 * @param label the column label, e.g. "Depth (m)"
 * @return the field code or FIELD_NONE when the label is unknown
 */
int EDFFormat::getFieldCode(std::string label) {

    std::string name = label.substr(0, label.find('('));
    while (!name.empty() && (name.back() == ' ' || name.back() == '\t')) {
        name.pop_back();
    }
    while (!name.empty() && (name.front() == ' ' || name.front() == '\t')) {
        name.erase(0, 1);
    }

    if (name == "Time") {
        return FIELD_TIME;
    }
    if (name == "Resistance") {
        return FIELD_RESISTANCE;
    }
    if (name == "Depth") {
        return FIELD_DEPTH;
    }
    if (name == "Temperature") {
        return FIELD_TEMPERATURE;
    }
    if (name == "Sound Velocity") {
        return FIELD_SOUND_VELOCITY;
    }
    return FIELD_NONE;
}
