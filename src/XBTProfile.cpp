/*
 * XBTProfile.cpp
 *
 *  Created on: Jul 16, 2019
 *      Author: pedro
 */

#include "XBTProfile.h"

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include "MessageType.h"
#include "XBTResistanceToTemperatureConverter.h"

XBTProfile::XBTProfile() {


}

void XBTProfile::setWMOID(std::string wmoId) {
	this->wmoId = wmoId;
}

std::string XBTProfile::getWMOId() {
	return wmoId;
}

XBTProfile::~XBTProfile() {
	// TODO Auto-generated destructor stub
}

int XBTProfile::getOldMessageType() {
	return oldMessageType;
}

void XBTProfile::setOldMessageType( int oldMessageType) {
	this->oldMessageType = oldMessageType;
}

int XBTProfile::getNewMessageType() {
	return newMessageType;
}

void XBTProfile::setNewMessageType( int newMessageType) {
	this->newMessageType = newMessageType;
}

double XBTProfile::getLatitude()  {
	return latitude;
}

void XBTProfile::setLatitude( double latitude) {
	this->latitude = latitude;
}

double XBTProfile::getLongitude(){
	return longitude;
}

void XBTProfile::setLongitude( double longitude ) {
	this->longitude = longitude;
}

 std::string XBTProfile::getSoopLine() {
	return soopLine;
}

void XBTProfile::setSoopLine( std::string soopLine ) {
	this->soopLine = soopLine;
}

int XBTProfile::getTransectNumber() {
	return transectNum;
}

void XBTProfile::setTransectNum( int transectNum ) {
	this->transectNum = transectNum;
}

int XBTProfile::getAgencyOwner()  {
	return agencyOwner;
}

void XBTProfile::setAgencyOwner(int agencyOwner ) {
	this->agencyOwner = agencyOwner;
}

int XBTProfile::getDataQuality()  {
	return dataQuality;
}

void XBTProfile::setDataQuality(int dataQuality ) {
	this->dataQuality = dataQuality;
}

int XBTProfile::getDay()  {
	return day;
}

void XBTProfile::setDay(int day ) {
	this->day = day;
}

double XBTProfile::getDryBulbTemperature()  {
	return dryBulbTemperature;
}

void XBTProfile::setDryBulbTemperature(double dryBulbTemperature ) {
	this->dryBulbTemperature = dryBulbTemperature;
}

int XBTProfile::getHour()  {
	return hour;
}

void XBTProfile::setHour(int hour ) {
	this->hour = hour;
}

int XBTProfile::getInstrumentType()  {
	return instrumentType;
}

void XBTProfile::setInstrumentType(int instrumentType ) {
	this->instrumentType = instrumentType;
}

double XBTProfile::getLaunchHeight()  {
	return launchHeight;
}

void XBTProfile::setLaunchHeight(double launchHeight ) {
	this->launchHeight = launchHeight;
}

int XBTProfile::getLloyds()  {
	return lloyds;
}

void XBTProfile::setLloyds(int lloyds ) {
	this->lloyds = lloyds;
}

 std::vector<double> XBTProfile::getMeasurementPoints()  {
	return measurementPoints;
}
 /**
  * <strong>(FXY22042S)</strong>-This method sets an array containing the
  * measurements made.
  *
  * @param measurementPoints An array containing the
  * measurements made.
  */
void XBTProfile::setMeasurementPoints(std::vector<double> measurementPoints) {
    this->measurementPoints = measurementPoints;
            if(!measurementPoints.empty()){
   this->seaSurfaceMeasurementPoint = measurementPoints[0];
    }//end if
}

int XBTProfile::getMinute()  {
	return minute;
}

void XBTProfile::setMinute(int minute ) {
	this->minute = minute;
}

int XBTProfile::getMonth()  {
	return month;
}

void XBTProfile::setMonth(int month ) {
	this->month = month;
}

int XBTProfile::getNumberOfRepeatedFields()  {
	return numberOfRepeatedFields;
}

void XBTProfile::setNumberOfRepeatedFields(int numberOfRepeatedFields ) {
	this->numberOfRepeatedFields = numberOfRepeatedFields;
}

int XBTProfile::getNumberOfRiderBlocks()  {
	return numberOfRiderBlocks;
}

void XBTProfile::setNumberOfRiderBlocks(int numberOfRiderBlocks ) {
	this->numberOfRiderBlocks = numberOfRiderBlocks;
}

int XBTProfile::getNumberOfRiderEmailBlocks()  {
	return numberOfRiderEmailBlocks;
}

void XBTProfile::setNumberOfRiderEmailBlocks( int numberOfRiderEmailBlocks ) {
	this->numberOfRiderEmailBlocks = numberOfRiderEmailBlocks;
}

int XBTProfile::getNumberOfRiderInstitutionBlocks()  {
	return numberOfRiderInstitutionBlocks;
}

void XBTProfile::setNumberOfRiderInstitutionBlocks( int numberOfRiderInstitutionBlocks ) {
	this->numberOfRiderInstitutionBlocks = numberOfRiderInstitutionBlocks;
}

int XBTProfile::getNumberOfRiderPhoneBlocks()  {
	return numberOfRiderPhoneBlocks;
}

void XBTProfile::setNumberOfRiderPhoneBlocks( int numberOfRiderPhoneBlocks ) {
	this->numberOfRiderPhoneBlocks = numberOfRiderPhoneBlocks;
}

int XBTProfile::getProbeSerialNumber()  {
	return probeSerialNumber;
}

void XBTProfile::setProbeSerialNumber(int probeSerialNumber ) {
	this->probeSerialNumber = probeSerialNumber;
}

int XBTProfile::getRecorderType()  {
	return recorderType;
}

void XBTProfile::setRecorderType(int recorderType ) {
	this->recorderType = recorderType;
}

 std::vector<double> XBTProfile::getResistancePoints()  {
	return resistancePoints;
}
 /**
  * <strong>(FXY22042S)</strong>-This method sets an array containing the
  * resistance measurements made.
  *
  * @param resistancePoints An array containing the temperature
  * measurements
  * made.
  */
void XBTProfile::setResistancePoints( std::vector<double> resistancePoints ) {

    this->resistancePoints = resistancePoints;
    if (!resistancePoints.empty()) {
        this->seaSurfaceResistance = resistancePoints[0];
    }//end if

    this->measurementPoints = resistancePoints;
    if (!measurementPoints.empty()) {
        this->seaSurfaceMeasurementPoint = measurementPoints[0];
    }//end if
}

 std::string XBTProfile::getRiderEmails()  {
	return riderEmail;
}

void XBTProfile::setRiderEmails( std::string riderEmail ) {
	this->riderEmail = riderEmail;
}

 std::string XBTProfile::getRiderInstitutions()  {
	return riderInstitution;
}

void XBTProfile::setRiderInstitutions( std::string riderInstitution ) {
	this->riderInstitution = riderInstitution;
}

 std::string XBTProfile::getRiderNames()  {
	return riderName;
}

void XBTProfile::setRiderNames( std::string riderName ) {
	this->riderName = riderName;
}

 std::string XBTProfile::getRiderPhones()  {
	return riderPhone;
}

void XBTProfile::setRiderPhones( std::string riderPhone ) {
	this->riderPhone = riderPhone;
}

double XBTProfile::getSeaDepth()  {
	return seaDepth;
}

void XBTProfile::setSeaDepth(double seaDepth ) {
	this->seaDepth = seaDepth;
}

int XBTProfile::getSeaSurfaceCurrentDirection()  {
	return seaSurfaceCurrentDirection;
}

void XBTProfile::setSeaSurfaceCurrentDirection(int seaSurfaceCurrentDirection ) {
	this->seaSurfaceCurrentDirection = seaSurfaceCurrentDirection;
}

int XBTProfile::getSeaSurfaceCurrentMeasurementMethod()  {
	return seaSurfaceCurrentMeasuringMethod;
}

void XBTProfile::setSeaSurfaceCurrentMeasurementMethod( int seaSurfaceCurrentMeasuringMethod ) {
	this->seaSurfaceCurrentMeasuringMethod = seaSurfaceCurrentMeasuringMethod;
}

double XBTProfile::getSeaSurfaceCurrentSpeed()  {
	return seaSurfaceCurrentSpeed;
}

void XBTProfile::setSeaSurfaceCurrentSpeed( double seaSurfaceCurrentSpeed ) {
	this->seaSurfaceCurrentSpeed = seaSurfaceCurrentSpeed;
}
/**
* <strong>(FXY22042S)</strong>-This method returns the SSM or the first
* measurement made.
*
* @return <strong>(FXY22042S)</strong>-This method returns the SSM or the
* first measurement made.
*/
double XBTProfile::getSeaSurfaceMeasurement()  {

    double ssm = this->seaSurfaceMeasurementPoint;
    if (this->newMessageType == MessageType::MESSAGE_TYPE_4) {
        XBTResistanceToTemperatureConverter *XBTR2T = new XBTResistanceToTemperatureConverter();
        ssm = XBTR2T->convertResistaceToTemperature(seaSurfaceMeasurementPoint);

    }//end if
    return ssm;
}

void XBTProfile::setSeaSurfaceMeasurementPoint( double seaSurfaceMeasurementPoint ) {
	this->seaSurfaceMeasurementPoint = seaSurfaceMeasurementPoint;
}

double XBTProfile::getSeaSurfaceResistance()  {
	return seaSurfaceResistance;
}

void XBTProfile::setSeaSurfaceResistance(double seaSurfaceResistance ) {
	this->seaSurfaceResistance = seaSurfaceResistance;
}
/**
 * <strong>(FXY22042S)</strong>-This method returns the SST or the first
 * temperature measurement made.
 *
 * @return <strong>(FXY22042S)</strong>-This method returns the SST or the
 * first temperature measurement made.
 */
double XBTProfile::getSeaSurfaceTemperature()  {

    double sst = seaSurfaceTemperature;
    if (this->newMessageType == MessageType::MESSAGE_TYPE_4) {
        XBTResistanceToTemperatureConverter *XBTR2T = new XBTResistanceToTemperatureConverter();
        sst = XBTR2T->convertResistaceToTemperature(seaSurfaceResistance);

    }//end if
    return sst;
}

void XBTProfile::setSeaSurfaceTemperature(double seaSurfaceTemperature ) {
	this->seaSurfaceTemperature = seaSurfaceTemperature;
}

int XBTProfile::getSeasVersion()  {
	return seasVersion;
}

void XBTProfile::setSeasVersion(int seasVersion ) {
	this->seasVersion = seasVersion;
}

int XBTProfile::getSequenceNumber()  {
	return sequenceNum;
}

void XBTProfile::setSequenceNum(int sequenceNum ) {
	this->sequenceNum = sequenceNum;
}

double XBTProfile::getShipDirection()  {
	return shipDirection;
}

void XBTProfile::setShipDirection(double shipDirection ) {
	this->shipDirection = shipDirection;
}

 std::string XBTProfile::getShipName()  {
	return shipName;
}

void XBTProfile::setShipName( std::string shipName ) {
	this->shipName = shipName;
}

double XBTProfile::getShipSpeed()  {
	return shipSpeed;
}

void XBTProfile::setShipSpeed(double shipSpeed ) {
	this->shipSpeed = shipSpeed;
}

/**
 * <strong>(FXY22042S)</strong>-This method returns an array containing the
 * temperature measurements made.
 *
 * @return <strong>(FXY22042S)</strong>-This method returns an array
 * containing the temperature measurements made.
 */
 std::vector<double> XBTProfile::getTemperaturePoints()  {
	 std::vector<double> tp = temperaturePoints;
     if (this->newMessageType == MessageType::MESSAGE_TYPE_4) {
         XBTResistanceToTemperatureConverter *XBTR2T = new XBTResistanceToTemperatureConverter();
         tp = XBTR2T->convertResistaceToTemperature(resistancePoints);

     }//end if
     return tp;
}

void XBTProfile::setTemperaturePoints( std::vector<double> temperaturePoints) {
    this->temperaturePoints = temperaturePoints;
    if(!temperaturePoints.empty()){
   this->seaSurfaceTemperature = temperaturePoints[0];
    }//end if

    this->measurementPoints = temperaturePoints;
    if (!measurementPoints.empty()) {
        this->seaSurfaceMeasurementPoint = measurementPoints[0];
    }//end if

}

int XBTProfile::getThisDataIs()  {
	return thisDataIs;
}

void XBTProfile::setThisDataIs(int thisDataIs ) {
	this->thisDataIs = thisDataIs;
}

int XBTProfile::getTimesReplicated()  {
	return timesReplicated;
}

void XBTProfile::setTimesReplicated(int timesReplicated ) {
	this->timesReplicated = timesReplicated;
}

int XBTProfile::getTotalWaterDepth()  {
	return totalWaterDepth;
}

void XBTProfile::setTotalWaterDepth(int totalWaterDepth ) {
	this->totalWaterDepth = totalWaterDepth;
}

int XBTProfile::getUniqueTag()  {
	return uniqueTag;
}

void XBTProfile::setUniqueTag(int uniqueTag ) {
	this->uniqueTag = uniqueTag;
}

double XBTProfile::getWindDirection()  {
	return windDirection;
}

void XBTProfile::setWindDirection(double windDirection ) {
	this->windDirection = windDirection;
}

int XBTProfile::getWindInstrumentType()  {
	return windInstrumentType;
}

void XBTProfile::setWindInstrumentType(int windInstrumentType ) {
	this->windInstrumentType = windInstrumentType;
}

double XBTProfile::getWindSpeed()  {
	return windSpeed;
}

void XBTProfile::setWindSpeed(double windSpeed ) {
	this->windSpeed = windSpeed;
}

int XBTProfile::getXBTLauncherType()  {
	return xbtLauncherType;
}

void XBTProfile::setXBTLauncherType(int xbtLauncherType ) {
	this->xbtLauncherType = xbtLauncherType;
}

int XBTProfile::getXBTProbeManufacturedDay()  {
	return xbtProbeManufactureDay;
}

void XBTProfile::setXBTProbeManufacturedDay( int xbtProbeManufactureDay ) {
	this->xbtProbeManufactureDay = xbtProbeManufactureDay;
}

int XBTProfile::getXBTProbeManufacturedMonth()  {
	return xbtProbeManufactureMonth;
}

void XBTProfile::setXBTProbeManufacturedMonth( int xbtProbeManufactureMonth ) {
	this->xbtProbeManufactureMonth = xbtProbeManufactureMonth;
}

int XBTProfile::getXBTProbeManufacturedYear()  {
	return xbtProbeManufactureYear;
}

void XBTProfile::setXBTProbeManufacturedYear( int xbtProbeManufactureYear ) {
	this->xbtProbeManufactureYear = xbtProbeManufactureYear;
}

int XBTProfile::getXBTRecorderManufacturedDay()  {
	return xbtRecorderManufactureDay;
}

void XBTProfile::setXBTRecorderManufacturedDay( int xbtRecorderManufactureDay ) {
	this->xbtRecorderManufactureDay = xbtRecorderManufactureDay;
}

int XBTProfile::getXBTRecorderManufacturedMonth()  {
	return xbtRecorderManufactureMonth;
}

void XBTProfile::setXBTRecorderManufacturedMonth( int xbtRecorderManufactureMonth ) {
	this->xbtRecorderManufactureMonth = xbtRecorderManufactureMonth;
}

int XBTProfile::getXBTRecorderManufacturedYear()  {
	return xbtRecorderManufactureYear;
}

void XBTProfile::setXBTRecorderManufacturedYear( int xbtRecorderManufactureYear ) {
	this->xbtRecorderManufactureYear = xbtRecorderManufactureYear;
}

 std::string XBTProfile::getXBTRecorderSerialNumber()  {
	return xbtRecorderSerialNumber;
}

void XBTProfile::setXBTRecorderSerialNumber( std::string xbtRecorderSerialNumber ) {
	this->xbtRecorderSerialNumber = xbtRecorderSerialNumber;
}

int XBTProfile::getYear()  {
	return year;
}

void XBTProfile::setYear(int year ) {
	this->year = year;
}

std::string XBTProfile::toString() {

    std::vector<double> tempPoints = getTemperaturePoints();
    std::string str =  "Callsign=" + getWMOId() + "\n"
            + "Old message type=" + std::to_string(getOldMessageType()) + "\n"
            + "New message type=" + std::to_string(getNewMessageType()) + "\n"
            + "Lat=" + std::to_string(getLatitude()) + "\n"
            + "Lon=" + std::to_string(getLongitude()) + "\n"
            + "Soopline=" + getSoopLine() + "\n"
            + "Transect#=" + std::to_string(getTransectNumber()) + "\n"
            + "Sequence#=" + std::to_string(getSequenceNumber()) + "\n"
            + "date=" + std::to_string(getMonth()) + "/" + std::to_string(getDay()) + "/" + std::to_string(getYear()) + "\n"
            + "time=" + std::to_string(getHour()) + ":" + std::to_string(getMinute()) + "\n"
            + "Second=" + std::to_string(getSecond()) + "\n"
            + "ShipName=" + getShipName() + "\n"
            + "Lloyds=" + std::to_string(getLloyds()) + "\n"
            + "Unique tag=" + intToHexString((getUniqueTag())) + "\n"
            + "SeasVersion=" + std::to_string(getSeasVersion()) + "\n"
            + "XBT Serial#=" + std::to_string(getProbeSerialNumber()) + "\n"
            + "ThisDatais=" + std::to_string(getThisDataIs()) + "\n"
            + "DataQuality=" + std::to_string(getDataQuality()) + "\n"
            + "LauncherHeight=" + std::to_string(getLaunchHeight()) + "\n"
            + "Ship direction=" + std::to_string(getShipDirection()) + "\n"
            + "Ship Speed=" + std::to_string(getShipSpeed()) + "\n"
            + "Instrument type=" + std::to_string(getInstrumentType()) + "\n"
            + "Recorder type=" + std::to_string(getRecorderType()) + "\n"
            + "Wind Instrument type=" + std::to_string(getWindInstrumentType()) + "\n"
            + "Wind Direction=" + std::to_string(getWindDirection()) + "\n"
            + "Wind Speed=" + std::to_string(getWindSpeed()) + "\n"
            + "Dry bulb temp=" + std::to_string(getDryBulbTemperature()) + "\n"
            + "Measure method=" + std::to_string(getSeaSurfaceCurrentMeasurementMethod()) + "\n"
            + "Current Dir=" + std::to_string(getSeaSurfaceCurrentDirection()) + "\n"
            + "Current Speed=" + std::to_string(getSeaSurfaceCurrentSpeed()) + "\n"
            + "Total water depth=" + std::to_string(getTotalWaterDepth()) + "\n"
            + "Agency Owner=" + std::to_string(getAgencyOwner()) + "\n"
            + "XBT launcher type=" + std::to_string(getXBTLauncherType()) + "\n"
            + "XBT Recorder Serial#=" + getXBTRecorderSerialNumber() + "\n"
            + "XBT Recorder Manufacture date=" + std::to_string(getXBTRecorderManufacturedMonth()) + "/" + std::to_string(getXBTRecorderManufacturedDay()) + "/" + std::to_string(this->getXBTRecorderManufacturedYear()) + "\n"
            + "XBT Probe Manufacture date=" + std::to_string(getXBTProbeManufacturedMonth()) + "/" + std::to_string(getXBTProbeManufacturedDay()) + "/" + std::to_string(getXBTProbeManufacturedYear()) + "\n"
            + "# of rider blocks=" + std::to_string(getNumberOfRiderBlocks()) + "\n"
            + "# of rider email blocks=" + std::to_string(getNumberOfRiderEmailBlocks()) + "\n"
            + "# of rider phone blocks=" + std::to_string(getNumberOfRiderPhoneBlocks()) + "\n"
            + "# of rider inst blocks=" + std::to_string(getNumberOfRiderInstitutionBlocks()) + "\n"
            + "# of rep fields=" + std::to_string(getNumberOfRepeatedFields()) + "\n"
            + "Times replicated=" + std::to_string(getTimesReplicated()) + "\n"
            + "Sea Temperature=" + std::to_string(getSeaSurfaceTemperature()) + "\n"
            + "SeaDepth=" + std::to_string(getSeaDepth()) + "\n"
            + "Rider=" + getRiderNames() + "\n"
            + "Email=" + getRiderEmails() + "\n"
            + "Phone=" + getRiderPhones() + "\n"
            + "Institution=" + getRiderInstitutions() + "\n"
            + "Salinity=" + std::to_string(getSalinity()) + "\n";

    bool isType4 = getNewMessageType() == MessageType::MESSAGE_TYPE_4;
    if (isType4)
        str += "Sea Resistance=" + std::to_string(getSeaSurfaceResistance()) + "\n";

    // one line per EDF header line, as the text can hold several
    std::istringstream edfLines(getEDFText());
    std::string edfLine;
    while (std::getline(edfLines, edfLine))
        str += "EDF text=" + edfLine + "\n";

    if (isType4) {
        std::vector<double> resPoints = getResistancePoints();
        str += "index,temperature,resistance\n";
        for (unsigned int i = 0; i < tempPoints.size() && i < resPoints.size(); i++) {
            str += std::to_string(i) + "," + std::to_string(tempPoints[i]) + "," + std::to_string(resPoints[i]) + "\n";
        }//end for
    } else {
        str += "index,temperature\n";
        for (unsigned int i = 0; i < tempPoints.size(); i++) {
            str += std::to_string(i) + "," + std::to_string(tempPoints[i]) + "\n";
        }//end for
    }//end if

    return str;

}

std::string XBTProfile::intToHexString(int int1) {

	char hex_string[20];
	std::string data;
	sprintf(hex_string, "%X", int1);
	data = std::string(hex_string);
	std::for_each(data.begin(), data.end(), [](char & c){
		c = ::toupper(c);
	});
	return data;
}

void XBTProfile::setCallsign(std::string wmoId) {
	setWMOID(wmoId);
}

int XBTProfile::getSecond() {
	return second;
}

void XBTProfile::setSecond(int second) {
	this->second = second;
}

double XBTProfile::getSalinity() {
	return salinity;
}

void XBTProfile::setSalinity(double salinity) {
	this->salinity = salinity;
}

/**
 * This method returns the lines of the EDF header that can not be made
 * from the other values of the profile, e.g. the raw data filename, the
 * units and the memo. Every line is stored as the section name, a tab and
 * the line as it was in the EDF file.
 *
 * @return the lines separated by line feeds
 */
std::string XBTProfile::getEDFText() {
	return edfText;
}

void XBTProfile::setEDFText(std::string edfText) {
	this->edfText = edfText;
}
