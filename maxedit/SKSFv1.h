#ifndef MAX_SKSFV1
#define MAX_SKSFV1

#include "defbas.h"

struct SKSFv1HEADER
{
	uint32_t dwVersion;	// Version de los steps
	uint32_t dwLSong;		// Largo de la canci�n (en ms)

	int nOffset;		//

	double dVoltage;	// Voltaje
	double dStrength;	// Resistencia
	double dFreeze;		// Freeze
	double dCaos;		// Caos
	double dAir;		// Aire

	int nDificulty;		// Dificultad (De 1 a 99)

	int nSizeArrows1;	// Tama�o de las flechas 1
	int nSizeArrows2;	// Tama�o de las flechas 2
	int nSizeArrows3;	// Tama�o de las flechas 3
	int nSizeBeatSplit;	// Tama�o de los Beat Split
	int nSizeChangeBPM;	// Tama�o de los Cambios de BPM
	int nSizeChangeVel;	// Tama�o de los Cambios de Velocidades
	int nSizeJumps;		// Tama�o de saltos de canci�n
	int nSizeTickcount;	// Tama�o de los tickcounts
	int nSizeStops;		// Tama�o de los stops

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

	int *nIJumps;		// Saltos de canci�n (comienzo, y en Arrows)
	int *nEJumps;		// Saltos de canci�n (t�rmino, y en Arrows)

	int *nWStops;		// Momentos en el cual ocurre un stop
	double *dStops;		// Stops

	int *nWTickCount;	// Momentos en el cual ocurren cambios de TickCount
	int *nTickCount;	// TickCount
};

#endif
