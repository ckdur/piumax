#include "MaxScreen.h"
#include "cmnpiu.h"
#include "basutil.h"

// ** Simply draw it!
// ** If the key state in the current transition it's zero
// ** doesn't draw anything
// ** The current event ONLY DRAWS referenced objects and it's transitions
void CMaxScreen::Draw(void)
{
	// Sounds
	if(m_nCurev < 0 || m_nCurev >= (int)(m_ev.size())) return;
	for(size_t i = 0; i < m_ev[m_nCurev].sound.size(); i++)
	{
		if(m_nTime >= m_ev[m_nCurev].sbeg[i] &&
			m_nTime < (m_ev[m_nCurev].sbeg[i]+m_ev[m_nCurev].sdur[i]))
		{
			int id = m_ev[m_nCurev].sound[i].id[0];
			int sid = m_ev[m_nCurev].sound[i].id[1];
			if(id >= int(m_sres.size())) continue;

			int nLoops = 1;

			CSoundStream* stream = NULL;
			stream = m_sres[id]->At(sid);
			if(stream == NULL) continue;
			stream->SetLoopCount(nLoops);
			int nCha = m_ev[m_nCurev].sound[i].cha;
			if(nCha >= MAX_CHANNELS) continue;

			if(m_sts[i] == 0)
			{
				g_ssMain->PlaySound(nCha, stream, false, channels[nCha]);
				(*channels[nCha]).SetPosition((unsigned int)(m_nTime - m_ev[m_nCurev].sbeg[i]), 0);
				m_sts[i] = 1;
			}


			if(m_ev[m_nCurev].smod[i] == SCR_SOUNDSMOD_STOP)
				(*channels[m_ev[m_nCurev].sound[i].cha]).Stop();
			if(m_ev[m_nCurev].smod[i] == SCR_SOUNDSMOD_PAUSE)
				(*channels[m_ev[m_nCurev].sound[i].cha]).SetPaused(true);
			if(m_ev[m_nCurev].smod[i] == SCR_SOUNDSMOD_PLAY)
				(*channels[m_ev[m_nCurev].sound[i].cha]).SetPaused(false);
			if(m_ev[m_nCurev].smod[i] == SCR_SOUNDSMOD_LOOP)
				nLoops = -1;


			// Find if another sound is playing
			/*for(size_t j = 0; j < m_ev[m_nCurev].sound.size(); j++)
			{
				if(nCha ==  m_ev[m_nCurev].sound[i].cha &&
					m_nTime >= m_ev[m_nCurev].sbeg[j] &&
					m_nTime < (m_ev[m_nCurev].sbeg[j]+m_ev[m_nCurev].sdur[j]) &&
					m_sts[j] == 1)
					m_sts[j] = 2;
			}*/
		}
		else if(m_nTime >= (m_ev[m_nCurev].sbeg[i]+m_ev[m_nCurev].sdur[i]) &&
			m_sts[i] == 1)
		{
			(*channels[m_ev[m_nCurev].sound[i].cha]).Stop();
			m_sts[i] = 2;
		}
	}
	// Draw for each object
	for(size_t i = 0; i < m_ev[m_nCurev].obj.size(); i++)
	{
		int obj = m_ev[m_nCurev].obj[i];
		__int64 nAuxTime = 0;
		// Find current transition applicated
		int tra = -1;
		for(size_t j = 0; j < m_ev[m_nCurev].trans[i].size(); j++)
		{
			int tr = m_ev[m_nCurev].trans[i][j];
			if(m_tr[tr].dur < 0 || (nAuxTime+m_tr[tr].dur) >= m_nTime || // ON TRANSITION
				((j+1) == m_ev[m_nCurev].trans[i].size() && (nAuxTime+m_tr[tr].dur) < m_nTime))	// LAST TRANSITION
			{
				tra = tr;
				break;
			}
			nAuxTime+=m_tr[tr].dur;
		}
		if(tra == -1) // no transition
			continue;
		int ef = m_tr[tra].ef;
		double* aef = m_tr[tra].aef;
		double dApt = 0.0;
		dApt = m_tr[tra].dur==0?0.0:(double(m_nTime-nAuxTime)/fabs(double(m_tr[tra].dur)));

		// Modificators
		dApt = Mod(dApt, m_tr[tra].mod, m_tr[tra].amod);

		// Effects
		g_gmMain->Color4d(1.0, 1.0, 1.0, 1.0);	// Secure coloration
		switch(ef)
		{
			case SCR_TRANSEF_NULL: DrawNull(obj, aef, dApt, m_nTime-nAuxTime, tra); break;		// Simple draw
			case SCR_TRANSEF_NODRAW: DrawNoDraw(obj, aef, dApt, m_nTime-nAuxTime, tra); break;		// No draw
			case SCR_TRANSEF_APPEAR: DrawAppear(obj, aef, dApt, m_nTime-nAuxTime, tra); break;		// Simple draw with alpha
			case SCR_TRANSEF_XAPPEAR: DrawXAppear(obj, aef, dApt, m_nTime-nAuxTime, tra); break;	// left-right appear using degrade
			case SCR_TRANSEF_YAPPEAR: DrawYAppear(obj, aef, dApt, m_nTime-nAuxTime, tra); break;	// up-down appear using degrade
			case SCR_TRANSEF_DEGAPPEAR: DrawDegAppear(obj, aef, dApt, m_nTime-nAuxTime, tra); break;	// X-degrees appear using degrade
		};
	}
}
// Modificator function
double CMaxScreen::Mod(double dApt, int mod, double* amod)
{
	double dApto = 1.0;
	switch (mod)
	{
	case SCR_TRANSMOD_LINEAL: {dApto = amod[0]*dApt+amod[1];} break;
	case SCR_TRANSMOD_HSTEP: {dApto = ((amod[2]*(dApt-amod[0]))<=0.0?0.0:amod[1])+amod[3];} break;
	case SCR_TRANSMOD_POW: {dApto = amod[2]*pow(dApt-amod[1], amod[0])+amod[3];} break;
	case SCR_TRANSMOD_SIN: {dApto = amod[2]*sin(M_PI/2*amod[0]*(dApt-amod[1]))+amod[3];} break;
	case SCR_TRANSMOD_PLINEAL:
		{
			double a = (dApt-amod[1])/amod[0];
			double n = (a-floor(a));
			if(n == 0.0 && a != 0.0) n = 1.0;
			dApto = amod[2]*n+amod[3];
		}
		break;
	case SCR_TRANSMOD_PTRIANGLE:
		{
			double a = (dApt-amod[1])/(amod[0]);
			double n = a-floor(a); double r = 0.0;
			if(0.0 <= n && n < 0.5) r = n*2;
			if(0.5 <= n && n < 1.0) r = -n*2+2.0;
			dApto = amod[2]*r + amod[3];
		}
		break;
	case SCR_TRANSMOD_PRECT:
		{
			double a = (dApt-amod[1])/amod[0];
			double n = a-floor(a); double r = 0.0;
			if(0.0 <= n && n < 0.5) r = 1.0;
			if(0.5 <= n && n < 1.0) r = 0.0;
			dApto = amod[2]*r + amod[3];
		}  break;
	case SCR_TRANSMOD_SIN_R:{dApto = amod[2]*fabs(sin(M_PI*amod[0]*(dApt-amod[1])))+amod[3];} break;
	case SCR_TRANSMOD_PTRIANGLE_R:
		{
			double a = (dApt-amod[1]+1.0)/(amod[0]*2.0);
			dApto = amod[2]*fabs((a-floor(a)-0.5)*2)+amod[3];
		}  break;
	};
	dApto = clamp(dApto, 0.0, 1.0);
	return dApto;
}

CGraphTexture* CMaxScreen::GetTex(int obj, bool* bDel)
{
	if(m_ob[obj].type >= SCR_OBJTYPE_SIZE) return NULL;
	if(m_ob[obj].type == SCR_OBJTYPE_TEXT) return NULL;
	int id = m_ob[obj].id[0];
	int sid = m_ob[obj].id[1];
	int type = m_ob[obj].type;
	if(type == SCR_OBJTYPE_IMAGE)
	{
		(*bDel) = false;
		// This catalog of resources doesn't exists in this screeen
		if(id >= int(m_ires.size())) return g_gtNoTex;

		CGraphTexture* tex = NULL;

		//m_ires[id]->At(sid)->SetTime((DWORD)m_nTime);
		// This resource doesn't exists in this catalog
		if(!(*m_ires[id]).AtTex(sid, &tex)) return g_gtNoTex;

		// Exists! Return it
		return tex;
	}
	if(type == SCR_OBJTYPE_VIDEO)
	{
		(*bDel) = false;
		// This catalog of resources doesn't exists in this screeen
		if(id >= int(m_vres.size())) return g_gtNoTex;

		CGraphTexture* tex;

		// This resource doesn't exists in this catalog
		(*m_vres[id]).AtTex(sid, &tex);

		// Exists! Return it
		return tex;
	}
	return NULL;
}

// Functions about Drawing effects
void CMaxScreen::DrawNull(int obj, double* aef, double dApt, __int64 nTime, int nTra)
{
	g_gmMain->PushMatrix();
	// Simple drawing
	TransformTransitionCommon(nTra, dApt);

	// ** If it's text, draw it!
	if(m_ob[obj].type == SCR_OBJTYPE_TEXT)
	{
		int id = m_ob[obj].id[0];
		int sid = m_ob[obj].id[1];
		if(id >= int(m_txt.size())) return;
		if(sid >= int(m_txt[id].size())) return;

		CString* msg = m_txt[id][sid];
		int fnt = m_ift[id][sid];
		DWORD dwFlags = (m_ob[obj].p[3][1]==0.0?DT_CENTER:(m_ob[obj].p[3][1]>=0.0?DT_LEFT:DT_RIGHT))|
			(m_ob[obj].p[3][2]==0.0?DT_VCENTER:(m_ob[obj].p[3][2]>=0.0?DT_TOP:DT_BOTTOM));
		m_fnt[fnt]->glDrawTextTol(
			m_ob[obj].p[0][1], m_ob[obj].p[0][0],	// Only top-left
			m_ob[obj].p[2][1], m_ob[obj].p[2][0],	// Only bottom-right
			m_ob[obj].p[1][0], m_ob[obj].p[2][2],	// Up and left alpha-degrade range
			m_ob[obj].p[1][2], m_ob[obj].p[1][1],	// Bottom and right alpha-degrade range
			m_ob[obj].v[0][0], m_ob[obj].v[0][1],	// V1 now is basepont
			m_ob[obj].p[0][2],						// Top-left Z
			m_ob[obj].v[1][0], m_ob[obj].v[1][1],	// V2 is Base and X-scale
			1.0,
			(*msg),									// Message
			// Flags set by P4
			dwFlags);
	}
	// ** If it's video or image, get texture and draw it
	else
	{
		bool bDel = false;
		CGraphTexture* tex = GetTex(obj, &bDel);
		if(tex == NULL) return;	// Fail?
		g_gmMain->BindTexture(tex);
		g_gmMain->Begin(PMG_TRIANGLE_STRIP);

		g_gmMain->TexCoord2d(m_ob[obj].v[1][0], m_ob[obj].v[0][1]);
		g_gmMain->Vertex3d(m_ob[obj].p[0][0], m_ob[obj].p[0][1], m_ob[obj].p[0][2]);
		g_gmMain->TexCoord2d(m_ob[obj].v[1][0], m_ob[obj].v[1][1]);
		g_gmMain->Vertex3d(m_ob[obj].p[1][0], m_ob[obj].p[1][1], m_ob[obj].p[1][2]);
		g_gmMain->TexCoord2d(m_ob[obj].v[0][0], m_ob[obj].v[0][1]);
		g_gmMain->Vertex3d(m_ob[obj].p[3][0], m_ob[obj].p[3][1], m_ob[obj].p[3][2]);
		g_gmMain->TexCoord2d(m_ob[obj].v[0][0], m_ob[obj].v[1][1]);
		g_gmMain->Vertex3d(m_ob[obj].p[2][0], m_ob[obj].p[2][1], m_ob[obj].p[2][2]);

		g_gmMain->End();
		if(bDel) delete tex;
	}
	g_gmMain->PopMatrix();
}

void CMaxScreen::DrawNoDraw(int obj, double* aef, double dApt, __int64 nTime, int nTra)
{
	// No draw anything!
}

// Extract Y-argument about pointer X
#define ARG(x,y) (*(x+y))

void CMaxScreen::DrawAppear(int obj, double* aef, double dApt, __int64 nTime, int nTra)
{
	g_gmMain->PushMatrix();
	// Same as draw, but color-missing

	// Simple drawing
	TransformTransitionCommon(nTra, dApt);

	// ** If it's text, draw it!
	if(m_ob[obj].type == SCR_OBJTYPE_TEXT)
	{
		int id = m_ob[obj].id[0];
		int sid = m_ob[obj].id[1];
		if(id >= int(m_txt.size())) goto DrawAppear_Fail;
		if(sid >= int(m_txt[id].size())) goto DrawAppear_Fail;

		CString* msg = m_txt[id][sid];
		int fnt = m_ift[id][sid];
		DWORD dwFlags = (m_ob[obj].p[3][1]==0.0?DT_CENTER:(m_ob[obj].p[3][1]>=0.0?DT_LEFT:DT_RIGHT))|
			(m_ob[obj].p[3][2]==0.0?DT_VCENTER:(m_ob[obj].p[3][2]>=0.0?DT_TOP:DT_BOTTOM));
		m_fnt[fnt]->glDrawTextTol(
			m_ob[obj].p[0][1], m_ob[obj].p[0][0],	// Only top-left
			m_ob[obj].p[2][1], m_ob[obj].p[2][0],	// Only bottom-right
			m_ob[obj].p[1][0], m_ob[obj].p[2][2],	// Up and left alpha-degrade range
			m_ob[obj].p[1][2], m_ob[obj].p[1][1],	// Bottom and right alpha-degrade range
			m_ob[obj].v[0][0], m_ob[obj].v[0][1],	// V1 now is basepont
			m_ob[obj].p[0][2],						// Top-left Z
			m_ob[obj].v[1][0], m_ob[obj].v[1][1],	// V2 is Base and X-scale
			ARG(aef,3)+(ARG(aef,7)-ARG(aef,3))*dApt,
			(*msg),									// Message
			// Flags set by P4
			dwFlags);
	}
	// ** If it's video or image, get texture and draw it
	else
	{
		bool bDel = false;
		CGraphTexture* tex = GetTex(obj, &bDel);
		if(tex == NULL) goto DrawAppear_Fail;	// Fail?
		g_gmMain->Color4d(
		clamp(ARG(aef,0)+(ARG(aef,4)-ARG(aef,0))*dApt, 0.0, 1.0),
		clamp(ARG(aef,1)+(ARG(aef,5)-ARG(aef,1))*dApt, 0.0, 1.0),
		clamp(ARG(aef,2)+(ARG(aef,6)-ARG(aef,2))*dApt, 0.0, 1.0),
		clamp(ARG(aef,3)+(ARG(aef,7)-ARG(aef,3))*dApt, 0.0, 1.0));
		g_gmMain->BindTexture(tex);
		g_gmMain->Begin(PMG_TRIANGLE_STRIP);
		g_gmMain->TexCoord2d(m_ob[obj].v[1][0], m_ob[obj].v[0][1]);
		g_gmMain->Vertex3d(m_ob[obj].p[0][0], m_ob[obj].p[0][1], m_ob[obj].p[0][2]);
		g_gmMain->TexCoord2d(m_ob[obj].v[1][0], m_ob[obj].v[1][1]);
		g_gmMain->Vertex3d(m_ob[obj].p[1][0], m_ob[obj].p[1][1], m_ob[obj].p[1][2]);
		g_gmMain->TexCoord2d(m_ob[obj].v[0][0], m_ob[obj].v[0][1]);
		g_gmMain->Vertex3d(m_ob[obj].p[3][0], m_ob[obj].p[3][1], m_ob[obj].p[3][2]);
		g_gmMain->TexCoord2d(m_ob[obj].v[0][0], m_ob[obj].v[1][1]);
		g_gmMain->Vertex3d(m_ob[obj].p[2][0], m_ob[obj].p[2][1], m_ob[obj].p[2][2]);
		g_gmMain->End();
		if(bDel) delete tex;
	}
DrawAppear_Fail:
	g_gmMain->PopMatrix();
}

void CMaxScreen::DrawXAppear(int obj, double* aef, double dApt, __int64 nTime, int nTra)
{
	// aef1 -> adv (relative, fit to size of image or maxX-size of text)
	// aef2 -> Advi (relative)
	// aef3 -> Degrade (absolute, non efective with text)
	// aef4 -> repetition? No(0) Yes(non-0)
	// aef5 -> Distance between reptetitions (relative for image size or limit text)
	// aef6 -> Vel for disp [rel/sec] (if 0 or neg, judges by Apt, relative by size of display if text, or size)
	// aef7 -> Continue? No(0) Yes(non-0) (Texture-image coordinates must repeat too?) (non aplicated to text)

	/*
	Description of actual function
            x = Advi + Adv*Vel*t (if vel=0 then Adv*t)
            ||<-- Degrade
            xx
	--------xx------------------
	|       xx                 |
	|       xx                 |
	|       xx                 |
	|       xx                 |
	|       xx                 |
	|       xx                 |
	--------xx------------------
            xx

	*/
	g_gmMain->PushMatrix();
	// Same as draw, but color-missing

	// Simple drawing
	TransformTransitionCommon(nTra, dApt);

	double dAdv = ARG(aef, 0);
	double dAdvi = ARG(aef, 1);
	double dDeg = ARG(aef, 2);
	bool bRep = ARG(aef, 3)<0.5?false:true;
	double dRep = ARG(aef, 4);
	double dVel = ARG(aef, 5);
	bool bCon = ARG(aef, 6)<0.5?false:true;

	// Now calculate displacement
	double dDisp = 0.0;
	// ** If it's constant velocity, judge it!
	if(dVel != 0.0) dDisp = dAdvi+dAdv*dVel*dApt*(1.0+dDeg);
	else dDisp = dAdv*dApt*(1.0+dDeg)+dAdvi;

	double dX1apt = clamp(clamp(dDisp,-dDeg,1.0+dDeg),dDisp,dDisp+1.0);
	double dX2apt = clamp(clamp(dDisp+1.0,-dDeg,1.0+dDeg),dDisp,dDisp+1.0);

	// ** If it's text, draw it!
	if(m_ob[obj].type == SCR_OBJTYPE_TEXT)
	{
        // Find
		int id = m_ob[obj].id[0];
		int sid = m_ob[obj].id[1];
		if(id >= int(m_txt.size())) goto DrawXAppear_Fail;
		if(sid >= int(m_txt[id].size())) goto DrawXAppear_Fail;

        // Degrade is not applicable

        // The message
		CString* msg = m_txt[id][sid];
		int fnt = m_ift[id][sid];
		DWORD dwFlags = (m_ob[obj].p[3][1]==0.0?DT_CENTER:(m_ob[obj].p[3][1]>=0.0?DT_LEFT:DT_RIGHT))|
			(m_ob[obj].p[3][2]==0.0?DT_VCENTER:(m_ob[obj].p[3][2]>=0.0?DT_TOP:DT_BOTTOM));

		double p[4][3];
		memcpy(p, m_ob[obj].p, sizeof(double)*4*3);
		for(int j = 0; j < 3; j++)
		{
			p[0][j]=m_ob[obj].p[0][j]+(m_ob[obj].p[3][j]-m_ob[obj].p[0][j])*dX1apt;
			p[1][j]=m_ob[obj].p[1][j]+(m_ob[obj].p[2][j]-m_ob[obj].p[1][j])*dX1apt;
			p[2][j]=m_ob[obj].p[1][j]+(m_ob[obj].p[2][j]-m_ob[obj].p[1][j])*dX2apt;
			p[3][j]=m_ob[obj].p[0][j]+(m_ob[obj].p[3][j]-m_ob[obj].p[0][j])*dX2apt;
		}

		m_fnt[fnt]->glDrawTextTol(
			p[0][1], p[0][0],	// Only top-left
			p[2][1], p[2][0],	// Only bottom-right
			p[1][0], p[2][2],	// Up and left alpha-degrade range
			p[1][2], p[1][1],	// Bottom and right alpha-degrade range
			m_ob[obj].v[0][0], m_ob[obj].v[0][1],	// V1 now is basepont
			m_ob[obj].p[0][2],						// Top-left Z
			m_ob[obj].v[1][0], m_ob[obj].v[1][1],	// V2 is Base and X-scale
			1.0,
			(*msg),									// Message
			// Flags set by P4
			dwFlags);
	}
	// ** If it's video or image, get texture and draw it
	else
	{
		bool bDel = false;
		CGraphTexture* tex = GetTex(obj, &bDel);
		if(tex == NULL) goto DrawXAppear_Fail;	// Fail?

        // dDeg is relative, we must find the absolute
        double dDegs = dDeg*fabs(m_ob[obj].p[1][0]-m_ob[obj].p[0][0]);
		// ** Repeating?
		g_gmMain->BindTexture(tex);

		// Calculate positions
		double p[4][3];
		memcpy(p, m_ob[obj].p, sizeof(double)*4*3);

		for(int j = 0; j < 3; j++)
		{
			p[0][j]=m_ob[obj].p[0][j]+(m_ob[obj].p[1][j]-m_ob[obj].p[0][j])*dX1apt;
			p[1][j]=m_ob[obj].p[0][j]+(m_ob[obj].p[1][j]-m_ob[obj].p[0][j])*dX2apt;
			p[2][j]=m_ob[obj].p[3][j]+(m_ob[obj].p[2][j]-m_ob[obj].p[3][j])*dX2apt;
			p[3][j]=m_ob[obj].p[3][j]+(m_ob[obj].p[2][j]-m_ob[obj].p[3][j])*dX1apt;
		}

		// Testing using DrawObjectDegrade
		DrawObjectDegrade(
        m_ob[obj].p[0][1], m_ob[obj].p[0][0], m_ob[obj].p[2][1], m_ob[obj].p[2][0],    // Normal coordinates
        m_ob[obj].v[0][1], m_ob[obj].v[1][1], m_ob[obj].v[0][0], m_ob[obj].v[1][0],   // Texture coordinates
        0.0, 0.0, 0.0, 0.0,                 // Coordinate displacement and rotation
        p[0][1], p[2][1],                   // Dissapear Top and Bottom
        p[0][0], p[2][0],                   // Dissapear Left and Right
        dDegs, 1.0,                          // General degrade length in borders
        false,                              // Inverse
        tex);                                // Object to draw
		if(bDel) delete tex;
	}
DrawXAppear_Fail:
	g_gmMain->PopMatrix();
}

void CMaxScreen::DrawXCont(int obj, double* aef, double dApt, __int64 nTime, int nTra)
{
	// TODO
}

void CMaxScreen::DrawYAppear(int obj, double* aef, double dApt, __int64 nTime, int nTra)
{
	// aef1 -> adv (relative, fit to size of image or maxX-size of text)
	// aef2 -> Advi (relative)
	// aef3 -> Degrade (relative, non efective with text)
	// aef4 -> repetition? No(0) Yes(non-0)
	// aef5 -> Distance between reptetitions (relative for image size or limit text)
	// aef6 -> Vel for disp [rel/sec] (if 0 or neg, judges by Apt, relative by size of display if text, or size)
	// aef7 -> Continue? No(0) Yes(non-0) (Texture-image coordinates must repeat too?) (non aplicated to text)

	/*
	Description of actual function
            x = Advi + Adv*Vel*t (if vel=0 then Adv*t)

	----------------------------
	|                          |
	|                          |
	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx--<-- Degrade
	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx--
	|                          |
	|                          |
	----------------------------


	*/
	g_gmMain->PushMatrix();
	// Same as draw, but color-missing

	// Simple drawing
	TransformTransitionCommon(nTra, dApt);

	double dAdv = ARG(aef, 0);
	double dAdvi = ARG(aef, 1);
	double dDeg = ARG(aef, 2);
	bool bRep = ARG(aef, 3)==0.0?false:true;
	double dRep = ARG(aef, 4);
	double dVel = ARG(aef, 5);
	bool bCon = ARG(aef, 6)==0.0?false:true;

	// Now calculate displacement
	double dDisp = 0.0;
	// ** If it's constant velocity, judge it!
	if(dVel != 0.0) dDisp = dAdvi+dAdv*dVel*dApt*(1.0+dDeg);
	else dDisp = dAdv*dApt*(1.0+dDeg)+dAdvi;

	double dX1apt = clamp(clamp(dDisp,-dDeg,1.0+dDeg),dDisp,dDisp+1.0);
	double dX2apt = clamp(clamp(dDisp+1.0,-dDeg,1.0+dDeg),dDisp,dDisp+1.0);

	// ** If it's text, draw it!
	if(m_ob[obj].type == SCR_OBJTYPE_TEXT)
	{
		int id = m_ob[obj].id[0];
		int sid = m_ob[obj].id[1];
		if(id >= int(m_txt.size())) return;
		if(sid >= int(m_txt[id].size())) return;

		CString* msg = m_txt[id][sid];
		int fnt = m_ift[id][sid];
		DWORD dwFlags = (m_ob[obj].p[3][1]==0.0?DT_CENTER:(m_ob[obj].p[3][1]>=0.0?DT_LEFT:DT_RIGHT))|
			(m_ob[obj].p[3][2]==0.0?DT_VCENTER:(m_ob[obj].p[3][2]>=0.0?DT_TOP:DT_BOTTOM));

		double p[4][3];
		memcpy(p, m_ob[obj].p, sizeof(double)*4*3);

		for(int j = 0; j < 3; j++)
		{
			p[0][j]=m_ob[obj].p[0][j]+(m_ob[obj].p[3][j]-m_ob[obj].p[0][j])*dX1apt;
			p[1][j]=m_ob[obj].p[1][j]+(m_ob[obj].p[2][j]-m_ob[obj].p[1][j])*dX1apt;
			p[2][j]=m_ob[obj].p[1][j]+(m_ob[obj].p[2][j]-m_ob[obj].p[1][j])*dX2apt;
			p[3][j]=m_ob[obj].p[0][j]+(m_ob[obj].p[3][j]-m_ob[obj].p[0][j])*dX2apt;
		}

		m_fnt[fnt]->glDrawTextTol(
			p[0][1], p[0][0],	// Only top-left
			p[2][1], p[2][0],	// Only bottom-right
			p[1][0], p[2][2],	// Up and left alpha-degrade range
			p[1][2], p[1][1],	// Bottom and right alpha-degrade range
			m_ob[obj].v[0][0], m_ob[obj].v[0][1],	// V1 now is basepont
			m_ob[obj].p[0][2],						// Top-left Z
			m_ob[obj].v[1][0], m_ob[obj].v[1][1],	// V2 is Base and X-scale
			1.0,
			(*msg),									// Message
			// Flags set by P4
			dwFlags);
	}
	// ** If it's video or image, get texture and draw it
	else
	{
		bool bDel = false;
		CGraphTexture* tex = GetTex(obj, &bDel);
		if(tex == NULL) goto DrawYAppear_Fail;	// Fail?

        // dDeg is relative, we must find the absolute
        double dDegs = dDeg*fabs(m_ob[obj].p[1][0]-m_ob[obj].p[0][0]);
		// ** Repeating?
		g_gmMain->BindTexture(tex);

		// Calculate positions
		double p[4][3];
		memcpy(p, m_ob[obj].p, sizeof(double)*4*3);

		for(int j = 0; j < 3; j++)
		{
			p[0][j]=m_ob[obj].p[0][j]+(m_ob[obj].p[3][j]-m_ob[obj].p[0][j])*dX1apt;
			p[1][j]=m_ob[obj].p[1][j]+(m_ob[obj].p[2][j]-m_ob[obj].p[1][j])*dX1apt;
			p[2][j]=m_ob[obj].p[1][j]+(m_ob[obj].p[2][j]-m_ob[obj].p[1][j])*dX2apt;
			p[3][j]=m_ob[obj].p[0][j]+(m_ob[obj].p[3][j]-m_ob[obj].p[0][j])*dX2apt;
		}

		// Testing using DrawObjectDegrade
		DrawObjectDegrade(
        m_ob[obj].p[0][1], m_ob[obj].p[0][0], m_ob[obj].p[2][1], m_ob[obj].p[2][0],    // Normal coordinates
        m_ob[obj].v[0][1], m_ob[obj].v[1][1], m_ob[obj].v[0][0], m_ob[obj].v[1][0],   // Texture coordinates
        0.0, 0.0, 0.0, 0.0,                 // Coordinate displacement and rotation
        p[0][1], p[2][1],                   // Dissapear Top and Bottom
        p[0][0], p[2][0],                   // Dissapear Left and Right
        dDegs, 1.0,                          // General degrade length in borders
        false,                              // Inverse
        tex);                                // Object to draw
		if(bDel) delete tex;
	}
DrawYAppear_Fail:
	g_gmMain->PopMatrix();
}

void CMaxScreen::DrawDegAppear(int obj, double* aef, double dApt, __int64 nTime, int nTra)
{
	DrawNull(obj, aef, dApt, nTime, nTra);
}

void CMaxScreen::TransformTransitionCommon(int nTra, double dApt)
{
	if(nTra<0 || nTra >= int(m_tr.size())) return;

	// Translate standard (Third)
	g_gmMain->Translate(
		(m_tr[nTra].tr[3]-m_tr[nTra].tr[0])*dApt+m_tr[nTra].tr[0],
		(m_tr[nTra].tr[4]-m_tr[nTra].tr[1])*dApt+m_tr[nTra].tr[1],
		(m_tr[nTra].tr[5]-m_tr[nTra].tr[2])*dApt+m_tr[nTra].tr[2]);

	// Rotate standard (Second)
	g_gmMain->Translate(m_tr[nTra].rot[0], m_tr[nTra].rot[1], m_tr[nTra].rot[2]);
	g_gmMain->Rotate(
		(m_tr[nTra].rot[7]-m_tr[nTra].rot[6])*dApt+m_tr[nTra].rot[6],
		m_tr[nTra].rot[3],
		m_tr[nTra].rot[4],
		m_tr[nTra].rot[5]);
	g_gmMain->Translate(-m_tr[nTra].rot[0], -m_tr[nTra].rot[1], -m_tr[nTra].rot[2]);

	// Scale standard (First)
	g_gmMain->Scale(
		(m_tr[nTra].sc[3]-m_tr[nTra].sc[0])*dApt+m_tr[nTra].sc[0],
		(m_tr[nTra].sc[4]-m_tr[nTra].sc[1])*dApt+m_tr[nTra].sc[1],
		(m_tr[nTra].sc[5]-m_tr[nTra].sc[2])*dApt+m_tr[nTra].sc[2]);
}
