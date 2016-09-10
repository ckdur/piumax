
#include "MaxScreen.h"
#include "AccReader.h"
#include "basutil.h"
#include "cmnpiu.h"

CMaxScreen::CMaxScreen(void)
{
	Reset();
}

CMaxScreen::~CMaxScreen(void)
{
	Delete();
}

void CMaxScreen::Delete(void)
{
	m_ob.clear();
	m_tr.clear();
	m_ev.clear();
	ClearSpriteRes();
	ClearVideoRes();
	ClearSoundRes();
	ClearStringCat();
	ClearFont();
	Reset();
}

// ** Opens from .acc the info about this screen
int CMaxScreen::OpenFromAcc(LPCTSTR strFile)
{
	CAccReader reader;
	int nDep = 0;

	// ** Set the format
	reader.AddAttrib(_T("OBJECTS"), ACCFORMAT_NORMAL, true);
	reader.AddAttrib(_T("EVENT"), ACCFORMAT_NORMAL, false);
	nDep = reader.m_nSizeAttrib-1;
	reader.AddAttrib(_T("EVENT_TRANSITIONS"), ACCFORMAT_NORMAL, true, nDep);
	reader.AddAttrib(_T("TRANSITIONS"), ACCFORMAT_NORMAL, true);
	reader.AddAttrib(_T("EVENT_SOUNDS"), ACCFORMAT_NORMAL, true, nDep);
	// ** Asegurate format
	if(reader.m_nSizeAttrib != 5) return 0;
	// ** Read the acc
	if(!reader.Read(strFile)) return 0;

	// ** Let's begin with object data

	// ****** OBJECTS *******
	// ** Asegurate good read
	if(reader.m_strData[0].size()<=0) return 0;
	if(reader.m_strData[0][0].size()<=0) return 0;
	// ** Read and format only the first
	for(size_t i = 0; i < reader.m_strData[0][0].size(); i++)
	{
		int nElem = reader.m_strData[0][0][i].size();
		if(nElem < crSCR_OBJECT_TOTAL) continue;	// Insuficent data
		// Declare object
		SCR_OBJECT obj;

		bool bP3 = true;
		bool bP4 = true;

		// Storing data (obligatory)
		for(int j = 0; j < 3; j++)
		{
			// P1 obligatory (up-left)
			if(!tx_double(reader.m_strData[0][0][i][j + crSCR_OBJECT_P1], &obj.p[0][j])) goto fail_reading_object;
			// P2 obligatory (bottom-right)
			if(!tx_double(reader.m_strData[0][0][i][j + crSCR_OBJECT_P2], &obj.p[2][j])) goto fail_reading_object;
		}
		for(int j = 0; j < 2; j++)
		{
			// V1 obligatory (up-left)
			if(!tx_double(reader.m_strData[0][0][i][j + crSCR_OBJECT_V1], &obj.v[0][j])) goto fail_reading_object;
			// V2 obligatory (bottom-right)
			if(!tx_double(reader.m_strData[0][0][i][j + crSCR_OBJECT_V2], &obj.v[1][j])) goto fail_reading_object;
		}
		// ID and SUBID
		if(!tx_uint(reader.m_strData[0][0][i][crSCR_OBJECT_ID], &obj.id[0])) goto fail_reading_object;
		if(!tx_uint(reader.m_strData[0][0][i][crSCR_OBJECT_SUBID], &obj.id[1])) goto fail_reading_object;
		// TYPE
		tx_int_cat(reader.m_strData[0][0][i][crSCR_OBJECT_TYPE], &obj.type, tc_OBJTYPE, SCR_OBJTYPE_SIZE);
		obj.type = clamp(obj.type, 0, SCR_OBJTYPE_SIZE-1);

		// Storing data (optional)
		for(int j = 0; j < 3; j++)
		{
			// P3 optional (up-right)
			if(!tx_double(reader.m_strData[0][0][i][j + crSCR_OBJECT_P3], &obj.p[1][j])) bP3 = false;
			// P4 optional (bottom-left)
			if(!tx_double(reader.m_strData[0][0][i][j + crSCR_OBJECT_P4], &obj.p[3][j])) bP4 = false;
		}
		// Optional data construction
		/*
		Form:
		P1(0)__________________P3(1)
		|                      |
		|                      |
		|                      |
		|                      |
		P4(3)__________________P2(2)
		*/
		if(!bP3 && !bP4)
		{
			// ** Fit P3 to X-Y from P2-P1
			obj.p[1][0] = obj.p[2][0];
			obj.p[1][1] = obj.p[0][1];
			// ** Fit P4 to X-Y from P1-P2
			obj.p[3][0] = obj.p[0][0];
			obj.p[3][1] = obj.p[2][1];
			// ** Make uppercase, no diferences in p# and P#
			reader.m_strData[0][0][i][2 + crSCR_OBJECT_P3].MakeUpper();
			// ** P3 Z-Reference to P1? or P2 (any is P2)?
			if(reader.m_strData[0][0][i][2 + crSCR_OBJECT_P3] == _T("P1"))
			{ obj.p[1][2] = obj.p[0][2]; obj.p[3][2] = obj.p[2][2]; }
			else
			{ obj.p[3][2] = obj.p[0][2]; obj.p[1][2] = obj.p[2][2]; }
		}
		else if(bP3 && !bP4)
		{
			// P3 is already defined, we must use geometry for find P4
			// The result must be a paralelogram in the same plane
			for(int j = 0; j < 3; j++) obj.p[3][j] = obj.p[0][j] + obj.p[2][j] - obj.p[1][j];
		}
		else if(!bP3 && bP4)
		{
			// P4 is already defined, we must use geometry for find P3
			// The result must be a paralelogram in the same plane
			for(int j = 0; j < 3; j++) obj.p[1][j] = obj.p[0][j] + obj.p[2][j] - obj.p[3][j];
		}
		// Clamp values of texture-vector V1 and V2
		for(int j = 0; j < 2; j++) for(int k = 0; k < 2; k++)
			obj.v[j][k] = clamp(obj.v[j][k], 0.0, 1.0);
		// Data sucesfully stored! We include in the stack
		m_ob.push_back(obj);
		// For failed reading
fail_reading_object:;
	}

	// ****** EVENT *******
	// ** Asegurate good read
	if(reader.m_strData[1].size()<=0) return 0;
	for(size_t i = 0; i < reader.m_strData[1].size(); i++)
	{
		SCR_EVENT ev;

		SCR_SOUND sound;
		int smod;
		__int64 sbeg;
		__int64 sdur;

		// Asegurate good read
		if(reader.m_strData[1][i].size()<=0) goto fail_reading_event;
		if(reader.m_strData[1][i][0].size()<crSCR_EVENT_TOTAL) goto fail_reading_event;
		// Storing data (obligatory)
		for(size_t j = 0; j < 2; j++)
		{
			// ID and SubID obligatory
			if(!tx_int(reader.m_strData[1][i][0][j + crSCR_EVENT_ID], &ev.id[j])) goto fail_reading_event;
		}
		// ** Data Stored!
		m_ev.push_back(ev);

		// ******* EVENTS_TRANSITIONS *******
		// ** This data set is dependent of EVENTS statement
		// ** Asegurate good read
		if(reader.m_strData[2].size()<i) goto fail_reading_event_transition;
		for(size_t k = 0; k < reader.m_strData[2][i].size(); k++)
		{
			unsigned int obj;
			vector< unsigned int > trans;
			if(reader.m_strData[2][i][k].size()<crSCR_EVENT_TRANSITION_TOTAL) goto fail_reading_event_transition;
			// Storing data (obligatory)
			if(!tx_uint(reader.m_strData[2][i][k][crSCR_EVENT_TRANSITION_OBJ], &obj)) goto fail_reading_event_transition;
			// Storing data (optional)
			for(size_t j = 0; j < reader.m_strData[2][i][k].size()-crSCR_EVENT_TRANSITION_TRANSITIONS; j++)
			{
				int tr;
				if(!tx_int(reader.m_strData[2][i][k][j + crSCR_EVENT_TRANSITION_TRANSITIONS], &tr)) continue;
				trans.push_back(tr);
			}
			// ** Data Stored!
			m_ev.at(m_ev.size()-1).obj.push_back(obj);
			m_ev.at(m_ev.size()-1).trans.push_back(trans);
			// Time stamp initialization (secure)
			m_ev.at(m_ev.size()-1).time.push_back(0);
		}
fail_reading_event_transition:

		// ******* EVENTS_SOUNDS *******
		// ** This data set is dependent of EVENTS statement
		// ** Asegurate good read
		if(reader.m_strData[4].size()<i) goto fail_reading_event_sound;
		for(size_t j = 0; j < reader.m_strData[4][i].size(); j++)
		{
			if(reader.m_strData[4][i][j].size()<crSCR_EVENT_SOUND_TOTAL) goto fail_reading_event_sound;
			// Storing data (obligatory)
			// Sound
			if(!tx_int(reader.m_strData[4][i][j][crSCR_EVENT_SOUND_SOUND], &sound.id[0])) goto fail_reading_event_sound;
			if(!tx_int(reader.m_strData[4][i][j][crSCR_EVENT_SOUND_SOUND+1], &sound.id[1])) goto fail_reading_event_sound;
			if(!tx_int(reader.m_strData[4][i][j][crSCR_EVENT_SOUND_SOUND+2], &sound.cha)) goto fail_reading_event_sound;
			// Sound Modificator
			tx_int_cat(reader.m_strData[4][i][j][crSCR_EVENT_SOUND_SMOD], &smod, tc_SOUNDSMOD, SCR_SOUNDSMOD_SIZE);
			smod = clamp(smod, 0, SCR_SOUNDSMOD_SIZE-1);
			// Begin
			if(!tx_int64(reader.m_strData[4][i][j][crSCR_EVENT_SOUND_BEG], &sbeg)) goto fail_reading_event_sound;
			// Duration
			if(!tx_int64(reader.m_strData[4][i][j][crSCR_EVENT_SOUND_DUR], &sdur)) goto fail_reading_event_sound;
			// ** Data Stored!
			m_ev.at(m_ev.size()-1).sound.push_back(sound);
			m_ev.at(m_ev.size()-1).smod.push_back(smod);
			m_ev.at(m_ev.size()-1).sdur.push_back(sdur);
			m_ev.at(m_ev.size()-1).sbeg.push_back(sbeg);
		}
fail_reading_event_sound:
fail_reading_event: ;
	}

	// ****** TRANSITIONS *******
	// ** Asegurate good read
	if(reader.m_strData[3].size()<=0) return 0;
	if(reader.m_strData[3][0].size()<=0) return 0;
	// ** Read and format only the first
	for(size_t i = 0; i < reader.m_strData[3][0].size(); i++)
	{
		int nElem = reader.m_strData[3][0][i].size();
		if(nElem < crSCR_TRANSITION_TOTAL) continue;	// Insuficent data
		// Declare transition
		SCR_TRANSITION tr;
		// Storing data (obligatory)
		// Translation
		for(int j = 0; j < 6; j++)
			if(!tx_double(reader.m_strData[3][0][i][j + crSCR_TRANSITION_TR], &tr.tr[j])) goto fail_reading_trans;
		// Rotation
		for(int j = 0; j < 8; j++)
			if(!tx_double(reader.m_strData[3][0][i][j + crSCR_TRANSITION_ROT], &tr.rot[j])) goto fail_reading_trans;
		// Scale
		for(int j = 0; j < 6; j++)
			if(!tx_double(reader.m_strData[3][0][i][j + crSCR_TRANSITION_SC], &tr.sc[j])) goto fail_reading_trans;
		// ID Effect
		tx_int_cat(reader.m_strData[3][0][i][crSCR_TRANSITION_EF], &tr.ef, tc_TRANSEF, SCR_TRANSEF_SIZE);
		tr.ef = clamp(tr.ef, 0, SCR_TRANSEF_SIZE-1);
		// Effect args (ABCD1 & ABCD2)
		for(int j = 0; j < 8; j++)
			if(!tx_double(reader.m_strData[3][0][i][j + crSCR_TRANSITION_EFARG], &tr.aef[j])) goto fail_reading_trans;
		// Duration
		if(!tx_int64(reader.m_strData[3][0][i][crSCR_TRANSITION_DUR], &tr.dur)) goto fail_reading_trans;
		// Modificator
		tx_int_cat(reader.m_strData[3][0][i][crSCR_TRANSITION_MOD], &tr.mod, tc_TRANSMOD, SCR_TRANSMOD_SIZE);
		tr.mod = clamp(tr.mod, 0, SCR_TRANSMOD_SIZE-1);
		// Modificator args
		for(int j = 0; j < 4; j++)
			if(!tx_double(reader.m_strData[3][0][i][j + crSCR_TRANSITION_MODARG], &tr.amod[j])) goto fail_reading_trans;
		// Sprite State
		tx_int_cat(reader.m_strData[3][0][i][crSCR_TRANSITION_SPSTAT], &tr.spstat, tc_TRANSSPSTAT, SCR_TRANSSPSTAT_SIZE);
		tr.spstat = clamp(tr.spstat, 0, SCR_TRANSSPSTAT_SIZE-1);
		// Data sucesfully stored! We include in the stack
		m_tr.push_back(tr);
		// For failed reading
fail_reading_trans: ;
	}

	return 1;
}

// ** Updates the time in the current stack
// ** This automatically updates sprites (if current event permits)
void CMaxScreen::Update(DWORD dwMilliseconds)
{
	if(m_nCurev < 0 || m_nCurev >= (int)(m_ev.size())) return;
	m_nTime += dwMilliseconds;

	//if(m_nTime >= 0) return;	// Crazy things?

	// Now we proceed updating all objects
	for(size_t i = 0; i < m_ev[m_nCurev].obj.size(); i++)
	{
		// ** Update the time stamp guide for each modify object
		m_ev[m_nCurev].time[i] += dwMilliseconds;

		// This is the modifiy objects
		// Only specific-event objects will be updated
		// According about transition data (you know, SCR_TRANSSPSTAT)
		int obj = m_ev[m_nCurev].obj[i];
		__int64 nSprTime = 0;	// Sugest elapsed time for sprite
		__int64 nEvTime = 0;	// Sugest elapsed time for current event object

		// Transition analisys
		int trans = -1;
		int spstat = 0;
		for(size_t j = 0; j < m_ev[m_nCurev].trans[i].size(); j++)
		{
			trans = m_ev[m_nCurev].trans[i][j];
			//trans = j;
			nEvTime += m_tr[trans].dur<0?0:m_tr[trans].dur;
			__int64 nPlusTime = 0;
			if(m_tr[trans].dur<0) nPlusTime = m_ev[m_nCurev].time[i] - nEvTime;
			else if(m_nTime < nEvTime) nPlusTime = m_ev[m_nCurev].time[i] - nEvTime + m_tr[trans].dur;
			else nPlusTime = m_tr[trans].dur;

			spstat = m_tr[trans].spstat;
evaluate_again_spstat:
			switch(spstat)
			{
			case SCR_TRANSSPSTAT_PLAY:
				{
					// Rescue last sprite state
					int spstatb = SCR_TRANSSPSTAT_PLAY;
					for(int k = j-1; k >= 0; k--)
					{
						int transb = m_ev[m_nCurev].trans[i][j];
						if(m_tr[transb].spstat != SCR_TRANSSPSTAT_PLAY)
						{
							spstatb = m_tr[transb].spstat;
							break;
						}
					}
					if(spstatb == SCR_TRANSSPSTAT_PLAY) nSprTime += nPlusTime;
					else
					{
						spstat = spstatb;
						goto evaluate_again_spstat;
					}
				}
				break;
			case SCR_TRANSSPSTAT_PAUSE:
				// Pause doesn't increment Sprite time
				if(m_tr[trans].spstat != SCR_TRANSSPSTAT_PAUSE)
					nSprTime += nPlusTime;
				break;
			case SCR_TRANSSPSTAT_STOP:
				if(m_tr[trans].spstat == SCR_TRANSSPSTAT_STOP)
					nSprTime = 0;
				else nSprTime += nPlusTime;
				break;
			case SCR_TRANSSPSTAT_PLAYLOOP:
				if(m_tr[trans].spstat == SCR_TRANSSPSTAT_PLAYLOOP)
					nSprTime = nPlusTime;
				else nSprTime += nPlusTime;
				break;
			case SCR_TRANSSPSTAT_PLAYONCE:
				if(m_tr[trans].spstat == SCR_TRANSSPSTAT_PLAYONCE)
					nSprTime = nPlusTime;
				else nSprTime += nPlusTime;
				break;
			case SCR_TRANSSPSTAT_PLAYONCEBEG:
				if(m_tr[trans].spstat == SCR_TRANSSPSTAT_PLAYONCEBEG)
					nSprTime = nPlusTime;
				else nSprTime += nPlusTime;
				break;
			}
			if(m_tr[trans].dur<0 || m_ev[m_nCurev].time[i] < nEvTime)
				break;
		}
		if(trans == -1) trans = m_ev[m_nCurev].trans[i][m_ev[m_nCurev].trans[i].size()-1];	// Last.. IDIOT!

		// ** Now we set current object at ID and subID about resources
		unsigned int id[2];
		int nType = 0;
		id[0] = m_ob[obj].id[0]; id[1] = m_ob[obj].id[1]; nType = m_ob[obj].type;

		int dur = 0;
		int fra = 0;
		if(nType == SCR_OBJTYPE_IMAGE)
		{
			// Get CSpriteRes
			CSpriteRes* res = m_ires[id[0]];

			// Get Sprite's duration and total frames
			dur = res->At((int)id[1])->m_dwDuration;
			fra = res->At((int)id[1])->m_nSize;

			// Now truncate sprite time accord cases
			// spstat returns last-stable sprite state
			switch(spstat)
			{
			case SCR_TRANSSPSTAT_PLAY:	// Unestable
				// It's possible get this value
				// If the theme-maker put this values to all transitions
				// in the event, so, we'll reproduce according Sprite settings
				nSprTime = dur<=0?0:(nSprTime%dur);
				break;
			case SCR_TRANSSPSTAT_PAUSE:
				nSprTime = dur<=0?0:(nSprTime%dur);	// Increment was done before
				break;
			case SCR_TRANSSPSTAT_STOP:
				nSprTime = 0;	// Must be this value
				break;
			case SCR_TRANSSPSTAT_PLAYLOOP:
				nSprTime = dur<=0?0:(nSprTime%dur);
				break;
			case SCR_TRANSSPSTAT_PLAYONCE:
				nSprTime = nSprTime>=dur?((fra-1)*dur/fra):(nSprTime);
				break;
			case SCR_TRANSSPSTAT_PLAYONCEBEG:
				nSprTime = nSprTime>=dur?0:(nSprTime);
				break;
			}

			// Set the time, and update shared resource
			res->At((int)id[1])->m_dwNext = (DWORD) nSprTime;
			res->At((int)id[1])->Update(0);
		}
		if(nType == SCR_OBJTYPE_VIDEO)
		{
			// Get CSpriteRes
			CVideoRes* res = m_vres[id[0]];

			// Now truncate sprite time accord cases
			// spstat returns last-stable sprite state
			switch(spstat)
			{
			case SCR_TRANSSPSTAT_PLAY:	// Unestable
				// Continue reproducing
				if((*(*res).At((int)id[1])).dwNext < nSprTime)
					(*(*res).At((int)id[1])).Update(dwMilliseconds);
				break;
			case SCR_TRANSSPSTAT_PAUSE:
				if((*(*res).At((int)id[1])).dwNext < nSprTime)
					(*(*res).At((int)id[1])).Update((DWORD)(nSprTime)-(*(*res).At((int)id[1])).dwNext);
				break;
			case SCR_TRANSSPSTAT_STOP:
				if((*(*res).At((int)id[1])).dwNext != 0)
					(*(*res).At((int)id[1])).SetZeroCount();
				break;
			case SCR_TRANSSPSTAT_PLAYLOOP:
				(*(*res).At((int)id[1])).bLoop = true;
				if((*(*res).At((int)id[1])).dwNext >= nSprTime)
				{
					(*(*res).At((int)id[1])).SetZeroCount();
					(*(*res).At((int)id[1])).dwNext=(DWORD)(nSprTime);
					(*(*res).At((int)id[1])).Update(0);
				}
				else (*(*res).At((int)id[1])).Update(dwMilliseconds);
				break;
			case SCR_TRANSSPSTAT_PLAYONCE:
				/*(*(*res).At((int)id[1])).bLoop = false;
				if((*(*res).At((int)id[1])).dwNext >= nSprTime)
				{
					(*(*res).At((int)id[1])).SetZeroCount();
					(*(*res).At((int)id[1])).dwNext=(DWORD)(nSprTime);
					(*(*res).At((int)id[1])).Update(0);
				}
				else (*(*res).At((int)id[1])).Update(dwMilliseconds);
				break;*/
			case SCR_TRANSSPSTAT_PLAYONCEBEG:
				(*(*res).At((int)id[1])).bLoop = false;
				if((*(*res).At((int)id[1])).dwNext >= nSprTime)
				{
					(*(*res).At((int)id[1])).SetZeroCount();
					(*(*res).At((int)id[1])).dwNext=(DWORD)(nSprTime);
					(*(*res).At((int)id[1])).Update(0);
				}
				else (*(*res).At((int)id[1])).Update(dwMilliseconds);
				break;
			}

			// Get Sprite's duration and total frames
			dur = (int)(*(*res).At((int)id[1])).dwDur;
		}


		// Now... jijiji... set secure timestamp for each element-object
		//m_ev[m_nCurev].time[i] = nSprTime;
	}
}

// ** Sets an event in the stack if this is found
// ** If found, resets time and the transitions will be
// ** played acoording to the event-saved transitions
int CMaxScreen::SetEvent(MID id)
{
	// ** Begin to search for ID and SUBID
	for(size_t i = 0; i < m_ev.size(); i++) if(m_ev[i].id[0] == id.nId && m_ev[i].id[1] == id.nSubId)
	{
		return SetEventIndex(i);
	}
	return 0;	// No event reached
}

// ** Forces a set of event acoording to the index
// ** only use forced if you don't use ID's
int CMaxScreen::SetEventIndex(int nIndex)
{
	if(nIndex < 0 || nIndex >= ((int)m_ev.size())) return 0;
	// Event reached! Set default times and set current event to Screen.

	// ** SECURE SHARED STOP OF CHANNELS
	if (m_nCurev < 0 || m_nCurev >= ((int)m_ev.size()) || m_sts.size() != m_ev[m_nCurev].sound.size()) goto finish_secure_sound;
	for(size_t k = 0; k < m_sts.size(); k++)
	{
		if(m_sts[k]==1)
		{
			bool bIs = false;
			(*channels[m_ev[m_nCurev].sound[k].cha]).IsPlaying(&bIs);
			if(!bIs) continue;	// Already stopped

			//CSoundStream stream, *stream2;
			int id = m_ev[m_nCurev].sound[k].id[0];
			int sid = m_ev[m_nCurev].sound[k].id[1];

			if(id >= int(m_sres.size())) continue;
			//stream2 = m_sres[id]->At(sid);
			//if((stream2 = m_sres[id]->At(sid)) == NULL) continue;

			//(*channels[m_ev[m_nCurev].sound[k].cha]).GetCurrentSound(&stream);
			//if(stream != (*stream2)) continue;
			(*channels[m_ev[m_nCurev].sound[k].cha]).Stop();
		}
	}
finish_secure_sound:
	// ** ////// ////// ///////// // ////////
	int i = nIndex;
	m_nTime = 0;
	m_nCurev = i;
	m_sts.clear();
	m_sts.resize(m_ev[m_nCurev].sound.size(), 0);
	// Look for the objects
	for(size_t j = 0; j < m_ev[i].trans.size(); j++)
	{
		// Look for first transition (for define first timestamp)
		if(m_ev[i].trans[j].size() > 0)
		{
			// Transition reached! Please, look if this has a sprite
			// state for playing or pause. Any other can reset times to default
			int trans = m_ev[i].trans[j][0];
			switch(m_tr[trans].spstat)
			{
			case SCR_TRANSSPSTAT_PLAY:	// Unestable (at begin?)
				// This value means continue as before. Must not change timestamp
			case SCR_TRANSSPSTAT_PAUSE:
				// This value means pause it. Must not change timestamp
				break;
			case SCR_TRANSSPSTAT_STOP:
				// Stop reproduction!
			case SCR_TRANSSPSTAT_PLAYLOOP:
				// Set it to begin and play looping
			case SCR_TRANSSPSTAT_PLAYONCE:
				// Play once
			case SCR_TRANSSPSTAT_PLAYONCEBEG:
				// Play once too
				m_ev[i].time[j] = 0;
				break;
			}
		}
	}
	return 1;
}

int CMaxScreen::Reset(void)
{
	m_nTime = 0;
	m_nCurev = -1;

	// Set all time-stamps to zero
	for(size_t i = 0; i < m_ev.size(); i++)
		for(size_t j = 0; j < m_ev[i].time.size(); j++)
			m_ev[i].time[j] = 0;
	return 0;
}
