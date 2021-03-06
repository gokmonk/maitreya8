/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/VedicChartConfig.cpp
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

#include "VedicChartConfig.h"

#include "ColorConfig.h"
#include "FileConfig.h"
#include "GraphConfig.h"
#include "JSonTool.h"
#include "maitreya.h"
#include "SheetConfig.h"

#include <wx/filename.h>
#include <wx/jsonwriter.h>
#include <wx/log.h>
#include <wx/wfstream.h>

/*****************************************************
**
**   VedicChartBehaviorConfig   ---   Constructor
**
******************************************************/
VedicChartBehaviorConfig::VedicChartBehaviorConfig()
{
	highlightOnMouseOver = false;
	zoomOnMouseOver = true;
	zoomFactor = 102;

	aspectedObjectsMode = 2;
	aspectingObjectsMode = 3;
	allAspectsMode = 4;

	// Graha Drishti
	aspectType = 0;
	grahaDrishtiMinimumStrength = VA_FULL;
	beneficMaleficDisplay = 0;
	includeAsMc = false;
};

/*****************************************************
**
**   VedicChartConfig   ---   Constructor
**
******************************************************/
VedicChartConfig::VedicChartConfig( wxString name )
		: ChartConfig()
{
	ChartConfig::setDefaultConfig();
	setDefaultConfig();
	this->name = name;
}

/*****************************************************
**
**   VedicChartConfig   ---   setDefaultConfig
**
******************************************************/
void VedicChartConfig::setDefaultConfig()
{
	ChartConfig::setDefaultConfig();

	useSymbols = true;
}

IMPLEMENT_SINGLETON( VedicChartConfigLoader )

/*****************************************************
**
**   VedicChartConfigLoader   ---   Constructor
**
******************************************************/
VedicChartConfigLoader::VedicChartConfigLoader()
: ConfigListLoader<VedicChartConfig>( CFG_VEDICCHART, CrtGlobalFile )	
{
}

/*****************************************************
**
**   VedicChartConfigLoader   ---   loadSingleConfig
**
******************************************************/
void VedicChartConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	JSonTool tool( globalResourcename );
	VedicChartConfig *cfg = (VedicChartConfig*)NULL;

	if ( ! v[ CFG_EXTENDS ].IsNull())
	{
		wxString baseconfig = v[ CFG_EXTENDS ].AsString();
		VedicChartConfig *basecfg = getByName( baseconfig );
		if ( basecfg ) cfg = new VedicChartConfig( *basecfg );
		else wxLogError(
			wxString::Format( wxT( "could not find base config for extension %s - using default configuration for that entry" ),
			baseconfig.c_str()));
		printf( "Found Extend relation: %s pointer is %ld\n", str2char( baseconfig ), (long)basecfg);
	}
	if ( ! cfg ) cfg = new VedicChartConfig;

	cfg->name = v[ CFG_NAME ].AsString();

	if ( ! v[ CFG_PEN ].IsNull()) tool.readPen( v[CFG_PEN ], cfg->pen );
	if ( ! v[ CFG_BRUSH ].IsNull()) tool.readBrush( v[ CFG_BRUSH ], cfg->brush );
	if ( ! v[ CFG_TEXTCOLOR ].IsNull()) cfg->textColor = wxColour( v[ CFG_TEXTCOLOR ].AsString());
	if ( ! v[ CFG_OUTER_RECTANGLE ].IsNull()) tool.readRectangle( v[ CFG_OUTER_RECTANGLE ], cfg->outerRectangle );
	if ( ! v[ CFG_FRAME_TYPE ].IsNull()) cfg->frameType = tool.readFrameType( v[ CFG_FRAME_TYPE ].AsString());
	if ( ! v[ CFG_ZODIACALSIGNS ].IsNull()) tool.readZodiacalSigns( v[ CFG_ZODIACALSIGNS ], cfg->signs );
	if ( ! v[ CFG_MARKED_FIELDS ].IsNull()) tool.readRegion( v[ CFG_MARKED_FIELDS ], cfg->markedFields );

	if ( ! v[ CFG_PALETTE ].IsNull()) cfg->palette = v[ CFG_PALETTE ].AsString();

	l.push_back( cfg );
}


