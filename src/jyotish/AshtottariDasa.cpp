/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/AshtottariDasa.cpp
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

#include "AshtottariDasa.h"

#include<wx/log.h>

#include "astrobase.h"
#include "Calculator.h"
#include "Dasa.h"
#include "Horoscope.h"
#include "Lang.h"

const int AshtottariDasaExpert::dasa_lord[8] = { OSUN, OMOON, OMARS, OMERCURY, OSATURN, OJUPITER, OMEANNODE, OVENUS };
const int AshtottariDasaExpert::dasa_dur[8] = { 6, 15, 8, 17, 10, 19, 12, 21  };

/*****************************************************
**
**   AshtottariDasaExpert   ---   getNextLevel
**
******************************************************/
vector<Dasa*> AshtottariDasaExpert::getNextLevel( Dasa *dasa )
{
	vector<Dasa*> ret;
	int i;

	double start_jd = dasa->getStartJD();
	double end_jd = dasa->getEndJD();
	double dasa_len = end_jd - start_jd;
	double antar_len = 0;
	int dlord = dasa->getDasaLord();
	int lordindex = -1;
	for ( i = 0; i < 8; i++ )
	{
		if ( dlord == dasa_lord[i] ) lordindex = i;
	}
	assert( lordindex >= 0 );

	// Guru Venus is left handed
	lordindex -= 1;
	if ( lordindex < 0 ) lordindex = 7;

	for ( i = 0; i < 8; i++ )
	{
		antar_len = ( dasa_len * dasa_dur[lordindex] ) / 108;
		end_jd = start_jd + antar_len;
		ret.push_back( new Dasa( D_ASHTOTTARI, dlord, start_jd, end_jd, dasa ));

		lordindex -= 1;
		if ( lordindex < 0 ) lordindex = 7;
		dlord = dasa_lord[lordindex];
		start_jd = end_jd;
	}

	return ret;
}

/*****************************************************
**
**   AshtottariDasaExpert   ---   getFirstLevel
**
******************************************************/
vector<Dasa*> AshtottariDasaExpert::getFirstLevel()
{
	vector<Dasa*> ret;

	int mnak = getNakshatra28( horoscope->getVedicLongitude( OMOON ));
	int startn = mnak - 5;
	if ( startn < 0 ) startn += 28;

	double nportion, portion;
	nportion = ::getNakshatraLongitude28( horoscope->getVedicLongitude( OMOON )) / 13.33333333;

	int pindex;

	// Ardra, Punarvasu, Pusyami, Ashlesha
	if ( startn < 4 )
	{
		// Sun dasa
		pindex = 0;
		portion = ( startn + nportion ) / 4;
	}
	// Makha, P.Phalguni, U.Phalguni
	else if ( startn < 7 )
	{
		// Moon dasa
		pindex = 1;
		portion = ( startn - 4 + nportion ) / 4;
	}
	// Hasta, Chitta, Swati, Vishakha
	else if ( startn < 11 )
	{
		// Mars dasa
		pindex = 2;
		portion = ( startn - 7 + nportion ) / 4;
	}
	// Anuradha, Jyeshta, Moola
	else if ( startn < 14 )
	{
		// Mercury dasa
		pindex = 3;
		portion = ( startn - 11 + nportion ) / 4;
	}
	// P.Shadha, U.Shadha, Abhijit, Sravana
	else if ( startn < 18 )
	{
		// Saturn dasa
		pindex = 4;
		portion = ( startn - 14 + nportion ) / 4;
	}
	// Dhanista, Satabhish, P.Bhadra
	else if ( startn < 21 )
	{
		// Jupiter dasa
		pindex = 5;
		portion = ( startn - 18 + nportion ) / 4;
	}
	// U.Bhadra, Revati, Aswini, Bharani
	else if ( startn < 25 )
	{
		// Rahu dasa
		pindex = 6;
		portion = ( startn - 21 + nportion ) / 4;
	}
	// Krittika, Rohini, Mrigasira
	else if ( startn < 28 )
	{
		// Venus dasa
		pindex = 7;
		portion = ( startn - 25 + nportion ) / 4;
	}
	else
	{
		wxLogError( wxT( "Error Nakshatra index in Ashtottari is %d, exiting." ), startn );
		exit(1);
	}

	int dlord = dasa_lord[pindex];
	double start_jd = horoscope->getJD() - portion * dasa_dur[pindex] * getYearLength( true );
	double end_jd;

	for ( int i = 0; i < 8; i++ )
	{
		end_jd = start_jd + dasa_dur[pindex] * getYearLength( true );
		ret.push_back( new Dasa( D_ASHTOTTARI, dlord, start_jd, end_jd, (Dasa*)NULL ));

		pindex = ( pindex + 1 ) % 8;
		dlord = dasa_lord[pindex];
		start_jd = end_jd;
	}

	return ret;
}

/*****************************************************
**
**   AshtottariDasaExpert   ---   getKPEventList
**
******************************************************/
vector<KPEvent> AshtottariDasaExpert::getKPEventList( const double&, const double&, const double& )
{
	// never used
	assert( false );
	vector<KPEvent> v;
	return v;
}

/*****************************************************
**
**   DasaExpertFactory   ---   create
**
******************************************************/
DasaExpert *DasaExpertFactory::getAshtottariDasaExpert( Horoscope *h ) { return new AshtottariDasaExpert( h ); }

