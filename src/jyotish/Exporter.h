/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Exporter.h
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

#ifndef _EXPORTER_H_
#define _EXPORTER_H_

#include "maitreya.h"

class Sheet;

/*************************************************//**
*
* 
*
******************************************************/
class Exporter
{
public:

	virtual ~Exporter() {}
	virtual wxString exportSheet( Sheet* ) = 0;

protected:
	wxString s;

};


/*************************************************//**
*
* 
*
******************************************************/
class ExporterFactory
{
public:
	Exporter *getExporter( const WidgetExportType& );

protected:
	Exporter *getHtmlExporter();

};

#endif


