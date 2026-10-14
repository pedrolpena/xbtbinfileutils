/**
 * This class contains the locations of where the data are located in an
 * XBT profile. The class also contains methods to retrieve these locations.
 *
 * @author pedro
 */

#ifndef INCLUDE_XBTPROFILEDATARANGES_H_
#define INCLUDE_XBTPROFILEDATARANGES_H_
#include <array>
class XBTProfileDataRanges {
public:

    static constexpr int WMOID = 0;
    static constexpr int OLD_MESSAGE_TYPE = 1;
    static constexpr int NEW_MESSAGE_TYPE = 2;
    static constexpr int LATITUDE = 3;
    static constexpr int LONGITUDE = 4;
    static constexpr int SOOP_LINE = 5;
    static constexpr int TRANSECT_NUM = 6;
    static constexpr int SEQUENCE_NUM = 7;
    static constexpr int YEAR = 8;
    static constexpr int MONTH = 9;
    static constexpr int DAY = 10;
    static constexpr int HOUR = 11;
    static constexpr int MINUTE = 12;
    static constexpr int SHIP_NAME = 13;
    static constexpr int LLOYDS = 14;
    static constexpr int UNIQUE_TAG = 15;
    static constexpr int SEAS_VERSION = 16;
    static constexpr int PROBE_SERIAL_NUMBER = 17;
    static constexpr int THIS_DATA_IS = 18;
    static constexpr int DATA_QUALITY = 19;
    static constexpr int LAUNCH_HEIGHT = 20;
    static constexpr int SHIP_DIRECTION = 21;
    static constexpr int SHIP_SPEED = 22;
    static constexpr int INSTRUMENT_TYPE = 23;
    static constexpr int RECORDER_TYPE = 24;
    static constexpr int WIND_INSTRUMENT_TYPE = 25;
    static constexpr int WIND_DIRECTION = 26;
    static constexpr int WIND_SPEED = 27;
    static constexpr int DRY_BULB_TEMPERATURE = 28;
    static constexpr int SEA_SURFACE_CURRENT_MEASURING_METHOD = 29;
    static constexpr int SEA_SURFACE_CURRENT_DIRECTION = 30;
    static constexpr int SEA_SURFACE_CURRENT_SPEED = 31;
    static constexpr int TOTAL_WATER_DEPTH = 32;
    static constexpr int AGENCY_OWNER = 33;
    static constexpr int XBT_LAUNCHER_TYPE = 34;
    static constexpr int XBT_RECORDER_SERIAL_NUMBER = 35;
    static constexpr int XBT_RECORDER_MANUFACTURE_YEAR = 36;
    static constexpr int XBT_RECORDER_MANUFACTURE_MONTH = 37;
    static constexpr int XBT_RECORDER_MANUFACTURE_DAY = 38;
    static constexpr int XBT_PROBE_MANUFACTURE_YEAR = 39;
    static constexpr int XBT_PROBE_MANUFACTURE_MONTH = 40;
    static constexpr int XBT_PROBE_MANUFACTURE_DAY =  41;
    static constexpr int NUMBER_OF_RIDER_BLOCKS = 42;
    static constexpr int NUMBER_OF_RIDER_INSTITUTION_BLOCKS = 43;
    static constexpr int NUMBER_OF_RIDER_EMAIL_BLOCKS = 44;
    static constexpr int NUMBER_OF_RIDER_PHONE_BLOCKS = 45;
    static constexpr int NUMBER_OF_REPEATED_FIELDS = 46;
    static constexpr int TIMES_REPLICATED = 47;
    static constexpr int SEA_SURFACE_TEMPERATURE = 48;
    static constexpr int SEA_SURFACE_RESISTANCE = 48;
    static constexpr int SEA_DEPTH = 49;
    static constexpr int TEMPERATURE_POINTS = 50;
    static constexpr int RESISTANCE_POINTS = 50;
    static constexpr int COMMENT_BLOCKS = 51;
    static constexpr int RIDER_NAMES = 52;
    static constexpr int RIDER_EMAILS = 53;
    static constexpr int RIDER_INSTITUTIONS = 54;
    static constexpr int RIDER_PHONES = 55;
    // Type 4 only, values from Sippican MK21 export data files (EDF).
    static constexpr int LAUNCH_SECOND = 56;
    static constexpr int SALINITY = 57;
    static constexpr int NUMBER_OF_EDF_TEXT_BLOCKS = 58;
    static constexpr int EDF_TEXT = 59;
    static constexpr int NUMBER_OF_DATA_LOCATIONS = 60;

    // Type 4 resistance points, ohms = (N - RESISTANCE_OFFSET) / RESISTANCE_SCALE
    static constexpr double RESISTANCE_SCALE = 1000.0;
    static constexpr int RESISTANCE_OFFSET = 1000000;
    // Type 4 profiles made from files without resistances, e.g. EDF files
    // that only hold temperatures, store the temperatures in the same
    // points as celsius = (N - TEMPERATURE_OFFSET) / TEMPERATURE_SCALE.
    // Every N below RESISTANCE_OFFSET would be a negative resistance, which
    // a thermistor can not have, so a first point below RESISTANCE_OFFSET
    // tells the decoder the points are temperatures. No resistances are
    // made up for them.
    static constexpr double TEMPERATURE_SCALE = 1000.0;
    static constexpr int TEMPERATURE_OFFSET = 500000;

	XBTProfileDataRanges();
	virtual ~XBTProfileDataRanges();
	static  std::array<int,2> getDataLocation(int,int);
private:
	static constexpr int _DATA_POSITION_IN_BIN[NUMBER_OF_DATA_LOCATIONS][2][5]={};
};
#endif /* INCLUDE_XBTPROFILEDATARANGES_H_ */
