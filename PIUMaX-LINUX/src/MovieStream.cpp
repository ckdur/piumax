#if 0
#include "cmnpiu.h"
#include "MovieStream.h"
#include <math.h>
#include <stdio.h>


void flipIt(void* buffer, int height, int width)
{
    char *b = (char *)buffer;
	__asm														// Assembler Code To Follow
	{
		mov ecx, 512*512										// Counter Set To Dimensions Of Our Memory Block
		mov ebx, b												// Points ebx To Our Data (b)
		label:													// Label Used For Looping
			mov al,[ebx+0]										// Loads Value At ebx Into al
			mov ah,[ebx+2]										// Loads Value At ebx+2 Into ah
			mov [ebx+2],al										// Stores Value In al At ebx+2
			mov [ebx+0],ah										// Stores Value In ah At ebx
			
			add ebx,3											// Moves Through The Data By 3 Bytes
			dec ecx												// Decreases Our Loop Counter
			jnz label											// If Not Zero Jump Back To Label
	}
}

int CMovieStream::GetPowerOf2(int nSize)	// Devuelve el numero elevado al 2 mas cercano al numero especificado
{
	int nTest = 0, power = 0;
	while(nTest < nSize)
	{
		power++;
		nTest = (int)pow((double)2, power);
	}
	return nTest;
}


CMovieStream::CMovieStream(void)
: bLoad(false)
, bLoop(false)
, bResize(false)
{
	data = 0;
}

CMovieStream::~CMovieStream(void)
{
}

bool CMovieStream::OpenAVI(LPCTSTR szFile)
{
	if (AVIStreamOpenFromFile(&pavi, szFile, streamtypeVIDEO, 0, OF_READ, NULL) !=0)
	{
		// An Error Occurred Opening The Stream
		wprintf(_T("Fallido al abrir el archivo AVI: %s\n"), szFile);;
		return false;
	}
	AVIStreamInfo(pavi, &psi, sizeof(psi));
	width=psi.rcFrame.right-psi.rcFrame.left;
	height=psi.rcFrame.bottom-psi.rcFrame.top;
	nwidth = GetPowerOf2(width);
	nheight = GetPowerOf2(height);
	lastframe=AVIStreamLength(pavi);
	mpf=AVIStreamSampleToTime(pavi,lastframe)/lastframe;

	SetResize();
	
	pgf=AVIStreamGetFrameOpen(pavi, NULL);
	if (pgf==NULL)
	{
		wprintf(_T("Fallido al abrir el frame del archivo AVI: %s\n"), szFile);
		return false;
	}
	bLoad = true;
	SetZeroCount();
	return true;
}

void CMovieStream::GrabAVIFrame()
{
	SelectObject (memdc, hBitmap);
	LPBITMAPINFOHEADER lpbi;
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);
	char *pdata;
	pdata=(char *)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBQUAD);
	DrawDibDraw (hdd, memdc, 0, 0, nwidth, nheight, lpbi, pdata, 0, 0, width, height, 0);
	pdata = 0;
	//flipIt(data, 512, 512);
}

int CMovieStream::Update(DWORD milliseconds)
{
	next+=milliseconds;
	frame=next/mpf;
	if (frame>=lastframe)
	{
		if(bLoop) SetZeroCount();
		else 
		{
			frame = lastframe-1;
			next -= milliseconds;
		}
		return 1;
	}
	return 0;
}
void CMovieStream::SetZeroCount(void)
{
	frame=0;
	next=0;
}

void CMovieStream::CloseAVI(void)
{
	bLoad = false;
	if(bResize) ClearResize();
	AVIStreamGetFrameClose(pgf);
	AVIStreamClose(pavi);
}

void CMovieStream::SetResize(void)
{
	BITMAPINFOHEADER	bmih;
	bmih.biSize = sizeof (BITMAPINFOHEADER);
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biWidth = nwidth;
	bmih.biHeight = nheight;
	bmih.biCompression = BI_RGB;

	hBitmap = CreateDIBSection (memdc, (BITMAPINFO*)(&bmih), DIB_RGB_COLORS, (void**)(&data), NULL, 0);
	bResize = true;
}

void CMovieStream::ClearResize(void)
{
	DeleteObject(hBitmap);
	data = 0;
	bResize = false;
}

#endif