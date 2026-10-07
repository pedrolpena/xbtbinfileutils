/*
 * FormatConverter.h
 *
 *  Created on: Aug 11, 2019
 *      Author: pedro
 */

#include <string>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <algorithm>


#include "Agency.h"
#include "DataQuality.h"
#include "DataResolution.h"
#include "DepthCalculator.h"
#include "Launchers.h"
#include "PressureCalculator.h"
#include "SoundSpeedInSeaWater.h"
#include "SurfaceCurrentInstrument.h"
#include "WindInstrument.h"
#include "XBTProbe.h"
#include "XBTProfile.h"
#include "XBTRecorder.h"


#ifndef FORMATCONVERTER_H_
#define FORMATCONVERTER_H_


class FormatConverter {
public:
    XBTProfile *xBTprofile;
    std::string prettyLat;
    std::string prettyLon;
    char buf[120];


    /**
     * This method returns a String containing the xBTprofile information using
     * the SEAS ASCII format.
     *
     * @return This method returns a String containing the xBTprofile
     * information using the SEAS ASCII format.
     */
    std::string getASCII();

    /**
     * This method returns a String containing the xBTprofile information emulating
     * the Sippican Export Data Format (EDF).
     *
     * @param fileName The filename to display.
     * @param salinity The salinty of the area where the measurement was made.
     * @return This method returns a String containing the xBTprofile
     * information emulating the Sippican Export Data Format (EDF).
     */
    std::string getASCIINDC();
    std::string getASCIIEDF(std::string fileName,double salinity);
    std::string decimalDegreesLatToDMSAOML(double lat);
    std::string decimalDegreesLonToDMSAOML(double lon);
    std::string decimalDegreesLatToDMSEDF(double lat);
    std::string decimalDegreesLonToDMSEDF(double lon);
    uint32_t computeCRC32(char *buf, uint32_t len);
    std::string intToHexString(int int1);


	FormatConverter(XBTProfile &xBTprofile);
	virtual ~FormatConverter();
};

#endif /* FORMATCONVERTER_H_ */
