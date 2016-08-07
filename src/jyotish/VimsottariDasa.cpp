/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VimsottariDasa.cpp
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

#include "VimsottariDasa.h"

#include "Calculator.h"
#include "Horoscope.h"

/*****************************************************
**
**   LagnaVimsottariDasaExpert   ---   getPortion
**
******************************************************/
double LagnaVimsottariDasaExpert::getPortion( Nakshatra &nak )
{
	Calculator *calculator = CalculatorFactory().getCalculator();
	double mlen = horoscope->getVedicLongitude( OASCENDANT );
	nak = (Nakshatra)( mlen / NAKSHATRA_LEN );
	return calculator->calcAscendantPortion( horoscope->getDataSet(), mlen );
}

/*****************************************************
**
**   VimsottariBaseDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId VimsottariBaseDasaExpert::getDasaLordByIndex( const int &i )
{
	static const ObjectId dasa_lord[9] = { OSUN, OMOON, OMARS, OMEANNODE, OJUPITER, OSATURN, OMERCURY, OMEANDESCNODE, OVENUS };
	assert( i >= 0 && i < 9 );
	return dasa_lord[i];
}

/*****************************************************
**
**   VimsottariBaseDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int VimsottariBaseDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	const Nakshatra pindex = Nakshatra(( (int)nak + 7 ) % 9 );
	return pindex;
}

/*****************************************************
**
**   VimsottariBaseDasaExpert   ---   getDasaDuration
**
******************************************************/
int VimsottariBaseDasaExpert::getDasaDuration( const int &i ) const
{
	static const int dasa_dur[9] = { 6, 10, 7, 18, 16, 19, 17, 7, 20  };
	printf( "VimsottariBaseDasaExpert::getDasaDuration %d\n", i );
	assert( i >= 0 && i < 9 );
	return dasa_dur[i];
}

/*****************************************************
**
**   DasaExpertFactory   ---   create
**
******************************************************/
DasaExpert *DasaExpertFactory::getVimsottariDasaExpert( Horoscope *h )
{
	return new VimsottariDasaExpert( h );
}

/*****************************************************
**
**   DasaExpertFactory   ---   create
**
******************************************************/
DasaExpert *DasaExpertFactory::getLagnaVimsottariDasaExpert( Horoscope *h )
{
	return new LagnaVimsottariDasaExpert( h );
}

/*****************************************************
**
**   VimsottariBaseDasaExpert   ---   getTara
**
******************************************************/
int VimsottariBaseDasaExpert::getTara( const int &planet, const int &nak )
{
	const bool upwards = true;
	int tara = 0;
	int count = 0;
	int lordindex = ( nak + 7 ) % 9;

	//assert(( planet >= ISUN ) && ( planet <= IKETU ));
	assert(( nak >= 0 ) && ( nak < 27 ));

	while (( getDasaLordByIndex( lordindex ) != planet ) && ( count++ < 20 ))
	{
		if ( upwards ) {
			if (++lordindex > 8 ) lordindex = 0;
		}
		else {
			if (--lordindex < 0 ) lordindex = 8;
		}
		tara++;
	}

	assert( count < 20 );
	assert( tara < 9 );
	return tara;
}

/*****************************************************
**
**   VimsottariBaseDasaExpert   ---   getMahaDasaLength
**
******************************************************/
int VimsottariBaseDasaExpert::getMahaDasaLength( const int &dasa )
{
	//assert( dasa >= 0 && dasa < 9 );
	for ( int i = 0; i < 9; i++ )
	{
		if (getDasaLordByIndex( i ) == dasa )
		{
			return getDasaDuration( i );
		}
	}
	assert( 0 );
	return -1;
}

/*****************************************************
**
**   VimsottariBaseDasaExpert   ---   getDasaLengthInts
**
******************************************************/
void VimsottariBaseDasaExpert::getDasaLengthInts( int &year, int &month, int &day, const int &mahadasa, const int &antardasa )
{
	int l1 = getMahaDasaLength( mahadasa );
	int l2 = getMahaDasaLength( antardasa );
	double len = l1 * l2;
	len /= 120;
	len += .0000000001;
	year = (int)len;
	double dmonth = len - (double)year;
	dmonth *= 12;
	month = (int)dmonth;
	day = (int)(( dmonth - (double)month ) * 30 );
}

