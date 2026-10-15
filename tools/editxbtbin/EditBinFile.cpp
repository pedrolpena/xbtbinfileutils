/**
 * EditBinFile – C++ port of the Java EditBinFile program.
 *
 * Reads an Amverseas XBT binary profile, optionally modifies fields
 * via command-line switches, and writes the result to a file or stdout.
 *
 * Compiles against the installed xbtbinfileutils C++ library.
 *
 * Supports reading from stdin (use "-i -") and writing to stdout
 * (omit -o, or use "-o -").  Pipeline example:
 *
 *     cat input.bin | ./EditBinFileCpp -i - [switches] -o - > output.bin
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <cstdio>
#include <cstdint>
#include <cmath>
#include <algorithm>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#endif

#include "BinDecoder.h"
#include "BinEncoder.h"
#include "XBTProfile.h"
#include "MessageType.h"
#include "XBTResistanceToTemperatureConverter.h"
#include "DepthCalculator.h"
#include "bitvector.h"

// ===================================================================
//  Minimal command-line parser (replaces Apache Commons CLI)
// ===================================================================

struct OptionDef {
    std::string name;
    bool hasArg;
    std::string description;
};

class CommandLine {
public:
    bool hasOption(const std::string& name) const {
        return values_.find(name) != values_.end();
    }
    std::string getOptionValue(const std::string& name) const {
        auto it = values_.find(name);
        if (it != values_.end()) return it->second;
        return "";
    }
    std::map<std::string, std::string> values_;
};

class OptionParser {
public:
    void addOption(const std::string& name, bool hasArg, const std::string& desc) {
        defs_.push_back({name, hasArg, desc});
    }

    CommandLine parse(int argc, char* argv[]) const {
        CommandLine cmd;
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            // Strip leading dashes
            while (!arg.empty() && arg[0] == '-') arg.erase(0, 1);
            if (arg.empty()) continue;

            const OptionDef* def = findDef(arg);
            if (!def) {
                throw std::runtime_error("Unknown option: " + std::string(argv[i]));
            }
            if (def->hasArg) {
                if (i + 1 >= argc) {
                    throw std::runtime_error("Option -" + arg + " requires a value");
                }
                cmd.values_[arg] = argv[++i];
            } else {
                cmd.values_[arg] = "true";
            }
        }
        return cmd;
    }

    void printHelp() const {
        for (const auto& d : defs_) {
            std::string left = "-" + d.name;
            while (left.size() < 15) left += ' ';
            std::cout << left << " " << d.description << "\n";
        }
    }

private:
    std::vector<OptionDef> defs_;

    const OptionDef* findDef(const std::string& name) const {
        for (const auto& d : defs_) {
            if (d.name == name) return &d;
        }
        return nullptr;
    }
};

// ===================================================================
//  Helper: split a string by a delimiter character
// ===================================================================
static std::vector<std::string> splitString(const std::string& s, char delim) {
    std::vector<std::string> tokens;
    std::istringstream stream(s);
    std::string token;
    while (std::getline(stream, token, delim))
        tokens.push_back(token);
    return tokens;
}

// ===================================================================
//  Stdin helper: read all bytes from stdin into memory.
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
//  True when stdin is an interactive terminal, i.e. nothing was piped
//  or redirected in, so reading it would wait for the user to type.
// ===================================================================
static bool stdinIsTerminal() {
#ifdef _WIN32
    return _isatty(_fileno(stdin)) != 0;
#else
    return isatty(fileno(stdin)) != 0;
#endif
}

// ===================================================================
//  Populate a BinDecoder from raw bytes entirely in memory.
//  Replicates the constructor's byte → bitvector → changeEndian
//  logic using the library's public `bits` member and public
//  changeEndian() method.  The decoder is constructed with an empty
//  path so its constructor skips the file read.
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
//  Temperature <-> resistance conversion for changing message types.
//  Types 1 to 3 store temperatures, type 4 stores resistances.
// ===================================================================

// Temperatures types 1 to 3 can store, (T * 100 + 400) in 12 bits
static constexpr double MIN_STORED_TEMPERATURE = -4.00;
static constexpr double MAX_STORED_TEMPERATURE = 36.94;

static std::vector<double> resistancesToTemperatures(const std::vector<double>& res) {
    XBTResistanceToTemperatureConverter converter;
    std::vector<double> temps;
    for (double r : res) {
        double t = r > 0 ? converter.convertResistaceToTemperature(r) : MAX_STORED_TEMPERATURE;
        temps.push_back(std::min(std::max(t, MIN_STORED_TEMPERATURE), MAX_STORED_TEMPERATURE));
    }
    return temps;
}

// Inverse of XBTResistanceToTemperatureConverter, solved with Newton's
// method on the same Steinhart-Hart coefficients.
static double temperatureToResistance(double temperature) {
    const double a = 0.001290123, b = 0.00023322529;
    const double c = 0.00000045791293, d = 0.000000071625593;
    double target = 1.0 / (temperature + 273.15);
    double x = std::log(10000.0);
    for (int i = 0; i < 50; ++i) {
        double f = a + b * x + c * x * x + d * x * x * x - target;
        double df = b + 2 * c * x + 3 * d * x * x;
        double step = f / df;
        x -= step;
        if (std::fabs(step) < 1e-12) break;
    }
    return std::exp(x);
}

static std::vector<double> temperaturesToResistances(const std::vector<double>& temps) {
    std::vector<double> res;
    for (double t : temps)
        res.push_back(temperatureToResistance(t));
    return res;
}

// ===================================================================
//  Stdout helper: write the encoded profile to stdout in binary mode.
// ===================================================================
static void writeEncoderToStdout(BinEncoder& encoder) {
#ifdef _WIN32
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    encoder.writeOutBin(std::cout);
    std::cout.flush();
    if (!std::cout)
        throw std::runtime_error("can not write to standard output");
}

// ===================================================================
//  Text output: the fields of XBTProfile::toString() plus the type 4
//  fields, printed the same way for every message type.  Values the
//  profile does not hold are printed as N/A.  The points are listed as
//  index, depth, resistance and temperature.
// ===================================================================
static const std::string NOT_AVAILABLE = "N/A";

static std::string intOrNA(int value) {
    return value == -999 ? NOT_AVAILABLE : std::to_string(value);
}

static std::string doubleOrNA(double value) {
    return value == -999 ? NOT_AVAILABLE : std::to_string(value);
}

static std::string hexString(int value) {
    char buf[20];
    std::snprintf(buf, sizeof(buf), "%X", static_cast<unsigned int>(value));
    return buf;
}

static std::string profileToString(XBTProfile& p) {
    // types 1 to 3 store temperatures only, so they have no resistances
    bool isType4 = p.getNewMessageType() == MessageType::MESSAGE_TYPE_4;
    std::vector<double> temps = p.getTemperaturePoints();
    std::vector<double> res = p.getResistancePoints();

    std::string str = "Callsign=" + p.getWMOId() + "\n"
        + "Old message type=" + std::to_string(p.getOldMessageType()) + "\n"
        + "New message type=" + std::to_string(p.getNewMessageType()) + "\n"
        + "Lat=" + std::to_string(p.getLatitude()) + "\n"
        + "Lon=" + std::to_string(p.getLongitude()) + "\n"
        + "Soopline=" + p.getSoopLine() + "\n"
        + "Transect#=" + std::to_string(p.getTransectNumber()) + "\n"
        + "Sequence#=" + std::to_string(p.getSequenceNumber()) + "\n"
        + "date=" + std::to_string(p.getMonth()) + "/" + std::to_string(p.getDay()) + "/" + std::to_string(p.getYear()) + "\n"
        + "time=" + std::to_string(p.getHour()) + ":" + std::to_string(p.getMinute()) + "\n"
        + "Second=" + intOrNA(p.getSecond()) + "\n"
        + "ShipName=" + p.getShipName() + "\n"
        + "Lloyds=" + std::to_string(p.getLloyds()) + "\n"
        + "Unique tag=" + hexString(p.getUniqueTag()) + "\n"
        + "SeasVersion=" + std::to_string(p.getSeasVersion()) + "\n"
        + "XBT Serial#=" + std::to_string(p.getProbeSerialNumber()) + "\n"
        + "ThisDatais=" + std::to_string(p.getThisDataIs()) + "\n"
        + "DataQuality=" + std::to_string(p.getDataQuality()) + "\n"
        + "LauncherHeight=" + std::to_string(p.getLaunchHeight()) + "\n"
        + "Ship direction=" + std::to_string(p.getShipDirection()) + "\n"
        + "Ship Speed=" + std::to_string(p.getShipSpeed()) + "\n"
        + "Instrument type=" + std::to_string(p.getInstrumentType()) + "\n"
        + "Recorder type=" + std::to_string(p.getRecorderType()) + "\n"
        + "Wind Instrument type=" + std::to_string(p.getWindInstrumentType()) + "\n"
        + "Wind Direction=" + std::to_string(p.getWindDirection()) + "\n"
        + "Wind Speed=" + std::to_string(p.getWindSpeed()) + "\n"
        + "Dry bulb temp=" + std::to_string(p.getDryBulbTemperature()) + "\n"
        + "Measure method=" + std::to_string(p.getSeaSurfaceCurrentMeasurementMethod()) + "\n"
        + "Current Dir=" + std::to_string(p.getSeaSurfaceCurrentDirection()) + "\n"
        + "Current Speed=" + std::to_string(p.getSeaSurfaceCurrentSpeed()) + "\n"
        + "Total water depth=" + std::to_string(p.getTotalWaterDepth()) + "\n"
        + "Agency Owner=" + std::to_string(p.getAgencyOwner()) + "\n"
        + "XBT launcher type=" + std::to_string(p.getXBTLauncherType()) + "\n"
        + "XBT Recorder Serial#=" + p.getXBTRecorderSerialNumber() + "\n"
        + "XBT Recorder Manufacture date=" + std::to_string(p.getXBTRecorderManufacturedMonth()) + "/" + std::to_string(p.getXBTRecorderManufacturedDay()) + "/" + std::to_string(p.getXBTRecorderManufacturedYear()) + "\n"
        + "XBT Probe Manufacture date=" + std::to_string(p.getXBTProbeManufacturedMonth()) + "/" + std::to_string(p.getXBTProbeManufacturedDay()) + "/" + std::to_string(p.getXBTProbeManufacturedYear()) + "\n"
        + "# of rider blocks=" + std::to_string(p.getNumberOfRiderBlocks()) + "\n"
        + "# of rider email blocks=" + std::to_string(p.getNumberOfRiderEmailBlocks()) + "\n"
        + "# of rider phone blocks=" + std::to_string(p.getNumberOfRiderPhoneBlocks()) + "\n"
        + "# of rider inst blocks=" + std::to_string(p.getNumberOfRiderInstitutionBlocks()) + "\n"
        + "# of rep fields=" + std::to_string(p.getNumberOfRepeatedFields()) + "\n"
        + "Times replicated=" + std::to_string(p.getTimesReplicated()) + "\n"
        + "Sea Temperature=" + std::to_string(p.getSeaSurfaceTemperature()) + "\n"
        + "Sea Resistance=" + (isType4 ? doubleOrNA(p.getSeaSurfaceResistance()) : NOT_AVAILABLE) + "\n"
        + "SeaDepth=" + std::to_string(p.getSeaDepth()) + "\n"
        + "Salinity=" + doubleOrNA(p.getSalinity()) + "\n"
        + "Rider=" + p.getRiderNames() + "\n"
        + "Email=" + p.getRiderEmails() + "\n"
        + "Phone=" + p.getRiderPhones() + "\n"
        + "Institution=" + p.getRiderInstitutions() + "\n";

    // one line per EDF header line, as the text can hold several
    auto edfLines = splitString(p.getEDFText(), '\n');
    if (edfLines.empty())
        str += "EDF text=" + NOT_AVAILABLE + "\n";
    for (const auto& edfLine : edfLines)
        str += "EDF text=" + edfLine + "\n";

    // the fall rate coefficients are zero when the probe type is unknown
    DepthCalculator depthCalculator(p);
    bool hasDepths = depthCalculator.A != 0.0 || depthCalculator.B != 0.0;

    str += "index,depth,resistance,temperature\n";
    for (size_t i = 0; i < temps.size(); ++i) {
        str += std::to_string(i) + ","
            + (hasDepths ? std::to_string(depthCalculator.getMeasurementDepth(static_cast<int>(i))) : NOT_AVAILABLE) + ","
            + (isType4 && i < res.size() ? std::to_string(res[i]) : NOT_AVAILABLE) + ","
            + std::to_string(temps[i]) + "\n";
    }
    return str;
}

// ===================================================================
//  main
// ===================================================================
int main(int argc, char* argv[]) {

    OptionParser options;
    options.addOption("help",           false, "this screen.");
    options.addOption("i",              true,  "input file (use - for stdin).");
    options.addOption("o",              true,  "output file name (use - for stdout).");
    options.addOption("callsign",       true,  "the ship's callsign.");
    options.addOption("messagetype",    true,  "a number denoting the bin file's message type.");
    options.addOption("latitude",       true,  "latitude in decimal degrees.");
    options.addOption("longitude",      true,  "longitude in decimal degrees.");
    options.addOption("transectname",   true,  "the name given to the transect. e.g. AX07");
    options.addOption("transectnum",    true,  "the number denoting which transect in the year.");
    options.addOption("sequencenum",    true,  "the number denoting which xbt in the cruise.");
    options.addOption("year",           true,  "four-digit year the probe was used.");
    options.addOption("month",          true,  "two-digit month the probe was used.");
    options.addOption("day",            true,  "two-digit day the probe was used.");
    options.addOption("hour",           true,  "two-digit hour the probe was used.");
    options.addOption("minute",         true,  "two-digit minute the probe was used.");
    options.addOption("shipname",       true,  "the ship's name.");
    options.addOption("imo",            true,  "the ship's IMO number.");
    options.addOption("seasversion",    true,  "three-digit Amverseas version.");
    options.addOption("probeserial",    true,  "the XBT's serial number.");
    options.addOption("probeyear",      true,  "four-digit year the probe was manufactured.");
    options.addOption("probemonth",     true,  "two-digit month the probe was manufactured.");
    options.addOption("probeday",       true,  "two-digit day the probe was manufactured.");
    options.addOption("datatype",       true,  "a number denoting the type of data.");
    options.addOption("dataquality",    true,  "a number denoting data quality.");
    options.addOption("height",         true,  "launch height in meters.");
    options.addOption("shipdir",        true,  "ship direction in degrees.");
    options.addOption("shipspeed",      true,  "ship speed in m/s.");
    options.addOption("probetype",      true,  "a number denoting XBT type.");
    options.addOption("recorder",       true,  "a number denoting recorder type.");
    options.addOption("windtype",       true,  "wind instrument type.");
    options.addOption("winddir",        true,  "wind direction.");
    options.addOption("windspeed",      true,  "wind speed in m/s.");
    options.addOption("drybulbtemp",    true,  "dry bulb temperature in K.");
    options.addOption("currentmethod",  true,  "sea surface current measurement method.");
    options.addOption("currentdir",     true,  "sea surface current direction.");
    options.addOption("currentspeed",   true,  "sea surface current speed.");
    options.addOption("launcher",       true,  "launcher type number.");
    options.addOption("recorderserial", true,  "recorder serial number.");
    options.addOption("recorderyear",   true,  "four-digit recorder manufacture year.");
    options.addOption("recordermonth",  true,  "two-digit recorder manufacture month.");
    options.addOption("recorderday",    true,  "two-digit recorder manufacture day.");
    options.addOption("agency",         true,  "agency code.");
    options.addOption("rider",          true,  "rider name(s).");
    options.addOption("riderphone",     true,  "rider phone number(s).");
    options.addOption("rideremail",     true,  "rider email(s).");
    options.addOption("institution",    true,  "rider institution(s).");
    options.addOption("depth",          true,  "total water depth.");
    options.addOption("temps",          true,  "temperature points delimited by \"@\"");
    options.addOption("res",            true,  "resistance points delimited by \"@\"");
    options.addOption("second",         true,  "two-digit second the probe was used (type 4 only).");
    options.addOption("salinity",       true,  "salinity in PSU (type 4 only).");
    options.addOption("edftext",        true,  "EDF header lines not held by other fields (type 4 only).");

    try {
        CommandLine line = options.parse(argc, argv);

        // --- help ---
        if (line.hasOption("help")) {
            options.printHelp();
            return 0;
        }

        // --- input ---
        XBTProfile xBTProfileOut;
        bool readFromFile = false;  // tracks whether -i was given with a filename

        // Reading stdin from a terminal would hang waiting for input, so
        // show the options and the Java error instead.
        if ((!line.hasOption("i") || line.getOptionValue("i") == "-") && stdinIsTerminal()) {
            options.printHelp();
            throw std::runtime_error("must provide an input file. use -i filename");
        }

        if (line.hasOption("i")) {
            std::string inFile = line.getOptionValue("i");

            if (inFile == "-") {
                // Read stdin into memory, populate decoder directly
                auto bytes = readStdinBytes();
                BinDecoder decodedXBTProfile("");
                decoderFromBytes(decodedXBTProfile, bytes);
                decodedXBTProfile.decodeXBTProfile(xBTProfileOut);
            } else {
                readFromFile = true;
                BinDecoder decodedXBTProfile(inFile);
                decodedXBTProfile.decodeXBTProfile(xBTProfileOut);
            }

        } else {
            // No -i given: read from stdin (supports bare piping)
            auto bytes = readStdinBytes();
            BinDecoder decodedXBTProfile("");
            decoderFromBytes(decodedXBTProfile, bytes);
            decodedXBTProfile.decodeXBTProfile(xBTProfileOut);
        }

        // --- apply edits (same order as Java EditBinFile) ---

        if (line.hasOption("callsign"))
            xBTProfileOut.setCallsign(line.getOptionValue("callsign"));

        if (line.hasOption("messagetype")) {
            int oldType = xBTProfileOut.getNewMessageType();
            int newType = std::stoi(line.getOptionValue("messagetype"));
            // getTemperaturePoints() converts the resistances of a type 4
            // profile, so read the points before the type changes.
            std::vector<double> temps = xBTProfileOut.getTemperaturePoints();
            std::vector<double> res = xBTProfileOut.getResistancePoints();
            xBTProfileOut.setNewMessageType(newType);

            if (oldType == MessageType::MESSAGE_TYPE_4 && newType != MessageType::MESSAGE_TYPE_4)
                xBTProfileOut.setTemperaturePoints(resistancesToTemperatures(res));
            else if (oldType != MessageType::MESSAGE_TYPE_4 && newType == MessageType::MESSAGE_TYPE_4)
                xBTProfileOut.setResistancePoints(temperaturesToResistances(temps));
        }

        if (line.hasOption("latitude"))
            xBTProfileOut.setLatitude(std::stod(line.getOptionValue("latitude")));

        if (line.hasOption("longitude"))
            xBTProfileOut.setLongitude(std::stod(line.getOptionValue("longitude")));

        if (line.hasOption("transectname"))
            xBTProfileOut.setSoopLine(line.getOptionValue("transectname"));

        if (line.hasOption("transectnum"))
            xBTProfileOut.setTransectNum(std::stoi(line.getOptionValue("transectnum")));

        if (line.hasOption("sequencenum"))
            xBTProfileOut.setSequenceNum(std::stoi(line.getOptionValue("sequencenum")));

        if (line.hasOption("year"))
            xBTProfileOut.setYear(std::stoi(line.getOptionValue("year")));

        if (line.hasOption("month"))
            xBTProfileOut.setMonth(std::stoi(line.getOptionValue("month")));

        if (line.hasOption("day"))
            xBTProfileOut.setDay(std::stoi(line.getOptionValue("day")));

        if (line.hasOption("hour"))
            xBTProfileOut.setHour(std::stoi(line.getOptionValue("hour")));

        if (line.hasOption("minute"))
            xBTProfileOut.setMinute(std::stoi(line.getOptionValue("minute")));

        if (line.hasOption("shipname"))
            xBTProfileOut.setShipName(line.getOptionValue("shipname"));

        if (line.hasOption("imo"))
            xBTProfileOut.setLloyds(std::stoi(line.getOptionValue("imo")));

        if (line.hasOption("seasversion"))
            xBTProfileOut.setSeasVersion(std::stoi(line.getOptionValue("seasversion")));

        if (line.hasOption("probeserial"))
            xBTProfileOut.setProbeSerialNumber(std::stoi(line.getOptionValue("probeserial")));

        if (line.hasOption("probeyear"))
            xBTProfileOut.setXBTProbeManufacturedYear(std::stoi(line.getOptionValue("probeyear")));

        if (line.hasOption("probemonth"))
            xBTProfileOut.setXBTProbeManufacturedMonth(std::stoi(line.getOptionValue("probemonth")));

        if (line.hasOption("probeday"))
            xBTProfileOut.setXBTProbeManufacturedDay(std::stoi(line.getOptionValue("probeday")));

        if (line.hasOption("datatype"))
            xBTProfileOut.setThisDataIs(std::stoi(line.getOptionValue("datatype")));

        if (line.hasOption("dataquality"))
            xBTProfileOut.setDataQuality(std::stoi(line.getOptionValue("dataquality")));

        if (line.hasOption("height"))
            xBTProfileOut.setLaunchHeight(std::stod(line.getOptionValue("height")));

        if (line.hasOption("shipdir"))
            xBTProfileOut.setShipDirection(std::stod(line.getOptionValue("shipdir")));

        if (line.hasOption("shipspeed"))
            xBTProfileOut.setShipSpeed(std::stod(line.getOptionValue("shipspeed")));

        if (line.hasOption("probetype"))
            xBTProfileOut.setInstrumentType(std::stoi(line.getOptionValue("probetype")));

        if (line.hasOption("recorder"))
            xBTProfileOut.setRecorderType(std::stoi(line.getOptionValue("recorder")));

        if (line.hasOption("windtype"))
            xBTProfileOut.setWindInstrumentType(std::stoi(line.getOptionValue("windtype")));

        if (line.hasOption("winddir"))
            xBTProfileOut.setWindDirection(std::stod(line.getOptionValue("winddir")));

        if (line.hasOption("windspeed"))
            xBTProfileOut.setWindSpeed(std::stod(line.getOptionValue("windspeed")));

        if (line.hasOption("drybulbtemp"))
            xBTProfileOut.setDryBulbTemperature(std::stod(line.getOptionValue("drybulbtemp")));

        if (line.hasOption("currentmethod"))
            xBTProfileOut.setSeaSurfaceCurrentMeasurementMethod(
                std::stoi(line.getOptionValue("currentmethod")));

        if (line.hasOption("currentdir"))
            xBTProfileOut.setSeaSurfaceCurrentDirection(
                std::stoi(line.getOptionValue("currentdir")));

        if (line.hasOption("currentspeed"))
            xBTProfileOut.setSeaSurfaceCurrentSpeed(
                std::stod(line.getOptionValue("currentspeed")));

        if (line.hasOption("launcher"))
            xBTProfileOut.setXBTLauncherType(std::stoi(line.getOptionValue("launcher")));

        if (line.hasOption("recorderserial"))
            xBTProfileOut.setXBTRecorderSerialNumber(line.getOptionValue("recorderserial"));

        if (line.hasOption("recorderyear"))
            xBTProfileOut.setXBTRecorderManufacturedYear(
                std::stoi(line.getOptionValue("recorderyear")));

        if (line.hasOption("recordermonth"))
            xBTProfileOut.setXBTRecorderManufacturedMonth(
                std::stoi(line.getOptionValue("recordermonth")));

        if (line.hasOption("recorderday"))
            xBTProfileOut.setXBTRecorderManufacturedDay(
                std::stoi(line.getOptionValue("recorderday")));

        if (line.hasOption("agency"))
            xBTProfileOut.setAgencyOwner(std::stoi(line.getOptionValue("agency")));

        if (line.hasOption("rider"))
            xBTProfileOut.setRiderNames(line.getOptionValue("rider"));

        if (line.hasOption("riderphone"))
            xBTProfileOut.setRiderPhones(line.getOptionValue("riderphone"));

        if (line.hasOption("rideremail"))
            xBTProfileOut.setRiderEmails(line.getOptionValue("rideremail"));

        if (line.hasOption("institution"))
            xBTProfileOut.setRiderInstitutions(line.getOptionValue("institution"));

        if (line.hasOption("depth"))
            xBTProfileOut.setTotalWaterDepth(std::stoi(line.getOptionValue("depth")));

        if (line.hasOption("second"))
            xBTProfileOut.setSecond(std::stoi(line.getOptionValue("second")));

        if (line.hasOption("salinity"))
            xBTProfileOut.setSalinity(std::stod(line.getOptionValue("salinity")));

        if (line.hasOption("edftext"))
            xBTProfileOut.setEDFText(line.getOptionValue("edftext"));

        // --- temps ---
        if (line.hasOption("temps")) {
            auto parts = splitString(line.getOptionValue("temps"), '@');
            std::vector<double> doubleTemps;

            if (!parts.empty() && parts[0] != "null") {
                for (const auto& s : parts)
                    doubleTemps.push_back(std::stod(s));
            }

            // Type 4 stores resistances, so convert the temperatures
            if (xBTProfileOut.getNewMessageType() == MessageType::MESSAGE_TYPE_4)
                xBTProfileOut.setResistancePoints(temperaturesToResistances(doubleTemps));
            else
                xBTProfileOut.setTemperaturePoints(doubleTemps);
        }

        // --- res ---
        if (line.hasOption("res")) {
            auto parts = splitString(line.getOptionValue("res"), '@');
            std::vector<double> doubleRes;

            if (!parts.empty() && parts[0] != "null") {
                for (const auto& s : parts) {
                    double v = std::stod(s);
                    if (v < 3199.61) v = 3199.61;
                    doubleRes.push_back(v);
                }
            }

            xBTProfileOut.setNewMessageType(MessageType::MESSAGE_TYPE_4);
            xBTProfileOut.setResistancePoints(doubleRes);
        }

        // --- output ---
        if (line.hasOption("o")) {
            std::string outFile = line.getOptionValue("o");

            if (outFile == "-") {
                // Encode and write to stdout
                BinEncoder encodedXBTProfile(xBTProfileOut);
                writeEncoderToStdout(encodedXBTProfile);
            } else {
                BinEncoder encodedXBTProfile(xBTProfileOut);
                std::ofstream out(outFile, std::ios::binary);
                if (!out)
                    throw std::runtime_error("can not create " + outFile);
                encodedXBTProfile.writeOutBin(out);
                if (!out)
                    throw std::runtime_error("can not write " + outFile);
            }
        } else if (readFromFile) {
            // -i <file> with no -o: print profile text (matches Java)
            std::cout << profileToString(xBTProfileOut);
        } else {
            // Piping (no -i, no -o): write binary to stdout
            BinEncoder encodedXBTProfile(xBTProfileOut);
            writeEncoderToStdout(encodedXBTProfile);
        }

    } catch (const std::exception& e) {
        std::cerr << "Parsing failed.  Reason: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
