/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/Calculator.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
************************************************************************/

#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "maitreya.h"

class DataSet;

/*************************************************//**
*
* \brief data for a single eclipse
*
******************************************************/
class EclipseData
{
public:
	EclipseData();

	double jd, sunpos, moonpos;
	int planet, type;
	wxString description;
};

/*************************************************//**
*
* \brief abstract class encapsulates all planetary calculations
*
******************************************************/
class Calculator
{
public:
	virtual ~Calculator() {}
	virtual void init() = 0;
	virtual wxString getVersion() = 0;

	virtual double calcAyanamsa( const double &jd, const AYANAMSA = AY_NONE ) = 0;
	virtual double calcSiderealTime( const double &jd , const double &longitude ) = 0;
	virtual void calcEps( double &eps, const double &jd ) = 0;
	virtual double calcNakshatraPortion( DataSet *d, const double &mlen, const bool usepada = false ) = 0;
	virtual double calcAscendantPortion( DataSet *d, const double &alen ) = 0;

	virtual bool calcSunRiseSunSet( DataSet *d, double *sunrise = 0, double *sunset = 0, double *noon = 0, double *midnight = 0) = 0;

	virtual double calcNextSolarEvent( const int &type, const double &jd, const double &lat, const double &lon ) = 0;

	virtual double calc_jd( const int &year, const int &month, const int &day, const double &time, const bool gregflag = true ) = 0;
	virtual bool calc_and_check_jd( const int &year, const int &month, const int &day, const double &time, double &jd ) = 0;
	virtual bool isGregorianCalendarDate( const int &year, const int &month, const int &day ) = 0;
	virtual bool isGregorianCalendarDate( const double &jd ) = 0;

	virtual bool getDateIntsFromJD( const double &jd, int &year, int &month, int &day, double &ut ) = 0;
	virtual double calcTajakaJd( const DataSet*, const int &year, const bool &vedic ) = 0;

	virtual void calcPosition( DataSet *d, const int &planet, double &slen, double &slat,
	                           const bool aya = false, const bool indian = true ) = 0;
	virtual void calcPositionSpeed( DataSet *d, const int &planet, double &slen, double &slat, double &sspeed,
	                                const bool aya = false, const bool indian = true ) = 0;

	virtual double calcMC( const double &jd, const double &lat, const double &lon ) = 0;
	virtual double calcMCAya( const double &jd, const double &lat, const double &lon, bool indian ) = 0;
	virtual double calcAscendant( const double &jd, const double &lat, const double &lon ) = 0;
	virtual double calcAscendantAya( const double &jd, const double &lat, const double &lon, bool indian ) = 0;
	virtual double calcAscendantByArmc( const double &armc, const double &eps, const double &lat, double *r = 0,
	                                    const int = HS_PLACIDUS ) = 0;
	virtual void calcHouses( const double &jd, const double &lon, const double &lat, const int &hsys, double *r ) = 0;
	virtual EclipseData calcNextEclipse( const double &jd, const int &planet, const int &type, const bool &vedic ) = 0;
	virtual double calcMcFromAsc( const double &ascpos, const double &jd, const double &lon, const double &lat ) = 0;
	virtual double calcPlanetaryEvent( const DataSet*, const double &targetlen, const int &planet, const bool &vedic ) = 0;
	virtual double calcSunMoonEvent( DataSet*, const double &targetdiff, double &slen, double &mlen ) = 0;
};

/*************************************************//**
*
* \brief creation of Calculator
*
******************************************************/
class CalculatorFactory
{
public:
	Calculator *getCalculator();

private:
	static 	Calculator *calculator;
};


#endif

