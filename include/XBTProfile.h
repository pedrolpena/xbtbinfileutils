/*
 * XBTProfile.h
 *
 *  Created on: Jul 16, 2019
 *      Author: pedro
 */
#include <string>
#include <vector>
#include<sstream>

#ifndef XBTPROFILE_H_
#define XBTPROFILE_H_

class XBTProfile {
public:
	XBTProfile();
	virtual ~XBTProfile();
	std::string getWMOId();
	void setWMOID(std::string);
	int getOldMessageType();
	void setOldMessageType(int);

	int getNewMessageType();
	void setNewMessageType(int);
	double getLatitude() ;
	void setLatitude(double);
	double getLongitude();
	void setLongitude(double);
	std::string getSoopLine();
	void setSoopLine(std::string);
	int getTransectNumber();
	void setTransectNum(int);
	int getAgencyOwner() ;
	void setAgencyOwner(int);
	int getDataQuality();
	void setDataQuality( int);
	int getDay();
	void setDay(int);
	double getDryBulbTemperature();
	void setDryBulbTemperature(double);
	int getHour();
	void setHour(int);
	int getInstrumentType();
	void setInstrumentType(int);
	double getLaunchHeight();
	void setLaunchHeight(double);
	int getLloyds();
	void setLloyds(int);
	std::vector<double> getMeasurementPoints();
	void setMeasurementPoints(std::vector<double>);
	int getMinute();
	void setMinute(int);
	int getMonth();
	void setMonth(int);
	int getNumberOfRepeatedFields();
	void setNumberOfRepeatedFields(int);
	int getNumberOfRiderBlocks();
	void setNumberOfRiderBlocks(int);
	int getNumberOfRiderEmailBlocks();
	void setNumberOfRiderEmailBlocks(int);
	int getNumberOfRiderInstitutionBlocks();
	void setNumberOfRiderInstitutionBlocks(int);
	int getNumberOfRiderPhoneBlocks();
	void setNumberOfRiderPhoneBlocks(int);
	int getProbeSerialNumber();
	void setProbeSerialNumber(int);
	int getRecorderType();
	void setRecorderType(int);
	std::vector<double> getResistancePoints();
	void setResistancePoints(std::vector<double>);
	bool hasResistancePoints();
	std::string getRiderEmails();
	void setRiderEmails(std::string);
	std::string getRiderInstitutions();
	void setRiderInstitutions(std::string);
	std::string getRiderNames();
	void setRiderNames(std::string);
	std::string getRiderPhones();
	void setRiderPhones(std::string);
	double getSeaDepth();
	void setSeaDepth(double);
	int getSeaSurfaceCurrentDirection();
	void setSeaSurfaceCurrentDirection(int);
	int getSeaSurfaceCurrentMeasurementMethod();
	void setSeaSurfaceCurrentMeasurementMethod(int);
	double getSeaSurfaceCurrentSpeed() ;
	void setSeaSurfaceCurrentSpeed(double);
	double getSeaSurfaceMeasurement();
	void setSeaSurfaceMeasurementPoint(double);
	double getSeaSurfaceResistance();
	void setSeaSurfaceResistance(double);
	double getSeaSurfaceTemperature() ;
	void setSeaSurfaceTemperature(double);
	int getSeasVersion();
	void setSeasVersion(int);
	int getSequenceNumber();
	void setSequenceNum(int);
	double getShipDirection();
	void setShipDirection(double);
	std::string getShipName();
	void setShipName(std::string);
	double getShipSpeed();
	void setShipSpeed(double);
	std::vector<double> getTemperaturePoints();
	void setTemperaturePoints(std::vector<double>);
	int getThisDataIs();
	void setThisDataIs(int);
	int getTimesReplicated();
	void setTimesReplicated(int);
	int getTotalWaterDepth();
	void setTotalWaterDepth(int);
	int getUniqueTag();
	void setUniqueTag(int);
	double getWindDirection();
	void setWindDirection(double);
	int getWindInstrumentType();
	void setWindInstrumentType(int);
	double getWindSpeed();
	void setWindSpeed(double);
	int getXBTLauncherType();
	void setXBTLauncherType(int);
	int getXBTProbeManufacturedDay();
	void setXBTProbeManufacturedDay(int);
	int getXBTProbeManufacturedMonth() ;
	void setXBTProbeManufacturedMonth(int);
	int getXBTProbeManufacturedYear();
	void setXBTProbeManufacturedYear(int);
	int getXBTRecorderManufacturedDay();
	void setXBTRecorderManufacturedDay(int);
	int getXBTRecorderManufacturedMonth();
	void setXBTRecorderManufacturedMonth(int);
	int getXBTRecorderManufacturedYear();
	void setXBTRecorderManufacturedYear(int);
	std::string getXBTRecorderSerialNumber();
	void setXBTRecorderSerialNumber(std::string);
	int getYear();
	void setYear(int);
	void setCallsign(std::string);
	std::string toString();

	// Type 4 only, values from Sippican MK21 export data files (EDF)
	int getSecond();
	void setSecond(int);
	double getSalinity();
	void setSalinity(double);
	std::string getEDFText();
	void setEDFText(std::string);

private:
	std::string wmoId = "";
	int oldMessageType = -999;
	int newMessageType = -999;
	double latitude = -999;
	double longitude = -999;
	std::string soopLine = "";
	int transectNum = -999;
	int sequenceNum = -999;
	int year = -999;
	int month = -999;
	int day = -999;
	int hour = -999;
	int minute = -999;
	std::string shipName = "";
	int lloyds = -999;
	int uniqueTag = -999;
	int seasVersion = -999;
	int probeSerialNumber = -999;
	int thisDataIs = -999;
	int dataQuality = -999;
	double launchHeight = -999;
	double shipDirection = -999;
	double shipSpeed = -999;
	int instrumentType = -999;
	int recorderType = -999;
	int windInstrumentType = -999;
	double windDirection = -999;
	double windSpeed = -999;
	double dryBulbTemperature = -999;
	int seaSurfaceCurrentMeasuringMethod = -999;
	int seaSurfaceCurrentDirection = -999;
	double seaSurfaceCurrentSpeed = -999;
	int totalWaterDepth = -999;
	int agencyOwner = -999;
	int xbtLauncherType = -999;
	std::string xbtRecorderSerialNumber = "";
	int xbtRecorderManufactureYear = -999;
	int xbtRecorderManufactureMonth = -999;
	int xbtRecorderManufactureDay = -999;
	int xbtProbeManufactureYear = -999;
	int xbtProbeManufactureMonth = -999;
	int xbtProbeManufactureDay = -999;
	int numberOfRiderBlocks = -999;
	int numberOfRiderInstitutionBlocks = -999;
	int numberOfRiderEmailBlocks = -999;
	int numberOfRiderPhoneBlocks = -999;
	int numberOfRepeatedFields = -999;
	int timesReplicated = -999;
	double seaSurfaceTemperature = -999;
	double seaSurfaceResistance = -999;
	double seaSurfaceMeasurementPoint=-999;
	double seaDepth = -999;
	std::vector <double> temperaturePoints;
	std::vector <double> resistancePoints;
	std::vector <double> measurementPoints;
	std::string riderName = "";
	std::string riderEmail = "";
	std::string riderInstitution = "";
	std::string riderPhone = "";
	int second = -999;
	double salinity = -999;
	std::string edfText = "";
	std::string intToHexString(int);
};

#endif /* XBTPROFILE_H_ */
