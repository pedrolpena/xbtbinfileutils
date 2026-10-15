//============================================================================
// Name        : edf2bin.cpp
// Description : Converts an export data file (EDF) to an Amverseas binary
//               XBT profile of message type 1, 2, 3 or 4.
//
// The EDF can be read from a file or from standard input and the binary can
// be written to a file or to standard output.
//
//   edf2bin [options] [input.edf | -]
//
// Every EDF layout is supported, the one written by WinMK21
// ("// This is a MK21 EXPORT DATA FILE  (EDF)"), the one written by the
// newer MK21 software ("// MK21 EXPORT DATA FILE  (EDF)") and the one
// written by xbtbinfileutils ("// AOML EXPORT DATA FILE  (EDF)").
//
// Type 4 stores the resistance values, or the temperatures when the EDF has
// no resistances, and the EDF header lines the other fields can not hold,
// types 1 to 3 store the temperatures and the fields Amverseas uses.
//============================================================================

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

#include <BinEncoder.h>
#include <DataQuality.h>
#include <DataResolution.h>
#include <EDFFormat.h>
#include <MessageType.h>
#include <XBTProbe.h>
#include <XBTProfile.h>
#include <XBTProfileDataRanges.h>
#include <XBTRecorder.h>
#include <XBTResistanceToTemperatureConverter.h>

namespace {

constexpr int MISSING = -999;
// Types 1 to 3 store temperatures in 12 bits as (T + 4.00) * 100, every bit
// set marks a missing value.
constexpr double MIN_STORED_TEMPERATURE = -4.00;
constexpr double MAX_STORED_TEMPERATURE = 36.94;

// Header fields of the binary that EDF files do not hold. They can be given
// on the command line.
struct Metadata {
    const char *shortName;  // e.g. "-c", or "" when there is none
    const char *name;       // e.g. "--callsign"
    const char *value;      // what the value is, shown in the help
    const char *help;
    enum Kind { TEXT, INTEGER, REAL, DATE, RIDER } kind;
    int field;              // the data location of the field
    double scale;           // stored value = value * scale + offset
    double offset;
};

const std::vector<Metadata> METADATA = {
    {"-c", "--callsign", "TEXT", "ship call sign", Metadata::TEXT, XBTProfileDataRanges::WMOID, 1, 0},
    {"-s", "--ship-name", "TEXT", "ship name (AOML files hold it in the memo)", Metadata::TEXT, XBTProfileDataRanges::SHIP_NAME, 1, 0},
    {"-i", "--imo", "NUMBER", "ship IMO (Lloyds) number", Metadata::INTEGER, XBTProfileDataRanges::LLOYDS, 1, 0},
    {"-l", "--soop-line", "TEXT", "SOOP line, e.g. AX07", Metadata::TEXT, XBTProfileDataRanges::SOOP_LINE, 1, 0},
    {"-t", "--transect", "NUMBER", "transect number", Metadata::INTEGER, XBTProfileDataRanges::TRANSECT_NUM, 1, 0},
    {"", "--sequence", "NUMBER", "sequence number, replaces the one of the EDF", Metadata::INTEGER, XBTProfileDataRanges::SEQUENCE_NUM, 1, 0},
    {"", "--seas-version", "NUMBER", "Amverseas version, e.g. 1023", Metadata::INTEGER, XBTProfileDataRanges::SEAS_VERSION, 1, 0},
    {"", "--data-quality", "NUMBER", "0 not suspect (default), 1 slightly, 2 highly suspect, 3 unfit", Metadata::INTEGER, XBTProfileDataRanges::DATA_QUALITY, 1, 0},
    {"", "--probe", "CODE", "WMO probe code, replaces the one found from the probe type", Metadata::INTEGER, XBTProfileDataRanges::INSTRUMENT_TYPE, 1, 0},
    {"", "--recorder", "CODE", "WMO recorder code, replaces the one found from the MK21 device", Metadata::INTEGER, XBTProfileDataRanges::RECORDER_TYPE, 1, 0},
    {"", "--recorder-serial", "TEXT", "recorder serial number", Metadata::TEXT, XBTProfileDataRanges::XBT_RECORDER_SERIAL_NUMBER, 1, 0},
    {"", "--recorder-date", "YYYY-MM-DD", "date the recorder was manufactured", Metadata::DATE, XBTProfileDataRanges::XBT_RECORDER_MANUFACTURE_YEAR, 1, 0},
    {"", "--probe-date", "YYYY-MM-DD", "date the probe was manufactured", Metadata::DATE, XBTProfileDataRanges::XBT_PROBE_MANUFACTURE_YEAR, 1, 0},
    {"", "--launcher", "CODE", "WMO launcher code", Metadata::INTEGER, XBTProfileDataRanges::XBT_LAUNCHER_TYPE, 1, 0},
    {"", "--launch-height", "METERS", "height of the launch above the sea", Metadata::REAL, XBTProfileDataRanges::LAUNCH_HEIGHT, 100, 0},
    {"", "--ship-direction", "DEGREES", "ship heading", Metadata::REAL, XBTProfileDataRanges::SHIP_DIRECTION, 1, 0},
    {"", "--ship-speed", "M/S", "ship speed in meters per second", Metadata::REAL, XBTProfileDataRanges::SHIP_SPEED, 100, 0},
    {"", "--wind-instrument", "CODE", "WMO wind instrument code", Metadata::INTEGER, XBTProfileDataRanges::WIND_INSTRUMENT_TYPE, 1, 0},
    {"", "--wind-direction", "DEGREES", "wind direction", Metadata::INTEGER, XBTProfileDataRanges::WIND_DIRECTION, 1, 0},
    {"", "--wind-speed", "M/S", "wind speed in meters per second", Metadata::REAL, XBTProfileDataRanges::WIND_SPEED, 10, 0},
    {"", "--air-temperature", "CELSIUS", "dry bulb temperature, stored in kelvin", Metadata::REAL, XBTProfileDataRanges::DRY_BULB_TEMPERATURE, 10, 2731.5},
    {"", "--current-method", "CODE", "WMO sea surface current measurement method code", Metadata::INTEGER, XBTProfileDataRanges::SEA_SURFACE_CURRENT_MEASURING_METHOD, 1, 0},
    {"", "--current-direction", "DEGREES", "sea surface current direction", Metadata::INTEGER, XBTProfileDataRanges::SEA_SURFACE_CURRENT_DIRECTION, 1, 0},
    {"", "--current-speed", "M/S", "sea surface current speed in meters per second", Metadata::REAL, XBTProfileDataRanges::SEA_SURFACE_CURRENT_SPEED, 100, 0},
    {"", "--water-depth", "METERS", "total water depth", Metadata::INTEGER, XBTProfileDataRanges::TOTAL_WATER_DEPTH, 1, 0},
    {"", "--agency", "CODE", "WMO code of the agency that owns the platform", Metadata::INTEGER, XBTProfileDataRanges::AGENCY_OWNER, 1, 0},
    {"", "--rider-name", "TEXT", "rider name", Metadata::RIDER, XBTProfileDataRanges::NUMBER_OF_RIDER_BLOCKS, 1, 0},
    {"", "--rider-email", "TEXT", "rider email", Metadata::RIDER, XBTProfileDataRanges::NUMBER_OF_RIDER_EMAIL_BLOCKS, 1, 0},
    {"", "--rider-institution", "TEXT", "rider institution", Metadata::RIDER, XBTProfileDataRanges::NUMBER_OF_RIDER_INSTITUTION_BLOCKS, 1, 0},
    {"", "--rider-phone", "TEXT", "rider phone number", Metadata::RIDER, XBTProfileDataRanges::NUMBER_OF_RIDER_PHONE_BLOCKS, 1, 0},
};

struct Options {
    std::string input = "-";
    std::string output = "-";
    int type = MessageType::MESSAGE_TYPE_3;
    bool quiet = false;
    // the metadata options that were given, in the order of METADATA
    std::vector<std::pair<const Metadata *, std::string>> metadata;
};

void usage(std::ostream &out) {
    out << "Usage: edf2bin [options] [input.edf | -]\n"
        << "\n"
        << "Converts an export data file (EDF) written by WinMK21, the MK21 software or\n"
        << "xbtbinfileutils (AOML) to an Amverseas binary XBT profile. The EDF is read\n"
        << "from standard input when no file or \"-\" is given. The binary is written to\n"
        << "standard output unless -o is used.\n"
        << "\n"
        << "Options:\n"
        << "  -T, --type NUMBER     message type to save, 1 to 4 (default 3). Only type 4\n"
        << "                        keeps the resistances and the EDF header, types 1 to 3\n"
        << "                        store the temperatures\n"
        << "  -o, --output FILE     write the binary to FILE (\"-\" is standard output)\n"
        << "  -q, --quiet           do not print warnings\n"
        << "  -h, --help            show this help\n"
        << "\n"
        << "Metadata that EDF files do not hold (not every message type has every field):\n";
    for (const Metadata &m : METADATA) {
        std::string option = std::string(m.shortName).empty() ? "    " : std::string(m.shortName) + ", ";
        option += std::string(m.name) + " " + m.value;
        out << "  " << option;
        if (option.size() < 28) {
            out << std::string(28 - option.size(), ' ');
        } else {
            out << "\n" << std::string(30, ' ');
        }
        out << m.help << "\n";
    }
    out << "\n"
        << "Examples:\n"
        << "  edf2bin PODS16001.edf -o PODS16001.bin\n"
        << "  edf2bin --type 4 PODS16001.edf -o PODS16001.bin\n"
        << "  edf2bin -c WTEC -s \"Oleander\" --ship-speed 9.5 PODS16001.edf -o PODS16001.bin\n"
        << "  cat TD_00001.EDF | edf2bin -c WTEC --rider-name \"Jane Doe\" > TD_00001.bin\n";
}

void warn(const Options &options, const std::string &message) {
    if (!options.quiet) {
        std::cerr << "edf2bin: warning: " << message << std::endl;
    }
}

std::string trim(const std::string &s) {
    size_t first = s.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = s.find_last_not_of(" \t");
    return s.substr(first, last - first + 1);
}

bool startsWith(const std::string &s, const std::string &prefix) {
    return s.compare(0, prefix.size(), prefix) == 0;
}

/**
 * Parses a number at the start of a string, e.g. "760 m" or "32.00 ppt".
 * Returns false when the string does not start with a number.
 */
bool parseNumber(const std::string &s, double &value) {
    std::string t = trim(s);
    char *end = nullptr;
    value = std::strtod(t.c_str(), &end);
    return end != t.c_str();
}

/**
 * Parses a position written as degrees, minutes and hemisphere, e.g.
 * "44 50.67871N" or "124 14.90039W". Decimal degrees are also accepted.
 * The number of decimals of the minutes is returned.
 */
bool parsePosition(const std::string &s, double &degrees, int &decimals) {
    std::string t = trim(s);
    if (t.empty()) {
        return false;
    }
    char hemisphere = t.back();
    double sign = 1;
    if (hemisphere == 'S' || hemisphere == 's' || hemisphere == 'W' || hemisphere == 'w') {
        sign = -1;
    }
    if (std::isalpha((unsigned char) hemisphere)) {
        t.pop_back();
    }
    std::istringstream ss(t);
    double d = 0;
    double m = 0;
    std::string minutes = "";
    if (!(ss >> d)) {
        return false;
    }
    if (ss >> minutes) {
        m = std::atof(minutes.c_str());
    }
    decimals = MISSING;
    size_t point = minutes.find('.');
    if (!minutes.empty()) {
        decimals = point == std::string::npos ? 0 : minutes.size() - point - 1;
    }
    degrees = sign * (std::fabs(d) + m / 60.0);
    if (d < 0) {
        degrees = -degrees;
    }
    return true;
}

/**
 * Returns the letters and digits of a name in lower case so names can be
 * compared without caring about spaces, dashes and case.
 */
std::string normalize(const std::string &name) {
    std::string normalized = "";
    for (char c : name) {
        if (std::isalnum((unsigned char) c)) {
            normalized += std::tolower((unsigned char) c);
        }
    }
    return normalized;
}

/**
 * Returns the WMO code of a probe. The name is compared with the probe
 * descriptions of the library, with and without the manufacturer when it
 * is Sippican, e.g. "Deep Blue" and "Sippican Deep Blue". When a probe has
 * an old and a new fall rate equation the code whose coefficient matches
 * the second depth coefficient is used, otherwise the newest code.
 */
int getInstrumentType(const std::string &probeType, double coefficient2) {
    std::string name = normalize(probeType);
    if (name.empty()) {
        return MISSING;
    }
    int found = MISSING;
    for (int code = 0; code < 1024; code++) {
        std::string description = XBTProbe::getProbeDescription(code);
        if (description == "Unknown") {
            continue;
        }
        std::string normalized = normalize(description);
        bool sameName = name == normalized
                || (normalized.compare(0, 8, "sippican") == 0 && name == normalized.substr(8));
        if (!sameName) {
            continue;
        }
        if (coefficient2 != MISSING && std::fabs(XBTProbe::getCoefficientA(code) - coefficient2) < 1e-6) {
            return code;
        }
        found = code;
    }
    return found;
}

/**
 * Returns the WMO code of the recorder named in the hardware section, e.g.
 * "MK21/Ethernet" or "Sippican MK-21". Every MK21 interface is a MK21.
 */
int getRecorderType(const std::string &device) {
    std::string name = normalize(device);
    for (int code = 0; code < 128; code++) {
        std::string description = XBTRecorder::getRecorderDescription(code);
        if (description != "Unknown" && name == normalize(description)) {
            return code;
        }
    }
    if (name.find("mk21") != std::string::npos) {
        return XBTRecorder::SIPPICAN_MK_21;
    }
    return MISSING;
}

/**
 * Returns the largest value a field can hold in a message type.
 */
long getMaximum(int field, int type) {
    std::array<int,2> range = XBTProfileDataRanges::getDataLocation(field, type);
    if (range[0] == -1) {
        return -1;
    }
    // every bit set marks a missing value
    return (1L << (range[1] - range[0] + 1)) - 2;
}

/**
 * Returns the field codes of a WinMK21 column header, e.g.
 * "Depth (m) - Temperature (C) - Sound Velocity (m/s)". The vector is
 * empty when the line is not a column header.
 */
std::vector<int> parseColumnHeader(const std::string &line) {
    std::vector<int> fields;
    size_t start = 0;
    while (start <= line.size()) {
        size_t end = line.find(" - ", start);
        if (end == std::string::npos) {
            end = line.size();
        }
        int field = EDFFormat::getFieldCode(line.substr(start, end - start));
        if (field == EDFFormat::FIELD_NONE) {
            return std::vector<int>();
        }
        fields.push_back(field);
        start = end + 3;
    }
    return fields;
}

std::vector<std::string> readLines(std::istream &in) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        lines.push_back(line);
    }
    return lines;
}

// A line of the EDF header that is kept in the EDF text of the profile
struct HeaderLine {
    std::string section;
    std::string line;
    // probe, serial number and MK21 device lines are only kept when the
    // probe table, the probe serial number and the recorder type can not
    // make them
    enum Kind { TEXT, PROBE, SERIAL, DEVICE } kind;
};

/**
 * Reads the EDF lines and fills the profile. The values the binary holds
 * are stored in their fields, the other header lines are kept in the EDF
 * text of the profile. Returns false and prints an error when the file can
 * not be converted.
 */
bool parseEDF(const std::vector<std::string> &lines, XBTProfile &profile, const Options &options) {

    // The title tells the layout. WinMK21 files start with "This is a MK21
    // EXPORT DATA FILE", every other EDF uses the MK21 layout with its own
    // title, e.g. "MK21 EXPORT DATA FILE" or "AOML EXPORT DATA FILE".
    int format = EDFFormat::NONE;
    for (const std::string &line : lines) {
        if (trim(line).empty()) {
            continue;
        }
        if (line.find("EXPORT DATA FILE") != std::string::npos) {
            if (line.find("This is a MK21") != std::string::npos) {
                format = EDFFormat::WINMK21;
            } else if (line.find("MK21") != std::string::npos) {
                format = EDFFormat::MK21;
            } else {
                format = EDFFormat::AOML;
                if (line.find("AOML") == std::string::npos) {
                    warn(options, "unknown EDF title \"" + trim(line) + "\", it is read as an AOML EDF");
                }
            }
        }
        break;
    }
    if (format == EDFFormat::NONE) {
        std::cerr << "edf2bin: the input is not an export data file (EDF)" << std::endl;
        return false;
    }

    // Sections of the MK21 layout, which the library writes. The WinMK21
    // layout has no section names, its lines are put in the matching
    // sections and changed to the MK21 lines.
    const std::vector<std::string> sections = {
            "File Information", "System Information", "Probe Information",
            "Launch Information", "Memo", "Hardware", "Information - XBT",
            "Post-Processing", "Data Fields", "Data" };
    std::string section = format == EDFFormat::WINMK21 ? "Launch Information" : "";
    bool inData = false;
    bool seenMemoSeparator = false;
    std::vector<int> fields;
    std::vector<HeaderLine> header;
    std::vector<std::string> probeLines;
    std::string probeType = "";
    std::string serialNumber = "";
    std::string device = "";
    std::string memo = "";
    double coefficient2 = MISSING;
    int infoFields = MISSING;
    std::vector<std::vector<double>> rows;

    for (size_t n = 1; n < lines.size(); n++) {
        std::string line = lines[n];
        std::string t = trim(line);

        if (inData) {
            if (t.empty()) {
                continue;
            }
            std::istringstream ss(t);
            std::vector<double> row;
            double value;
            while (ss >> value) {
                row.push_back(value);
            }
            if (row.size() != fields.size()) {
                std::cerr << "edf2bin: line " << n + 1 << " has " << row.size()
                          << " values, expected " << fields.size() << std::endl;
                return false;
            }
            rows.push_back(row);
            continue;
        }

        if (startsWith(t, "//")) {
            std::string comment = trim(t.substr(2));
            if (format != EDFFormat::WINMK21) {
                bool isSection = false;
                for (const std::string &s : sections) {
                    if (comment == s) {
                        isSection = true;
                    }
                }
                if (isSection) {
                    section = comment;
                    inData = section == "Data";
                } else {
                    header.push_back({section, line, HeaderLine::TEXT});
                }
                continue;
            }

            // WinMK21 comment lines, the fixed ones are written by the library
            if (comment.empty()) {
                if (section == "Memo") {
                    if (seenMemoSeparator) {
                        section = "Probe Information";
                    }
                    seenMemoSeparator = true;
                }
                continue;
            }
            if (startsWith(comment, "Here are the contents of the memo fields")) {
                section = "Memo";
            } else if (startsWith(comment, "Here is some probe information")) {
                section = "Probe Information";
            } else if (startsWith(comment, "Sound velocity derived with assumed salinity:")) {
                double salinity;
                if (parseNumber(comment.substr(comment.find(':') + 1), salinity)) {
                    profile.setSalinity(salinity);
                }
                section = "Information - XBT";
            } else if (comment.find("export file") != std::string::npos) {
                // e.g. "This XBT export file has not been noise reduced or averaged."
                section = "Post-Processing";
                std::string operations = comment.find("has not been") != std::string::npos ? "None" : comment;
                header.push_back({section, "Operations       :  " + operations, HeaderLine::TEXT});
            } else {
                header.push_back({section, line, HeaderLine::TEXT});
            }
            continue;
        }

        // every line of the memo section is memo text
        if (section == "Memo") {
            memo += line + "\n";
            header.push_back({section, line, HeaderLine::TEXT});
            continue;
        }

        if (format == EDFFormat::WINMK21) {
            std::vector<int> columns = parseColumnHeader(t);
            if (!columns.empty()) {
                fields = columns;
                inData = true;
                continue;
            }
        }

        size_t colon = line.find(':');
        if (colon == std::string::npos) {
            if (!t.empty()) {
                header.push_back({section, line, HeaderLine::TEXT});
            }
            continue;
        }
        std::string key = trim(line.substr(0, colon));
        std::string value = trim(line.substr(colon + 1));
        double number;
        int decimals;

        if (key == "Probe Type" || key == "Terminal Depth" || key == "Depth Equation" || startsWith(key, "Depth Coeff.")) {
            if (key == "Probe Type") {
                probeType = value;
            }
            if (key == "Depth Coeff. 2" && parseNumber(value, number)) {
                coefficient2 = number;
            }
            probeLines.push_back(line);
            header.push_back({"Probe Information", line, HeaderLine::PROBE});
        } else if (key == "Num Info Fields" && parseNumber(value, number)) {
            infoFields = (int) number;
        } else if (key == "Date of Launch") {
            int month, day, year;
            if (std::sscanf(value.c_str(), "%d/%d/%d", &month, &day, &year) == 3) {
                profile.setMonth(month);
                profile.setDay(day);
                profile.setYear(year);
            } else {
                warn(options, "can not read the date \"" + value + "\"");
            }
        } else if (key == "Time of Launch") {
            int hour, minute, second = 0;
            if (std::sscanf(value.c_str(), "%d:%d:%d", &hour, &minute, &second) >= 2) {
                profile.setHour(hour);
                profile.setMinute(minute);
                profile.setSecond(second);
            } else {
                warn(options, "can not read the time \"" + value + "\"");
            }
        } else if (key == "Sequence Number" || key == "Sequence #") {
            if (parseNumber(value, number)) {
                profile.setSequenceNum((int) number);
            }
        } else if (key == "Latitude" && parsePosition(value, number, decimals)) {
            profile.setLatitude(number);
        } else if (key == "Longitude" && parsePosition(value, number, decimals)) {
            profile.setLongitude(number);
        } else if (key == "Serial Number" || key == "Serial #") {
            serialNumber = value;
            char *end = nullptr;
            long serial = std::strtol(value.c_str(), &end, 10);
            if (end != value.c_str() && *end == '\0' && serial >= 0
                    && serial <= getMaximum(XBTProfileDataRanges::PROBE_SERIAL_NUMBER, options.type)) {
                profile.setProbeSerialNumber((int) serial);
            }
            header.push_back({"Launch Information", line, HeaderLine::SERIAL});
        } else if (key == "Salinity" && parseNumber(value, number)) {
            profile.setSalinity(number);
        } else if (key == "Raw Data Filename") {
            // the EDF written from the binary names the binary instead
            if (format == EDFFormat::WINMK21) {
                section = "File Information";
            }
        } else if (key == "Num Data Fields") {
            // the number of fields is known from the field labels
        } else if (startsWith(key, "Field")) {
            int field = EDFFormat::getFieldCode(value);
            if (field == EDFFormat::FIELD_NONE) {
                std::cerr << "edf2bin: unknown data field \"" << value << "\"" << std::endl;
                return false;
            }
            fields.push_back(field);
        } else {
            // e.g. the units and the MK21 device
            if (format == EDFFormat::WINMK21 && key == "Display Units") {
                section = "System Information";
                line = "Units            :  " + value;
            }
            HeaderLine::Kind kind = HeaderLine::TEXT;
            if (key == "MK21 Device") {
                int recorder = getRecorderType(value);
                if (recorder != MISSING) {
                    profile.setRecorderType(recorder);
                } else {
                    warn(options, "unknown recorder \"" + value + "\", a MK21 is assumed");
                }
                device = line;
                kind = HeaderLine::DEVICE;
            }
            header.push_back({section, line, kind});
        }
    }

    if (fields.empty()) {
        std::cerr << "edf2bin: no data fields were found" << std::endl;
        return false;
    }

    // the probe lines and the serial number line are only kept when they
    // can not be made from the probe table and the probe serial number
    profile.setInstrumentType(getInstrumentType(probeType, coefficient2));
    if (profile.getInstrumentType() == MISSING) {
        warn(options, "unknown probe type \"" + probeType + "\"");
    }
    // only the probe lines the probe table can not make are kept
    std::vector<std::string> tableLines = EDFFormat::getProbeLines(profile.getInstrumentType());
    auto fromTable = [&](const std::string &line) {
        return std::find(tableLines.begin(), tableLines.end(), line) != tableLines.end();
    };
    bool keepProbe = false;
    for (const std::string &line : probeLines) {
        keepProbe = keepProbe || !fromTable(line);
    }
    bool keepSerial = !serialNumber.empty() && serialNumber != EDFFormat::getSerialNumber(profile.getProbeSerialNumber());
    bool keepDevice = trim(device.substr(device.find(':') + 1))
            != XBTRecorder::getRecorderDescription(profile.getRecorderType());
    // the lines are stored in the order of the sections of the AOML layout
    std::string text = "";
    int launchLines = 0;
    for (const std::string &s : sections) {
        for (const HeaderLine &h : header) {
            if (h.section != s || (h.kind == HeaderLine::PROBE && fromTable(h.line))
                    || (h.kind == HeaderLine::SERIAL && !keepSerial) || (h.kind == HeaderLine::DEVICE && !keepDevice)) {
                continue;
            }
            text += h.section + "\t" + h.line + "\n";
            if (h.section == "Launch Information" && h.kind == HeaderLine::TEXT) {
                launchLines++;
            }
        }
    }
    if (format != EDFFormat::WINMK21 && infoFields != MISSING && infoFields != 6 + launchLines) {
        warn(options, "\"Num Info Fields\" is " + std::to_string(infoFields) + ", it will be written as "
                + std::to_string(6 + launchLines));
    }

    int timeColumn = -1;
    int resistanceColumn = -1;
    int temperatureColumn = -1;
    for (size_t i = 0; i < fields.size(); i++) {
        if (fields[i] == EDFFormat::FIELD_TIME) {
            timeColumn = i;
        } else if (fields[i] == EDFFormat::FIELD_RESISTANCE) {
            resistanceColumn = i;
        } else if (fields[i] == EDFFormat::FIELD_TEMPERATURE) {
            temperatureColumn = i;
        }
    }
    if (resistanceColumn == -1 && temperatureColumn == -1) {
        std::cerr << "edf2bin: the file has neither resistance nor temperature values" << std::endl;
        return false;
    }
    std::vector<int> written = EDFFormat::getDataFields(options.type == MessageType::MESSAGE_TYPE_4 && resistanceColumn != -1);
    if (options.type == MessageType::MESSAGE_TYPE_4 && fields != written && format != EDFFormat::WINMK21) {
        std::string columns = "";
        for (int field : written) {
            columns += (columns.empty() ? "" : ", ") + EDFFormat::getFieldLabel(field);
        }
        warn(options, "the data columns will be written back as " + columns);
    }

    if (options.type == MessageType::MESSAGE_TYPE_4) {
        // type 4 stores the resistances. Files without them store the
        // temperatures, resistances are not made up from them.
        std::vector<double> points;
        for (const std::vector<double> &row : rows) {
            points.push_back(row[resistanceColumn != -1 ? resistanceColumn : temperatureColumn]);
        }
        if (resistanceColumn != -1) {
            profile.setResistancePoints(points);
        } else {
            warn(options, "the file has no resistance values, the temperatures are stored instead");
            profile.setTemperaturePoints(points);
        }
        profile.setEDFText(text);
    } else {
        // types 1 to 3 store the temperatures
        XBTResistanceToTemperatureConverter converter;
        std::vector<double> temperatures;
        int clamped = 0;
        // Copies, std::min and std::max take references and MSVC does not
        // export the library's constants from the DLL.
        const double minResistance = EDFFormat::MIN_RESISTANCE;
        const double maxResistance = EDFFormat::MAX_RESISTANCE;
        for (const std::vector<double> &row : rows) {
            double temperature;
            if (temperatureColumn != -1) {
                temperature = row[temperatureColumn];
            } else {
                // the MK21 limits the resistance, not the temperature
                double resistance = std::min(std::max(row[resistanceColumn], minResistance), maxResistance);
                temperature = converter.convertResistaceToTemperature(resistance);
            }
            if (temperature < MIN_STORED_TEMPERATURE || temperature > MAX_STORED_TEMPERATURE) {
                temperature = std::min(std::max(temperature, MIN_STORED_TEMPERATURE), MAX_STORED_TEMPERATURE);
                clamped++;
            }
            temperatures.push_back(temperature);
        }
        if (clamped > 0) {
            warn(options, std::to_string(clamped) + " temperatures are outside of the range a type "
                    + std::to_string(options.type) + " message can store and were limited to it");
        }
        profile.setTemperaturePoints(temperatures);
        profile.setMeasurementPoints(temperatures);
        if (keepProbe) {
            std::string lost = "";
            for (const std::string &line : probeLines) {
                if (!fromTable(line)) {
                    lost += "\n  " + line;
                }
            }
            warn(options, "these probe lines are not stored in a type " + std::to_string(options.type)
                    + " message, the probe table is used instead:" + lost);
        }
    }

    // the depths use the sample frequency of the recorder
    if (timeColumn != -1 && rows.size() > 1) {
        double interval = (rows.back()[timeColumn] - rows.front()[timeColumn]) / (rows.size() - 1);
        double frequency = XBTRecorder::getRecorderFrequency(profile.getRecorderType());
        if (interval <= 0 || std::fabs(1.0 / interval - frequency) > 0.01) {
            warn(options, "the samples are not " + EDFFormat::formatNumber(frequency)
                    + " Hz apart, the times will use the frequency of the recorder");
        }
        if (std::fabs(rows.front()[timeColumn]) > 1e-9) {
            warn(options, "the first sample is not at time 0, the times will start at 0 in the binary");
        }
    }

    // AOML files write the ship name in the memo
    if (format == EDFFormat::AOML && profile.getShipName().empty() && !memo.empty()) {
        profile.setShipName(memo.substr(0, memo.find('\n')));
    }

    if (profile.getProbeSerialNumber() == MISSING && !serialNumber.empty()) {
        warn(options, "the serial number \"" + serialNumber + "\" does not fit the probe serial number field"
                + (options.type == MessageType::MESSAGE_TYPE_4 ? ", it is kept in the EDF text" : ", it is not stored"));
    }
    return true;
}

bool parseArguments(int argc, char *argv[], Options &options) {
    bool haveInput = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        auto next = [&](std::string &value) {
            if (i + 1 >= argc) {
                std::cerr << "edf2bin: " << arg << " needs a value" << std::endl;
                return false;
            }
            value = argv[++i];
            return true;
        };
        std::string value;
        const Metadata *metadata = nullptr;
        for (const Metadata &m : METADATA) {
            if (arg == m.name || arg == m.shortName) {
                metadata = &m;
            }
        }
        if (arg == "-h" || arg == "--help") {
            usage(std::cout);
            std::exit(0);
        } else if (arg == "-q" || arg == "--quiet") {
            options.quiet = true;
        } else if (arg == "-T" || arg == "--type") {
            if (!next(value)) return false;
            options.type = std::atoi(value.c_str());
            if (value.size() != 1 || options.type < MessageType::MESSAGE_TYPE_1 || options.type > MessageType::MESSAGE_TYPE_4) {
                std::cerr << "edf2bin: the type must be 1, 2, 3 or 4" << std::endl;
                return false;
            }
        } else if (arg == "-o" || arg == "--output") {
            if (!next(options.output)) return false;
        } else if (metadata != nullptr) {
            if (!next(value)) return false;
            options.metadata.push_back({metadata, value});
        } else if (arg != "-" && startsWith(arg, "-")) {
            std::cerr << "edf2bin: unknown option " << arg << std::endl;
            return false;
        } else if (haveInput) {
            std::cerr << "edf2bin: only one input file can be given" << std::endl;
            return false;
        } else {
            options.input = arg;
            haveInput = true;
        }
    }
    return true;
}

/**
 * Returns true when a value fits a field of the message type. Every bit set
 * marks a missing value so it can not be used.
 */
bool fits(long long value, int field, int type) {
    std::array<int,2> range = XBTProfileDataRanges::getDataLocation(field, type);
    return value >= 0 && value <= (1LL << (range[1] - range[0] + 1)) - 2;
}

/**
 * Stores the metadata given on the command line in the profile. Returns
 * false and prints an error when a value is not valid or does not fit its
 * field.
 */
bool applyMetadata(XBTProfile &profile, const Options &options) {
    for (const auto &given : options.metadata) {
        const Metadata &m = *given.first;
        const std::string &value = given.second;
        std::string name = m.name;
        auto error = [&](const std::string &message) {
            std::cerr << "edf2bin: " << name << " " << message << std::endl;
            return false;
        };

        std::array<int,2> range = XBTProfileDataRanges::getDataLocation(m.field, options.type);
        if (range[0] == -1) {
            warn(options, name + " is not stored in a type " + std::to_string(options.type) + " message");
            continue;
        }

        if (m.kind == Metadata::TEXT) {
            int characters = (range[1] - range[0] + 1) / 8;
            if ((int) value.size() > characters) {
                return error("can hold at most " + std::to_string(characters) + " characters");
            }
            if (m.field == XBTProfileDataRanges::WMOID) profile.setWMOID(value);
            else if (m.field == XBTProfileDataRanges::SHIP_NAME) profile.setShipName(value);
            else if (m.field == XBTProfileDataRanges::SOOP_LINE) profile.setSoopLine(value);
            else if (m.field == XBTProfileDataRanges::XBT_RECORDER_SERIAL_NUMBER) profile.setXBTRecorderSerialNumber(value);
            continue;
        }

        if (m.kind == Metadata::RIDER) {
            // the count field gives the number of 5 character blocks
            int characters = 5 * ((1 << (range[1] - range[0] + 1)) - 1);
            if ((int) value.size() > characters) {
                return error("can hold at most " + std::to_string(characters) + " characters");
            }
            if (m.field == XBTProfileDataRanges::NUMBER_OF_RIDER_BLOCKS) profile.setRiderNames(value);
            else if (m.field == XBTProfileDataRanges::NUMBER_OF_RIDER_EMAIL_BLOCKS) profile.setRiderEmails(value);
            else if (m.field == XBTProfileDataRanges::NUMBER_OF_RIDER_INSTITUTION_BLOCKS) profile.setRiderInstitutions(value);
            else if (m.field == XBTProfileDataRanges::NUMBER_OF_RIDER_PHONE_BLOCKS) profile.setRiderPhones(value);
            continue;
        }

        if (m.kind == Metadata::DATE) {
            int year, month, day;
            char extra;
            if (std::sscanf(value.c_str(), "%d-%d-%d%c", &year, &month, &day, &extra) != 3
                    || month < 1 || month > 12 || day < 1 || day > 31 || year < 0
                    || !fits(year, m.field, options.type)) {
                return error("needs a date as YYYY-MM-DD, \"" + value + "\" is not valid");
            }
            if (m.field == XBTProfileDataRanges::XBT_RECORDER_MANUFACTURE_YEAR) {
                profile.setXBTRecorderManufacturedYear(year);
                profile.setXBTRecorderManufacturedMonth(month);
                profile.setXBTRecorderManufacturedDay(day);
            } else {
                profile.setXBTProbeManufacturedYear(year);
                profile.setXBTProbeManufacturedMonth(month);
                profile.setXBTProbeManufacturedDay(day);
            }
            continue;
        }

        // numbers
        char *end = nullptr;
        double number = std::strtod(value.c_str(), &end);
        if (end == value.c_str() || *end != '\0' || (m.kind == Metadata::INTEGER && number != std::floor(number))) {
            return error(std::string("needs ") + (m.kind == Metadata::INTEGER ? "a whole number" : "a number")
                    + ", \"" + value + "\" is not valid");
        }
        long long stored = std::llround(number * m.scale + m.offset);
        if (!fits(stored, m.field, options.type)) {
            double largest = (((1LL << (range[1] - range[0] + 1)) - 2) - m.offset) / m.scale;
            double smallest = -m.offset / m.scale;
            return error("must be from " + EDFFormat::formatNumber(smallest) + " to "
                    + EDFFormat::formatNumber(largest) + ", \"" + value + "\" does not fit");
        }

        switch (m.field) {
            case XBTProfileDataRanges::LLOYDS: profile.setLloyds((int) number); break;
            case XBTProfileDataRanges::TRANSECT_NUM: profile.setTransectNum((int) number); break;
            case XBTProfileDataRanges::SEQUENCE_NUM: profile.setSequenceNum((int) number); break;
            case XBTProfileDataRanges::SEAS_VERSION: profile.setSeasVersion((int) number); break;
            case XBTProfileDataRanges::DATA_QUALITY: profile.setDataQuality((int) number); break;
            case XBTProfileDataRanges::INSTRUMENT_TYPE: profile.setInstrumentType((int) number); break;
            case XBTProfileDataRanges::RECORDER_TYPE: profile.setRecorderType((int) number); break;
            case XBTProfileDataRanges::XBT_LAUNCHER_TYPE: profile.setXBTLauncherType((int) number); break;
            case XBTProfileDataRanges::LAUNCH_HEIGHT: profile.setLaunchHeight(number); break;
            case XBTProfileDataRanges::SHIP_DIRECTION: profile.setShipDirection(number); break;
            case XBTProfileDataRanges::SHIP_SPEED: profile.setShipSpeed(number); break;
            case XBTProfileDataRanges::WIND_INSTRUMENT_TYPE: profile.setWindInstrumentType((int) number); break;
            case XBTProfileDataRanges::WIND_DIRECTION: profile.setWindDirection(number); break;
            case XBTProfileDataRanges::WIND_SPEED: profile.setWindSpeed(number); break;
            case XBTProfileDataRanges::DRY_BULB_TEMPERATURE: profile.setDryBulbTemperature(number + 273.15); break;
            case XBTProfileDataRanges::SEA_SURFACE_CURRENT_MEASURING_METHOD: profile.setSeaSurfaceCurrentMeasurementMethod((int) number); break;
            case XBTProfileDataRanges::SEA_SURFACE_CURRENT_DIRECTION: profile.setSeaSurfaceCurrentDirection((int) number); break;
            case XBTProfileDataRanges::SEA_SURFACE_CURRENT_SPEED: profile.setSeaSurfaceCurrentSpeed(number); break;
            case XBTProfileDataRanges::TOTAL_WATER_DEPTH: profile.setTotalWaterDepth((int) number); break;
            case XBTProfileDataRanges::AGENCY_OWNER: profile.setAgencyOwner((int) number); break;
        }
    }
    return true;
}

} // namespace

int main(int argc, char *argv[]) {

    Options options;
    if (!parseArguments(argc, argv, options)) {
        usage(std::cerr);
        return 1;
    }

    std::vector<std::string> lines;
    if (options.input == "-") {
        if (isatty(fileno(stdin))) {
            usage(std::cerr);
            return 1;
        }
        lines = readLines(std::cin);
    } else {
        std::ifstream in(options.input, std::ios::binary);
        if (!in) {
            std::cerr << "edf2bin: can not open " << options.input << std::endl;
            return 1;
        }
        lines = readLines(in);
    }

    XBTProfile profile;
    profile.setOldMessageType(MessageType::OLD_MESSAGE_TYPE);
    profile.setNewMessageType(options.type);
    profile.setRecorderType(XBTRecorder::SIPPICAN_MK_21);
    profile.setThisDataIs(DataResolution::FULL_RESOLUTION);
    profile.setDataQuality(DataQuality::DATA_NOT_SUSPECT);

    if (!parseEDF(lines, profile, options)) {
        return 1;
    }
    // the command line values replace the ones from the EDF
    if (!applyMetadata(profile, options)) {
        return 1;
    }

    BinEncoder encoder(profile);

    if (options.output == "-") {
        if (isatty(fileno(stdout))) {
            std::cerr << "edf2bin: refusing to write binary data to a terminal, use -o or redirect the output" << std::endl;
            return 1;
        }
#ifdef _WIN32
        _setmode(_fileno(stdout), _O_BINARY);
#endif
        encoder.writeOutBin(std::cout);
        std::cout.flush();
        if (!std::cout) {
            std::cerr << "edf2bin: can not write to standard output" << std::endl;
            return 1;
        }
    } else {
        std::ofstream out(options.output, std::ios::binary);
        if (!out) {
            std::cerr << "edf2bin: can not create " << options.output << std::endl;
            return 1;
        }
        encoder.writeOutBin(out);
        if (!out) {
            std::cerr << "edf2bin: can not write " << options.output << std::endl;
            return 1;
        }
    }

    return 0;
}
