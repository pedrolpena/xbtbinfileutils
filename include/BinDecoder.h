/*
 * BinDecoder.h
 *
 *  Created on: Jul 21, 2019
 *      Author: pedro
 */

#ifndef INCLUDE_BINDECODER_H_
#define INCLUDE_BINDECODER_H_
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

#include "bitvector.h"
#include "XBTProfile.h"


class BinDecoder {
public:
	stdex::bitvector bits; //holds bits from file
	std::ifstream inputFile;
	XBTProfile *xBTProfile;
	BinDecoder(std::string);
	virtual ~BinDecoder();
	stdex::bitvector changeEndian(stdex::bitvector b);
	std::string getBinarySequence(int,int);
	int toInteger(int , int);
	int getAvailablePoints(int, int, int);
	std::string toString(int,int);
	int getOldMessageType();
	int getNewMessageType();
	int getAgencyOwner();
	std::string getCallsign();
	int getDataQuality();
	int getDay();
	double getDryBulbTemperature();
	int getHour();
	int getInstrumentType();
	double getLatitude();
	double getLaunchHeight();
	int getLloyds();
	double getLongitude();
	int getMinute();
	int getMonth();
	int getNumberOfRepeatedFields();
	int getNumberOfRiderBlocks();
	int getNumberOfRiderEmailBlocks();
	int getNumberOfRiderInstitutionBlocks();
	int getNumberOfRiderPhoneBlocks();
	int getRecorderType();
	std::vector<double> getResistance();
	std::vector<double> getResistancePoints();
	std::string getRiderEmails();
	std::string getRiderInstitutions();
	std::string getRiderNames();
	std::string getRiderPhones();
	double getSeaDepth();
	double getSeaResistance();
	int getSeaSurfaceCurrentDirection();
	int getSeaSurfaceCurrentMeasurementMethod();
	double getSeaSurfaceCurrentSpeed();
	double getSeaTemperature();
	int getSeasVersion();
	int getProbeSerialNumber();
	int getSequenceNumber();
	double getShipDirection();
	std::string getShipName();
	double getShipSpeed();
	std::string getSoopLine();
	std::vector<double >getTemperaturePoints();
	int getThisDataIs();
	int getTimesReplicated();
	int getTotalWaterDepth();
	int getTransectNumber();
	int getUniqueTag();
	std::string getWMOID();
	double getWindDirection();
	int getWindInstrumentType();
	double getWindSpeed();
	int getXBTLauncherType();
	int getXBTProbeManufacturedDay();
	int getXBTProbeManufacturedMonth();
	int getXBTProbeManufacturedYear();
	XBTProfile getXBTProfile();
	int getXBTRecorderManufacturedMonth();
	int getXBTRecorderManufacturedDay();
	int getXBTRecorderManufacturedYear();
	std::string getXBTRecorderSerialNumber();
	int getYear();
	void decodeXBTProfile(XBTProfile &xBTProfile);
	int getBitsSize();













};

#endif /* INCLUDE_BINDECODER_H_ */
