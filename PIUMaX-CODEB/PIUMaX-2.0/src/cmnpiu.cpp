#include "stdafx.h"
#include "cmnpiu.h"
#include "libc_max_common.h"

CGraphMain* g_gmMain = NULL;
CSoundSys* g_ssMain = NULL;
CGraphTexture* g_gtNoTex;

DWORD dwTick;
DWORD dwLastTick;

PIUCONFIG piuconfig;

int nTotalCredit = 0;
CSoundChannel* channels[MAX_CHANNELS];

TCHAR* tc_MIND_TYPE[] =
{_T("IMAGE"), _T("VIDEO"), _T("TEXT")};

TCHAR chMethod[32];
TCHAR chFamily[32];

void SetCurDir()
{
	TCHAR chCore[MAX_PATH];
	GetMaxProcessDirectory(chCore);
	SetMaxCurrentDirectory(chCore);
	_printf(_T("Set dir to process: %s\n"), chCore);
}

bool operator == (MID id1, MID id2)
{return id1.nId == id2.nId && id1.nSubId == id2.nSubId;}
bool operator != (MID id1, MID id2)
{return id1.nId != id2.nId || id1.nSubId != id2.nSubId;}

void DrawObjectDegrade(
    double t, double l, double b, double r,           // Normal coordinates
    double v_t, double v_b, double v_l, double v_r,   // Texture coordinates
    double dX, double dY, double dZ, double dRotate,  // Coordinate displacement and rotation
    double dYOpSup, double dYOpInf,                   // Dissapear Top and Bottom
    double dXOpSup, double dXOpInf,                   // Dissapear Left and Right
    double dOpLen, double dOpMult,                     // General degrade length in borders
    bool bInverse,                                    // Inverse
    CGraphTexture* tex)                               // Object to draw
{
    /*
    Description:
    You draw a common textured square, but with degrade options.
    **************************************************
    ********************dYOpSup***********************
    **                                              **
    **     -----------t+dY--------                  **
    **     |                     |                  **
    **     |                     |                  **
    **     l+dX                  r+dX               **
    **     |                     |                  **
    **     -----------b+dY--------                  **
    dXOpInf                                         dXOpSup
    **                                              **
    ********************dYOpInf***********************dOpLen
    **************************************************
    dRotate affects all graphic around dX,dY,dZ
    dZ only translates the 2D graphic to Z-dir
    dOpMult will bring multiplicity opacity to all Alpha
    */

    // Swipe t, b, r, l if they are upside down
    if(t < b) {double temp = t; t = b; b = temp;}
    if(r < l) {double temp = r; r = l; l = temp;}

    // Same for limits
    if(dYOpSup < dYOpInf) {double temp = dYOpSup; dYOpSup = dYOpInf; dYOpInf = temp;}
    if(dXOpSup < dXOpInf) {double temp = dXOpSup; dXOpSup = dXOpInf; dXOpInf = temp;}

    if(dOpLen < 0) dOpLen = -dOpLen;
    if(dOpMult == 0.0) return; // If this is zero, so what the fuck is this func doing?

    // Begin with common transformations
	g_gmMain->MatrixMode(PMG_MODELVIEW);
	g_gmMain->PushMatrix();
	g_gmMain->Translate(dX, dY, dZ);
	g_gmMain->Rotate(dRotate, 0.0, 0.0, 1.0);

	g_gmMain->BindTexture(tex);

	// Center position of dissapear
	double dSizeY = t-b;
	double dSizeX = r-l;

	double dYSup = dYOpSup-dY;
	double dYSupF = dYSup+dOpLen;
	double dYInf = dYOpInf-dY;
	double dYInfF = dYInf-dOpLen;

	double dXSup = dXOpSup-dX;
	double dXSupF = dXSup+dOpLen;
	double dXInf = dXOpInf-dX;
	double dXInfF = dXInf-dOpLen;

	// There is a maximum of 25 areas, each one has 4 points
	// But there is only 12 lines, this is the smallest data form
	/*

	+-----------0-+-----t-dx (worst)--------------
	|   0    |   0    |   0    |   0    |   0    |
	|        |        |        |        |        |
	--------------------dYSupF--------------------
	|   0    | degout | degup  | degout |   0    |
	|  100   | degin  | degdwn | degin  |        |
	--------------------dYSup---------------------
	|   0    | deglft |  100%  | degrgt |   0    |
	|  100   | degrgt |   0%   | deglft |        |
	--------------------dYInf---------------------
	|   0    | degout | degdwn | degout |   0%   |
	|  100   | degin  | degup  | degin  |  100%  |
	--------------------dYInfF--------------------
	|   0%   |   0%   |   0%   |   0%   |   0%   |
	|  100%  |  100%  |  100%  |  100%  |  100%  |
	--------------------b-dx (worst)-------------
	*/

	// We need to extract coords of each one
	double dh[6], dv[6];
	dh[0] = t;
	dh[1] = min(max(dYSupF, b), t);
	dh[2] = min(max(dYSup, b), t);
	dh[3] = max(min(dYInf, t), b);
	dh[4] = max(min(dYInfF, t), b);
	dh[5] = b;

	dv[0] = r;
	dv[1] = min(max(dXSupF, l), r);
	dv[2] = min(max(dXSup, l), r);
	dv[3] = max(min(dXInf, r), l);
	dv[4] = max(min(dXInfF, r), l);
	dv[5] = l;

	// Assign opacity
	double oh[6], ov[6];
	oh[0] = 0.0;
	oh[1] = (dOpLen>0.0?clamp( (dYSupF-t)/dOpLen, 0.0, 1.0):0.0);
	oh[2] = 1.0;
	oh[3] = 1.0;
    oh[4] = (dOpLen>0.0?clamp( (b-dYInfF)/dOpLen, 0.0, 1.0):0.0);
	oh[5] = 0.0;

	ov[0] = 0.0;
	ov[1] = (dOpLen>0.0?clamp( (dXSupF-r)/dOpLen, 0.0, 1.0):0.0);
    ov[2] = 1.0;
	ov[3] = 1.0;
	ov[4] = (dOpLen>0.0?clamp( (l-dXInfF)/dOpLen, 0.0, 1.0):0.0);
	ov[5] = 0.0;


	// So, we will draw these 25 pieces of shit
	//int i=0, j=2;
	for(int i=0; i < 5; i++)
	{
        for(int j=0; j < 5; j++)
        {
            // So, we need to draw?
            if(dh[i]==dh[i+1] || dv[j]==dv[j+1]) continue;
            if(oh[i]==0.0 && oh[i+1]==0.0 && ov[j]==0.0 && ov[j+1]==0.0) continue;

            // Assign TexCoords
            double lb = dh[i+1];
            double lt = dh[i];
            double ob = oh[i+1];
            double ot = oh[i];
            double ta = clamp(t, lb, lt);
			double ba = clamp(b, lb, lt);
			double vt = ((ta-b)/dSizeY)*(v_b-v_t)+v_t;
			double vb = ((ba-b)/dSizeY)*(v_b-v_t)+v_t;

			double ll = dv[j+1];
            double lr = dv[j];
            double ofr = ov[j+1];
            double ol = ov[j];
            double ra = clamp(r, ll, lr);
			double la = clamp(l, ll, lr);
			double vr = ((ra-l)/dSizeX)*(v_r-v_l)+v_l;
			double vl = ((la-l)/dSizeX)*(v_r-v_l)+v_l;

			// Draw
			g_gmMain->Begin(PMG_TRIANGLE_FAN);
			g_gmMain->Color4d(1.0, 1.0, 1.0, ov[j+1]*oh[i]*dOpMult);
			g_gmMain->TexCoord2d(vl, vt); g_gmMain->Vertex3d(dv[j+1], dh[i], -50.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, ov[j]*oh[i]*dOpMult);
			g_gmMain->TexCoord2d(vr, vt); g_gmMain->Vertex3d(dv[j], dh[i], -50.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, ov[j]*oh[i+1]*dOpMult);
			g_gmMain->TexCoord2d(vr, vb); g_gmMain->Vertex3d(dv[j], dh[i+1], -50.0);
			g_gmMain->Color4d(1.0, 1.0, 1.0, ov[j+1]*oh[i+1]*dOpMult);
			g_gmMain->TexCoord2d(vl, vb); g_gmMain->Vertex3d(dv[j+1], dh[i+1], -50.0);
			g_gmMain->End();
        }
	}

	g_gmMain->PopMatrix();
}
