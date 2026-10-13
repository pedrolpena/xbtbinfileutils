/*
 * SoundSpeedInSeaWater.h
 *
 *  Created on: Aug 7, 2019
 *      Author: pedro
 */

/*

An example of an Export Data File is shown here:
// This is a MK21 EXPORT DATA FILE  (EDF)
//
Date of Launch:  07/06/2010
Time of Launch:  05:49:51
Sequence #    :  45
Latitude      :  71 50.45264N
Longitude     :  165 10.30078W
Serial #      :  00000000
//
// Here are the contents of the memo fields.
//
HLY1001
//
// Here is some probe information for this drop.
//
Probe Type       :  T-7
Terminal Depth   :  760 m
Depth Equation   :  Standard
Depth Coeff. 1   :  0.0
Depth Coeff. 2   :  6.691
Depth Coeff. 3   :  -0.00225
Depth Coeff. 4   :  0.0
Pressure Pt Correction:  100.0%
//
Raw Data Filename:  C:\Program Files\Sippican\WinMK21\DATA\T7_00045.RDF
//
Display Units    :  Metric
//
// This XBT export file has not been noise reduced or averaged.
//
// Sound velocity derived with assumed salinity: 34.00 ppt
//
Depth (m) - Temperature (<B0>C) - Sound Velocity (m/s)
0.0     1.30    1453.67
0.7     1.22    1453.36
1.3     1.15    1453.05
2.0     1.10    1452.84
2.7     1.09    1452.81
3.3     1.07    1452.72
4.0     0.97    1452.29
4.7     0.89    1451.95
5.4     0.88    1451.87

*/





#ifndef INCLUDE_SOUNDSPEEDINSEAWATER_H_
#define INCLUDE_SOUNDSPEEDINSEAWATER_H_
#include <cmath>

class SoundSpeedInSeaWater {
public:
	// from
	//http://resource.npl.co.uk/acoustics/techguides/soundseawater/content.html
	static constexpr double A00 = 1.389;
	static constexpr double A01 = -1.262e-2;
	static constexpr double A02 = 7.166e-5;
	static constexpr double A03 = 2.008e-6;
	static constexpr double A04 = -3.21e-8;
	static constexpr double A10 = 9.4742e-5;
	static constexpr double A11 = -1.2583e-5;
	static constexpr double A12 = -6.4928e-8;
	static constexpr double A13 = 1.0515e-8;
	static constexpr double A14 = -2.0142e-10;
	static constexpr double A20 = -3.9064e-7;
	static constexpr double A21 = 9.1061e-9;
	static constexpr double A22 = -1.6009e-10;
	static constexpr double A23 = 7.994e-12;
	static constexpr double A30 = 1.100e-10;
	static constexpr double A31 = 6.651e-12;
	static constexpr double A32 = -3.391e-13;
	static constexpr double B00 = -1.922e-2;
	static constexpr double B01 = -4.42e-5;
	static constexpr double B10 = 7.3637e-5;
	static constexpr double B11 = 1.7950e-7;
	static constexpr double C00 = 1402.388;
	static constexpr double C01 = 5.03830;
	static constexpr double C02 = -5.81090e-2;
	static constexpr double C03 = 3.3432e-4;
	static constexpr double C04 = -1.47797e-6;
	static constexpr double C05 = 3.1419e-9;
	static constexpr double C10 = 0.153563;
	static constexpr double C11 = 6.8999e-4;
	static constexpr double C12 = -8.1829e-6;
	static constexpr double C13 = 1.3632e-7;
	static constexpr double C14 = -6.1260e-10;
	static constexpr double C20 = 3.1260e-5;
	static constexpr double C21 = -1.7111e-6;
	static constexpr double C22 = 2.5986e-8;
	static constexpr double C23 = -2.5353e-10;
	static constexpr double C24 = 1.0415e-12;
	static constexpr double C30 = -9.7729e-9;
	static constexpr double C31 = 3.8513e-10;
	static constexpr double C32 = -2.3654e-12;
	static constexpr double D00 = 1.727e-3;
	static constexpr double D10 = -7.9836e-6;

    /**
     * This method returns the sound speed at a point using the UNESCO form of the
     * Chen and Millero equation.
     *
     * @param pressure The pressure in KPa (kilo pascal).
     * @param salinity The salinity in parts per thousand.
     * @param temp The temperature in degrees celcius.
     * @return The sound speed in meters per second.
     */
	static double getSoundSpeedChenMillero(double pressure, double salinity, double temp);
	static double getSoundSpeedChenMilleroUNESCO1983(double pressure, double salinity, double temp);


	SoundSpeedInSeaWater();
	virtual ~SoundSpeedInSeaWater();
};

#endif /* INCLUDE_SOUNDSPEEDINSEAWATER_H_ */
