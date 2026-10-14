/**
 * This class contains constants that hold the value for the type of XBT Probe
 * , the A and B coefficients used in the XBT fall rate equation, The Maximum
 * Depth for the probe and a Description of the probe. There are methods to
 * get the vales for each probe type.
 * To calculate the depth, use the following equation.
 * Z = A*t + .001 * B*t*t
 * Z is depth and t is time elapsed in seconds.
 * This values were copied from
 * https://www.nodc.noaa.gov/GTSPP/document/codetbls/wmocodes/table1770.html
 *
 * @author Pedro Pena
 * @version 1.0
 *
 */

#include "XBTProbe.h"

constexpr int         SIPPICAN_T4_OLD = 1;
constexpr int         SIPPICAN_T4 = 2;
constexpr int         SIPPICAN_T5 = 11;
constexpr int         SIPPICAN_FAST_DEEP = 21;
constexpr int         SIPPICAN_T6_OLD = 31;
constexpr int         SIPPICAN_T6 = 32;
constexpr int         SIPPICAN_T7_OLD = 41;
constexpr int         SIPPICAN_T7 = 42;
constexpr int         SIPPICAN_DEEP_BLUE_OLD = 51;
constexpr int         SIPPICAN_DEEP_BLUE = 52;
constexpr int         SIPPICAN_T10 = 61;
constexpr int         SIPPICAN_T11 = 71;
constexpr int         SIPPICAN_AXBT = 81;
constexpr int         TSK_T4_OLD = 201;
constexpr int         TSK_T4 = 202;
constexpr int         TSK_T6_OLD = 211;
constexpr int         TSK_T6 = 212;
constexpr int         TSK_T7_OLD = 221;
constexpr int         TSK_T7 = 222;
constexpr int         TSK_T5 = 231;
constexpr int         TSK_T10 = 241;
constexpr int         TSK_DEEP_BLUE_OLD = 251;
constexpr int         TSK_DEEP_BLUE = 252;
constexpr int         TSK_AXBT = 261;
constexpr int         SPARTON_XBT1 = 401;
constexpr int         SPARTON_XBT3 = 411;
constexpr int         SPARTON_XBT4 = 421;
constexpr int         SPARTON_XBT5 = 431;
constexpr int         SPARTON_XBT5DB = 441;
constexpr int         SPARTON_XBT6 = 451;
constexpr int         SPARTON_XBT7_OLD = 461;
constexpr int         SPARTON_XBT7 = 462;
constexpr int         SPARTON_XBT7DB = 471;
constexpr int         SPARTON_XBT10 = 481;
constexpr int         SPARTON_XBT20 = 491;
constexpr int         SPARTON_XBT20DB = 501;
constexpr int         SPARTON_536AXBT = 510;
constexpr int         SIPPICAN_12 = 900;

constexpr double      _SIPPICAN_T4_OLD_COEFFICIENT_A = 6.472;
constexpr double      _SIPPICAN_T4_OLD_COEFFICIENT_B = -2.16;
constexpr double      _SIPPICAN_T4_OLD_MAX_DEPTH = 460.00;
constexpr const char* _SIPPICAN_T4_OLD_DESCRIPTION = "Sippican T-4";

constexpr double      _SIPPICAN_T4_COEFFICIENT_A = 6.691;
constexpr double      _SIPPICAN_T4_COEFFICIENT_B = -2.25;
constexpr double      _SIPPICAN_T4_MAX_DEPTH = 460.00;
constexpr const char* _SIPPICAN_T4_DESCRIPTION = "Sippican T-4";

constexpr double      _SIPPICAN_T5_COEFFICIENT_A = 6.828;
constexpr double      _SIPPICAN_T5_COEFFICIENT_B = -1.82;
constexpr double      _SIPPICAN_T5_MAX_DEPTH = 1830.00;
constexpr const char* _SIPPICAN_T5_DESCRIPTION = "Sippican T-5";

constexpr double      _SIPPICAN_FAST_DEEP_COEFFICIENT_A = 6.390;
constexpr double      _SIPPICAN_FAST_DEEP_COEFFICIENT_B = -1.82;
constexpr double      _SIPPICAN_FAST_DEEP_MAX_DEPTH = 1000.00;
constexpr const char* _SIPPICAN_FAST_DEEP_DESCRIPTION = "Sippican Fast Deep";

constexpr double      _SIPPICAN_T6_OLD_COEFFICIENT_A = 6.472;
constexpr double      _SIPPICAN_T6_OLD_COEFFICIENT_B = -2.16;
constexpr double      _SIPPICAN_T6_OLD_MAX_DEPTH = 460.00;
constexpr const char* _SIPPICAN_T6_OLD_DESCRIPTION = "Sippican T-6";

constexpr double      _SIPPICAN_T6_COEFFICIENT_A = 6.691;
constexpr double      _SIPPICAN_T6_COEFFICIENT_B = -2.25;
constexpr double      _SIPPICAN_T6_MAX_DEPTH = 460.00;
constexpr const char* _SIPPICAN_T6_DESCRIPTION = "Sippican T-6";

constexpr double      _SIPPICAN_T7_OLD_COEFFICIENT_A = 6.472;
constexpr double      _SIPPICAN_T7_OLD_COEFFICIENT_B = -2.16;
constexpr double      _SIPPICAN_T7_OLD_MAX_DEPTH = 760.00;
constexpr const char* _SIPPICAN_T7_OLD_DESCRIPTION = "Sippican T-7";

constexpr double      _SIPPICAN_T7_COEFFICIENT_A = 6.691;
constexpr double      _SIPPICAN_T7_COEFFICIENT_B = -2.25;
constexpr double      _SIPPICAN_T7_MAX_DEPTH = 760.00;
constexpr const char* _SIPPICAN_T7_DESCRIPTION = "Sippican T-7";

constexpr double      _SIPPICAN_DEEP_BLUE_OLD_COEFFICIENT_A = 6.472;
constexpr double      _SIPPICAN_DEEP_BLUE_OLD_COEFFICIENT_B = -2.16;
constexpr double      _SIPPICAN_DEEP_BLUE_OLD_MAX_DEPTH = 760.00;
constexpr const char* _SIPPICAN_DEEP_BLUE_OLD_DESCRIPTION = "Sippican Deep Blue";

constexpr double      _SIPPICAN_DEEP_BLUE_COEFFICIENT_A = 6.691;
constexpr double      _SIPPICAN_DEEP_BLUE_COEFFICIENT_B = -2.25;
constexpr double      _SIPPICAN_DEEP_BLUE_MAX_DEPTH = 760.00;
constexpr const char* _SIPPICAN_DEEP_BLUE_DESCRIPTION = "Sippican Deep Blue";

constexpr double      _SIPPICAN_T10_COEFFICIENT_A = 6.301;
constexpr double      _SIPPICAN_T10_COEFFICIENT_B = -2.16;
constexpr double      _SIPPICAN_T10_MAX_DEPTH = 200.00;
constexpr const char* _SIPPICAN_T10_DESCRIPTION = "Sippican T-10";

constexpr double      _SIPPICAN_T11_COEFFICIENT_A = 1.779;
constexpr double      _SIPPICAN_T11_COEFFICIENT_B = -0.255;
constexpr double      _SIPPICAN_T11_MAX_DEPTH = 460.00;
constexpr const char* _SIPPICAN_T11_DESCRIPTION = "Sippican T-11";

constexpr double      _SIPPICAN_AXBT_COEFFICIENT_A = 1.52;
constexpr double      _SIPPICAN_AXBT_COEFFICIENT_B = 0.0;
constexpr double      _SIPPICAN_AXBT_MAX_DEPTH = 460.00;
constexpr const char* _SIPPICAN_AXBT_DESCRIPTION = "Sippican AXBT";

constexpr double      _TSK_T4_OLD_COEFFICIENT_A = 6.472;
constexpr double      _TSK_T4_OLD_COEFFICIENT_B = -2.16;
constexpr double      _TSK_T4_OLD_MAX_DEPTH = 460.00;
constexpr const char* _TSK_T4_OLD_DESCRIPTION = "TSK T-4";

constexpr double      _TSK_T4_COEFFICIENT_A = 6.691;
constexpr double      _TSK_T4_COEFFICIENT_B = -2.25;
constexpr double      _TSK_T4_MAX_DEPTH = 460.00;
constexpr const char* _TSK_T4_DESCRIPTION = "TSK T-4";

constexpr double      _TSK_T6_OLD_COEFFICIENT_A = 6.472;
constexpr double      _TSK_T6_OLD_COEFFICIENT_B = -2.16;
constexpr double      _TSK_T6_OLD_MAX_DEPTH = 460.00;
constexpr const char* _TSK_T6_OLD_DESCRIPTION = "TSK T-6";

constexpr double      _TSK_T6_COEFFICIENT_A = 6.691;
constexpr double      _TSK_T6_COEFFICIENT_B = -2.25;
constexpr double      _TSK_T6_MAX_DEPTH = 460.00;
constexpr const char* _TSK_T6_DESCRIPTION = "TSK T-6";

constexpr double      _TSK_T7_OLD_COEFFICIENT_A = 6.472;
constexpr double      _TSK_T7_OLD_COEFFICIENT_B = -2.16;
constexpr double      _TSK_T7_OLD_MAX_DEPTH = 760.00;
constexpr const char* _TSK_T7_OLD_DESCRIPTION = "TSK T-7";

constexpr double      _TSK_T7_COEFFICIENT_A = 6.691;
constexpr double      _TSK_T7_COEFFICIENT_B = -2.25;
constexpr double      _TSK_T7_MAX_DEPTH = 760.00;
constexpr const char* _TSK_T7_DESCRIPTION = "TSK T-7";

constexpr double      _TSK_T5_COEFFICIENT_A = 6.828;
constexpr double      _TSK_T5_COEFFICIENT_B = -1.82;
constexpr double      _TSK_T5_MAX_DEPTH = 1830.00;
constexpr const char* _TSK_T5_DESCRIPTION = "TSK T-5";

constexpr double      _TSK_T10_COEFFICIENT_A = 6.301;
constexpr double      _TSK_T10_COEFFICIENT_B = -2.16;
constexpr double      _TSK_T10_MAX_DEPTH = 200.00;
constexpr const char* _TSK_T10_DESCRIPTION = "TSK T-10";

constexpr double      _TSK_DEEP_BLUE_OLD_COEFFICIENT_A = 6.472;
constexpr double      _TSK_DEEP_BLUE_OLD_COEFFICIENT_B = -2.16;
constexpr double      _TSK_DEEP_BLUE_OLD_MAX_DEPTH = 760.00;
constexpr const char* _TSK_DEEP_BLUE_OLD_DESCRIPTION = "TSK Deep Blue";

constexpr double      _TSK_DEEP_BLUE_COEFFICIENT_A = 6.691;
constexpr double      _TSK_DEEP_BLUE_COEFFICIENT_B = -2.25;
constexpr double      _TSK_DEEP_BLUE_MAX_DEPTH = 760.00;
constexpr const char* _TSK_DEEP_BLUE_DESCRIPTION = "TSK Deep Blue";

constexpr double      _TSK_AXBT_COEFFICIENT_A = -1;
constexpr double      _TSK_AXBT_COEFFICIENT_B = -1;
constexpr double      _TSK_AXBT_MAX_DEPTH = 4000.00;
constexpr const char* _TSK_AXBT_DESCRIPTION = "TSK AXBT";

constexpr double      _SPARTON_XBT1_COEFFICIENT_A = 6.301;
constexpr double      _SPARTON_XBT1_COEFFICIENT_B = -2.16;
constexpr double      _SPARTON_XBT1_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT1_DESCRIPTION = "Sparton XBT-1";

constexpr double      _SPARTON_XBT3_COEFFICIENT_A = 5.861;
constexpr double      _SPARTON_XBT3_COEFFICIENT_B = -0.0904;
constexpr double      _SPARTON_XBT3_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT3_DESCRIPTION = "Sparton XBT-3";

constexpr double      _SPARTON_XBT4_COEFFICIENT_A = 6.472;
constexpr double      _SPARTON_XBT4_COEFFICIENT_B = -2.16;
constexpr double      _SPARTON_XBT4_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT4_DESCRIPTION = "Sparton XBT-4";

constexpr double      _SPARTON_XBT5_COEFFICIENT_A = 6.828;
constexpr double      _SPARTON_XBT5_COEFFICIENT_B = -1.82;
constexpr double      _SPARTON_XBT5_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT5_DESCRIPTION = "Sparton XBT-5";

constexpr double      _SPARTON_XBT5DB_COEFFICIENT_A = 6.828;
constexpr double      _SPARTON_XBT5DB_COEFFICIENT_B = -1.82;
constexpr double      _SPARTON_XBT5DB_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT5DB_DESCRIPTION = "Sparton XBT-5DB";

constexpr double      _SPARTON_XBT6_COEFFICIENT_A = 6.472;
constexpr double      _SPARTON_XBT6_COEFFICIENT_B = -2.16;
constexpr double      _SPARTON_XBT6_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT6_DESCRIPTION = "Sparton XBT-6";

constexpr double      _SPARTON_XBT7_OLD_COEFFICIENT_A = 6.472;
constexpr double      _SPARTON_XBT7_OLD_COEFFICIENT_B = -2.16;
constexpr double      _SPARTON_XBT7_OLD_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT7_OLD_DESCRIPTION = "Sparton XBT-7";

constexpr double      _SPARTON_XBT7_COEFFICIENT_A = 6.705;
constexpr double      _SPARTON_XBT7_COEFFICIENT_B = -2.28;
constexpr double      _SPARTON_XBT7_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT7_DESCRIPTION = "Sparton XBT-7";

constexpr double      _SPARTON_XBT7DB_COEFFICIENT_A = 6.472;
constexpr double      _SPARTON_XBT7DB_COEFFICIENT_B = -2.16;
constexpr double      _SPARTON_XBT7DB_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT7DB_DESCRIPTION = "Sparton XBT-7DB";

constexpr double      _SPARTON_XBT10_COEFFICIENT_A = 6.301;
constexpr double      _SPARTON_XBT10_COEFFICIENT_B = -2.16;
constexpr double      _SPARTON_XBT10_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT10_DESCRIPTION = "Sparton XBT-10";

constexpr double      _SPARTON_XBT20_COEFFICIENT_A = 6.472;
constexpr double      _SPARTON_XBT20_COEFFICIENT_B = -2.16;
constexpr double      _SPARTON_XBT20_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT20_DESCRIPTION = "Sparton XBT-20";

constexpr double      _SPARTON_XBT20DB_COEFFICIENT_A = 6.472;
constexpr double      _SPARTON_XBT20DB_COEFFICIENT_B = -2.16;
constexpr double      _SPARTON_XBT20DB_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_XBT20DB_DESCRIPTION = "Sparton XBT-20DB";

constexpr double      _SPARTON_536AXBT_COEFFICIENT_A = 1.524;
constexpr double      _SPARTON_536AXBT_COEFFICIENT_B = 0.0;
constexpr double      _SPARTON_536AXBT_MAX_DEPTH = 4000.00;
constexpr const char* _SPARTON_536AXBT_DESCRIPTION = "Sparton 536 AXBT-7";

constexpr double      _SIPPICAN_12_COEFFICIENT_A = 9.727;
constexpr double      _SIPPICAN_12_COEFFICIENT_B = -0.0000473;
constexpr double      _SIPPICAN_12_MAX_DEPTH = 2000.00;
constexpr const char* _SIPPICAN_12_DESCRIPTION = "Sippican T-12 (LMP5-T1)";

XBTProbe::XBTProbe() {


}

XBTProbe::~XBTProbe() {

}
/**
* This method returns the A coefficient used in the fall rate equation.
*
* @return This method returns the A coefficient used in the
* fall rate equation.
* @param probeType the type of probe used.
*/
double XBTProbe::getCoefficientA(int probeType) {
    double A = 0.0;
    switch (probeType ) {
        case SIPPICAN_T4_OLD:
            A = _SIPPICAN_T4_OLD_COEFFICIENT_A;
            break;
        case SIPPICAN_T4:
            A = _SIPPICAN_T4_COEFFICIENT_A;
            break;
        case SIPPICAN_T5:
            A = _SIPPICAN_T5_COEFFICIENT_A;
            break;
        case SIPPICAN_FAST_DEEP:
            A = _SIPPICAN_FAST_DEEP_COEFFICIENT_A;
            break;
        case SIPPICAN_T6_OLD:
            A = _SIPPICAN_T6_OLD_COEFFICIENT_A;
            break;
        case SIPPICAN_T6:
            A = _SIPPICAN_T6_COEFFICIENT_A;
            break;
        case SIPPICAN_T7_OLD:
            A = _SIPPICAN_T7_OLD_COEFFICIENT_A;
            break;
        case SIPPICAN_T7:
            A = _SIPPICAN_T7_COEFFICIENT_A;
            break;
        case SIPPICAN_DEEP_BLUE_OLD:
            A = _SIPPICAN_DEEP_BLUE_OLD_COEFFICIENT_A;
            break;
        case SIPPICAN_DEEP_BLUE:
            A = _SIPPICAN_DEEP_BLUE_COEFFICIENT_A;
            break;
        case SIPPICAN_T10:
            A = _SIPPICAN_T10_COEFFICIENT_A;
            break;
        case SIPPICAN_T11:
            A = _SIPPICAN_T11_COEFFICIENT_A;
            break;
        case SIPPICAN_AXBT:
            A = _SIPPICAN_AXBT_COEFFICIENT_A;
            break;
        case TSK_T4_OLD:
            A = _TSK_T4_OLD_COEFFICIENT_A;
            break;
        case TSK_T4:
            A = _TSK_T4_COEFFICIENT_A;
            break;
        case TSK_T6_OLD:
            A = _TSK_T6_OLD_COEFFICIENT_A;
            break;
        case TSK_T6:
            A = _TSK_T6_COEFFICIENT_A;
            break;
        case TSK_T7_OLD:
            A = _TSK_T7_OLD_COEFFICIENT_A;
            break;
        case TSK_T7:
            A = _TSK_T7_COEFFICIENT_A;
            break;
        case TSK_T5:
            A = _TSK_T5_COEFFICIENT_A;
            break;
        case TSK_T10:
            A = _TSK_T10_COEFFICIENT_A;
            break;
        case TSK_DEEP_BLUE_OLD:
            A = _TSK_DEEP_BLUE_OLD_COEFFICIENT_A;
            break;
        case TSK_DEEP_BLUE:
            A = _TSK_DEEP_BLUE_COEFFICIENT_A;
            break;
        case TSK_AXBT:
            A = _TSK_AXBT_COEFFICIENT_A;
            break;
        case SPARTON_XBT1:
            A = _SPARTON_XBT1_COEFFICIENT_A;
            break;
        case SPARTON_XBT3:
            A = _SPARTON_XBT3_COEFFICIENT_A;
            break;
        case SPARTON_XBT4:
            A = _SPARTON_XBT4_COEFFICIENT_A;
            break;
        case SPARTON_XBT5:
            A = _SPARTON_XBT5_COEFFICIENT_A;
            break;
        case SPARTON_XBT5DB:
            A = _SPARTON_XBT5DB_COEFFICIENT_A;
            break;
        case SPARTON_XBT7_OLD:
            A = _SPARTON_XBT7_OLD_COEFFICIENT_A;
            break;
        case SPARTON_XBT7:
            A = _SPARTON_XBT7_COEFFICIENT_A;
            break;
        case SPARTON_XBT7DB:
            A = _SPARTON_XBT7DB_COEFFICIENT_A;
            break;
        case SPARTON_XBT10:
            A = _SPARTON_XBT10_COEFFICIENT_A;
            break;
        case SPARTON_XBT20:
            A = _SPARTON_XBT20_COEFFICIENT_A;
            break;
        case SPARTON_XBT20DB:
            A = _SPARTON_XBT20DB_COEFFICIENT_A;
            break;
        case SPARTON_536AXBT:
            A = _SPARTON_536AXBT_COEFFICIENT_A;
            break;
        case SIPPICAN_12:
            A = _SIPPICAN_12_COEFFICIENT_A;
            break;

    }//end switch

    return A;
}

     /**
     * This method returns the B coefficient used in the fall rate equation.
     *
     * @return This method returns the B coefficient used in the
     * fall rate equation.
     * @param probeType the type of probe used.
     */
    double XBTProbe::getCoefficientB(int probeType) {
        double B = 0.0;
        switch (probeType) {
            case SIPPICAN_T4_OLD:
                B = _SIPPICAN_T4_OLD_COEFFICIENT_B;
                break;
            case SIPPICAN_T4:
                B = _SIPPICAN_T4_COEFFICIENT_B;
                break;
            case SIPPICAN_T5:
                B = _SIPPICAN_T5_COEFFICIENT_B;
                break;
            case SIPPICAN_FAST_DEEP:
                B = _SIPPICAN_FAST_DEEP_COEFFICIENT_B;
                break;
            case SIPPICAN_T6_OLD:
                B = _SIPPICAN_T6_OLD_COEFFICIENT_B;
                break;
            case SIPPICAN_T6:
                B = _SIPPICAN_T6_COEFFICIENT_B;
                break;
            case SIPPICAN_T7_OLD:
                B = _SIPPICAN_T7_OLD_COEFFICIENT_B;
                break;
            case SIPPICAN_T7:
                B = _SIPPICAN_T7_COEFFICIENT_B;
                break;
            case SIPPICAN_DEEP_BLUE_OLD:
                B = _SIPPICAN_DEEP_BLUE_OLD_COEFFICIENT_B;
                break;
            case SIPPICAN_DEEP_BLUE:
                B = _SIPPICAN_DEEP_BLUE_COEFFICIENT_B;
                break;
            case SIPPICAN_T10:
                B = _SIPPICAN_T10_COEFFICIENT_B;
                break;
            case SIPPICAN_T11:
                B = _SIPPICAN_T11_COEFFICIENT_B;
                break;
            case SIPPICAN_AXBT:
                B = _SIPPICAN_AXBT_COEFFICIENT_B;
                break;
            case TSK_T4_OLD:
                B = _TSK_T4_OLD_COEFFICIENT_B;
                break;
            case TSK_T4:
                B = _TSK_T4_COEFFICIENT_B;
                break;
            case TSK_T6_OLD:
                B = _TSK_T6_OLD_COEFFICIENT_B;
                break;
            case TSK_T6:
                B = _TSK_T6_COEFFICIENT_B;
                break;
            case TSK_T7_OLD:
                B = _TSK_T7_OLD_COEFFICIENT_B;
                break;
            case TSK_T7:
                B = _TSK_T7_COEFFICIENT_B;
                break;
            case TSK_T5:
                B = _TSK_T5_COEFFICIENT_B;
                break;
            case TSK_T10:
                B = _TSK_T10_COEFFICIENT_B;
                break;
            case TSK_DEEP_BLUE_OLD:
                B = _TSK_DEEP_BLUE_OLD_COEFFICIENT_B;
                break;
            case TSK_DEEP_BLUE:
                B = _TSK_DEEP_BLUE_COEFFICIENT_B;
                break;
            case TSK_AXBT:
                B = _TSK_AXBT_COEFFICIENT_B;
                break;
            case SPARTON_XBT1:
                B = _SPARTON_XBT1_COEFFICIENT_B;
                break;
            case SPARTON_XBT3:
                B = _SPARTON_XBT3_COEFFICIENT_B;
                break;
            case SPARTON_XBT4:
                B = _SPARTON_XBT4_COEFFICIENT_B;
                break;
            case SPARTON_XBT5:
                B = _SPARTON_XBT5_COEFFICIENT_B;
                break;
            case SPARTON_XBT5DB:
                B = _SPARTON_XBT5DB_COEFFICIENT_B;
                break;
            case SPARTON_XBT7_OLD:
                B = _SPARTON_XBT7_OLD_COEFFICIENT_B;
                break;
            case SPARTON_XBT7:
                B = _SPARTON_XBT7_COEFFICIENT_B;
                break;
            case SPARTON_XBT7DB:
                B = _SPARTON_XBT7DB_COEFFICIENT_B;
                break;
            case SPARTON_XBT10:
                B = _SPARTON_XBT10_COEFFICIENT_B;
                break;
            case SPARTON_XBT20:
                B = _SPARTON_XBT20_COEFFICIENT_B;
                break;
            case SPARTON_XBT20DB:
                B = _SPARTON_XBT20DB_COEFFICIENT_B;
                break;
            case SPARTON_536AXBT:
                B = _SPARTON_536AXBT_COEFFICIENT_B;
                break;
            case SIPPICAN_12:
                B = _SIPPICAN_12_COEFFICIENT_B;
                break;

        }//end switch

        return B;
    }
     /**
     * This method returns the maximumm depth that the probe is designed for.
     *
     * @return This method returns the maximumm depth that the probe is designed for.
     * @param probeType the type of probe used.
     */
double XBTProbe::getMaxDepth(int probeType ) {
        double maxDepth = 4000.00;
        switch (probeType ) {
            case SIPPICAN_T4_OLD:
                maxDepth = _SIPPICAN_T4_OLD_MAX_DEPTH;
                break;
            case SIPPICAN_T4:
                maxDepth = _SIPPICAN_T4_MAX_DEPTH;
                break;
            case SIPPICAN_T5:
                maxDepth = _SIPPICAN_T5_MAX_DEPTH;
                break;
            case SIPPICAN_FAST_DEEP:
                maxDepth = _SIPPICAN_FAST_DEEP_MAX_DEPTH;
                break;
            case SIPPICAN_T6_OLD:
                maxDepth = _SIPPICAN_T6_OLD_MAX_DEPTH;
                break;
            case SIPPICAN_T6:
                maxDepth = _SIPPICAN_T6_MAX_DEPTH;
                break;
            case SIPPICAN_T7_OLD:
                maxDepth = _SIPPICAN_T7_OLD_MAX_DEPTH;
                break;
            case SIPPICAN_T7:
                maxDepth = _SIPPICAN_T7_MAX_DEPTH;
                break;
            case SIPPICAN_DEEP_BLUE_OLD:
                maxDepth = _SIPPICAN_DEEP_BLUE_OLD_MAX_DEPTH;
                break;
            case SIPPICAN_DEEP_BLUE:
                maxDepth = _SIPPICAN_DEEP_BLUE_MAX_DEPTH;
                break;
            case SIPPICAN_T10:
                maxDepth = _SIPPICAN_T10_MAX_DEPTH;
                break;
            case SIPPICAN_T11:
                maxDepth = _SIPPICAN_T11_MAX_DEPTH;
                break;
            case SIPPICAN_AXBT:
                maxDepth = _SIPPICAN_AXBT_MAX_DEPTH;
                break;
            case TSK_T4_OLD:
                maxDepth = _TSK_T4_OLD_MAX_DEPTH;
                break;
            case TSK_T4:
                maxDepth = _TSK_T4_MAX_DEPTH;
                break;
            case TSK_T6_OLD:
                maxDepth = _TSK_T6_OLD_MAX_DEPTH;
                break;
            case TSK_T6:
                maxDepth = _TSK_T6_MAX_DEPTH;
                break;
            case TSK_T7_OLD:
                maxDepth = _TSK_T7_OLD_MAX_DEPTH;
                break;
            case TSK_T7:
                maxDepth = _TSK_T7_MAX_DEPTH;
                break;
            case TSK_T5:
                maxDepth = _TSK_T5_MAX_DEPTH;
                break;
            case TSK_T10:
                maxDepth = _TSK_T10_MAX_DEPTH;
                break;
            case TSK_DEEP_BLUE_OLD:
                maxDepth = _TSK_DEEP_BLUE_OLD_MAX_DEPTH;
                break;
            case TSK_DEEP_BLUE:
                maxDepth = _TSK_DEEP_BLUE_MAX_DEPTH;
                break;
            case TSK_AXBT:
                maxDepth = _TSK_AXBT_MAX_DEPTH;
                break;
            case SPARTON_XBT1:
                maxDepth = _SPARTON_XBT1_MAX_DEPTH;
                break;
            case SPARTON_XBT3:
                maxDepth = _SPARTON_XBT3_MAX_DEPTH;
                break;
            case SPARTON_XBT4:
                maxDepth = _SPARTON_XBT4_MAX_DEPTH;
                break;
            case SPARTON_XBT5:
                maxDepth = _SPARTON_XBT5_MAX_DEPTH;
                break;
            case SPARTON_XBT5DB:
                maxDepth = _SPARTON_XBT5DB_MAX_DEPTH;
                break;
            case SPARTON_XBT7_OLD:
                maxDepth = _SPARTON_XBT7_OLD_MAX_DEPTH;
                break;
            case SPARTON_XBT7:
                maxDepth = _SPARTON_XBT7_MAX_DEPTH;
                break;
            case SPARTON_XBT7DB:
                maxDepth = _SPARTON_XBT7DB_MAX_DEPTH;
                break;
            case SPARTON_XBT10:
                maxDepth = _SPARTON_XBT10_MAX_DEPTH;
                break;
            case SPARTON_XBT20:
                maxDepth = _SPARTON_XBT20_MAX_DEPTH;
                break;
            case SPARTON_XBT20DB:
                maxDepth = _SPARTON_XBT20DB_MAX_DEPTH;
                break;
            case SPARTON_536AXBT:
                maxDepth = _SPARTON_536AXBT_MAX_DEPTH;
                break;
            case SIPPICAN_12:
                maxDepth = _SIPPICAN_12_MAX_DEPTH;
                break;

        }//end switch

        return maxDepth;
    }
     /**
     * This method returns a description for the probe.
     *
     * @return This method returns a description for the probe
     * @param probeType the type of probe used..
     */
std::string XBTProbe::getProbeDescription(int probeType ) {
	const char* probeDescription = "Unknown";
        switch (probeType ) {
            case SIPPICAN_T4_OLD:
                probeDescription = _SIPPICAN_T4_OLD_DESCRIPTION;
                break;
            case SIPPICAN_T4:
                probeDescription = _SIPPICAN_T4_DESCRIPTION;
                break;
            case SIPPICAN_T5:
                probeDescription = _SIPPICAN_T5_DESCRIPTION;
                break;
            case SIPPICAN_FAST_DEEP:
                probeDescription = _SIPPICAN_FAST_DEEP_DESCRIPTION;
                break;
            case SIPPICAN_T6_OLD:
                probeDescription = _SIPPICAN_T6_OLD_DESCRIPTION;
                break;
            case SIPPICAN_T6:
                probeDescription = _SIPPICAN_T6_DESCRIPTION;
                break;
            case SIPPICAN_T7_OLD:
                probeDescription = _SIPPICAN_T7_OLD_DESCRIPTION;
                break;
            case SIPPICAN_T7:
                probeDescription = _SIPPICAN_T7_DESCRIPTION;
                break;
            case SIPPICAN_DEEP_BLUE_OLD:
                probeDescription = _SIPPICAN_DEEP_BLUE_OLD_DESCRIPTION;
                break;
            case SIPPICAN_DEEP_BLUE:
                probeDescription = _SIPPICAN_DEEP_BLUE_DESCRIPTION;
                break;
            case SIPPICAN_T10:
                probeDescription = _SIPPICAN_T10_DESCRIPTION;
                break;
            case SIPPICAN_T11:
                probeDescription = _SIPPICAN_T11_DESCRIPTION;
                break;
            case SIPPICAN_AXBT:
                probeDescription = _SIPPICAN_AXBT_DESCRIPTION;
                break;
            case TSK_T4_OLD:
                probeDescription = _TSK_T4_OLD_DESCRIPTION;
                break;
            case TSK_T4:
                probeDescription = _TSK_T4_DESCRIPTION;
                break;
            case TSK_T6_OLD:
                probeDescription = _TSK_T6_OLD_DESCRIPTION;
                break;
            case TSK_T6:
                probeDescription = _TSK_T6_DESCRIPTION;
                break;
            case TSK_T7_OLD:
                probeDescription = _TSK_T7_OLD_DESCRIPTION;
                break;
            case TSK_T7:
                probeDescription = _TSK_T7_DESCRIPTION;
                break;
            case TSK_T5:
                probeDescription = _TSK_T5_DESCRIPTION;
                break;
            case TSK_T10:
                probeDescription = _TSK_T10_DESCRIPTION;
                break;
            case TSK_DEEP_BLUE_OLD:
                probeDescription = _TSK_DEEP_BLUE_OLD_DESCRIPTION;
                break;
            case TSK_DEEP_BLUE:
                probeDescription = _TSK_DEEP_BLUE_DESCRIPTION;
                break;
            case TSK_AXBT:
                probeDescription = _TSK_AXBT_DESCRIPTION;
                break;
            case SPARTON_XBT1:
                probeDescription = _SPARTON_XBT1_DESCRIPTION;
                break;
            case SPARTON_XBT3:
                probeDescription = _SPARTON_XBT3_DESCRIPTION;
                break;
            case SPARTON_XBT4:
                probeDescription = _SPARTON_XBT4_DESCRIPTION;
                break;
            case SPARTON_XBT5:
                probeDescription = _SPARTON_XBT5_DESCRIPTION;
                break;
            case SPARTON_XBT5DB:
                probeDescription = _SPARTON_XBT5DB_DESCRIPTION;
                break;
            case SPARTON_XBT7_OLD:
                probeDescription = _SPARTON_XBT7_OLD_DESCRIPTION;
                break;
            case SPARTON_XBT7:
                probeDescription = _SPARTON_XBT7_DESCRIPTION;
                break;
            case SPARTON_XBT7DB:
                probeDescription = _SPARTON_XBT7DB_DESCRIPTION;
                break;
            case SPARTON_XBT10:
                probeDescription = _SPARTON_XBT10_DESCRIPTION;
                break;
            case SPARTON_XBT20:
                probeDescription = _SPARTON_XBT20_DESCRIPTION;
                break;
            case SPARTON_XBT20DB:
                probeDescription = _SPARTON_XBT20DB_DESCRIPTION;
                break;
            case SPARTON_536AXBT:
                probeDescription = _SPARTON_536AXBT_DESCRIPTION;
                break;
            case SIPPICAN_12:
                probeDescription = _SIPPICAN_12_DESCRIPTION;
                break;

        }//end switch

        return std::string(probeDescription);
    }
