#ifndef MAX_GENERICBGA
#define MAX_GENERICBGA

#include "stdafx.h"
#include "SoundChannel.h"
#include "fft.hpp"

#define NUM_POW2_SAMPLES_FFT 10
#define NUM_SAMPLES_FFT (1<<NUM_POW2_SAMPLES_FFT)

class CMaxGenericBGA
{
public:
	CMaxGenericBGA();
	~CMaxGenericBGA();

	// Function for updating by time
	void Update(DWORD dwMilliseconds);
	// Function for drawing
	void Draw(void);
	// Function for setting the channel
	void SetChannel(CSoundChannel* sc);

private:
	// The channel auditor
	CSoundChannel *m_scAnalisys;

	bool m_bUmbral1;
	int m_color;

	// The fft calculator
	GFFT<NUM_POW2_SAMPLES_FFT, float> m_fft;
};

#endif // MAX_GENERICBGA
