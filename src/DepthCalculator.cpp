/*
 * DepthCalculator.cpp
 *
 *  Created on: Aug 7, 2019
 *      Author: pedro
 */

#include "DepthCalculator.h"

DepthCalculator::DepthCalculator(XBTProfile &xBTProfile) {

    this->xBTProfile = xBTProfile;
    numberOfMeasurements = xBTProfile.getTemperaturePoints().size();
    setRecorderFrequency(xBTProfile.getRecorderType());
    setProbeCoefficients(xBTProfile.getInstrumentType());

}

DepthCalculator::~DepthCalculator() {
	// TODO Auto-generated destructor stub
}

/**
 * This method sets the coefficients used by the XBT fall rate equation to
 * determine the measurement depths.
 *
 * @return None
 */
void DepthCalculator::setProbeCoefficients(int probeType) {
    A = XBTProbe::getCoefficientA(probeType);
    B = XBTProbe::getCoefficientB(probeType);
}

/**
 * This method sets the recorders measurement frequency in Hertz.
 *
 * @return None
 */
void DepthCalculator::setRecorderFrequency(int recorderType) {

	sampleFrequency = XBTRecorder::getRecorderFrequency(recorderType);

}


/**
 * This method returns a double that is the depth where the measurement was
 * made.
 *
 * @param sequenceNumber A number from 0 to n that is the position of the
 * measurement in the list of points.
 * @return This method returns a double that is the depth where the
 * measurement was made.
 */
double DepthCalculator::getMeasurementDepth(int sequenceNumber) {

    double depth;
    double time;

    time = ((double) sequenceNumber + 1) / sampleFrequency;
    depth = (A * time) + (.001 * B * time * time);

    return depth;

}


/**
 * This method returns an array of doubles containing the depths where each
 * measurement was made.
 *
 * @return returns an array of doubles containing the depths where each
 * measurement was made.
 */
std::vector<double> DepthCalculator::getMeasurementDepths() {
	std::vector<double> depths(numberOfMeasurements);
    double time;
    for (int i = 0; i < numberOfMeasurements; i++) {
        time = ((double) i + 1) / sampleFrequency;
        depths[i] = (A * time) + (.001 * B * time * time);

    }//end for
    return depths;

}


/**
 * This method returns a two dimensional array of doubles containing the
 * depths ad temperatures as measured by the recorder.
 *
 * @return returns a two dimensional array of doubles containing the depths
 * ad temperatures as measured by the recorder.
 */
std::vector<std::vector<double> > DepthCalculator::getDepthsAndTemperaturePoints() {

    double time;
    std::vector<std::vector<double> > depthsAndTemps(numberOfMeasurements,std::vector<double>(2));
    std::vector<double> temps = xBTProfile.getTemperaturePoints();

    for (int i = 0; i < numberOfMeasurements; i++) {
        time = ((double) i + 1) / sampleFrequency;
        depthsAndTemps[i][0] = (A * time) + (.001 * B * time * time);
        depthsAndTemps[i][1] = temps[i];
    }//end for

    return depthsAndTemps;

}

DepthCalculator::DepthCalculator(XBTProfile &xBTProfile, int recorderType, int probeType) {

    this->xBTProfile = xBTProfile;
    numberOfMeasurements = xBTProfile.getTemperaturePoints().size();
    setRecorderFrequency(recorderType);
    setProbeCoefficients(probeType);

}
/**
 * This method returns a two dimensional array of doubles containing the
 * depths ad temperatures with a resolution of two meters. A linear
 * interpolation is performed to get the depths at 1 meter increments.
 *
 * @return returns a two dimensional array of doubles containing the depths
 * ad temperatures with a resolution of two meters. A linear interpolation
 * is performed to get the depths at 1 meter increments.
 */
std::vector<std::vector<double> > DepthCalculator::getDepthsAndTemperaturePointsOneMeterResolution() {
	std::vector<std::vector<double> > depthsAndTemps = getDepthsAndTemperaturePoints();
	double inputTemps[numberOfMeasurements];
	double inputDepths[numberOfMeasurements];
	int finalDepth = (int) depthsAndTemps[numberOfMeasurements - 1][0];
	std::vector<std::vector<double> > depthsAndTempsOneMeterResolution(
			finalDepth, std::vector<double>(2));

	// copy depths and temps to arrays needed for the interpolator
	for (int i = 0; i < numberOfMeasurements; i++) {

		inputDepths[i] = depthsAndTemps[i][0];
		inputTemps[i] = depthsAndTemps[i][1];
	} //end for

	double oneMeterDepths[finalDepth];

	//generate array of depths used by the interpolator to generate temps
	for (int i = 0; i < finalDepth; i++) { // place depths
		oneMeterDepths[i] = (double) (i + 1);

	} //end for

	// holds the result
	double oneMeterTemps[finalDepth];

	// this one element array holds the number of points
	int inputSize[] = { numberOfMeasurements };

	//interpoalte
	mlinterp::interp(

	  inputSize,   finalDepth,
	 inputTemps,   oneMeterTemps,
	inputDepths,   oneMeterDepths);

	//copy results to a 2 dimensional vector;
	for (int i = 0; i < finalDepth; i++) {

		depthsAndTempsOneMeterResolution[i][1] = oneMeterTemps[i];
		depthsAndTempsOneMeterResolution[i][0] = oneMeterDepths[i];
	}

	return depthsAndTempsOneMeterResolution;
}


/**
 * This method returns a two dimensional array of doubles containing the
 * depths ad temperatures with a resolution of two meters. A linear
 * interpolation is performed to get the depths at 2 meter increments.
 *
 * @return returns a two dimensional array of doubles containing the depths
 * ad temperatures with a resolution of two meters. A linear interpolation
 * is performed to get the depths at 2 meter increments.
 */
std::vector<std::vector<double> > DepthCalculator::getDepthsAndTemperaturePointsTwoMeterResolution() {
	std::vector<std::vector<double> > depthsAndTemps =
			getDepthsAndTemperaturePointsOneMeterResolution();

	int finalDepth = 0;
	if (!depthsAndTemps.empty()) {
		finalDepth = (int) depthsAndTemps[depthsAndTemps.size() - 1][0];
	}

	if (finalDepth % 2 != 0) {
		finalDepth--;
	}

	std::vector<std::vector<double> > depthsAndTempsTwoMeterResolution(
			finalDepth / 2, std::vector<double>(2));

	for (int i = 0; i < finalDepth / 2; i++) {

		depthsAndTempsTwoMeterResolution[i][1] = depthsAndTemps[(2 * i) + 1][1];
		depthsAndTempsTwoMeterResolution[i][0] = depthsAndTemps[(2 * i) + 1][0];
	}

	return depthsAndTempsTwoMeterResolution;

}


/**
 * This method returns a two dimensional array of doubles containing the
 * depths and temperatures for the inflection points in the profile.
 * The algorithm used in this method was adapted from Paul Chinn's code
 * in JJVV_1.CPP.
 *
 * @return returns a two dimensional array of doubles containing the depths
 * and temperatures for the inflection points in the profile.
 */
std::vector<std::vector<double>> DepthCalculator::getDepthsAndTemperaturePointsInflectionPoints() {

	return getInflectionPoints(getDepthsAndTemperaturePoints());

	/*//pauls code
	std::vector<std::vector<double>> Inflections;
	std::vector<double> *smoothedPoints = new std::vector<double>(0);

	int lastDepth =
			(int) getDepthsAndTemperaturePoints()[getDepthsAndTemperaturePoints().size()
					- 1][0];

	if (lastDepth < 5) {
		return Inflections;
	}

	ApplyMedianFilter(smoothedPoints);

	int td = ComputeTailDepth();

	float q = (float) 0.15;

	std::vector<double> IPdata;

	while (q < 2.0) //910
	{
		bool CompNewLimits = false;
		Inflections.clear();

		double t1 = GetSmoothTempAtDepth(2, smoothedPoints);
		double d1 = 2.0;

		IPdata.clear();
		IPdata.push_back(d1);
		IPdata.push_back(t1);
		Inflections.push_back(IPdata);

		float t2 = GetSmoothTempAtDepth(3, smoothedPoints);
		float d2 = 3.0;

		float deltat = t2 - t1;
		float deltad = d2 - d1;
		float lsr = (deltat + q) / deltad;  // right limiting slope
		float lsl = (deltat - q) / deltad;	// Left limiting slope

		int i;
		for (i = 4; i < td; i++) //930
				{

			t2 = GetSmoothTempAtDepth(i, smoothedPoints);

			d2 = i;
			if (d1 > td) {
				break;
			}
			float Ndeltat = t2 - t1;
			float Ndeltad = d2 - d1;
			if (CompNewLimits) {
				lsr = (Ndeltat + q) / Ndeltad;
				lsl = (Ndeltat - q) / Ndeltad;
				CompNewLimits = false;
			}

			float Tslope = Ndeltat / Ndeltad;
			if (Tslope > lsr) {

				t1 = t2;
				d1 = d2;

				IPdata.clear();
				IPdata.push_back(d1);
				IPdata.push_back(t1);
				Inflections.push_back(IPdata);

				CompNewLimits = true;
				continue;
			}

			if (Tslope < lsl) {

				t1 = t2;
				d1 = d2;

				IPdata.clear();
				IPdata.push_back(d2);
				IPdata.push_back(t2);
				Inflections.push_back(IPdata);
				CompNewLimits = true;
				continue;
			}
			Tslope = (Ndeltat + q) / Ndeltad;
			if (Tslope < lsr) {
				lsr = Tslope;
			}

			Tslope = (Ndeltat - q) / Ndeltad;
			if (Tslope > lsl) {
				lsl = Tslope;
			}

		}

		if (Inflections.size() < MAXINFPTS) {

			IPdata.clear();
			IPdata.push_back(td);
			IPdata.push_back(GetSmoothTempAtDepth(td, smoothedPoints));
			Inflections.push_back(IPdata);

			for (unsigned int i = 0; i < Inflections.size() - 1; i++) {

				std::vector<double> first, next;

				first = Inflections.at(i);
				next = Inflections.at(i + 1);

				if (first[0] == next[0]) {
					Inflections.erase(Inflections.begin() + (i + 1));
				}

			}
			int k = Inflections.size();
			for (i = 0; i < k - 2; i++) {
				std::vector<double> first, secnd, last;

				first = Inflections.at(i);
				while (1) {
					if ((i + 2) <= (k - 1)) {

						secnd = Inflections.at(i + 1);

						last = Inflections.at(i + 2);
					} else {
						break;
					}

					int T1, T2, T3;
					T1 = (int) ((first[1] + .05) * 10.0);
					T2 = (int) ((secnd[1] + .05) * 10.0);
					T3 = (int) ((last[1] + .05) * 10.0);

					if ((T1 == T2) && (T2 == T3)) {

						Inflections.erase(Inflections.begin() + (i + 1));
						k = Inflections.size();
					} else {
						break;
					}
				}

				if ((i + 2) <= (k - 1)) {
					continue;
				} else {
					break;
				}

			}

			return Inflections;
		} else {

			q = q * 1.10;
		}

	} //end while

	return Inflections;
	*/
}


void DepthCalculator::ApplyMedianFilter(std::vector <double> *s) {
	int Nopts = 0;
	std::vector<double> Pts;
	std::vector<double>* smoothedPoints = s;
	smoothedPoints->clear();

	std::vector<double> temps(getDepthsAndTemperaturePoints().size());
	for (unsigned int i = 0; i < getDepthsAndTemperaturePoints().size() - 1; i++) {
		temps[i] = this->getDepthsAndTemperaturePoints()[i][1];
	}

	int i;
	for (i = 1; i < (int) temps.size() - 1; i++) {
		int PtsSize = temps.size() - 1;
		Pts.clear();
		if ((i - 5 >= 0) && (i + 5 <= PtsSize)) {
			Nopts = 5;
		} else if ((i - 4 >= 0) && (i + 4 <= PtsSize)) {
			Nopts = 4;
		} else if ((i - 3 >= 0) && (i + 3 <= PtsSize)) {
			Nopts = 3;
		} else if ((i - 2 >= 0) && (i + 2 <= PtsSize)) {
			Nopts = 2;
		} else {
			Nopts = 1;
		}

		if (i > 1561) {

		}
		for (int k = i - Nopts; k < i + (Nopts + 1); k++) {
			Pts.push_back(temps[k]);

		}


		smoothedPoints->push_back(GetMedian(&Pts));
	}

	for (i = (int) smoothedPoints->size() - 1; i > 1; i--)   // Paul's Error
			{
		double temp = smoothedPoints->at(i);
		if ((temp >= 34.5)) { // changed from 35.5

			smoothedPoints->erase(smoothedPoints->begin() + i);
			continue;
		}
		break;
	}
	//Smoothed = true;
}

double DepthCalculator::GetMedian(std::vector<double> *pts) {

	std::vector<double> mytemps;

	for (unsigned int i = 0; i < pts->size(); i++) {
		double x = pts->at(i);
		mytemps.push_back(x);
	}
	std::sort(mytemps.begin(), mytemps.end());
	return mytemps.at(mytemps.size() / 2);
}

int DepthCalculator::ComputeTailDepth() {

	int TailDepth = -1;

	double DeltaT, t1, t2;

	std::vector<std::vector<double> > depthsAndTemps;

	depthsAndTemps = getDepthsAndTemperaturePointsOneMeterResolution();

	TailDepth = depthsAndTemps.size();
	int k = TailDepth;

	for (int i = k-1; i >= 0; i--) {

		t1 = depthsAndTemps[i][1];
		t2 = depthsAndTemps[i - 1][1];
		if (t2 >= 34.5) // changed from 35.5
			continue;

		DeltaT = t1 - t2;
		if((DeltaT <=-0.2)|| (DeltaT >=0.1))
		{
			continue;
		}

		TailDepth = i;

		//TailsRem = true;
		break;
	}

	return TailDepth;

}

double DepthCalculator::GetSmoothTempAtDepth(int Depth,std::vector <double> *s) {
	double RetTemp = (double) -9999;
	std::vector <double>* smoothedPoints = s;
	if (Depth >= 1) {

		double a = .001 * B;
		double b = A;
		double c = -Depth;
		double x1 = std::sqrt(b * b - 4.0 * a * c);
		double tim1 = (-b + x1) / (2.0 * a) * 10.0;
		unsigned int pos;
		double pos1;
		double remain = std::modf(tim1, &pos1);
		pos = (int) pos1;
		if (pos <= smoothedPoints->size() - 1)

		{
			float tb = smoothedPoints->at(pos - 1);
			float tt = smoothedPoints->at(pos);
			float dif = tt - tb;
			RetTemp = (tb + dif * remain);

		}

	}
	return (float) RetTemp;
}

namespace {

// Temperatures at or above this value are treated as bad data at the bottom
// of the profile. Amverseas uses the same limit.
constexpr double MAX_VALID_TEMPERATURE = 34.5;
// Temperatures below this value are treated as bad data at the bottom of the
// profile.
constexpr double MIN_VALID_TEMPERATURE = -2.5;
// Largest temperature step allowed between the last two points of the
// profile. Larger steps are removed from the bottom of the profile, they
// are usually caused by the wire breaking or the probe hitting the bottom.
constexpr double TAIL_MAX_STEP = 0.2;
// Number of points on each side of a point used by the median filter.
constexpr int MEDIAN_HALF_WINDOW = 5;
// Largest temperature difference allowed between the profile and the line
// drawn through the inflection points. Amverseas starts with the same value.
constexpr double INFLECTION_TOLERANCE = 0.15;

/**
 * Returns the median of the points from start to end inclusive. The number
 * of points is always odd.
 */
double median(const std::vector<double> &values, int start, int end) {
	std::vector<double> window(values.begin() + start, values.begin() + end + 1);
	std::sort(window.begin(), window.end());
	return window[window.size() / 2];
}

/**
 * Returns the temperature difference between a point and the line drawn
 * between two other points.
 */
double deviation(const std::vector<double> &depths, const std::vector<double> &temps, int first, int last, int point) {
	double fraction = (depths[point] - depths[first]) / (depths[last] - depths[first]);
	double lineTemp = temps[first] + (temps[last] - temps[first]) * fraction;
	return std::fabs(temps[point] - lineTemp);
}

} // namespace

/**
 * This method selects the inflection points of a profile. Linear
 * interpolation between the inflection points reproduces the profile within
 * INFLECTION_TOLERANCE degrees, using at most MAXINFPTS points.
 *
 * Bad data at the bottom of the profile is removed first and the profile is
 * smoothed with a median filter. The first and last points are always
 * selected, then the point farthest from the line through the selected
 * points is added until every point is within the tolerance or MAXINFPTS
 * points have been selected. When two points are equally far the shallower
 * one is selected.
 *
 * The Java version of this library implements the same algorithm and must
 * return the same points.
 *
 * @param depthsAndTemps the depths and temperatures of the profile
 * @return the depths and smoothed temperatures of the inflection points
 */
std::vector<std::vector<double> > DepthCalculator::getInflectionPoints( std::vector<std::vector<double> > depthsAndTemps) {

	std::vector<std::vector<double> > infPoints; // holds computed inflection points

	// remove bad data from the bottom of the profile
	int size = (int) depthsAndTemps.size();
	while (size > 0 && (depthsAndTemps[size - 1][1] >= MAX_VALID_TEMPERATURE
			|| depthsAndTemps[size - 1][1] < MIN_VALID_TEMPERATURE)) {
		size--;
	}

	std::vector<double> depths(size);
	std::vector<double> rawTemps(size);
	for (int i = 0; i < size; i++) {
		depths[i] = depthsAndTemps[i][0];
		rawTemps[i] = depthsAndTemps[i][1];
	}

	// median filter, the window gets smaller near the ends of the profile
	std::vector<double> temps(size);
	for (int i = 0; i < size; i++) {
		int halfWindow = std::min(MEDIAN_HALF_WINDOW, std::min(i, size - 1 - i));
		temps[i] = median(rawTemps, i - halfWindow, i + halfWindow);
	}

	// remove large steps from the bottom of the profile
	while (size > 1 && std::fabs(temps[size - 1] - temps[size - 2]) > TAIL_MAX_STEP) {
		size--;
	}

	if (size < 2) {
		return infPoints;
	}

	// selected[i] is true when point i is an inflection point
	std::vector<bool> selected(size, false);
	selected[0] = true;
	selected[size - 1] = true;
	int selectedCount = 2;

	while (selectedCount < MAXINFPTS) {

		double maxDeviation = 0;
		int maxPoint = -1;
		int first = 0;

		// check the points between each pair of neighboring selected points
		while (first < size - 1) {
			int last = first + 1;
			while (!selected[last]) {
				last++;
			}
			for (int i = first + 1; i < last; i++) {
				double d = deviation(depths, temps, first, last, i);
				if (d > maxDeviation) {
					maxDeviation = d;
					maxPoint = i;
				}
			}
			first = last;
		}

		if (maxPoint == -1 || maxDeviation <= INFLECTION_TOLERANCE) {
			break;
		}

		selected[maxPoint] = true;
		selectedCount++;
	}

	for (int i = 0; i < size; i++) {
		if (selected[i]) {
			infPoints.push_back({depths[i], temps[i]});
		}
	}

	return infPoints;
}
