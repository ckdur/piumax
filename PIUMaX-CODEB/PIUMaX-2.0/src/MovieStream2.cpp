#include "basutil.h"
#include "cmnpiu.h"
#include "MovieStream2.h"

//#define MS_ACTIVATEAPI

CMovieStream2::CMovieStream2(void)
: bLoad(false)
, bLoop(false)
, bResize(false)
{
	bActive = true;
	bBusy = false;
	data = 0;
	dwNext = 0;
	dNext = 0;
	width = 0;
	height = 0;
	nwidth = 0;
	nheight = 0;
	dwFrame = 0;
	dwFrameSys = 0;
	dPts = 0.0;
	tex = new CGraphTexture();
	g_gmMain->CreateTexture(1, PMT_RGB, 0, 0, tex);
	g_gmMain->BindTexture(tex);
	tex->m_bFlipY = true;
	/*g_gmMain->TexParameter(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	g_gmMain->TexParameter(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);*/
}

CMovieStream2::~CMovieStream2(void)
{
	delete tex;
}

bool CMovieStream2::OpenAVI(LPCTSTR szFile)
{
	Max_strcpy(chFile, szFile);
	if(!CreateContext()) return 0;

	pFrameRGB=avcodec_alloc_frame();
    if(pFrameRGB==NULL)
	{
		printf("FFMPEG: Can't Alloc Frame! \"%s\"\n", chFile);
		DestroyContext();
        return false;
	}

	nwidth = GetPowerOf2(pCodecCtx->width), nheight = GetPowerOf2(pCodecCtx->height);

    numBytes=avpicture_get_size(PIX_FMT_RGB24, nwidth, nheight);
    buffer=new uint8_t[numBytes];

    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24, nwidth, nheight);

	SetResize();
	dwFrame=0;
	dwFrameSys = 0;
	dwNext=0;
	dNext=0;
	dPts=0;
	bLoad = true;
	bActive = true;
	AnalogGetFrame();
	return true;
}

void CMovieStream2::AnalogGetFrame()
{
	int k = 0;
	while(k < 1)
	{
		if(av_read_frame(pFormatCtx, &packet)<0)
		{
			bActive = false;
			break;
		}
		if(packet.stream_index==videoStream)
		{
			dPts = 0;
			// Decode video frame
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
				&packet/*.data, packet.size*/);

			if(packet.dts == AV_NOPTS_VALUE
			   && pFrame->opaque && *(uint64_t*)pFrame->opaque != AV_NOPTS_VALUE)
			{
				dPts = double(*(uint64_t *)pFrame->opaque);
			}
			else if(packet.dts != AV_NOPTS_VALUE)
			{
				dPts = double(packet.dts);
			}
			else
			{
				// No se sabe el tiempo, entonces se asigna al tiempo actual
				dPts = 0.0;
			}
			dPts *= av_q2d(video_st->time_base);
			if(dPts == 0.0) dPts = dNext;

			// Did we get a video frame?
			if(frameFinished)
			{
				k++;
			}
		}
		if(k != 0) av_free_packet(&packet);
	}
	if(!bActive)
	{
		if(bLoop) SetZeroCount();
		return;
	}
	sws_scale (img_convert_ctx, pFrame->data, pFrame->linesize,
				0, pCodecCtx->height,
				pFrameRGB->data,pFrameRGB->linesize);
	data = pFrameRGB->data[0];
	g_gmMain->BindTexture(tex);
	tex->TexImage2D(1, PMT_RGB, 0, nwidth, nheight, data);
	av_free_packet(&packet);
}

//#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)
void*
#ifdef MS_ACTIVATEAPI
WINAPI
#endif // MS_ACTIVATEAPI
CMovieStream2::DecodeFrame(void*)
{
	if(bBusy) return (void*)NULL;
	bBusy = true;
	if(!bActive)
	{
		bBusy = false;
		return (void*)NULL;
	}
	int nDifFrame = dwFrame - dwFrameSys;
	if(nDifFrame > 0)
	{
		int k = 0;
		while(k < nDifFrame)
		{
			if(av_read_frame(pFormatCtx, &packet)<0)
			{
				bActive = false;
				break;
			}
			if(packet.stream_index==videoStream)
			{
				dPts = 0;
				// Decode video frame
				avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished,
					&packet/*.data, packet.size*/);

				if(packet.dts == AV_NOPTS_VALUE
				   && pFrame->opaque && *(uint64_t*)pFrame->opaque != AV_NOPTS_VALUE)
				{
					dPts = double(*(uint64_t *)pFrame->opaque);
				}
				else if(packet.dts != AV_NOPTS_VALUE)
				{
					dPts = double(packet.dts);
				}
				else
				{
					// No se sabe el tiempo, entonces se asigna al tiempo actual
					dPts = 0.0;
				}
				dPts *= av_q2d(video_st->time_base);
				if(dPts == 0.0) dPts = dNext;

				// Did we get a video frame?
				if(frameFinished)
				{
					k++;
				}
			}
			if(k != (nDifFrame-1)) av_free_packet(&packet);
		}
		if(!bActive)
		{
			if(bLoop) SetZeroCount();
			bBusy = false;
			return (void*)NULL;
		}
		sws_scale (img_convert_ctx, pFrame->data, pFrame->linesize,
					0, pCodecCtx->height,
					pFrameRGB->data,pFrameRGB->linesize);
		data = pFrameRGB->data[0];
		g_gmMain->BindTexture(tex);
		tex->TexImage2D(1, PMT_RGB, 0, nwidth, nheight, data);
		av_free_packet(&packet);
	}
	bBusy = false;
	return (void*)NULL;
}

void* Dec(void *Arg)
{
	CMovieStream2* movie;
	movie = (CMovieStream2*) Arg;
	movie->DecodeFrame(NULL);
	return (void*)NULL;
}

#ifdef MS_ACTIVATEAPI
pthread_t thread;
#endif

void CMovieStream2::GrabAVIFrame()
{
	if(bBusy && !bLoad) return;
#ifdef MS_ACTIVATEAPI
	 int rc = pthread_create(&thread, NULL, Dec, (void*)this);
      if (rc){
		  _printf(_T("ERROR: no se puede crear el hilo para DecodeFrame() en codigo %d\n"), rc);
      }
#else
	DecodeFrame((void*)0);
#endif // MS_ACTIVATEAPI
}

int CMovieStream2::Update(DWORD milliseconds)
{
	dwNext += milliseconds;
	dNext = double(dwNext)/1000;	// Convertir a segundos
	dwFrame = DWORD(dNext/dTimeBase);
	dwFrameSys = DWORD(dPts/dTimeBase);
	return 0;
}

int CMovieStream2::SetTime(DWORD dwTime)
{
	dwNext = dwTime;
	return Update(0);
}

void CMovieStream2::SetZeroCount(void)
{
	dwFrame=0;
	dwFrameSys = 0;
	dwNext=0;
	dNext=0;
	dPts=0;
	DestroyContext();
	CreateContext();
	bActive = true;
}

void CMovieStream2::CloseAVI(void)
{
	if(bBusy)
	{
		bBusy = false;
	}
	if(bLoad)
	{
		bLoad = false;
		DestroyContext();
		//delete buffer;
		if(pFrameRGB != NULL) av_free(pFrameRGB);
	}
	if(bResize) ClearResize();
}

void CMovieStream2::SetResize(void)
{
	static int sws_flags = SWS_FAST_BILINEAR;
	img_convert_ctx = sws_getContext(pCodecCtx->width,
									pCodecCtx->height,
									pCodecCtx->pix_fmt,
									nwidth,
									nheight,
									PIX_FMT_RGB24,
									sws_flags, NULL, NULL, NULL);
	bResize = true;
}

void CMovieStream2::ClearResize(void)
{
	sws_freeContext(img_convert_ctx);
	bResize = false;
}

// ** Funciones privadas
int CMovieStream2::CreateContext(void)
{
	pFormatCtx = avformat_alloc_context();
	if(avformat_open_input(&pFormatCtx, chFile, NULL, NULL)!=0)
	{
		printf("FFMPEG: Can't open file! \"%s\"\n", chFile);
        return 0;
	}

    if(avformat_find_stream_info(pFormatCtx, NULL)<0)
	{
		printf("FFMPEG: Couldn't find stream information! \"%s\"\n", chFile);
        return 0;
	}

    av_dump_format(pFormatCtx, 0, chFile, false);
    videoStream=-1;
	int i;
    for(i=0; i<int(pFormatCtx->nb_streams); i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoStream=i;
            break;
        }
    if(videoStream==-1)
	{
		printf("FFMPEG: Didn't find a video stream! \"%s\"\n", chFile);
        return 0;
	}

    // Get a pointer to the codec context for the video stream
	video_st=pFormatCtx->streams[videoStream];
    pCodecCtx=pFormatCtx->streams[videoStream]->codec;

    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL)
	{
		printf("FFMPEG: Codec not found! \"%s\"\n", chFile);
        return 0;
	}

    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
	{
		printf("FFMPEG: Could not open codec! \"%s\"\n", chFile);
        return 0;
	}

    pFrame=avcodec_alloc_frame();

	dTimeBase = av_q2d(video_st->time_base);
	dDur = double(video_st->duration)*dTimeBase;
	dwDur = DWORD(dDur*1000);

	GrabAVIFrame();

	return 1;
}

void CMovieStream2::DestroyContext(void)
{
    av_free(pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}
