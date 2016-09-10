#pragma once

#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"

#ifdef __cplusplus
}
#endif

//#pragma comment( lib, "avcodec.lib")
//#pragma comment( lib, "avformat.lib")
//#pragma comment( lib, "swscale.lib")
//#pragma comment( lib, "avutil.lib")

#define MOVIESTREAM_MAXRWIDTH 1024
#define MOVIESTREAM_MAXRHEIGHT 512

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
	void SetZeroCount(void);
	void CloseAVI(void);
	bool bLoad;
	bool bLoop;
	bool bResize;
	bool bActive;
	bool bBusy;
	void SetResize(void);
	void ClearResize(void);
	void* WINAPI DecodeFrame(void*);

	GLuint tex;
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
