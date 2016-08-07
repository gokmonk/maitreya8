/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ImageProvider.cpp
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

#include "ImageProvider.h"

#include "Conf.h"
#include "FileConfig.h"
#include "maitreya.h"

#include <wx/file.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/log.h>
#include <map>
#include <utility>

#include "chart_images.h"
#include "xpm_images.h"

#define DEFAULT_BITMAP_SIZE 32
#define SMALL_IMAGE_SIZE 16

extern Config *config;

IMPLEMENT_SINGLETON( ImageProvider )

/*************************************************//**
*
* 
*
******************************************************/
class BitmapCache
{
	DECLARE_SINGLETON( BitmapCache )

public:

	wxBitmap *findBitmapRef( const wxString &s, const int rotateHue = 0 )
	{
		//printf( "BitmapCache::GET 1 size %d name %s rh %d\n", (int)mm.size(), str2char( s ), rotateHue );
		const pair<wxString, int > k( s, rotateHue );

		/*
		int cc = 0;
		for( map<pair<wxString, int>, wxBitmap*>::iterator iter = mm.begin(); iter != mm.end(); iter++ )
		{
			pair<wxString, int> p = iter->first;
			printf( "Cache item %d name %s rh %d\n", cc, str2char( p.first ), p.second );
			cc++;
		}
		*/

		map<pair<wxString, int>, wxBitmap*>::iterator iter = mm.find( k );
		if ( iter != mm.end() )
		{
			//printf( "found\n" );
			return mm[k];
		}
		else
		{
			//printf( "not found\n" );
			return (wxBitmap*)NULL;
		}
	}

	void putBitmapRef( const wxString &s, const int rotateHue, wxBitmap *b )
	{
		//printf( "BitmapCache::PUT 1 size %d name %s rh %d\n", (int)mm.size(), str2char( s ), rotateHue );
		//const BmpCacheKey k( s, rotateHue );
		const pair<wxString, int > k( s, rotateHue );
		mm[k] = b;
		//printf( "BitmapCache::PUT 2 size %d\n", (int)mm.size() );
	}

private:
	
	map< pair<wxString, int>, wxBitmap*> mm;
};

IMPLEMENT_SINGLETON( BitmapCache )

/*****************************************************
**
**   ImageProvider   ---   Constructor
**
******************************************************/
ImageProvider::ImageProvider()
{
	errorBmp = getBitmap( BM_ERROR );
	dasaImageList = (wxImageList*)NULL;
}

/*****************************************************
**
**   ImageProvider   ---   Destructor
**
******************************************************/
ImageProvider::~ImageProvider()
{
	if ( dasaImageList ) delete dasaImageList;
}

/*****************************************************
**
**   ImageProvider   ---   getDefaultBitmapSize
**
******************************************************/
int ImageProvider::getDefaultBitmapSize() const
{
	return DEFAULT_BITMAP_SIZE;
}

/*****************************************************
**
**   ImageProvider   ---   getToolBitmapSize
**
******************************************************/
int ImageProvider::getToolBitmapSize() const
{
	return config->toolbar->toolbarIconSize ? 32 : 24;
}

/*****************************************************
**
**   ImageProvider   ---   getFileBasedBitmap
**
******************************************************/
wxBitmap ImageProvider::getFileBasedBitmap( const wxString &s, const int rotateHue, const bool storeInCache )
{
	wxBitmap *b = getFileBasedBitmapRef( s, rotateHue, storeInCache );
	if ( b ) return wxBitmap( *b );
	else return errorBmp;
}

/*****************************************************
**
**   ImageProvider   ---   getFileBasedBitmapRef
**
******************************************************/
wxBitmap *ImageProvider::getFileBasedBitmapRef( const wxString &s, const int rotateHue, const bool storeInCache )
{
	BitmapCache *cache = BitmapCache::get();
	wxBitmap *b = cache->findBitmapRef( s, rotateHue );

	if ( ! b )
	{
		wxString filename =  FileConfig::get()->getPicDir() + s;
		if ( ! wxFile::Exists( filename ))
		{
			wxLogError( wxString::Format( wxT( "bitmap \"%s\" not found in directory %s" ),
				s.c_str( ), FileConfig::get()->getPicDir().c_str()));
			b = &errorBmp;
		}
		else if ( ! wxFileName::IsFileReadable( filename ))
		{
			wxLogError( wxString::Format( wxT( "bitmap \"%s\" not readable in directory %s" ),
				s.c_str( ), FileConfig::get()->getPicDir().c_str()));
			b = &errorBmp;
		}
		else // file exists and is readable
		{
			if ( rotateHue )
			{
				//printf( "ImageProvider::loadBitmap BMP %s rotateHue %d NEW IS OK %d\n", str2char( s ), rotateHue, bmp.IsOk());
				wxBitmap bmp( filename, wxBITMAP_TYPE_ANY );
				wxImage image = bmp.ConvertToImage();
				image.RotateHue( rotateHue / 360.0 );
				b = new wxBitmap( image );
			}
			else
			{
				//printf( "ImageProvider::loadBitmap ROTATED BMP rotateHue %d NEW IS OK %d\n", rotateHue, bmp.IsOk());
				b = new wxBitmap( filename, wxBITMAP_TYPE_ANY );
			}
		}

		assert( b );
		if ( storeInCache )
		{
			//printf( "PUT TO CACHE name %s rh %d\n", str2char( s ), rotateHue );
			cache->putBitmapRef( s, rotateHue, b );
		}
	}
	return b;
}

/*****************************************************
**
**   ImageProvider   ---   getDasaImageList
**
******************************************************/
wxImageList *ImageProvider::getDasaImageList()
{
	if ( ! dasaImageList )
	{
		const int max_image = 15;
		const BitmapId imageIdList[max_image] = { BM_DASA, BM_FOLDER, BM_MAHADASA, BM_DASATREE1,
			BM_DASATREE2, BM_DASATREE3, BM_DASATREE4, BM_DASATREE5, BM_FOLDERE,
			BM_MAHADASAE, BM_DASATREE1E, BM_DASATREE2E, BM_DASATREE3E, BM_DASATREE4E,
			BM_DASATREE5E };

		dasaImageList = new wxImageList( DEFAULT_BITMAP_SIZE, DEFAULT_BITMAP_SIZE, true );

		for ( int i = 0; i < max_image; i++ )
		{
			dasaImageList->Add( getBitmap( imageIdList[i] ));
		}
	}
	return dasaImageList;
}

/*****************************************************
**
**   ImageProvider   ---   getViewImageListIndex4BitmapId
**
******************************************************/
int ImageProvider::getViewImageListIndex4BitmapId( const BitmapId &bid ) const
{
	return bid - (int)BM_ECLIPSE;
}

/*****************************************************
**
**   ImageProvider   ---   createViewImageList
**
******************************************************/
wxImageList *ImageProvider::createViewImageList( const bool small )
{
	const int size = small ? SMALL_IMAGE_SIZE : getDefaultBitmapSize();
	wxImageList *imageList = new wxImageList( size, size );

	for ( int id = (int)BM_ECLIPSE; id <= (int)BM_CHARTB; id++ )
	{
		if ( small ) imageList->Add( getScaledBitmap( (BitmapId)id, size ));
		else imageList->Add( getBitmap( (BitmapId)id ));
	}
	return imageList;
}

/*****************************************************
**
**   ImageProvider   ---   bitmapId2CharArray
**
******************************************************/
char **ImageProvider::bitmapId2CharArray( const BitmapId &index )
{
	if( ! ( IS_VALID_BITMAP_ID( index )))
	{
		wxLogError( wxT( "invalid bitmap id %d, allowed values are between %d and %d" ), (int)index, (int)BM_FIRST, (int)BM_LAST );
		return error_xpm;
	}

	static char **xpms[] = { error_xpm, 
		maitreya_xpm,

		// global commands
		close_xpm,
		new_xpm,
		open_xpm,
		quit_xpm,
		save_xpm,
		saveas_xpm,

		// view without document
		eclipse_xpm,
		ephem_xpm,
		hora_xpm,
		yogaeditor_xpm,

		// view with document
		ashtakavarga_xpm,
		dasa_xpm,
		graphicaldasa_xpm,
		mview_xpm,
		partner_xpm,
		sbc_xpm,
		shadbala_xpm,
		solar_xpm,
		sview_xpm,
		text_xpm,
		transit_xpm,
		uranian_xpm,
		varga_xpm,
		wchart_xpm,
		yoga_xpm,

		// vedic charts
		chart1_xpm, chart9_xpm, chart2_xpm, chart3_xpm, chart4_xpm, chart6_xpm, chart7_xpm, chart8_xpm,
		chart10_xpm, chart12_xpm, chart16_xpm, chart20_xpm, chart24_xpm, chart27_xpm, chart30_xpm, chart40_xpm,
		chart45_xpm, chart60_xpm, chart108_xpm, chart144_xpm, chartb_xpm,

		// misc modal dialogs
		about_xpm,
		atlas_xpm,
		config_xpm,

		// commands on document dependend views
		edit_xpm,
		print_xpm,
		quickprint_xpm,

		// dasa tree view
		folder_xpm, foldere_xpm,
		dasatree1_xpm, dasatree1e_xpm,
		dasatree2_xpm, dasatree2e_xpm,
		dasatree3_xpm, dasatree3e_xpm,
		dasatree4_xpm, dasatree4e_xpm,
		dasatree5_xpm, dasatree5e_xpm,
		mahadasa_xpm, mahadasae_xpm,

		// general toolbar command
		arrow_down_xpm, arrow_left_xpm, arrow_right_xpm, arrow_up_xpm,
		copy_xpm,
		export_csv_xpm, export_html_xpm, export_image_xpm, export_pdf_xpm, export_text_xpm,
		filter_xpm,
		goto_xpm,
		help_xpm,
		now_xpm,
		split_xpm,
		tip_xpm,
		zoom_in_xpm, zoom_original_xpm, zoom_out_xpm,

		separator_xpm
	};

	
	return xpms[index];
}

/*****************************************************
**
**   ImageProvider   ---   getIcon
**
******************************************************/
wxIcon ImageProvider::getIcon( const BitmapId &index )
{
	char **bmp = bitmapId2CharArray( index );
	return wxIcon( bmp );
}

/*****************************************************
**
**   ImageProvider   ---   getScaledBitmap
**
******************************************************/
wxBitmap ImageProvider::getScaledBitmap( const BitmapId &index, const int &size )
{
	return wxBitmap( getBitmap( index ).ConvertToImage().Scale( size, size ));
}

/*****************************************************
**
**   ImageProvider   ---   getScaledBitmap
**
******************************************************/
wxBitmap ImageProvider::getScaledBitmap( const wxString s, const int &size )
{
	return wxBitmap( getFileBasedBitmapRef( s )->ConvertToImage().Scale( size, size ));
}

/*****************************************************
**
**   ImageProvider   ---   getMenuBitmap
**
******************************************************/
wxBitmap ImageProvider::getMenuBitmap( const BitmapId &index )
{
	return getScaledBitmap( index, SMALL_IMAGE_SIZE );
}

/*****************************************************
**
**   ImageProvider   ---   getImage
**
******************************************************/
wxImage ImageProvider::getImage( const wxString &s )
{
	return getFileBasedBitmap( s ).ConvertToImage();
}

/*****************************************************
**
**   ImageProvider   ---   getBitmap
**
******************************************************/
wxBitmap ImageProvider::getBitmap( const BitmapId &index, const int rotateHue )
{
	//char **bmp = getBitmapArray( index );
	char **bmp = bitmapId2CharArray( index );

	if ( rotateHue )
	{
		//printf( "ROTATE NOW %d\n", rotateHue );
		wxImage image( bmp );
		image.RotateHue( rotateHue / 360 );
		return wxBitmap( image );

	}
	else return wxBitmap( bmp );
}

