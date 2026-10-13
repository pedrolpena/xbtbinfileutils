/*
 * FormatConverter.cpp
 *
 *  Created on: Aug 11, 2019
 *      Author: pedro
 */

#include "FormatConverter.h"

FormatConverter::FormatConverter(XBTProfile &xBTprofile) {
	this->xBTprofile = &xBTprofile;

}

FormatConverter::~FormatConverter() {
	// TODO Auto-generated destructor stub
}

std::string FormatConverter::decimalDegreesLatToDMSAOML(double lat) {

    double tmp = std::abs(lat);
    int x = (int) tmp;
    double y = 60 * (tmp - (double) x);
    std::string dir = "N";
    if (lat < 0) {
        dir = "S";
    }//end if
    char buf[18];
    sprintf(buf,"%02d %05.2f " ,x ,y);
    std::string out = buf + dir;
    return out;
}

std::string FormatConverter::decimalDegreesLonToDMSAOML(double lon) {

    double tmp = std::abs(lon);
    int x = (int) tmp;
    double y = 60 * (tmp - (double) x);
    std::string dir = "E";
    if (lon < 0) {
        dir = "W";
    }//end if
    char buf[18];
    sprintf(buf,"%03d %05.2f " ,x ,y);
    std::string out = buf + dir;
    return out;



}

std::string FormatConverter::decimalDegreesLatToDMSEDF(double lat) {

    double tmp = std::abs(lat);
    int x = (int) tmp;
    double y = 60 * (tmp - (double) x);
    std::string dir = "N";
    if (lat < 0) {
        dir = "S";
    }//end if
    char buf[19];
    sprintf(buf,"%02d %07.4f" ,x ,y);
    std::string out = buf + dir;
    return out;

}

std::string FormatConverter::decimalDegreesLonToDMSEDF(double lon) {

    double tmp = std::abs(lon);
    int x = (int) tmp;
    double y = 60 * (tmp - (double) x);
    std::string dir = "E";
    if (lon < 0) {
        dir = "W";
    }//end if
    char buf[19];
    sprintf(buf,"%03d %07.4f" ,x ,y);
    std::string out = buf + dir;
    return out;
}




/**
 * This method returns a String containing the xBTprofile information using
 * the SEAS ASCII format.
 *
 * @return This method returns a String containing the xBTprofile
 * information using the SEAS ASCII format.
 */
std::string FormatConverter::getASCII() {
    std::string tmp = "";
    std::vector<std::vector<double>> depthsAndTemps;  //array of doubles that holds depths & temperature measurements
    DepthCalculator *dc = new DepthCalculator(*xBTprofile);
    depthsAndTemps = dc->getDepthsAndTemperaturePoints();
    //char buf[120];

    tmp += "Call Sign                      | " + xBTprofile->getWMOId() + "\r\n";
    tmp += "Latitude                       | " + decimalDegreesLatToDMSAOML(xBTprofile->getLatitude()) + "\r\n";
    tmp += "Longitude                      | " + decimalDegreesLonToDMSAOML(xBTprofile->getLongitude()) + "\r\n";
    tmp += "Transect Name                  | " + xBTprofile->getSoopLine() + "\r\n";
    tmp += "Transect Number                | " + std::to_string(xBTprofile->getTransectNumber()) + "\r\n";
    tmp += "Sequence Number                | " + std::to_string(xBTprofile->getSequenceNumber()) + "\r\n";
    sprintf(buf,"%02d", xBTprofile->getYear());
    tmp += "Year                           | " + std::string(buf) + "\r\n";
    sprintf(buf,"%02d", xBTprofile->getMonth());
    tmp += "Month                          | " + std::string(buf) + "\r\n";
    sprintf(buf,"%02d", xBTprofile->getDay());
    tmp += "Day                            | " + std::string(buf) + "\r\n";
    sprintf(buf,"%02d", xBTprofile->getHour());
    tmp += "Hour                           | " + std::string(buf) + "\r\n";
    sprintf(buf,"%02d", xBTprofile->getMinute());
    tmp += "Minute                         | " + std::string(buf) + "\r\n";
    tmp += "Ship Name                      | " + xBTprofile->getShipName() + "\r\n";
    tmp += "IMO Number                     | " + std::to_string(xBTprofile->getLloyds()) + "\r\n";
    sprintf(buf,"%x", xBTprofile->getUniqueTag());
    std::string s = std::string(buf);

    for(unsigned int i =0; i < s.size() ; i++) {
		s[i]=std::toupper(s[i]);
	}

    tmp += "SEAS ID                        | " + s + "\r\n";
    tmp += "SEAS Version                   | " + std::to_string(xBTprofile->getSeasVersion()) + "\r\n";
    tmp += "Probe Serial Number            | " + std::to_string(xBTprofile->getProbeSerialNumber()) + "\r\n";
    sprintf(buf,"%02d/%02d/%04d\r\n",
            xBTprofile->getXBTProbeManufacturedMonth(),
            xBTprofile->getXBTProbeManufacturedDay(),
            xBTprofile->getXBTProbeManufacturedYear());
    tmp += "Probe Manufacture Date         | " + std::string(buf);
    tmp += "Data Type                      | " + std::to_string(xBTprofile->getThisDataIs()) + " (" + DataResolution::getDataResolutionDescription(xBTprofile->getThisDataIs()) + ")\r\n";
    tmp += "Data Quality                   | " + std::to_string(xBTprofile->getDataQuality()) + " (" + DataQuality::getDataQualityDescription(xBTprofile->getDataQuality()) + ")\r\n";
    sprintf(buf,"%4.2f", xBTprofile->getLaunchHeight());
    tmp += "Deployment Height (meters)     | " + std::string(buf) + "\r\n";
    sprintf(buf,"%03d", (int) xBTprofile->getShipDirection());
    tmp += "Ship Direction                 | " + std::string(buf) + "\r\n";
    sprintf(buf,"%05.2f", xBTprofile->getShipSpeed() * 1.94384);
    tmp += "Ship Speed (knots)             | " +  std::string(buf) + "\r\n";
    tmp += "Instrument Type                | " + XBTProbe::getProbeDescription(xBTprofile->getInstrumentType())
            + " (Code " + std::to_string(xBTprofile->getInstrumentType()) + ")\r\n";
    tmp += "Recorder Type                  | " + XBTRecorder::getRecorderDescription(xBTprofile->getRecorderType())
            + " (Code " + std::to_string(xBTprofile->getRecorderType()) + ")\r\n";
    tmp += "Wind Instrument Type           | " + std::to_string(xBTprofile->getWindInstrumentType()) + "\r\n";
    sprintf(buf,"%04.1f",  xBTprofile->getWindDirection());
    tmp += "Wind Direction                 | " + std::string(buf) + "\r\n";
    sprintf(buf,"%04.1f",  xBTprofile->getWindSpeed());
    tmp += "Wind Speed (knots)             | " + std::string(buf) + "\r\n";
    sprintf(buf,"%05.2f", xBTprofile->getDryBulbTemperature() - 273.15);
    tmp += "Dry Bulb Temperature (celsius) | " + std::string(buf) + "\r\n";
    tmp += "Current Measurement Method     | " + std::to_string(xBTprofile->getSeaSurfaceCurrentMeasurementMethod()) + "\r\n";
    tmp += "Current Direction              | " + std::to_string(xBTprofile->getSeaSurfaceCurrentDirection()) + "\r\n";
    sprintf(buf,"%2.2f",  xBTprofile->getSeaSurfaceCurrentSpeed());
    tmp += "Current Speed (knots)          | " + std::string(buf) + "\r\n";
    tmp += "Total Water Depth (meters)     | " + std::to_string(xBTprofile->getTotalWaterDepth()) + "\r\n";
    tmp += "XBT Launcher Type              | " + Launchers::getLauncherDescription(xBTprofile->getXBTLauncherType()) + "\r\n";
    tmp += "XBT Recorder Serial Number     | " + xBTprofile->getXBTRecorderSerialNumber() + "\r\n";
    sprintf(buf,"%02d/%02d/%04d\r\n",
            xBTprofile->getXBTRecorderManufacturedMonth(),
            xBTprofile->getXBTRecorderManufacturedDay(),
            xBTprofile->getXBTRecorderManufacturedYear());
    tmp += "XBT Recorder Manufacture Date  | " + std::string(buf);
    tmp += "Agency in charge of Operation  | " + Agency::getAgencyDescription(xBTprofile->getAgencyOwner()) + "\r\n";
    tmp += "Ship Rider                     | " + xBTprofile->getRiderNames() + "\r\n";
    tmp += "Ship Rider Institution         | " + xBTprofile->getRiderInstitutions() + "\r\n";
    tmp += "Ship Rider Email               | " + xBTprofile->getRiderEmails() + "\r\n";
    tmp += "Ship Rider Telephone Number    | " + xBTprofile->getRiderPhones() + "\r\n";
    tmp += "===================================================================\r\n";
    tmp += "Full Resolution Values\r\n";
    tmp += "   Depth       Temperature        \r\n";


    for (std::vector<double> depthsAndTemp : depthsAndTemps) {
    	double depth = depthsAndTemp[0];
    	sprintf(buf," %7.2f       %5.2f\r\n", depth, depthsAndTemp[1]);
    	tmp+= buf;
    } //end for

    delete dc;
    return tmp;

}


/**
 * This method returns a String containing the xBTprofile information emulating
 * the Sippican Export Data Format (EDF).
 *
 * @param fileName The filename to display.
 * @param salinity The salinty of the area where the measurement was made.
 * @return This method returns a String containing the xBTprofile
 * information emulating the Sippican Export Data Format (EDF).
 */
std::string FormatConverter::getASCIIEDF(std::string fileName, double salinity) {

    // Type 4 profiles made from an EDF file hold the header lines of that
    // file and the salinity.
    if (xBTprofile->getNewMessageType() == MessageType::MESSAGE_TYPE_4 && !xBTprofile->getEDFText().empty()) {
        XBTProfile p = *xBTprofile;
        if (p.getSalinity() == -999) {
            p.setSalinity(salinity);
        }
        return writeEDF(p, fileName);
    }

    // Other profiles get the values a MK21 would have used.
    XBTProfile p = *xBTprofile;
    p.setSalinity(salinity);
    std::string text = "";
    text += "System Information\tUnits            :  Metric\n";
    text += "Memo\t" + p.getShipName() + "\n";
    text += "Post-Processing\tOperations       :  None\n";
    p.setEDFText(text);

    return writeEDF(p, fileName);
}

namespace {

/**
 * Formats a position the way the MK21 does, degrees followed by minutes
 * with 5 decimals and the hemisphere, e.g. 44 50.67871N.
 */
std::string formatPosition(double degrees, char positive, char negative) {
    long long total = std::llround(std::fabs(degrees) * 60.0 * 100000.0);
    char buf[64];
    snprintf(buf, sizeof(buf), "%lld %lld.%05lld%c", total / 6000000, (total % 6000000) / 100000,
            total % 100000, degrees < 0 ? negative : positive);
    return std::string(buf);
}

/**
 * Returns the stored EDF lines of a section, every line ends with CR LF.
 * Every stored line is the section name, a tab and the line.
 */
std::string getLines(const std::string &edfText, const std::string &section) {
    std::string lines = "";
    std::stringstream ss(edfText);
    std::string line;
    while (std::getline(ss, line)) {
        size_t tab = line.find('\t');
        if (tab != std::string::npos && line.compare(0, tab, section) == 0) {
            lines += line.substr(tab + 1) + "\r\n";
        }
    }
    return lines;
}

/**
 * Returns true when a section of the stored EDF lines has a line that
 * starts with the given key.
 */
bool hasLine(const std::string &edfText, const std::string &section, const std::string &key) {
    std::stringstream ss(edfText);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.compare(0, section.size() + 1, section + "\t") == 0
                && line.compare(section.size() + 1, key.size(), key) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Returns the key of an EDF line, the text in front of the colon without
 * the spaces that line up the values.
 */
std::string getKey(const std::string &line) {
    std::string key = line.substr(0, line.find(':'));
    while (!key.empty() && key.back() == ' ') {
        key.pop_back();
    }
    return key;
}

/**
 * Returns the number of stored lines of a section.
 */
int countLines(const std::string &edfText, const std::string &section) {
    int count = 0;
    std::stringstream ss(edfText);
    std::string line;
    while (std::getline(ss, line)) {
        if (line.compare(0, section.size() + 1, section + "\t") == 0) {
            count++;
        }
    }
    return count;
}

/**
 * Returns true when a value is missing, either -999 or every bit of the
 * field set, which is how Amverseas marks a missing value.
 */
bool isMissing(int value, int field, int messageType) {
    if (value == -999) {
        return true;
    }
    std::array<int,2> range = XBTProfileDataRanges::getDataLocation(field, messageType);
    if (range[0] < 0 || range[1] < range[0] || range[1] - range[0] >= 31) {
        return false;
    }
    return value == (1 << (range[1] - range[0] + 1)) - 1;
}

} // namespace

/**
 * This method writes a profile as an AOML export data file (EDF), which
 * has the layout of the files written by the MK21 software. The lines that
 * can be made from the fields of the profile are made from them, e.g. the
 * date, the position and the probe information. The other lines come from
 * the EDF text of the profile. A stored probe or serial number line
 * replaces the line made from the fields. Depth, temperature and sound
 * velocity are computed from the resistance points or, when there are
 * none, from the temperature points.
 *
 * @param profile the profile to write
 * @param fileName the binary file the profile came from, written as the raw
 * data filename
 * @return the EDF file
 */
std::string FormatConverter::writeEDF(XBTProfile &profile, std::string fileName) {
    XBTProfile *p = &profile;
    std::string tmp = "";
    std::string text = p->getEDFText();
    char b[256];

    double frequency = XBTRecorder::getRecorderFrequency(p->getRecorderType());
    double salinity = p->getSalinity() == -999 ? 35.0 : p->getSalinity();
    std::vector<double> resistances = p->getResistancePoints();
    std::vector<int> fields = EDFFormat::getDataFields(!resistances.empty());

    // The probe lines come from the probe table, a stored line replaces the
    // line with the same key, e.g. a terminal depth set for the launch.
    std::vector<std::string> stored;
    std::stringstream probeStored(getLines(text, "Probe Information"));
    std::string storedLine;
    while (std::getline(probeStored, storedLine)) {
        stored.push_back(storedLine.substr(0, storedLine.find('\r')));
    }
    std::vector<std::string> keys;
    std::string probeLines = "";
    for (const std::string &line : EDFFormat::getProbeLines(p->getInstrumentType())) {
        std::string out = line;
        for (const std::string &s : stored) {
            if (getKey(s) == getKey(line)) {
                out = s;
            }
        }
        keys.push_back(getKey(line));
        probeLines += out + "\r\n";
    }
    // the other stored lines of the section, e.g. the pressure point correction
    for (const std::string &s : stored) {
        if (std::find(keys.begin(), keys.end(), getKey(s)) == keys.end()) {
            probeLines += s + "\r\n";
        }
    }
    double coefficientA = XBTProbe::getCoefficientA(p->getInstrumentType());
    double coefficientB = XBTProbe::getCoefficientB(p->getInstrumentType()) * .001;
    // the depths use the coefficients of a stored probe block
    std::stringstream probeText(getLines(text, "Probe Information"));
    std::string probeLine;
    while (std::getline(probeText, probeLine)) {
        if (probeLine.compare(0, 14, "Depth Coeff. 2") == 0) {
            coefficientA = std::atof(probeLine.substr(probeLine.find(':') + 1).c_str());
        } else if (probeLine.compare(0, 14, "Depth Coeff. 3") == 0) {
            coefficientB = std::atof(probeLine.substr(probeLine.find(':') + 1).c_str());
        }
    }

    snprintf(b, sizeof(b), "%02d/%02d/%04d", p->getMonth(), p->getDay(), p->getYear());
    std::string date = b;
    snprintf(b, sizeof(b), "%02d:%02d:%02d", p->getHour(), p->getMinute(), p->getSecond() == -999 ? 0 : p->getSecond());
    std::string time = b;
    std::string latitude = formatPosition(p->getLatitude(), 'N', 'S');
    std::string longitude = formatPosition(p->getLongitude(), 'E', 'W');
    // missing values are left empty
    std::string sequenceNumber = "";
    if (!isMissing(p->getSequenceNumber(), XBTProfileDataRanges::SEQUENCE_NUM, p->getNewMessageType())) {
        sequenceNumber = std::to_string(p->getSequenceNumber());
    }
    std::string serialNumber = "";
    if (!isMissing(p->getProbeSerialNumber(), XBTProfileDataRanges::PROBE_SERIAL_NUMBER, p->getNewMessageType())) {
        serialNumber = EDFFormat::getSerialNumber(p->getProbeSerialNumber());
    }
    snprintf(b, sizeof(b), "%.2f", salinity);
    std::string salinityText = b;

    // the six launch fields of the MK21 and the extra ones that were stored
    int infoFields = 6 + countLines(text, "Launch Information")
            - (hasLine(text, "Launch Information", "Serial Number") ? 1 : 0);
    tmp += EDFFormat::getTitle() + "\r\n";
    tmp += "// File Information\r\n";
    tmp += "Raw Data Filename:  " + fileName.substr(fileName.find_last_of("/\\") + 1) + "\r\n";
    // binaries made by older versions of edf2bin also stored the filename
    // of the EDF, the name of the binary replaces it
    std::stringstream fileLines(getLines(text, "File Information"));
    std::string fileLine;
    while (std::getline(fileLines, fileLine)) {
        if (fileLine.compare(0, 17, "Raw Data Filename") != 0) {
            tmp += fileLine + "\n";
        }
    }
    tmp += "// System Information\r\n";
    tmp += getLines(text, "System Information");
    tmp += "// Probe Information\r\n";
    tmp += probeLines;
    tmp += "// Launch Information\r\n";
    tmp += "Num Info Fields  :  " + std::to_string(infoFields) + "\r\n";
    tmp += "Date of Launch   :  " + date + "\r\n";
    tmp += "Time of Launch   :  " + time + "\r\n";
    tmp += "Sequence Number  :  " + sequenceNumber + "\r\n";
    tmp += "Latitude         :  " + latitude + "\r\n";
    tmp += "Longitude        :  " + longitude + "\r\n";
    if (!hasLine(text, "Launch Information", "Serial Number")) {
        tmp += "Serial Number    :  " + serialNumber + "\r\n";
    }
    tmp += getLines(text, "Launch Information");
    tmp += "// Memo\r\n";
    tmp += getLines(text, "Memo");
    tmp += "// Hardware\r\n";
    if (!hasLine(text, "Hardware", "MK21 Device")) {
        tmp += "MK21 Device      :  " + XBTRecorder::getRecorderDescription(p->getRecorderType()) + "\r\n";
    }
    tmp += getLines(text, "Hardware");
    tmp += "// Information - XBT\r\n";
    tmp += "Salinity          :  " + salinityText + " ppt\r\n";
    tmp += getLines(text, "Information - XBT");
    tmp += "// Post-Processing\r\n";
    tmp += getLines(text, "Post-Processing");
    tmp += "// Data Fields\r\n";
    tmp += "Num Data Fields   :  " + std::to_string(fields.size()) + "\r\n";
    for (unsigned int i = 0; i < fields.size(); i++) {
        tmp += "Field" + std::to_string(i + 1) + "            :  " + EDFFormat::getFieldLabel(fields[i]) + "\r\n";
    }
    tmp += "// Data\r\n";

    XBTResistanceToTemperatureConverter converter;
    SoundSpeedInSeaWater ss;
    std::vector<double> temperatures;
    if (resistances.empty()) {
        temperatures = p->getTemperaturePoints();
    }
    size_t points = resistances.empty() ? temperatures.size() : resistances.size();
    const char *depthFormat = "%.2f";

    // The MK21 computes the depth with single precision floats, using them
    // gives the same rounding as the EDF files.
    const float c2 = (float) coefficientA;
    const float c3 = (float) coefficientB;

    for (unsigned int i = 0; i < points; i++) {
        double t = i / frequency;
        float ft = (float) t;
        float fdepth = ft * (c2 + ft * c3);
        double depth = fdepth;
        double resistance = -999;
        double temperature;
        if (resistances.empty()) {
            temperature = temperatures[i];
        } else {
            resistance = resistances[i];
            temperature = EDFFormat::MAX_TEMPERATURE;
            if (resistance > 0) {
                temperature = converter.convertResistaceToTemperature(resistance);
            }
            temperature = std::min(std::max(temperature, EDFFormat::MIN_TEMPERATURE), EDFFormat::MAX_TEMPERATURE);
        }
        // pressure in kPa, the MK21 uses 1 bar for every 10 meters
        double soundVelocity = ss.getSoundSpeedChenMilleroUNESCO1983(depth * 10.0, salinity, temperature);

        std::string line = "";
        for (unsigned int j = 0; j < fields.size(); j++) {
            switch (fields[j]) {
                case EDFFormat::FIELD_TIME:
                    snprintf(b, sizeof(b), "%.1f", t);
                    break;
                case EDFFormat::FIELD_RESISTANCE:
                    snprintf(b, sizeof(b), "%.3f", resistance);
                    break;
                case EDFFormat::FIELD_DEPTH:
                    snprintf(b, sizeof(b), depthFormat, depth);
                    break;
                case EDFFormat::FIELD_TEMPERATURE:
                    snprintf(b, sizeof(b), "%.2f", temperature);
                    break;
                case EDFFormat::FIELD_SOUND_VELOCITY:
                    snprintf(b, sizeof(b), "%.2f", soundVelocity);
                    break;
                default:
                    b[0] = '\0';
            }
            line += (j > 0 ? "\t" : "") + std::string(b);
        }
        tmp += line + "\r\n";
    }

    return tmp;
}

std::string FormatConverter::getASCIINDC() {

	std::string tmp = "";
	DepthCalculator *dc = new DepthCalculator(*xBTprofile);
	std::vector<std::vector<double>> inflectionPoints = dc->getDepthsAndTemperaturePointsInflectionPoints();
	std::vector<std::vector<double>> twoMeterResolution = dc->getDepthsAndTemperaturePointsTwoMeterResolution();
	std::vector<std::vector<double>> fullResolution = dc->getDepthsAndTemperaturePoints();
	sprintf(buf,"%05.2f",(double)xBTprofile->getSeasVersion()/100.00);
	tmp += " SEAS Version:  "  + std::string(buf) + "\r\n";
	tmp += " Ship Name: "      + xBTprofile->getShipName() + "\r\n";
	tmp += " Call Sign: "      + xBTprofile->getWMOId() + "\r\n";
	tmp += " Lloyds Number:  " + std::to_string(xBTprofile->getLloyds()) + "\r\n";
    sprintf(buf,"%02d/%02d/%04d %02d:%02d GMT\r\n",
            xBTprofile->getDay(),
            xBTprofile->getMonth(),
            xBTprofile->getYear(),
			xBTprofile->getHour(),
			xBTprofile->getMinute());
	tmp += " Date/Time(dd/mm/yyyy): " + std::string(buf);
	tmp += " Latitude(ddd.ddd): " + std::string("0") + decimalDegreesLatToDMSAOML(xBTprofile->getLatitude()) + "\r\n";
	tmp += " Longitude(ddd.ddd): " + decimalDegreesLonToDMSAOML(xBTprofile->getLongitude()) + "\r\n";
	tmp += " Probe Type: " + XBTProbe::getProbeDescription(xBTprofile->getInstrumentType()) + "\r\n";
	tmp += " Probe Code:  " + std::to_string(xBTprofile->getInstrumentType()) + "\r\n";
	tmp += " Probe Serial No: " + std::to_string(xBTprofile->getProbeSerialNumber()) + "\r\n";
	tmp += " Recorder Type: " + XBTRecorder::getRecorderDescription(xBTprofile->getRecorderType()) + "\r\n";
	tmp += " Recorder Code:  " + std::to_string(xBTprofile->getRecorderType()) + "\r\n";
	sprintf(buf,"%05.2f", xBTprofile->getDryBulbTemperature() - 273.15);
	tmp += " Dry Bulb Temp:  " + std::string(buf) + "\r\n";
	tmp += " Wind Instr Type: " + WindInstrument::getInstrumentDescription(xBTprofile->getWindInstrumentType()) + "\r\n";
	sprintf(buf,"%04.1f",  xBTprofile->getWindSpeed());
	tmp += " Wind Speed: " + std::string(buf) + "\r\n";
	sprintf(buf,"%04.1f",  xBTprofile->getWindDirection());
	tmp += " Wind Dir:   " + std::string(buf) + "\r\n";
	tmp += " Current Measurement Method: " +
			SurfaceCurrentInstrument::getInstrumentDescription(
					xBTprofile->getSeaSurfaceCurrentMeasurementMethod()) + "\r\n";
	sprintf(buf,"%2.2f",  xBTprofile->getSeaSurfaceCurrentSpeed());
	tmp += " Current Speed: " + std::string(buf) + "\r\n";
	tmp += " Current Dir:   " + std::to_string(xBTprofile->getSeaSurfaceCurrentDirection()) + "\r\n";
	tmp += " Bottom Depth:    " + std::to_string(xBTprofile->getTotalWaterDepth()) + " M\r\n";

    sprintf(buf,"%x", xBTprofile->getUniqueTag());
    std::string s = std::string(buf);

    for(unsigned int i =0; i < s.size() ; i++) {
		s[i]=std::toupper(s[i]);
	}
	sprintf(buf,"%s","FFFFFFFF");
	tmp += " GTS CRC: " + std::string(buf) + "\r\n";
	tmp += " SEAS ID: " + s + "\r\n";
	sprintf(buf,"%4.2f",  xBTprofile->getShipSpeed() * 1.94384);
	tmp += " Ship Speed at Launch (knots): " + std::string(buf) + "\r\n";
	sprintf(buf,"%02d", (int) xBTprofile->getShipDirection());
	tmp += " Ship Direction at Launch (Degrees):   " + std::string(buf) + "\r\n";
	tmp += " Sequence Number:   "+ std::to_string(xBTprofile->getSequenceNumber()) + "\r\n";
	tmp += " Transect Number:    " + std::to_string(xBTprofile->getTransectNumber()) + "\r\n";
	sprintf(buf,"%4.2f", xBTprofile->getLaunchHeight());
	tmp += " Launch Height (Meters):  " + std::string(buf) +  "\r\n";
	tmp += " SOOP Line: " + xBTprofile->getSoopLine() + "\r\n";
	tmp += " XBT Launcher Type: " + Launchers::getLauncherDescription(xBTprofile->getXBTLauncherType()) + "\r\n";
	tmp += " XBT Recorder Serial Number: " + xBTprofile->getXBTRecorderSerialNumber() + "\r\n";
    sprintf(buf,"%02d/%02d/%04d\r\n",
            xBTprofile->getXBTRecorderManufacturedMonth(),
            xBTprofile->getXBTRecorderManufacturedDay(),
            xBTprofile->getXBTRecorderManufacturedYear());
	tmp += " XBT Recorder Manufacture Date: " + std::string(buf);
    sprintf(buf,"%02d/%02d/%04d\r\n",
            xBTprofile->getXBTProbeManufacturedMonth(),
            xBTprofile->getXBTProbeManufacturedDay(),
            xBTprofile->getXBTProbeManufacturedYear());
	tmp += " Probe Manufacture Date: " + std::string(buf);
	tmp += " Agency in charge of Operation: " + Agency::getAgencyDescription(xBTprofile->getAgencyOwner()) + "\r\n";
	tmp += " Ship Rider: " + xBTprofile->getRiderNames() + "\r\n";
	tmp += " Ship Rider Institution: " + xBTprofile->getRiderInstitutions() + "\r\n";
	tmp += " Ship Rider Email: " + xBTprofile->getRiderEmails() + "\r\n";
	tmp += " Ship Rider Telephone Number: " + xBTprofile->getRiderPhones() + "\r\n";

	if (inflectionPoints.size() > 0) {
		tmp += "\r\n\r\nINFPTS: " + std::to_string(inflectionPoints.size()) ;

		int count = 0;
		for (std::vector<double> inflectionPoint : inflectionPoints) {
			int depth = (int) (inflectionPoint[0] * 10);
			int temp = (int) (inflectionPoint[1] * 100);
			sprintf(buf, " %4d %4d", depth, temp);

			if ((count) % 8 != 0) {
				tmp = tmp + buf;
			} else {
				tmp = tmp + "\r\n" + buf + " ";
			}
			count++;
		} //end for
	}

	if (twoMeterResolution.size() > 0) {
		tmp += "\r\n\r\n\r\nTWOMETER: " + std::to_string(twoMeterResolution.size()) ;

		int count = 0;
		for (std::vector<double> twoMeterProfile : twoMeterResolution) {
			int depth = (int) (twoMeterProfile[0] * 10);
			int temp = (int) (twoMeterProfile[1] * 100 + .5);
			sprintf(buf, " %4d %4d", depth, temp);

			if ((count) % 8 != 0) {
				tmp = tmp + buf;
			} else {
				tmp = tmp + "\r\n" + buf + " ";
			}
			count++;
		} //end for
	}

	if (fullResolution.size() > 0) {
		tmp += "\r\n\r\n\r\nXBT: " + std::to_string(fullResolution.size()) ;

		int count = 0;
		for (std::vector<double> fullProfile : fullResolution) {
			int temp = (int) (fullProfile[1] * 100 + .5);
			sprintf(buf, "%4d", temp);

			if ((count) % 20 != 0) {
				tmp = tmp + buf;
			} else {
				tmp = tmp + "\r\n" + buf ;
			}
			count++;
		} //end for
	}


	// calculating the GTS CRC from the string, weird.......
	uint32_t crc;
	char * GTSCRC = new char [ tmp.size() + 1];
	std:copy(tmp.begin(),tmp.end(),GTSCRC);
	GTSCRC[tmp.size()] = '\0';
	crc = FormatConverter::computeCRC32(GTSCRC,tmp.size() + 1);
	s = FormatConverter::intToHexString(crc);
	tmp.replace(tmp.find("GTS CRC: FFFFFFFF") , 17 , "GTS CRC: " + s);
	 
	 
	 delete[] GTSCRC;
	 delete dc;
	return tmp;

}

/**
 * This method calculates the CRC32 lookup table on the fly
 * and returns the checksum. This way is slower but it's
 * easier to read. This method was posted on stackoverflow
 * https://stackoverflow.com/questions/26049150/calculate-a-32-bit-crc-lookup-table-in-c-c
 */
uint32_t FormatConverter::computeCRC32(char *buf, uint32_t len) {

    uint32_t val, crc;
    uint8_t i;

    crc = 0xFFFFFFFF;
    while(len--){
        val=(crc^*buf++)&0xFF;
        for(i=0; i<8; i++){
            val = val & 1 ? (val>>1)^0xEDB88320 : val>>1;
        }
        crc = val^crc>>8;
    }
    return crc^0xFFFFFFFF;
}
std::string FormatConverter::intToHexString(int int1) {

	char hex_string[20];
	std::string data;
	sprintf(hex_string, "%X", int1);
	data = std::string(hex_string);
	std::for_each(data.begin(), data.end(), [](char & c){
		c = ::toupper(c);
	});
	return data;
}
