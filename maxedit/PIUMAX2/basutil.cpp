#include "basutil.h"

#include "cmnpiu.h"
#include "libc_max_common.h"

// ** Graphic utilities

int imgWidth = 0;
int imgHeight = 0;

void GetSeccion(void *pSrcData, int nWidth, int nHeight, int nChan, int nx, int ny, int cx, int cy, void *pDstData)
{
	unsigned char *b = (unsigned char *)pSrcData;
	unsigned char *c = (unsigned char *)pDstData;
	for(int y = ny; y < (ny+cy); y++)
	{
		memcpy(c+(y-ny)*cx*nChan, b+(y*nWidth+nx)*nChan, nChan*cx*sizeof(unsigned char));
	}
	return;
}

int GetPowerOf2(int nSize)	// Devuelve el numero elevado al 2 mas cercano al numero especificado
{
	int nTest = 0, power = 0;
	while(nTest < nSize)
	{
		power++;
		nTest = 1<<power;
	}
	return nTest;
}

CGraphTexture* GenTexImage(LPCTSTR chName)
{
	/*TCHAR strDrive[260];
	TCHAR strDir[260];
	TCHAR strFile[260];
	TCHAR strExt[260];
	_tsplitpath(chName, strDrive, strDir, strFile, strExt);*/
	//CString str = strExt;
	CString stra = chName;
    CString str = stra.Right(stra.GetLength()-max(stra.ReverseFind('.'), 0));
	if(str != _T(".png"))
	{
	    return GenTexEmpty(64, 64, 255, 0x10, 255, 255);
	}
	CGraphTexture* rt = GenTexImagePNG(chName);
	if(rt == NULL)
		{
		return GenTexEmpty(64, 64, 255, 0x10, 255, 255);
	}
	return rt;
}

CGraphTexture* GenTexEmpty(int nx, int ny, unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na)
{
	unsigned char* data = TexEmpty(nx,ny,nr,ng,nb,na);
	CGraphTexture* texture = new CGraphTexture();
	g_gmMain->CreateTexture(0, PMT_RGBA, nx, ny, texture);
	texture->TexImage2D(0, PMT_RGBA, 0, nx, ny, data);
	delete data;
	return texture;
}

unsigned char* TexEmpty(int nx, int ny, unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na)
{
	imgWidth = nx; imgHeight = ny;
	unsigned char* data = new unsigned char[nx*ny*4];
	unsigned char bytep[4];
	bytep[0] = nr; bytep[1] = ng; bytep[2] = nb; bytep[3] = na;
	for(int i = 0; i < (nx*ny); i++)
		memcpy(data+i*4, bytep, 4*sizeof(unsigned char));
	return data;
}

void userReadData(png_structp pngPtr, png_bytep data, png_size_t length)
{
	png_voidp a = png_get_io_ptr(pngPtr);
	fread((unsigned char*)data, length, 1, (FILE*)a);
}

CGraphTexture* GenTexImagePNG(LPCTSTR chName)
{
	unsigned char* data = NULL;
	if(!OpenReadImagePNG(chName, &data, &imgWidth, &imgHeight))
	{
	    return NULL;
	}
	int mode = PMT_RGBA;

	int nRx = GetPowerOf2(imgWidth);
	int nRy = GetPowerOf2(imgHeight);
	bool bIsResize = (nRx != imgWidth || nRy != imgHeight);
	unsigned char *rdata;
	if(bIsResize) rdata = new unsigned char[nRx*nRy*4];
	CGraphTexture* texture = new CGraphTexture();
	g_gmMain->CreateTexture(-1, mode, nRx, nRy, texture);
	g_gmMain->BindTexture(texture);
	if(bIsResize)
		ZoomSurface(imgWidth, imgHeight, data, nRx, nRy, rdata);
	if(bIsResize)
		texture->TexImage2D(-1, mode, 0, nRx, nRy, rdata);
	else
		texture->TexImage2D(-1, mode, 0, nRx, nRy, data);
	// Ahora vamos a almacenar las variables de acceso a las texturas
	if(bIsResize) delete rdata;
	delete data;
	return texture;
}

int OpenReadImagePNG(LPCTSTR chName, unsigned char** data, int* nWidth, int* nHeight)
{
    png_byte pngsig[PNGSIGSIZE];
    int is_png = 0;
	FILE *hFile = _tfopen(chName, _T("r+b"));
	if(hFile == NULL) return 0;
	fread(pngsig, 1, PNGSIGSIZE, hFile);
	is_png = !png_sig_cmp(pngsig, 0, PNGSIGSIZE);
	if(!is_png)
	{
		fclose(hFile);
		return 0;
	}

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr)
	{
		fclose(hFile);
		return 0;
	}
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		fclose(hFile);
		return 0;
	}
	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
		fclose(hFile);
		return 0;
	}

	png_set_read_fn(pngPtr, (voidp)hFile, userReadData);
	png_set_sig_bytes(pngPtr, PNGSIGSIZE);
    png_read_info(pngPtr, infoPtr);

	png_uint_32 pngImgWidth =  png_get_image_width(pngPtr, infoPtr);
    png_uint_32 pngImgHeight = png_get_image_height(pngPtr, infoPtr);
    png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, infoPtr);
    png_uint_32 channels   = png_get_channels(pngPtr, infoPtr);
    png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

	switch (color_type)
	{
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            break;
        case PNG_COLOR_TYPE_GRAY:
            if (bitdepth < 8)
				png_set_expand_gray_1_2_4_to_8(pngPtr);
            break;
    }
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(pngPtr);
    if (bitdepth == 16)
        png_set_strip_16(pngPtr);

	// ** Volver a leer la información, asegurarnos que sea bpp = 4
	png_read_update_info(pngPtr, infoPtr);
	pngImgWidth =  png_get_image_width(pngPtr, infoPtr);
    pngImgHeight = png_get_image_height(pngPtr, infoPtr);
    bitdepth   = png_get_bit_depth(pngPtr, infoPtr);
    channels   = png_get_channels(pngPtr, infoPtr);
    color_type = png_get_color_type(pngPtr, infoPtr);

	if((channels != 3 && channels != 4) || bitdepth != 8)
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, NULL); fclose(hFile);
		return 0;
	}

	png_bytep* rowPtrs = new png_bytep[pngImgHeight];
	(*data) = new unsigned char[pngImgWidth * pngImgHeight * bitdepth * channels / 8];
    const unsigned int stride = pngImgWidth * bitdepth * channels / 8;
    for (size_t i = 0; i < pngImgHeight; i++)
	{
        unsigned int q = (pngImgHeight - i - 1) * stride;
        rowPtrs[i] = (png_bytep)(*data) + q;
    }

	png_read_image(pngPtr, rowPtrs);

	png_read_end(pngPtr, NULL);
	png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
	fclose(hFile);
	delete rowPtrs;

	(*nWidth) = (int)pngImgWidth;
	(*nHeight) = (int)pngImgHeight;
	return 1;
}

// ** Utilidades de zoom
// ** Este codigo fue extraido de Stepmania
// ** Porque no se como mierdas se hace...

static void InitVectors( vector<int> &s0, vector<int> &s1, vector<float> &percent, int src, int dst )
{
	if( src >= dst )
	{
		float sx = float(src) / dst;
		for( int x = 0; x < dst; x++ )
		{
			/* sax is the exact (floating-point) x coordinate in the source
			 * that the destination pixel at x should from.  For example, if we're
			 * going 512->256, then dst[0] should come from the pixels from 0..1 and
			 * 1..2, so sax[0] is 1. sx is the total number of pixels, so sx/2 is the
			 * distance from the start of the sample to its center. */
			const float sax = sx*x + sx/2;

			/* sx/2 is the distance from the start of the sample to the center;
			 * sx/4 is the distance from the center of the sample to the center of
			 * either pixel. */
			const float xstep = sx/4;

			/* source x coordinates of left and right pixels to sample */
			s0.push_back(int(sax-xstep));
			s1.push_back(int(sax+xstep));

			if( s0[x] == s1[x] )
			{
				/* If the sampled pixels happen to be the same, the distance
				 * will be 0.  Avoid division by zero. */
				percent.push_back( 1.f );
			} else {
				const int xdist = s1[x] - s0[x];

				/* fleft is the left pixel sampled; +.5 is the center: */
				const float fleft = s0[x] + .5f;

				/* sax is somewhere between the centers of both sampled
				 * pixels; find the percentage: */
				const float p = (sax - fleft) / xdist;
				percent.push_back(1-p);
			}
		}
	}
	else
	{
		/*
		 * Fencepost: If we have source:
		 *    abcd
		 * and dest:
		 *    xyz
		 * then we want x to be sampled entirely from a, and z entirely from d;
		 * the inner pixels are interpolated.  (This behavior mimics Photoshop's
		 * resize.)
		 */
		float sx = float(src-1) / (dst-1);
		for( int x = 0; x < dst; x++ )
		{
			const float sax = sx*x;

			/* source x coordinates of left and right pixels to sample */
			s0.push_back( clamp(int(sax), 0, src-1));
			s1.push_back( clamp(int(sax+1), 0, src-1) );

			const float p = 1 - (sax - floorf(sax));
			percent.push_back( p );
		}
	}
}

// ** Este codigo fue adaptado desde Stepmania
// ** Modificación RageTexture -> w, h, bpp & pixels
// ** Parece que solo trabaja con bpp = 4 (RGBA o RGBX)

void ZoomSurface(int win, int hin, void* pInData, int wout, int hout, void* pOutData)
{
	int bppin = 4;
	int bppout = 4;
	/* For each destination coordinate, two source rows, two source columns
	 * and the percentage of the first row and first column: */
	vector<int> esx0, esx1, esy0, esy1;
	vector<float> ex0, ey0;

	InitVectors( esx0, esx1, ex0, win, wout );
	InitVectors( esy0, esy1, ey0, hin, hout );

	/* This is where all of the real work is done. */
	const uint8_t *sp = (uint8_t *) pInData;
    for( int y = 0; y < hout; y++ )
	{
		uint32_t *dp = (uint32_t *) ((uint8_t*)(pOutData) + wout*bppout*y);
		/* current source pointer and next source pointer (first and second
		 * rows sampled for this row): */
		const uint8_t *csp = sp + esy0[y] * win*bppin;
		const uint8_t *ncsp = sp + esy1[y] * win*bppin;

		for( int x = 0; x < wout; x++ )
		{
			/* Grab pointers to the sampled pixels: */
			const uint8_t *c00 = csp + esx0[x]*4;
			const uint8_t *c01 = csp + esx1[x]*4;
			const uint8_t *c10 = ncsp + esx0[x]*4;
			const uint8_t *c11 = ncsp + esx1[x]*4;

			uint8_t color[4];
			for( int c = 0; c < 4; ++c )
			{
				float x0, x1;
				x0 = c00[c] * ex0[x];
				x0 += c01[c] * (1-ex0[x]);
				x1 = c10[c] * ex0[x];
				x1 += c11[c] * (1-ex0[x]);

				const float res = (x0 * ey0[y]) + (x1 * (1-ey0[y])) + 0.5f;
				color[c] = uint8_t(res);
			}
			*dp = *(uint32_t *) color;

			/* Advance destination pointer. */
			++dp;
		}
    }
}

#ifdef PIUMAX_FMOD
int ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d)%s\n", result, FMOD_ErrorString(result));
		return 0;
    }
	else return 1;
}
#endif // PIUMAX_FMOD

void GetDirStr(CString strFile, CString* strODir)
{
	/*TCHAR strDri[260];
	TCHAR strDir[260];
	TCHAR strFil[260];
	TCHAR strExt[260];
	_tsplitpath(strFile, strDri, strDir, strFil, strExt);

	strODir->Format(_T("%ls%ls"), strDri, strDir);*/
	(*strODir) = strFile.Left(max(strFile.ReverseFind('/'), strFile.ReverseFind('\\')+1));
}

int wrapi(int val, int low, int high)
{
	if(low >= high) return val;
	return (val-low)%(high-low+1)+ (val<low?(high-low+1):0) + low;
}

int MLUA_readMID(lua_State* L, int idx, MID* mid)
{
	if(!lua_istable(L, idx))
	{
		_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", %d idx don't match TABLE"), chFamily, chMethod, idx);
		(*mid) = MID(0, 0);
		return 0;
	}
	lua_pushstring(L, "id"); lua_gettable(L, idx);
	if(!lua_isnumber(L, -1))
	{
		_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", %d_from_stack.id is not NUMBER"), chFamily, chMethod, idx);
		(*mid) = MID(0, 0);
		return 0;
	}
	mid->nId = (int)lua_tonumber(L, -1);
	lua_pop(L, -1);
	lua_pushstring(L, "subid"); lua_gettable(L, idx);
	if(!lua_isnumber(L, -1))
	{
		_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", %d_from_stack.subid is not NUMBER"), chFamily, chMethod, idx);
		(*mid) = MID(0, 0);
		return 0;
	}
	mid->nSubId = (int)lua_tonumber(L, -1);
	lua_pop(L, -1);
	return 1;
}

int MLUA_setMID(lua_State* L, int idx, MID mid)
{
	if(!lua_istable(L, idx))
	{
		_printf(_T("(ERROR) basutil: MLUA_setMID, %d idx don't match TABLE"), idx);
		return 0;
	}
	lua_pushstring(L, "id"); lua_pushnumber(L, mid.nId); lua_settable(L, idx<0?(idx-2):idx);
	lua_pushstring(L, "subid"); lua_pushnumber(L, mid.nSubId); lua_settable(L, idx<0?(idx-2):idx);
	return 1;
}

int MLUA_TraduceArgs(lua_State* L, int nArg, ...)
{
	int nArgs = lua_gettop(L);
	if(nArgs < nArg) { _printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", %d argument required\n"), chFamily, chMethod, nArg); return 0;	}
	int nArg2 = nArg*2;
	int nType = 0;
	va_list vl;
	va_start(vl, nArg2);
	for(int i = 1; i <= nArg; i++)
	{
		nType = va_arg(vl, int);
		switch(nType)
		{
		case MAXLUA_TYPE_INT:
			{
				if(!lua_isnumber(L, i))	{_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", argument %d must be INTEGER\n"),
					chFamily, chMethod, i); return 0;}
				int* arg = va_arg(vl, int*);
				(*arg) = (int)lua_tonumber(L, i);
			}
			break;
		case MAXLUA_TYPE_INT64:
			{
				if(!lua_isnumber(L, i))	{_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", argument %d must be INTEGER\n"),
					chFamily, chMethod, i); return 0;}
				__int64* arg = va_arg(vl, __int64*);
				(*arg) = (__int64)lua_tonumber(L, i);
			}
			break;
		case MAXLUA_TYPE_FLOAT:
			{
				if(!lua_isnumber(L, i))	{_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", argument %d must be NUMBER\n"),
					chFamily, chMethod, i); return 0;}
				float* arg = va_arg(vl, float*);
				(*arg) = (float)lua_tonumber(L, i);
			}
			break;
		case MAXLUA_TYPE_DOUBLE:
			{
				if(!lua_isnumber(L, i))	{_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", argument %d must be NUMBER\n"),
					chFamily, chMethod, i); return 0;}
				double* arg = va_arg(vl, double*);
				(*arg) = lua_tonumber(L, i);
			}
			break;
		case MAXLUA_TYPE_BOOL:
			{
				if(!lua_isboolean(L, i))	{_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", argument %d must be BOOLEAN\n"),
					chFamily, chMethod, i); return 0;}
				bool* arg = va_arg(vl, bool*);
				(*arg) = lua_toboolean(L, i)?true:false;
			}
			break;
		case MAXLUA_TYPE_STRING:
			{
				if(!lua_isstring(L, i))	{_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", argument %d must be STRING\n"),
					chFamily, chMethod, i); return 0;}
				LPTSTR arg = va_arg(vl, LPTSTR);
				LPCSTR str = lua_tostring(L, 1);
				Max_strcpy(arg, str);
			}
			break;
		case MAXLUA_TYPE_MID:
			{
				if(!lua_isnumber(L, i))	{_printf(_T("(ERROR) ") STROP _T(" : ") STROP _T(", argument %d must be TABLE\n"),
					chFamily, chMethod, i); return 0;}
				MID* arg = va_arg(vl, MID*);
				MLUA_readMID(L, i, arg);
			}
			break;
		}
	}

	return 1;
}

// THE NEXT CODES WERE COPIED FROM STEPMANIA
// THESE ARE ONLY USED IF SM DECLARATIONS ARE ON PIUMaX
// LICENCE SHIT AND ETC...

void fapproach( float& val, float other_val, float to_move )
{
	ASSERT( to_move >= 0);
	if( val == other_val )
		return;
	float fDelta = other_val - val;
	float fSign = fDelta / fabsf( fDelta );
	float fToMove = fSign*to_move;
	if( fabsf(fToMove) > fabsf(fDelta) )
		fToMove = fDelta;	// snap
	val += fToMove;
}

/* Return a positive x mod y. */
float fmodfp(float x, float y)
{
	x = fmodf(x, y);	/* x is [-y,y] */
	x += y;				/* x is [0,y*2] */
	x = fmodf(x, y);	/* x is [0,y] */
	return x;
}

int power_of_two(int input)
{
    int value = 1;

	while ( value < input ) value <<= 1;

	return value;
}

bool IsAnInt( const CString &s )
{
	if( !s.size() )
		return false;

	for( int i=0; s[i]; i++ )
		if( s[i] < '0' || s[i] > '9' )
			return false;

	return true;
}

bool IsHexVal( const CString &s )
{
	if( !s.size() )
		return false;

	for( int i=0; s[i]; i++ )
		if( !(s[i] >= '0' && s[i] <= '9') &&
			!(toupper(s[i]) >= 'A' && toupper(s[i]) <= 'F'))
			return false;

	return true;
}

float HHMMSSToSeconds( const CString &sHHMMSS )
{
	CStringArray arrayBits;
	split( sHHMMSS, ":", arrayBits, false );

	while( arrayBits.size() < 3 )
		arrayBits.insert(arrayBits.begin(), "0" );	// pad missing bits

	float fSeconds = 0;
	fSeconds += atoi( arrayBits[0] ) * 60 * 60;
	fSeconds += atoi( arrayBits[1] ) * 60;

	// TODO: NO C++11 IN MY COMPILER, USING VC 2012
	float fLast = 0.0;
	int nRet = sscanf(arrayBits[2], "%f", &fLast);
	if(nRet != 1) fLast = 0.0;
	fSeconds += fLast;

	return fSeconds;
}

CString SecondsToHHMMSS( float fSecs )
{
	const int iMinsDisplay = (int)fSecs/60;
	const int iSecsDisplay = (int)fSecs - iMinsDisplay*60;
	CString sReturn = ssprintf( "%02d:%02d:%02d", iMinsDisplay/60, iMinsDisplay%60, iSecsDisplay );
	return sReturn;
}

CString SecondsToMMSSMsMs( float fSecs )
{
	const int iMinsDisplay = (int)fSecs/60;
	const int iSecsDisplay = (int)fSecs - iMinsDisplay*60;
	const int iLeftoverDisplay = (int) ( (fSecs - iMinsDisplay*60 - iSecsDisplay) * 100 );
	CString sReturn = ssprintf( "%02d:%02d.%02d", iMinsDisplay, iSecsDisplay, min(99,iLeftoverDisplay) );
	return sReturn;
}

CString SecondsToMMSSMsMsMs( float fSecs )
{
	const int iMinsDisplay = (int)fSecs/60;
	const int iSecsDisplay = (int)fSecs - iMinsDisplay*60;
	const int iLeftoverDisplay = (int) ( (fSecs - iMinsDisplay*60 - iSecsDisplay) * 1000 );
	CString sReturn = ssprintf( "%02d:%02d.%03d", iMinsDisplay, iSecsDisplay, min(999,iLeftoverDisplay) );
	return sReturn;
}

CString PrettyPercent( float fNumerator, float fDenominator)
{
	return ssprintf("%0.2f%%",fNumerator/fDenominator*100);
}

CString Commify( int iNum )
{
	CString sNum = ssprintf("%d",iNum);
	CString sReturn;
	for( unsigned int i=0; i<sNum.length(); i++ )
	{
		char cDigit = sNum[(unsigned int)sNum.length()-1-i];
		if( i!=0 && i%3 == 0 )
			sReturn = ',' + sReturn;
		sReturn = cDigit + sReturn;
	}
	return sReturn;
}

struct tm GetLocalTime()
{
	const time_t t = time(NULL);
	struct tm *tm;
	// TODO: NO C++11 IN MY COMPILER, USING VC 2012
	tm = localtime( &t );
	return *tm;
}

CString ssprintf( const char *fmt, ...)
{
    va_list	va;
    va_start(va, fmt);
	return vssprintf(fmt, va);
}


CString vssprintf( const char *fmt, va_list argList)
{
	CString str;
	str.FormatV(fmt, argList);
	return str;
}

CString join( const CString &Deliminator, const CStringArray& Source)
{
	if( Source.empty() )
		return "";

	CString csTmp;

	// Loop through the Array and Append the Deliminator
	for( unsigned iNum = 0; iNum < Source.size()-1; iNum++ ) {
		csTmp += Source[iNum];
		csTmp += Deliminator;
	}
	csTmp += Source.back();
	return csTmp;
}

CString join( const CString &Delimitor, CStringArray::const_iterator begin, CStringArray::const_iterator end )
{
	if( begin == end )
		return "";

	CString ret;
	while( begin != end )
	{
		ret += *begin;
		++begin;
		if( begin != end )
			ret += Delimitor;
	}

	return ret;
}


template <class S>
void do_split( const S &Source, const S &Delimitor, vector<S> &AddIt, const bool bIgnoreEmpty )
{
	size_t startpos = 0;

	do {
		size_t pos;
		if( Delimitor.size() == 1 )
			pos = Source.find( Delimitor[0], startpos );
		else
			pos = Source.find( Delimitor, startpos );
		if( pos == Source.npos )
			pos = Source.size();

		if( pos-startpos > 0 || !bIgnoreEmpty )
		{
			const S AddCString = Source.substr(startpos, pos-startpos);
			AddIt.push_back(AddCString);
		}

		startpos = pos+Delimitor.size();
	} while ( startpos <= Source.size() );
}


void split( const CString &Source, const CString &Deliminator, CStringArray &AddIt, const bool bIgnoreEmpty )
{
	do_split(Source, Deliminator, AddIt, bIgnoreEmpty );
}

void split( const wstring &Source, const wstring &Deliminator, vector<wstring> &AddIt, const bool bIgnoreEmpty )
{
	do_split(Source, Deliminator, AddIt, bIgnoreEmpty );
}

/* Use:

CString str="a,b,c";
int start = 0, size = -1;
while( 1 )
{
	do_split( str, ",", begin, size );
	if( begin == str.end() )
		break;
	str[begin] = 'Q';
}

 */

template <class S>
void do_split( const S &Source, const S &Delimitor, int &begin, int &size, int len, const bool bIgnoreEmpty )
{
	if( size != -1 )
	{
		/* Start points to the beginning of the last delimiter.  Move it up. */
		begin += size+Delimitor.size();
		begin = min( begin, len );
	}

	size = 0;

	if( bIgnoreEmpty )
	{
		/* Skip delims. */
		while( begin + Delimitor.size() < Source.size() &&
			!Source.compare( begin, Delimitor.size(), Delimitor ) )
			++begin;
	}

	/* Where's the string function to find within a substring?  C++ strings apparently
	 * are missing that ... */
	size_t pos;
	if( Delimitor.size() == 1 )
		pos = Source.find( Delimitor[0], begin );
	else
		pos = Source.find( Delimitor, begin );
	if( pos == Source.npos || (int) pos > len )
		pos = len;
	size = pos - begin;
}

void split( const CString &Source, const CString &Delimitor, int &begin, int &size, int len, const bool bIgnoreEmpty )
{
	do_split( Source, Delimitor, begin, size, len, bIgnoreEmpty );

}

void split( const wstring &Source, const wstring &Delimitor, int &begin, int &size, int len, const bool bIgnoreEmpty )
{
	do_split( Source, Delimitor, begin, size, len, bIgnoreEmpty );
}

void split( const CString &Source, const CString &Delimitor, int &begin, int &size, const bool bIgnoreEmpty )
{
	do_split( Source, Delimitor, begin, size, Source.size(), bIgnoreEmpty );
}

void split( const wstring &Source, const wstring &Delimitor, int &begin, int &size, const bool bIgnoreEmpty )
{
	do_split( Source, Delimitor, begin, size, Source.size(), bIgnoreEmpty );
}

/*
 * foo\fum\          -> "foo\fum\", "", ""
 * c:\foo\bar.txt    -> "c:\foo\", "bar", ".txt"
 * \\foo\fum         -> "\\foo\", "fum", ""
 */
void splitpath( const CString &Path, CString& Dir, CString& Filename, CString& Ext )
{
	Dir = Filename = Ext = "";

	CStringArray mat;

	/* One level of escapes for the regex, one for C. Ew.
	 * This is really:
	 * ^(.*[\\/])?(.*)$    */
	static Regex sep("^(.*[\\\\/])?(.*)$");
	bool check = sep.Compare(Path, mat);
	ASSERT(check);

	Dir = mat[0];
	const CString Base = mat[1];

	/* ^(.*)(\.[^\.]+)$ */
	static Regex SplitExt("^(.*)(\\.[^\\.]+)$");
	if( SplitExt.Compare(Base, mat) )
	{
		Filename = mat[0];
		Ext = mat[1];
	} else
		Filename = Base;
}


/* "foo.bar", "baz" -> "foo.baz"
 * "foo", "baz" -> "foo.baz"
 * "foo.bar", "" -> "foo" */
CString SetExtension( const CString &path, const CString &ext )
{
	CString Dir, FName, OldExt;
	splitpath( path, Dir, FName, OldExt );
	return Dir + FName + (ext.size()? ".":"") + ext;
}

CString GetExtension( const CString &sPath )
{
	size_t pos = sPath.rfind( '.' );
	if( pos == sPath.npos )
		return "";

	size_t slash = sPath.find( '/', pos );
	if( slash != sPath.npos )
		return ""; /* rare: path/dir.ext/fn */

	slash = sPath.find( '\\', pos );
	if( slash != sPath.npos )
		return ""; /* rare: path\dir.ext\fn */

	return sPath.substr( pos+1, sPath.size()-pos+1 );
}

void StripCrnl(CString &s)
{
	while( s.size() && (s[(int)s.size()-1] == '\r' || s[(int)s.size()-1] == '\n') )
		s.erase(s.size()-1);
}

#if 1
/* PCRE */
#include "pcre.h"
void Regex::Compile()
{
	const char *error;
	int offset;
	reg = pcre_compile(pattern.c_str(), PCRE_CASELESS, &error, &offset, NULL);

    if(reg == NULL)
	{
		printf("Invalid regex: '%s' (%s)", pattern.c_str(), error);
		exit(3);
	}

	int ret = pcre_fullinfo( (pcre *) reg, NULL, PCRE_INFO_CAPTURECOUNT, &backrefs);
	ASSERT(ret >= 0);

	backrefs++;
    ASSERT(backrefs < 128);
}

void Regex::Set(const CString &str)
{
	Release();
    pattern=str;
	Compile();
}

void Regex::Release()
{
    pcre_free(reg);
	reg = NULL;
	pattern = "";
}

Regex::Regex(const CString &str)
{
	reg = NULL;
	Set(str);
}

Regex::Regex(const Regex &rhs)
{
	reg = NULL;
    Set(rhs.pattern);
}

Regex &Regex::operator=(const Regex &rhs)
{
	if(this != &rhs) Set(rhs.pattern);
	return *this;
}

Regex::~Regex()
{
    Release();
}

bool Regex::Compare(const CString &str)
{
    int mat[128*3];
	int ret = pcre_exec( (pcre *) reg, NULL,
		str.data(), str.size(), 0, 0, mat, 128*3);

	if( ret < -1 )
	{
		printf("Unexpected return from pcre_exec('%s'): %i",
			pattern.c_str(), ret);
		exit(3);
	}

	return ret >= 0;
}

bool Regex::Compare(const CString &str, vector<CString> &matches)
{
    matches.clear();

    int mat[128*3];
	int ret = pcre_exec( (pcre *) reg, NULL,
		str.data(), str.size(), 0, 0, mat, 128*3);

	if( ret < -1 )
	{
		printf("Unexpected return from pcre_exec('%s'): %i",
			pattern.c_str(), ret);
		exit(3);
	}

	if(ret == -1)
		return false;

    for(unsigned i = 1; i < backrefs; ++i)
    {
		const int start = mat[i*2], end = mat[i*2+1];
        if(start == -1)
            matches.push_back(""); /* no match */
        else
            matches.push_back(str.substr(start, end - start));
    }

    return true;
}
#else
/* GNU regex */
#include "regex.h"
void Regex::Compile()
{
    reg = new regex_t;

    int ret = regcomp((regex_t *) reg, pattern.c_str(), REG_EXTENDED|REG_ICASE);
    if(ret != 0)
		RageException::Throw("Invalid regex: '%s'", pattern.c_str());

    /* Count the number of backreferences. */
    backrefs = 0;
    for(int i = 0; i < int(pattern.size()); ++i)
        if(pattern[i] == '(') backrefs++;
    ASSERT(backrefs+1 < 128);
}

void Regex::Set(const CString &str)
{
	Release();
    pattern=str;
	Compile();
}

void Regex::Release()
{
    delete (regex_t *)reg;
	reg = NULL;
	pattern = "";
}

Regex::Regex(const CString &str)
{
	reg = NULL;
	Set(str);
}

Regex::Regex(const Regex &rhs)
{
	reg = NULL;
    Set(rhs.pattern);
}

Regex &Regex::operator=(const Regex &rhs)
{
	if(this != &rhs) Set(rhs.pattern);
	return *this;
}

Regex::~Regex()
{
    Release();
}

bool Regex::Compare(const CString &str)
{
    return regexec((regex_t *) reg, str.c_str(), 0, NULL, 0) != REG_NOMATCH;
}

bool Regex::Compare(const CString &str, vector<CString> &matches)
{
    matches.clear();

    regmatch_t mat[128];
    int ret = regexec((regex_t *) reg, str.c_str(), 128, mat, 0);

	if(ret == REG_NOMATCH)
        return false;

    for(unsigned i = 1; i < backrefs+1; ++i)
    {
        if(mat[i].rm_so == -1)
            matches.push_back(""); /* no match */
        else
            matches.push_back(str.substr(mat[i].rm_so, mat[i].rm_eo - mat[i].rm_so));
    }

    return true;
}
#endif

void TrimLeft(CString &str, const char *s)
{
	int n = 0;
	while(n < int(str.size()) && strchr(s, str[n]))
		n++;

	str.erase(str.begin(), str.begin()+n);
}

void TrimRight(CString &str, const char *s)
{
	int n = str.size();
	while(n > 0 && strchr(s, str[n-1]))
		n--;

	/* Delete from n to the end.  If n == str.size(), nothing is deleted;
	 * if n == 0, the whole string is erased. */
	str.erase(str.begin()+n, str.end());
}
