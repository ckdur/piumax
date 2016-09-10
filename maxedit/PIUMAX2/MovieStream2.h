#ifndef PIUMAX_MOVIESTREAM2
#define PIUMAX_MOVIESTREAM2

#include "stdafx.h"
#include "GraphTexture.h"
//#include "defbas.h"



#ifdef __cplusplus
extern "C" {
#endif

#define __STDC_CONSTANT_MACROS

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#ifdef __cplusplus
}
#endif

#ifdef _WIN32
#endif // WIN32

class CMovieStream2
{
public:
	CMovieStream2(void);
	bool OpenAVI(LPCTSTR szFile);
	void GrabAVIFrame();
	void AnalogGetFrame();	// Obliga a obtener el siguiente frame
	int Update(DWORD milliseconds);
public:
	virtual ~CMovieStream2(void);
public:
	uint8_t 			*data;
	int					width;
	int					height;
	int					nwidth;
	int					nheight;
	DWORD dwNext;
	double dNext;
	double dDur;
	DWORD dwDur;
	DWORD dwFrame;
	DWORD dwFrameSys;
	CGraphTexture* tex;
	void SetZeroCount(void);
	void CloseAVI(void);
	bool bLoad;
	bool bLoop;
	bool bResize;
	bool bActive;
	bool bBusy;
	void SetResize(void);
	void ClearResize(void);
	void*
#ifdef MS_ACTIVATEAPI
		WINAPI
#endif // MS_ACTIVATEAPI
		DecodeFrame(void*);
private:
	char chFile[MAX_PATH];
	int CreateContext();
	void DestroyContext();
	AVFormatContext *pFormatCtx;
    int             videoStream;
    AVCodecContext  *pCodecCtx;
    AVCodec         *pCodec;
    AVFrame         *pFrame;
    AVFrame         *pFrameRGB;
    AVPacket        packet;
	AVStream* video_st;
	struct SwsContext *img_convert_ctx;
    int             frameFinished;
    int             numBytes;
    uint8_t         *buffer;
	double dTimeBase;
	double dPts;
};

#endif // PIUMAX_MOVIESTREAM2
