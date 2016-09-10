#include "MaxGenericBGA.h"
#include "cmnpiu.h"

#define SPECTROGRAM_DIVS 128

CMaxGenericBGA::CMaxGenericBGA()
{
	m_bUmbral1 = false;
	m_color = 1;
}


CMaxGenericBGA::~CMaxGenericBGA()
{
}

void CMaxGenericBGA::SetChannel(CSoundChannel* sc)
{
	m_scAnalisys = sc;
}

void CMaxGenericBGA::Update(DWORD dwMilliseconds)
{
	// TODO: Something that depends directly by time
}

void CMaxGenericBGA::Draw(void)
{
	float dataR[NUM_SAMPLES_FFT*2];
	float dataL[NUM_SAMPLES_FFT*2];
 
	// Obtain waveform
	m_scAnalisys->GetWaveData(dataL, NUM_SAMPLES_FFT*2, 0);
	m_scAnalisys->GetWaveData(dataR, NUM_SAMPLES_FFT*2, 1);

	// Putting on the order real-imag
	for (int i = NUM_SAMPLES_FFT - 1; i >= 0; i--)
	{
		dataL[i << 1] += dataR[i << 1];
		//dataL[i << 1] = dataL[i];
		dataL[(i << 1) + 1] = 0.0;
		//dataR[i << 1] = dataR[i];
		dataR[(i << 1) + 1] = 0.0;
	}

	// Do fft
	m_fft.fft(dataL);

	// Get the square pow of abs
	for (int i = 0; i < NUM_SAMPLES_FFT; i += 2)
	{
		dataL[i/2] = dataL[i] * dataL[i] + dataL[i + 1] * dataL[i + 1];
	}

	// Store our sample frequency
	float frec;
	m_scAnalisys->GetFrequency(&frec);

	// Get the bass
	// Ranges through 60 to 250 Hz
	int nfbass_low = (int)(60.0f / frec * (NUM_SAMPLES_FFT));
	int nfbass_high = (int)(0.5f * (NUM_SAMPLES_FFT));

	// Get total power using parsevall
	float bass_pow = 0.0f;
	for (int i = nfbass_low; i < nfbass_high; i++)
	{
		bass_pow += dataL[i];	// Not necesary to square this, because before
	}
	bass_pow /= (float)(nfbass_high - nfbass_low) / 2;
	bass_pow = 10 * log10(bass_pow + 1);

	// Use it to draw a square, scaled by bass_pow
	if (bass_pow > 24 && !m_bUmbral1)
	{
		m_color++;
		m_bUmbral1 = true;
	}
	else if (bass_pow < 23 && m_bUmbral1)
		m_bUmbral1 = false;
	g_gmMain->MatrixMode(PMG_MODELVIEW);
	g_gmMain->PushMatrix();
	g_gmMain->Scale(1.0+0.1*bass_pow, 1.0+0.01*bass_pow, 1.0);
	g_gmMain->BindTexture(NULL);
	g_gmMain->Color4d(1.0*(float)(m_color & 1), 1.0*(float)((m_color >> 1) & 1), 1.0*(float)((m_color >> 2) & 1), 1.0);
	g_gmMain->Begin(PMG_QUADS);
	g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(-8.0, -6.0, -100.0);
	g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(8.0, -6.0, -100.0);
	g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(8.0, 6.0, -100.0);
	g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(-8.0, 6.0, -100.0);
	g_gmMain->End();
	g_gmMain->PopMatrix();

	// Draw a spectrogram
	int j=0;
	const float BAR_WIDTH = (80.0f / SPECTROGRAM_DIVS);
	for (int i = 0; i < SPECTROGRAM_DIVS; i++)
	{
		int c = 0;
		float val = 0.0;
		while (((i+1) * NUM_SAMPLES_FFT / SPECTROGRAM_DIVS) > j && j < NUM_SAMPLES_FFT)
		{
			val += dataL[j];
			j++;
			c++;
		}
		if (c > 0) val /= (float)c;
		val = 10 * log10(val+1.0);
		g_gmMain->MatrixMode(PMG_MODELVIEW);
		g_gmMain->PushMatrix();
		g_gmMain->Translate(BAR_WIDTH*(0.5+(double)i*2)-80.0, -60.0, 0.0);
		g_gmMain->BindTexture(NULL);
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);
		g_gmMain->Begin(PMG_QUADS);
		g_gmMain->TexCoord2d(0.0, 1.0); g_gmMain->Vertex3d(-BAR_WIDTH, 0.0, -100.0);
		g_gmMain->TexCoord2d(1.0, 1.0); g_gmMain->Vertex3d(BAR_WIDTH, 0.0, -100.0);
		g_gmMain->TexCoord2d(1.0, 0.0); g_gmMain->Vertex3d(BAR_WIDTH, val, -100.0);
		g_gmMain->TexCoord2d(0.0, 0.0); g_gmMain->Vertex3d(-BAR_WIDTH, val, -100.0);
		g_gmMain->End();
		g_gmMain->PopMatrix();
	}
}
 