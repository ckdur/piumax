#ifndef MAX_SKSFV1
#define MAX_SKSFV1

#include "defbas.h"

struct SKSFv1HEADER
{
	uint32_t dwVersion;	// Version de los steps
	uint32_t dwLSong;		// Largo de la canción (en ms)

	int nOffset;		//

	double dVoltage;	// Voltaje
	double dStrength;	// Resistencia
	double dFreeze;		// Freeze
	double dCaos;		// Caos
	double dAir;		// Aire

	int nDificulty;		// Dificultad (De 1 a 99)

	int nSizeArrows1;	// Tamaño de las flechas 1
	int nSizeArrows2;	// Tamaño de las flechas 2
	int nSizeArrows3;	// Tamaño de las flechas 3
	int nSizeBeatSplit;	// Tamaño de los Beat Split
	int nSizeChangeBPM;	// Tamaño de los Cambios de BPM
	int nSizeChangeVel;	// Tamaño de los Cambios de Velocidades
	int nSizeJumps;		// Tamaño de saltos de canción
	int nSizeTickcount;	// Tamaño de los tickcounts
	int nSizeStops;		// Tamaño de los stops

	bool bGetArrowInStop;	// Coger flechas despues de un stop?
	uint32_t dwArgArrows1;	// Argumentos de las flechas 1
	uint32_t dwArgArrows2;	// Argumentos de las flechas 2
	uint32_t dwArgArrows3;	// Argumentos de las flechas 3
	int nSkin1;			// Skin de el grupo de flechas 1
	int nSkin2;			// Skin de el grupo de flechas 2
	int nSkin3;			// Skin de el grupo de flechas 3
	double dClipMin1;	// Clip minimo de las flechas 1
	double dClipMin2;	// Clip minimo de las flechas 2
	double dClipMin3;	// Clip minimo de las flechas 3
	double dClipMax1;	// Clip maximo de las flechas 1
	double dClipMax2;	// Clip maximo de las flechas 2
	double dClipMax3;	// Clip maximo de las flechas 3
}
#ifdef __GNUC__
__attribute__((ms_struct))
#endif
;

struct SKSFv1
{
	SKSFv1HEADER hStep;

	uint32_t *dwArrows1;	// Flechas 1
	uint32_t *dwArrows2;	// Flechas 2
	uint32_t *dwArrows3;	// Flechas 3

	int *nWBeatSplit;	// Momentos en el cual ocurre un BeatSplit (en Arrows)
	int *nBeatSplit;	// Beat Splits (En Arrows)

	int *nWChangeBPM;	// Momentos en el cual ocurre un Cambio de BPM (En Arrows)
	double *dChangeBPM;	// Cambios de BPM

	int *nIChangeVel;	// Momentos iniciales en el cual ocurre un Cambio de Velocidades
	int *nEChangeVel;	// Momentos finales en el cual ocurre un Cambio de Velocidades
	double *dChanges;	// Cambios de Velocidades

	int *nIJumps;		// Saltos de canción (comienzo, y en Arrows)
	int *nEJumps;		// Saltos de canción (término, y en Arrows)

	int *nWStops;		// Momentos en el cual ocurre un stop
	double *dStops;		// Stops

	int *nWTickCount;	// Momentos en el cual ocurren cambios de TickCount
	int *nTickCount;	// TickCount
};

#endif
