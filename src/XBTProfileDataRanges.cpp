/**
 * This class contains the locations of where the data are located in an
 * XBT profile. The class also contains methods to retrieve these locations.
 *
 * @author pedro
 */

#include "XBTProfileDataRanges.h"

#include "MessageType.h"

constexpr int dataLocations[XBTProfileDataRanges::NUMBER_OF_DATA_LOCATIONS][2][5] = {
    //                  start                         end
    //                    0                            1
    //          0    1    2    3    4        0    1    2    3    4    // message types
    /*0*/  {{   0,   0,   0,   0,   0 }, {  71,  71,  71,  71,  71 }},// WMO number/call sign
    /*1*/  {{  72,  72,  72,  72,  72 }, {  77,  77,  77,  77,  77 }},// old message type
    /*2*/  {{  78,  78,  78,  78,  78 }, {  87,  87,  87,  87,  87 }},// new message type
    /*3*/  {{  -1,  88,  88,  88,  88 }, {  -1, 112, 112, 112, 112 }},// lattitude
    /*4*/  {{  -1, 113, 113, 113, 113 }, {  -1, 138, 138, 138, 138 }},// longitude
    /*5*/  {{  -1,  -1, 139, 139, 139 }, {  -1,  -1, 186, 186, 186 }},// soop line
    /*6*/  {{  -1,  -1, 187, 187, 187 }, {  -1,  -1, 193, 193, 193 }},// transect number
    /*7*/  {{  -1,  -1, 194, 194, 194 }, {  -1,  -1, 209, 209, 209 }},// sequence number
    /*8*/  {{  -1, 139, 210, 210, 210 }, {  -1, 150, 221, 221, 221 }},// year
    /*9*/  {{  -1, 151, 222, 222, 222 }, {  -1, 154, 225, 225, 225 }},// month
    /*10*/ {{  -1, 155, 226, 226, 226 }, {  -1, 160, 231, 231, 231 }},// day
    /*11*/ {{  -1, 161, 232, 232, 232 }, {  -1, 165, 236, 236, 236 }},// hour
    /*12*/ {{  -1, 166, 237, 237, 237 }, {  -1, 171, 242, 242, 242 }},// minute
    /*13*/ {{  -1, 172, 243, 243, 243 }, {  -1, 411, 482, 482, 482 }},// ship name
    /*14*/ {{  -1, 412, 483, 483, 483 }, {  -1, 435, 506, 506, 506 }},// lloyds
    /*15*/ {{  -1, 436, 507, 507, 507 }, {  -1, 467, 538, 538, 538 }},// unique tag
    /*16*/ {{  -1, 468, 539, 539, 539 }, {  -1, 477, 548, 548, 548 }},// seas version
    /*17*/ {{  -1, 478, 549, 549, 549 }, {  -1, 497, 572, 572, 572 }},// probe serial number
    /*18*/ {{  -1, 498, 573, 573, 573 }, {  -1, 500, 575, 575, 575 }},// data resolution "thisDataIs"
    /*19*/ {{  -1, 501, 576, 576, 576 }, {  -1, 503, 578, 578, 578 }},// data quality
    /*20*/ {{  -1,  -1, 579, 579, 579 }, {  -1,  -1, 591, 591, 591 }},// launcher height
    /*21*/ {{  -1,  -1, 592, 592, 592 }, {  -1,  -1, 600, 600, 600 }},// ship direction
    /*22*/ {{  -1,  -1, 601, 601, 601 }, {  -1,  -1, 613, 613, 613 }},// ship speed
    /*23*/ {{  -1, 504, 614, 614, 614 }, {  -1, 513, 623, 623, 623 }},// instrument type
    /*24*/ {{  -1, 514, 624, 624, 624 }, {  -1, 520, 630, 630, 630 }},// recorder type
    /*25*/ {{  -1, 521, 631, 631, 631 }, {  -1, 524, 634, 634, 634 }},// wind instrument type
    /*26*/ {{  -1, 525, 635, 635, 635 }, {  -1, 533, 643, 643, 643 }},// wind direction
    /*27*/ {{  -1, 534, 644, 644, 644 }, {  -1, 545, 655, 655, 655 }},// wind speed
    /*28*/ {{  -1, 546, 656, 656, 656 }, {  -1, 557, 667, 667, 667 }},// dry bulb termperature
    /*29*/ {{  -1, 558, 668, 668, 668 }, {  -1, 560, 670, 670, 670 }},// sea surface current measurement
    /*30*/ {{  -1, 561, 671, 671, 671 }, {  -1, 569, 679, 679, 679 }},// sea surface current direction
    /*31*/ {{  -1, 570, 680, 680, 680 }, {  -1, 582, 692, 692, 692 }},// sea surface current speed
    /*32*/ {{  -1, 583, 693, 693, 693 }, {  -1, 596, 706, 706, 706 }},// total water depth
    /*33*/ {{  -1,  -1,  -1, 707, 707 }, {  -1,  -1, -1,  726, 726 }},// agency owner
    /*34*/ {{  -1,  -1,  -1, 727, 727 }, {  -1,  -1, -1,  734, 734 }},// launcher type
    /*35*/ {{  -1,  -1,  -1, 735, 735 }, {  -1,  -1, -1,  798, 798 }},// xbt Recorder serial number
    /*36*/ {{  -1,  -1,  -1, 799, 799 }, {  -1,  -1, -1,  810, 810 }},// xbt recorder manufactured year
    /*37*/ {{  -1,  -1,  -1, 811, 811 }, {  -1,  -1, -1,  814, 814 }},// xbt recorder mannufactured month
    /*38*/ {{  -1,  -1,  -1, 815, 815 }, {  -1,  -1, -1,  820, 820 }},// xbt recorder manufactured day
    /*39*/ {{  -1,  -1,  -1, 821, 821 }, {  -1,  -1, -1,  832, 832 }},// xbt manufactured year
    /*40*/ {{  -1,  -1,  -1, 833, 833 }, {  -1,  -1, -1,  836, 836 }},// xbt probe manufactured month
    /*41*/ {{  -1,  -1,  -1, 837, 837 }, {  -1,  -1, -1,  842, 842 }},// xbt probe manufactured day
    /*42*/ {{  -1,  -1,  -1, 843, 843 }, {  -1,  -1, -1,  848, 848 }},// number of rider blocks
    /*43*/ {{  -1,  -1,  -1, 849, 849 }, {  -1,  -1, -1,  854, 854 }},// number of rider institution blocks
    /*44*/ {{  -1,  -1,  -1, 855, 855 }, {  -1,  -1, -1,  860, 860 }},// number of rider email blocks
    /*45*/ {{  -1,  -1,  -1, 861, 861 }, {  -1,  -1, -1,  866, 866 }},// number of rider phone blocks
    /*46*/ {{  -1, 597, 707, 867, 896 }, {  -1, 604, 714, 874, 903 }},// number of repeated fields
    /*47*/ {{  -1, 605, 715, 875, 904 }, {  -1, 620, 730, 890, 919 }},// times replicated
    /*48*/ {{  -1, 621, 731, 891, 920 }, {  -1, 632, 742, 902, 946 }},// sea surface temperature/resistance
    /*49*/ {{  -1, 633,  -1,  -1,  -1 }, {  -1, 646,  -1,  -1,  -1 }},// sea depth
    /*50*/ {{  -1, 621, 731, 891, 920 }, {  -1,  -1,  -1,  -1,  -1 }},// temperature points
    /*51*/ {{  -1,  -1,  -1,  -1,  -1 }, {  -1,  -1,  -1,  -1,  -1 }},// comment blocks
    /*52*/ {{  -1,  -1,  -1, 891, 920 }, {  -1,  -1,  -1,  -1,  -1 }},// rider names
    /*53*/ {{  -1,  -1,  -1, 891, 920 }, {  -1,  -1,  -1,  -1,  -1 }},// rider emails
    /*54*/ {{  -1,  -1,  -1, 891, 920 }, {  -1,  -1,  -1,  -1,  -1 }},// rider institutions
    /*55*/ {{  -1,  -1,  -1, 891, 920 }, {  -1,  -1,  -1,  -1,  -1 }},// rider phones
    // The fields below are only stored in type 4 messages. They hold the
    // values of Sippican MK21 export data files (EDF) that the fields above
    // can not hold.
    /*56*/ {{  -1,  -1,  -1,  -1, 867 }, {  -1,  -1,  -1,  -1, 872 }},// launch second
    /*57*/ {{  -1,  -1,  -1,  -1, 873 }, {  -1,  -1,  -1,  -1, 885 }},// salinity
    /*58*/ {{  -1,  -1,  -1,  -1, 886 }, {  -1,  -1,  -1,  -1, 895 }},// number of EDF text blocks
    /*59*/ {{  -1,  -1,  -1,  -1, 920 }, {  -1,  -1,  -1,  -1,  -1 }},// EDF text
};

constexpr int WMOID = 0;
constexpr int OLD_MESSAGE_TYPE = 1;
constexpr int NEW_MESSAGE_TYPE = 2;
constexpr int LATITUDE = 3;
constexpr int LONGITUDE = 4;
constexpr int SOOP_LINE = 5;
constexpr int TRANSECT_NUM = 6;
constexpr int SEQUENCE_NUM = 7;
constexpr int YEAR = 8;
constexpr int MONTH = 9;
constexpr int DAY = 10;
constexpr int HOUR = 11;
constexpr int MINUTE = 12;
constexpr int SHIP_NAME = 13;
constexpr int LLOYDS = 14;
constexpr int UNIQUE_TAG = 15;
constexpr int SEAS_VERSION = 16;
constexpr int PROBE_SERIAL_NUMBER = 17;
constexpr int THIS_DATA_IS = 18;
constexpr int DATA_QUALITY = 19;
constexpr int LAUNCH_HEIGHT = 20;
constexpr int SHIP_DIRECTION = 21;
constexpr int SHIP_SPEED = 22;
constexpr int INSTRUMENT_TYPE = 23;
constexpr int RECORDER_TYPE = 24;
constexpr int WIND_INSTRUMENT_TYPE = 25;
constexpr int WIND_DIRECTION = 26;
constexpr int WIND_SPEED = 27;
constexpr int DRY_BULB_TEMPERATURE = 28;
constexpr int SEA_SURFACE_CURRENT_MEASURING_METHOD = 29;
constexpr int SEA_SURFACE_CURRENT_DIRECTION = 30;
constexpr int SEA_SURFACE_CURRENT_SPEED = 31;
constexpr int TOTAL_WATER_DEPTH = 32;
constexpr int AGENCY_OWNER = 33;
constexpr int XBT_LAUNCHER_TYPE = 34;
constexpr int XBT_RECORDER_SERIAL_NUMBER = 35;
constexpr int XBT_RECORDER_MANUFACTURE_YEAR = 36;
constexpr int XBT_RECORDER_MANUFACTURE_MONTH = 37;
constexpr int XBT_RECORDER_MANUFACTURE_DAY = 38;
constexpr int XBT_PROBE_MANUFACTURE_YEAR = 39;
constexpr int XBT_PROBE_MANUFACTURE_MONTH = 40;
constexpr int XBT_PROBE_MANUFACTURE_DAY =  41;
constexpr int NUMBER_OF_RIDER_BLOCKS = 42;
constexpr int NUMBER_OF_RIDER_INSTITUTION_BLOCKS = 43;
constexpr int NUMBER_OF_RIDER_EMAIL_BLOCKS = 44;
constexpr int NUMBER_OF_RIDER_PHONE_BLOCKS = 45;
constexpr int NUMBER_OF_REPEATED_FIELDS = 46;
constexpr int TIMES_REPLICATED = 47;
constexpr int SEA_SURFACE_TEMPERATURE = 48;
constexpr int SEA_SURFACE_RESISTANCE = 48;
constexpr int SEA_DEPTH = 49;
constexpr int TEMPERATURE_POINTS = 50;
constexpr int RESISTANCE_POINTS = 50;
constexpr int COMMENT_BLOCKS = 51;
constexpr int RIDER_NAMES = 52;
constexpr int RIDER_EMAILS = 53;
constexpr int RIDER_INSTITUTIONS = 54;
constexpr int RIDER_PHONES = 55;

XBTProfileDataRanges::XBTProfileDataRanges() {
	// TODO Auto-generated constructor stub

}

XBTProfileDataRanges::~XBTProfileDataRanges() {
	// TODO Auto-generated destructor stub
}


std::array<int, 2> XBTProfileDataRanges::getDataLocation(int data ,int messageType) {

	std::array<int,2> startEnd{dataLocations[data][0][messageType],dataLocations[data][1][messageType]};

	return startEnd;

}

