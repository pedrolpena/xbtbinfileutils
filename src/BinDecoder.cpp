/*
 * BinDecoder.cpp
 *
 *  Created on: Jul 21, 2019
 *      Author: pedro
 */

#include "BinDecoder.h"

#include <algorithm>
#include <string>
#include "MessageType.h"
#include "XBTProfileDataRanges.h"


BinDecoder::BinDecoder(std::string filePath) {


	inputFile.open(filePath,std::ios_base::binary | std::ios_base::ate);

	size_t size = 0;
	if (inputFile.is_open()){
		char* bytes = 0;
		inputFile.seekg( 0 , std::ios_base::end );
		size = inputFile.tellg();
		inputFile.seekg( 0 , std::ios_base::beg );
		bytes = new char[ size ];
		inputFile.read(bytes,size);
		bits = stdex::bitvector(8*size);
int max = 0;
		for( unsigned int i = 0; i < size ; i++){
			for( unsigned int j = 0; j < 8 ; j++){
				bool val;
				char bit;
				bit = ( bytes[i] >> j ) & 0x01;

				if ( bit == 1 )
					val = true;
				else
					val = false;

				bits.set(max,val);


				max ++;
		}//end for


		}//endfor
		bits = changeEndian(bits);

	}//end if

	inputFile.close();
}

BinDecoder::~BinDecoder() {

	if(inputFile.is_open()){

		inputFile.close();
	}
}

/**
 * <strong>(FXY)</strong>-This method flips the "endianes" of a BitSet
 * object and returns a BitSet object with flipped "endianess".
 *
 * @return <strong>(FXY)</strong>-This method flips the "endianes" of a
 * BitSet object and returns a BitSet object with flipped "endianess".
 */
stdex::bitvector BinDecoder::changeEndian(stdex::bitvector b) {
    bool temp;
    int width = b.size();
    for (int i = 0; i < width; i = i + 8) {
        for (int j = 0; j < 4; j++) {
            temp = b.test(i + j);
            b.set(i + j, b.test(i - j + 7));
            b.set(i - j + 7, temp);
        }//end for

    }//end for
    return b;
}

/**
 * <strong>(FXY205030A)</strong>-This method computes and returns an integer
 * from a sequence of bits.
 *
 * @param start the position from which to start extracting a binary
 * sequence.
 * @param end the position from which to end extracting a binary sequence.
 * @return <strong>(FXY205030A)</strong>-This method computes and returns an
 * integer from a sequence of bits.
 */
int BinDecoder::toInteger(int start, int end) {

        if (start < 0 || end < 0 || end - start <= 0) {
            return -999;
        }
        // the file is too short to hold this value
        if (end >= (int)bits.size()) {
            return -999;
        }
        end++;
        int bLength = end - start;
        stdex::bitvector b;
         b = stdex::bitvector(bLength);

        int integer = 0;
        char bit;

        for (int i = 0; i < bLength; i++) {
        	b.set(i,bits.test(start + i));
            if (i < bLength + 1) {
                if (b.test(i)) {
                    bit = 0x01;
                } else {
                    bit = 0x00;
                }
                integer = integer | bit;
            }//end if
            if (i < bLength - 1) {
                integer = integer << 1;
            }

        }//end for

        return integer;


}

/**
 * This method returns how many of the measurement points are actually
 * stored in the file. Truncated files hold fewer points than the times
 * replicated field says.
 *
 * @param start the bit position of the first measurement point
 * @param points the number of points the profile says it holds
 * @param bSize the number of bits used by each point
 * @return the number of points that can be read from the file
 */
int BinDecoder::getAvailablePoints(int start, int points, int bSize) {
    int available = ((int)bits.size() - start) / bSize;
    if (available < 0) {
        available = 0;
    }
    return std::min(points, available);
}

int BinDecoder::getOldMessageType() {
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::OLD_MESSAGE_TYPE,1)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::OLD_MESSAGE_TYPE,1)[1];

    try {

        return toInteger(start, end);
    } catch (int e) {
        return 0;
    }
}

/**
 * <strong>(FXY48216)</strong>-This method returns the new message types
 * used in Amverseas
 *
 * @return  <strong>(FXY48216)</strong>-This method returns the new message
 * types used in Amverseas The types can be a value from 1-3. Returns 0 when
 * there is an exception.
 */
int BinDecoder::getNewMessageType() {

    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NEW_MESSAGE_TYPE,1)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NEW_MESSAGE_TYPE,1)[1];

    try {

        return toInteger(start, end);
    } catch (int e) {
        return 0;
    }//end catch
}
/**
 * This method returns a string representation of the binary sequence in the
 * specified range in the bin file.
 *
 * @param start the bit position from which to start constructing the binary
 * sequence.
 * @param end the bit position from which to stop construction the binary
 * sequence.
 * @return <strong>(FXY)</strong>-This method returns a string
 * representation of the binary sequence in the specified range in the bin
 * file.
 */
std::string BinDecoder::getBinarySequence(int start, int end) {
	std::string str = "";
    for (int i = start; i <= end; i++) {

        if (bits.test(i)) {
            str += "1";
        }
        if (!bits.test(i)) {
            str += "0";
        }
        if ((i - start + 1) % 8 == 0) {
            str += " ";
        }

    }//end for
    return str;
}

/**
 * <strong>(FXY)</strong>-This method computes and returns a string from a
 * sequence of bits.
 *
 * @param start the position from which to start extracting a binary
 * sequence.
 * @param end the position from which to end extracting a binary sequence.
 * @return <strong>(FXY)</strong>-This method computes and returns a string
 * from a sequence of bits.
 */
std::string BinDecoder::toString(int start, int end) {

	if (start < 0 || end < 0 || end - start <= 0) {
		return "";
	}

	std::string str = "";
	char x =0;
	char bit;
	unsigned int bLength = end - start;
	stdex::bitvector b;

	b = stdex::bitvector(bLength + 1);
	for (unsigned int i = 0; i <= bLength; i++) {

		if((start + i) < bits.size()){
			b.set( i , bits.test( start + i));
		}
	}

	for (unsigned int i = 0; i < b.size(); i++) {

		if (b.test(i)) {
			bit = 0x01;
		}
		else {
			bit = 0x00;
		}
		x = x << 1;
		x |= bit;
		if ( ( i + 1 ) % 8 == 0 ) {
			if (x >= 32 && x <= 126) { //only add printable characters
				str+=x;
			}
			x = 0;
		}//end if
	}//end for

	return str;
}
/**
 * <strong>(FXY1011)</strong>-This method returns the ship's WMO ID
 *
 * @return    <strong>(FXY1011)</strong>-This method returns a the ship's WMO
 * ID
 */
std::string BinDecoder::getWMOID() {
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::WMOID,1)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::WMOID,1)[1];
    return toString(start, end);

}

/**
 * <strong>(FXY1200)</strong>-This method returns the name of the ship. If
 * there is no
 *
 * @return <strong>(FXY1200)</strong>-This method returns the name of the
 * ship. If there is no name then the string null is returned.
 */
std::string BinDecoder::getShipName() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SHIP_NAME,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SHIP_NAME,mt)[1];
    return toString(start, end);

}

/**
 * <strong>(FXY1036)</strong>-This method returns the agency in charge of
 * operating the observation platform.
 *
 * @return <strong>(FXY1036)</strong>-This method returns the agency in
 * charge of operating the observation platform. A value of -999 is returned
 * when there is no value.
 */
int BinDecoder::getAgencyOwner() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::AGENCY_OWNER,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::AGENCY_OWNER,mt)[1];
    return toInteger(start, end);

}

/**
 * <strong>(FXY1011)</strong>-This method returns the ship's WMO ID
 *
 * @return    <strong>(FXY1011)</strong>-This method returns a the ship's WMO
 * ID
 */
std::string BinDecoder::getCallsign() {
	return getWMOID();
}
/**
 * <strong>(FXY33003)</strong>-This method returns the quality of the data
 * as entered by the user.
 *
 * @return <strong>(FXY33003)</strong>-This method returns the quality of
 * the data as entered by the user. If the user suspects the data is of a
 * quality other than good, the user can add it to the profile after the
 * measurement has been made. A value of -999 is returned when there is no
 * value.
 */
int BinDecoder::getDataQuality() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::DATA_QUALITY,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::DATA_QUALITY,mt)[1];
    return toInteger(start, end);
}

/**
 * <strong>(FXY4003)</strong>-This method returns the day in which the
 * measurement was made.
 *
 * @return <strong>(FXY4003)</strong>-This method returns the day in which
 * the measurement was made. A value of -999 is returned when there is no
 * value.
 */
int BinDecoder::getDay() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::DAY,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::DAY,mt)[1];
    return toInteger(start, end);
}

/**
 * <strong>(FXY12004)</strong>-This method returns the dry bulb temperature
 * at the time of the measurement.
 *
 * @return <strong>(FXY12004)</strong>-This method returns the dry bulb
 * temperature at the time of the measurement. A value of -99.9 is returned
 * when there is no value.
 */
double BinDecoder::getDryBulbTemperature() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::DRY_BULB_TEMPERATURE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::DRY_BULB_TEMPERATURE,mt)[1];
    return toInteger(start, end)/10.0;
}
/**
 * <strong>(FXY4004)</strong>-This method returns the GMT hour when the
 * measurement was made.
 *
 * @return <strong>(FXY4004)</strong>-This method returns the GMT hour when
 * the measurement was made. A value of -999 is returned when there is no
 * value.
 */
int BinDecoder::getHour() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::HOUR,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::HOUR,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY22067)</strong>-This method returns a table value that
 * represents the type of probe used to make the measurement.
 *
 * @return <strong>(FXY22067)</strong>-This method returns a table value
 * that represents the type of probe used to make the measurement.<br>
 * e.g. a value of 52 refers to a Deep Blue XBT. A value of -999 is returned
 * when there is no value.
 */
int BinDecoder::getInstrumentType() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::INSTRUMENT_TYPE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::INSTRUMENT_TYPE,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY5001)</strong>-This method returns the latitude where the
 * measurement was made.
 *
 * @return  <strong>(FXY5001)</strong>-This method returns the latitude where
 * the measurement was made. The value is returned in decimal degrees.
 */
double BinDecoder::getLatitude() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::LATITUDE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::LATITUDE,mt)[1];

    return (toInteger(start, end) - 9000000) / 100000.0;
}
/**
 * <strong>(FXY48305)</strong>-This method returns the approximate height of
 * the autolauncher from the surface of the water.
 *
 * @return <strong>(FXY48305)</strong>-This method returns the approximate
 * height of the autolauncher from the surface of the water. A value of
 * -9.99 is returned when there is no value.
 */
double BinDecoder::getLaunchHeight() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::LAUNCH_HEIGHT,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::LAUNCH_HEIGHT,mt)[1];
    return toInteger(start, end) / 100.00;
}
/**
 * <strong>(FXY1201)</strong>-This method returns the ships International
 * Maritime Organization (IMO) number.
 *
 * @return <strong>(FXY1201)</strong>-This method returns the ships
 * International Maritime Organization (IMO) number. A value of -999 is
 * returned when there is no value.
 */
int BinDecoder::getLloyds() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::LLOYDS,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::LLOYDS,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY6001)</strong>-This method returns the longitude where the
 * measurement was made
 *
 * @return  <strong>(FXY6001)</strong>-This method returns the longitude
 * where the measurement was made. The value is returned in decimal degrees.
 */
double BinDecoder::getLongitude() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::LONGITUDE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::LONGITUDE,mt)[1];
    return (toInteger(start, end) - 18000000) / 100000.0;
}
/**
 * <strong>(FXY4005)</strong>-This method returns the GMT minute when the
 * measurement was made
 *
 * @return <strong>(FXY4005)</strong>-This method returns the GMT minute
 * when the measurement was made A value of -999 is returned when there is
 * no value.
 */
int BinDecoder::getMinute() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::MINUTE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::MINUTE,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY4002)</strong>-This method returns the month in which the
 * measurement was made.
 *
 * @return <strong>(FXY4002)</strong>-This method returns the month in which
 * the measurement was made. A value of -999 is returned when there is no
 * value.
 */
int BinDecoder::getMonth() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::MONTH,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::MONTH,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY100000)</strong>-Delayed Replicator
 *
 * @return <strong>(FXY100000)</strong>-Delayed Replicator! Different from
 * BUFR. The number contained within is the number of following consecutive
 * fields to replicate. This field must be followed by FXY31200 to identify
 * how many groups of replicated fields follow. Always delayed replication.
 * value of -999 is returned when there is no value.
 */
int BinDecoder::getNumberOfRepeatedFields() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_REPEATED_FIELDS,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_REPEATED_FIELDS,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY31001B)</strong>-This method returns the number of 40 bit
 * blocks or 5 characters, used to store the rider name.
 *
 * @return <strong>(FXY31001B)</strong>-This method returns the number of 40
 * bit blocks or 5 characters, used to store the rider name. A value of -999
 * is returned when there is no value.
 */
int BinDecoder::getNumberOfRiderBlocks() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_RIDER_BLOCKS,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_RIDER_BLOCKS,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY31001B)</strong>-This method returns the number of 40 bit
 * blocks or 5 characters, used to store the rider's email address.
 *
 * @return <strong>(FXY31001B)</strong>-This method returns the number of 40
 * bit blocks or 5 characters, used to store the rider's email address. A
 * value of -999 is returned when there is no value.
 */
int BinDecoder::getNumberOfRiderEmailBlocks() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_RIDER_EMAIL_BLOCKS,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_RIDER_EMAIL_BLOCKS,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY31001B)</strong>-This method returns the number of 40 bit
 * blocks or 5 characters, used to store the riders institution's name.
 *
 * @return <strong>(FXY31001B)</strong>-This method returns the number of 40
 * bit blocks or 5 characters, used to store the riders institution's name.
 * A value of -999 is returned when there is no value.
 */
int BinDecoder::getNumberOfRiderInstitutionBlocks() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_RIDER_INSTITUTION_BLOCKS,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_RIDER_INSTITUTION_BLOCKS,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY31001B)</strong>-This method returns the number of 40 bit
 * blocks or 5 characters, used to store the rider's phone number.
 *
 * @return <strong>(FXY31001B)</strong>-This method returns the number of 40
 * bit blocks or 5 characters, used to store the rider's phone number. A
 * value of -999 is returned when there is no value.
 */
int BinDecoder::getNumberOfRiderPhoneBlocks() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_RIDER_PHONE_BLOCKS,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::NUMBER_OF_RIDER_PHONE_BLOCKS,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY48300)</strong>-This method returns the serial number of the
 * XBT used to make the measurement.
 *
 * @return <strong>(FXY48300)</strong>-This method returns the serial number
 * of the XBT used to make the measurement. A value of -999 is returned when
 * there is no value.
 */
int BinDecoder::getProbeSerialNumber() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::PROBE_SERIAL_NUMBER,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::PROBE_SERIAL_NUMBER,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY22068)</strong>-This method returns a table value that
 * represents the device that determines the thermistor value in the XBT.
 *
 * @return <strong>(FXY22068)</strong>-This method returns a table value
 * that represents the device that determines the thermistor value in the
 * XBT.<br>
 * e.g. a value of 6 refers to any of the different Sippican MK21 models. A
 * value of -999 is returned when there is no value.
 *
 */
int BinDecoder::getRecorderType() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::RECORDER_TYPE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::RECORDER_TYPE,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY22042S)</strong>-This method returns an array containing the
 * resistance measurements made.
 *
 * @return <strong>(FXY22042S)</strong>-This method returns an array
 * containing the resistance measurements made. An empty array is returned
 * when there are no values.
 */
std::vector<double> BinDecoder::getResistance() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::RESISTANCE_POINTS,mt)[0];
    int points = getTimesReplicated();
    int bSize = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[1]
            - XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[0] + 1;

    if (points < 0) {
        return  std::vector<double>(1,0);
    }
    points = getAvailablePoints(start, points, bSize);

    std::vector<double> temps(points,0);
    int counter = 0;
    for (int i = start; i < start + points * bSize; i += bSize) {
        temps[counter] = (toInteger(i, i + bSize - 1) / 100 + 0);
        counter++;

    }//end for

    return temps;
}
/**
 * <strong>(FXY31200)</strong>-This method returns the number of temperature
 * measurement points recorded.
 *
 * @return <strong>(FXY31200)</strong>-This method returns the number of
 * temperature measurement points recorded. A value of -999 is returned when
 * there is no value.
 */
int BinDecoder::getTimesReplicated() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::TIMES_REPLICATED,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::TIMES_REPLICATED,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY22042S)</strong>-This method returns an array containing the
 * Resistance measurements made.
 *
 * @return <strong>(FXY22042S)</strong>-This method returns an array
 * containing the resistance measurements made. An empty array is returned
 * when there are no values.
 */
std::vector<double> BinDecoder::getResistancePoints() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::RESISTANCE_POINTS,mt)[0];
    int points = getTimesReplicated();
    int bSize = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[1]
            - XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[0] + 1;
    if (points < 0) {
    	return  std::vector<double>(1,0);
    }
    points = getAvailablePoints(start, points, bSize);

    std::vector<double> resistance(points,0);
    int counter = 0;
    for (int i = start; i < start + points * bSize; i += bSize) {
        resistance[counter] = (((double)toInteger(i, i + bSize - 1) - 0.00 ) / 100.00);
        counter++;

    }//end for

    return resistance;
}
/**
 * <strong>(FXY205030A)</strong>-This method returns the rider's email.
 *
 * @return <strong>(FXY205030A)</strong>-This method returns the rider's
 * email. A value of null is returned when there is no value.
 */
std::string BinDecoder::getRiderEmails() {

    int mt = getNewMessageType();
    int bSize = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[1]
            - XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[0] + 1;
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::RIDER_EMAILS,mt)[0];

    int s = start + bSize * getTimesReplicated() + getNumberOfRiderBlocks() * 40;
    return toString(s, s + getNumberOfRiderEmailBlocks() * 40);
}
/**
 * <strong>(FXY205030A)</strong>-This method returns the rider's
 * institution.
 *
 * @return <strong>(FXY205030A)</strong>-This method returns the rider's
 * institution. A value of null is returned when there is no value.
 */
std::string BinDecoder::getRiderInstitutions() {
    int mt = getNewMessageType();
    int bSize = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[1]
            - XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[0] + 1;
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::RIDER_INSTITUTIONS,mt)[0];
    int s = start + bSize * getTimesReplicated() + getNumberOfRiderBlocks() * 40 + getNumberOfRiderEmailBlocks() * 40;
    return toString(s, s + getNumberOfRiderInstitutionBlocks() * 40);
}
/**
 * <strong>(FXY205030A)</strong>-This method returns the rider's name.
 *
 * @return <strong>(FXY205030A)</strong>-This method returns the rider's
 * name. A value of null is returned when there is no value.
 */
std::string BinDecoder::getRiderNames() {
    int mt = getNewMessageType();

    int bSize = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[1]
            - XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[0] + 1;
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::RIDER_NAMES,mt)[0];

    int s = start + bSize * getTimesReplicated();
    return toString(s, s + getNumberOfRiderBlocks() * 40);
}
/**
 * <strong>(FXY205030A)</strong>-This method returns the rider's phone
 * number.
 *
 * @return <strong>(FXY205030A)</strong>-This method returns the rider's
 * phone number. A value of null is returned when there is no value.
 */
std::string BinDecoder::getRiderPhones() {
    int mt = getNewMessageType();
    int bSize = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[1]
            - XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[0] + 1;
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::RIDER_PHONES,mt)[0];

    int s = start + bSize * getTimesReplicated() + getNumberOfRiderBlocks() * 40 + getNumberOfRiderEmailBlocks() * 40 + getNumberOfRiderInstitutionBlocks() * 40;
    return toString(s, s + getNumberOfRiderPhoneBlocks() * 40);
}		// end method
/**
 * <strong>(FXY7062S)</strong>-This method returns the depth below the
 * sea/water surface
 *
 * @return <strong>(FXY7062S)</strong>-This method returns the depth below
 * the sea/water surface A value of -999 is returned when there is no value.
 */
double BinDecoder::getSeaDepth() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_DEPTH,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_DEPTH,mt)[1];
    return (double) toInteger(start, end);
}
/**
 * <strong>(FXY22042S)</strong>-This method returns the SST or the first
 * temperature measurement made.
 *
 * @return <strong>(FXY22042S)</strong>-This method returns the SST or the
 * first temperature measurement made. A value of -13.99 is returned when
 * there is no value.
 */
double BinDecoder::getSeaResistance() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_RESISTANCE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_RESISTANCE,mt)[1];
    return (((double)toInteger(start, end) + 0.00 )/ 100.00);
}
/**
 * <strong>(FXY22004)</strong>-This method returns the sea surface current
 * direction .
 *
 * @return <strong>(FXY22004)</strong>-This method returns the sea surface
 * current direction . A value of -999 is returned when there is no value.
 */
int BinDecoder::getSeaSurfaceCurrentDirection() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_CURRENT_DIRECTION,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_CURRENT_DIRECTION,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY2030)</strong>-This method returns the sea current
 * measurement method.
 *
 * @return <strong>(FXY2030)</strong>-This method returns the sea current
 * measurement method. A value of -999 is returned when there is no value.
 */
int BinDecoder::getSeaSurfaceCurrentMeasurementMethod() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_CURRENT_MEASURING_METHOD,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_CURRENT_MEASURING_METHOD,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY22031)</strong>-This method returns the sea surface current
 * speed
 *
 * @return <strong>(FXY22031)</strong>-This method returns the sea surface
 * current speed A value of -999 is returned when there is no value.
 */
double BinDecoder::getSeaSurfaceCurrentSpeed() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_CURRENT_SPEED,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_CURRENT_SPEED,mt)[1];
    return toInteger(start, end) / 100.00;
}
/**
 * <strong>(FXY22042S)</strong>-This method returns the SST or the first
 * temperature measurement made.
 *
 * @return <strong>(FXY22042S)</strong>-This method returns the SST or the
 * first temperature measurement made. A value of -13.99 is returned when
 * there is no value.
 */
double BinDecoder::getSeaTemperature() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEA_SURFACE_TEMPERATURE,mt)[1];
    return (toInteger(start, end) - 400.00) / 100.00 +.005;
}
/**
 * <strong>(FXY48201)</strong>-This method returns the version of Amverseas
 * used to make the measurement.
 *
 * @return <strong>(FXY48201)</strong>-This method returns the version of
 * Amverseas used to make the measurement. This value must later be massaged
 * to get the actual version. e.g. 920 must be dived by 100 to get Amverseas
 * version 9.20 A value of -999 is returned when there is no value.
 */
int BinDecoder::getSeasVersion() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEAS_VERSION,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEAS_VERSION,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY48304)</strong>-
 *
 * @return <strong>(FXY48304)</strong>-This method returns the Amverseas Sequence
 * Number.
 */
int BinDecoder::getSequenceNumber() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEQUENCE_NUM,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SEQUENCE_NUM,mt)[1];

    return toInteger(start, end);
}
/**
 * <strong>(FXY1012)</strong>-This method returns the direction in degrees
 * that the ship was heading when the measurement was made.
 *
 * @return <strong>(FXY1012)</strong>-This method returns the direction in
 * degrees that the ship was heading when the measurement was made. A value
 * of -999.0 is returned when there is no value.
 */
double BinDecoder::getShipDirection() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SHIP_DIRECTION,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SHIP_DIRECTION,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY22067)</strong>-This method returns a table value that
 * represents the type of probe used to make the measurement.
 *
 * @return <strong>(FXY22067)</strong>-This method returns a table value
 * that represents the type of probe used to make the measurement.<br>
 * e.g. a value of 52 refers to a Deep Blue XBT. A value of -999 is returned
 * when there is no value.
 */
double BinDecoder::getShipSpeed() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SHIP_SPEED,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SHIP_SPEED,mt)[1];
    return toInteger(start, end)/100.0;
}
/**
 * <strong>(FXY1080)</strong>-This method returns the name given to the
 * transect.
 *
 * @return  <strong>(FXY1080)</strong>-This method returns the name given to
 * the transect. e.g. AX10 This field became available beginning with
 * message type 2
 */
std::string BinDecoder::getSoopLine() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SOOP_LINE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::SOOP_LINE,mt)[1];
    return toString(start, end);
}
/**
 * <strong>(FXY22042S)</strong>-This method returns an array containing the
 * temperature measurements made.
 *
 * @return <strong>(FXY22042S)</strong>-This method returns an array
 * containing the temperature measurements made. An empty array is returned
 * when there are no values.
 */
std::vector<double> BinDecoder::getTemperaturePoints() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::TEMPERATURE_POINTS,mt)[0];
    int points = getTimesReplicated();

    if (points < 0) {
    	return std::vector<double>(1,0);
    }
    points = getAvailablePoints(start, points, 12);

    std::vector<double> temps(points,0);
    int counter = 0;
    for (int i = start; i < start + points * 12; i += 12) {
        temps[counter] = ((double)toInteger(i, i + 11) - 400.00) / 100.00 + .000;
        counter++;

    }//end for

    return temps;
}
/**
 * <strong>(FXY48303)</strong>-This method returns the resolution of the
 * profile.
 *
 * @return <strong>(FXY48303)</strong>-This method returns the resolution of
 * the profile. A value of 1 is full resolution a value of 2 is 1 meter
 * resolution and a value of 3 is two meter resolution. A value of -999 is
 * returned when there is no value.
 */
int BinDecoder::getThisDataIs() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::THIS_DATA_IS,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::THIS_DATA_IS,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY22063)</strong>-This method returns the total water depth at
 * the location where the measurement was made.
 *
 * @return <strong>(FXY22063)</strong>-This method returns the total water
 * depth at the location where the measurement was made. A value of -999 is
 * returned when there is no value.
 */
int BinDecoder::getTotalWaterDepth() {
     int mt = getNewMessageType();
     int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::TOTAL_WATER_DEPTH,mt)[0];
     int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::TOTAL_WATER_DEPTH,mt)[1];
     return toInteger(start, end);
}
/**
 * <strong>(FXY5036)</strong>-This method returns the transect number for
 * the line.
 *
 * @return <strong>(FXY5036)</strong>-This method returns the transect
 * number for the line. This is the number assigned to a transect repeated
 * in the same calendar year. e.g. The second transect would have a value of
 * 2. A value of -999 is returned when there is no value.
 */
int BinDecoder::getTransectNumber() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::TRANSECT_NUM,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::TRANSECT_NUM,mt)[1];

    return toInteger(start, end);
}
/**
 * <strong>(FXY48302)</strong>-This method returns the CRC value for the bin
 * file.
 *
 * @return <strong>(FXY48302)</strong>-This method returns the CRC value for
 * the bin file. This value is computed by WRITE LOGIC HERE Once computed,
 * the value returned by this method must match. If there is not a match,
 * then the bin file is possibly corrupt. A value of -999 is returned when
 * there is no value.
 */
int BinDecoder::getUniqueTag() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::UNIQUE_TAG,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::UNIQUE_TAG,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY11001)</strong>-This method returns the wind direction in
 * degrees at the time of the measurement.
 *
 * @return <strong>(FXY11001)</strong>-This method returns the wind
 * direction in degrees at the time of the measurement. A value of -999 is
 * returned when there is no value.
 */
double BinDecoder::getWindDirection() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::WIND_DIRECTION,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::WIND_DIRECTION,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY2002)</strong>-This method returns a table value that
 * represents the type of anemometer used.
 *
 * @return <strong>(FXY2002)</strong>-This method returns a table value that
 * represents the type of anemometer used. A value of -999 is returned when
 * there is no value.
 */
int BinDecoder::getWindInstrumentType() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::WIND_INSTRUMENT_TYPE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::WIND_INSTRUMENT_TYPE,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY11002)</strong>-This method returns the wind speed in degrees
 * at the time of the measurement.
 *
 * @return <strong>(FXY11002)</strong>-This method returns the wind speed in
 * m/s at the time of the measurement. A value of -99.9 is returned when
 * there is no value.
 */
double BinDecoder::getWindSpeed() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::WIND_SPEED,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::WIND_SPEED,mt)[1];
    return toInteger(start, end) / 10.0;
}
/**
 * <strong>(FXY22178)</strong>-This method returns a table value that
 * represents the type of launcher used.
 *
 * @return <strong>(FXY22178)</strong>-This method returns a table value
 * that represents the type of launcher used. A value of -999 is returned
 * when there is no value.
 */
int BinDecoder::getXBTLauncherType() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_LAUNCHER_TYPE,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_LAUNCHER_TYPE,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY4003)</strong>-This method returns the day in which the XBT
 * was manufactured.
 *
 * @return <strong>(FXY4003)</strong>-This method returns the day in which
 * the XBT was manufactured. A value of -999 is returned when there is no
 * value.
 */
int BinDecoder::getXBTProbeManufacturedDay() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_PROBE_MANUFACTURE_DAY,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_PROBE_MANUFACTURE_DAY,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY4002)</strong>-This method returns the month in which the XBT
 * was manufactured.
 *
 * @return <strong>(FXY4002)</strong>-This method returns the month in which
 * the XBT was manufactured. A value of -999 is returned when there is no
 * value.
 */
int BinDecoder::getXBTProbeManufacturedMonth() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_PROBE_MANUFACTURE_MONTH,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_PROBE_MANUFACTURE_MONTH,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY4001)</strong>-This method returns the year the XBT was
 * manufactured.
 *
 * @return <strong>(FXY4001)</strong>-This method returns the year the XBT
 * was manufactured. A value of -999 is returned when there is no value.
 */
int BinDecoder::getXBTProbeManufacturedYear() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_PROBE_MANUFACTURE_YEAR,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_PROBE_MANUFACTURE_YEAR,mt)[1];
    return toInteger(start, end);
}

XBTProfile BinDecoder::getXBTProfile() {
	return *xBTProfile;
}
/**
 * <strong>(FXY4003)</strong>-This method returns the day in which the
 * recorder was manufactured.
 *
 * @return <strong>(FXY4003)</strong>-This method returns the day in which
 * the recorder was manufactured. A value of -999 is returned when there is
 * no value.
 */
int BinDecoder::getXBTRecorderManufacturedDay() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_RECORDER_MANUFACTURE_DAY,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_RECORDER_MANUFACTURE_DAY,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY4002)</strong>-This method returns the month in which the
 * recorder was manufactured.
 *
 * @return <strong>(FXY4002)</strong>-This method returns the month in which
 * the recorder was manufactured. A value of -999 is returned when there is
 * no value.
 */
int BinDecoder::getXBTRecorderManufacturedMonth() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_RECORDER_MANUFACTURE_MONTH,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_RECORDER_MANUFACTURE_MONTH,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY4001)</strong>-This method returns the year in which the
 * recorder was manufactured.
 *
 * @return <strong>(FXY4001)</strong>-This method returns the year in which
 * the recorder was manufactured. A value of -999 is returned when there is
 * no value.
 */
int BinDecoder::getXBTRecorderManufacturedYear() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_RECORDER_MANUFACTURE_YEAR,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_RECORDER_MANUFACTURE_YEAR,mt)[1];
    return toInteger(start, end);
}
/**
 * <strong>(FXY2171)</strong>-This method returns the serial number of the
 * recorder used.
 *
 * @return <strong>(FXY2171)</strong>-This method returns the serial number
 * of the recorder used. A value of -999 is returned when there is no value.
 */
std::string BinDecoder::getXBTRecorderSerialNumber() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_RECORDER_SERIAL_NUMBER,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::XBT_RECORDER_SERIAL_NUMBER,mt)[1];
    return toString(start, end);
}
/**
 * <strong>(FXY4001)</strong>-This method returns the year in which the
 * measurement was made.
 *
 * @return <strong>(FXY4001)</strong>-This method returns the year in which
 * the measurement was made. A value of -999 is returned when there is no
 * value.
 */
int BinDecoder::getYear() {
    int mt = getNewMessageType();
    int start = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::YEAR,mt)[0];
    int end = XBTProfileDataRanges::getDataLocation(XBTProfileDataRanges::YEAR,mt)[1];
    return toInteger(start, end);
}

void BinDecoder::decodeXBTProfile(XBTProfile &xBTProfile) {
    int messageType = getNewMessageType();
    //XBTProfile *xBTProfile = new XBTProfile();
    xBTProfile.setAgencyOwner(getAgencyOwner());
    xBTProfile.setDataQuality(getDataQuality());
    xBTProfile.setDay(getDay());
    xBTProfile.setDryBulbTemperature(getDryBulbTemperature());
    xBTProfile.setHour(getHour());
    xBTProfile.setInstrumentType(getInstrumentType());
    xBTProfile.setLatitude(getLatitude());
    xBTProfile.setLaunchHeight(getLaunchHeight());
    xBTProfile.setLloyds(getLloyds());
    xBTProfile.setLongitude(getLongitude());
    xBTProfile.setMinute(getMinute());
    xBTProfile.setMonth(getMonth());
    xBTProfile.setNewMessageType(messageType);
    xBTProfile.setNumberOfRepeatedFields(getNumberOfRepeatedFields());
    xBTProfile.setNumberOfRiderBlocks(getNumberOfRiderBlocks());
    xBTProfile.setNumberOfRiderEmailBlocks(getNumberOfRiderEmailBlocks());
    xBTProfile.setNumberOfRiderInstitutionBlocks(getNumberOfRiderInstitutionBlocks());
    xBTProfile.setNumberOfRiderPhoneBlocks(getNumberOfRiderPhoneBlocks());
    xBTProfile.setOldMessageType(getOldMessageType());
    xBTProfile.setProbeSerialNumber(getProbeSerialNumber());
    xBTProfile.setRecorderType(getRecorderType());
    xBTProfile.setRiderNames(getRiderNames());
    xBTProfile.setRiderEmails(getRiderEmails());
    xBTProfile.setRiderInstitutions(getRiderInstitutions());
    xBTProfile.setRiderPhones(getRiderPhones());
    xBTProfile.setSeaDepth(getSeaDepth());
    xBTProfile.setSeaSurfaceCurrentDirection(getSeaSurfaceCurrentDirection());
    xBTProfile.setSeaSurfaceCurrentMeasurementMethod(getSeaSurfaceCurrentMeasurementMethod());
    xBTProfile.setSeaSurfaceCurrentSpeed(getSeaSurfaceCurrentSpeed());
    if (messageType == MessageType::MESSAGE_TYPE_4) {
        xBTProfile.setSeaSurfaceResistance(getSeaResistance());
    }//end if
    else {
        xBTProfile.setSeaSurfaceTemperature(getSeaTemperature());
    }//end else

    xBTProfile.setSeasVersion(getSeasVersion());
    xBTProfile.setSequenceNum(getSequenceNumber());
    xBTProfile.setShipDirection(getShipDirection());
    xBTProfile.setShipName(getShipName());
    xBTProfile.setShipSpeed(getShipSpeed());
    xBTProfile.setSoopLine(getSoopLine());
    if (messageType == MessageType::MESSAGE_TYPE_4) {
        xBTProfile.setResistancePoints(getResistancePoints());
    }//end if
    else {
        xBTProfile.setTemperaturePoints(getTemperaturePoints());
    }//end else

    xBTProfile.setThisDataIs(getThisDataIs());
    xBTProfile.setTimesReplicated(getTimesReplicated());
    xBTProfile.setTotalWaterDepth(getTotalWaterDepth());
    xBTProfile.setTransectNum(getTransectNumber());
    xBTProfile.setUniqueTag(getUniqueTag());
    xBTProfile.setWindDirection(getWindDirection());
    xBTProfile.setWindInstrumentType(getWindInstrumentType());
    xBTProfile.setWindSpeed(getWindSpeed());
    xBTProfile.setCallsign(getCallsign());
    xBTProfile.setXBTLauncherType(getXBTLauncherType());
    xBTProfile.setXBTProbeManufacturedDay(getXBTProbeManufacturedDay());
    xBTProfile.setXBTProbeManufacturedMonth(getXBTProbeManufacturedMonth());
    xBTProfile.setXBTProbeManufacturedYear(getXBTProbeManufacturedYear());
    xBTProfile.setXBTRecorderManufacturedDay(getXBTRecorderManufacturedDay());
    xBTProfile.setXBTRecorderManufacturedMonth(getXBTRecorderManufacturedMonth());
    xBTProfile.setXBTRecorderManufacturedYear(getXBTRecorderManufacturedYear());
    xBTProfile.setXBTRecorderSerialNumber(getXBTRecorderSerialNumber());
    xBTProfile.setYear(getYear());

    //return *xBTProfile;
}

int BinDecoder::getBitsSize() {
	return bits.size();
}
