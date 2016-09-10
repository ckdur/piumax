#include "main_view.h"
#include "main_window.h"
#include "SetSizeDlg.h"
#include "StepOptionsDlg.h"
#include "BeatSplitDlg.h"
#include "ChangeBPMDlg.h"
#include "ChangeVelDlg.h"
#include "TickCountDlg.h"
#include "IncrStepDlg.h"
#include "TypeNoteDlg.h"
#include "ComandOptionsDlg.h"
#include "MaxSMConvChoose.h"
#include "defbas.h"
#include "basutil.h"
#include "libc_max_common.h"

CMaxStepSmu* g_stepsmu = NULL;

CMaxDocument* CMaxView::GetDocument(void)
{
	return g_mdDocument.at( m_uDoc );
}

CMaxView::CMaxView()
{
	memset(&arg, 0, sizeof(ARGSTEP));
	arg.fUserVel = 1.0f;
	arg.fRush = 1.0f;
	arg.fDecrBar = 0.07f;
	arg.fIncrBar = 0.005f;

	fVel = arg.fUserVel;
	m_nEdit = 1;
	m_bPressed = false;
	m_bPreviewDraw = false;
	m_bRestoreDraw = false;
	m_xa = 0;
	m_ya = 0;
	m_cra = NULL;
	m_sra = NULL;
	m_bSelDraw = false;
	m_bDelDraw = false;
	m_bArrowPreDraw = false;
	m_bDrawing = false;
	m_bSelected = false;
	m_bFull = false;

	m_xsel = 0;
	m_ysel = 0;
	m_wsel = 0;
	m_hsel = 0;
	m_nBackX = 0;
	m_nBackY = 0;
	m_nGroup = 0;
	m_nFamily = 0;
	m_bRestoreDrawMouse = false;
	m_nAddType = 1;
	m_nAddPar = 1;
	m_bInitialDraw = false;
	m_bModPreDraw = false;

	m_nRX = 0;
	m_nRY = 0;
	m_nRGroup = 0;
	m_nRFamily = 0;
	m_bRPressed = false;

	m_nTest = 0;

	m_strMusic = _T("");
	m_strSplash = _T("");
	m_strBGA = _T("");
	m_strFile = _T("");
	m_strLights = _T("");
}

CMaxView::~CMaxView()
{
}

CMaxView* mainview_allocator(void)
{
    return (new CMaxView());
}

void CMaxView::Init(void)
{
	CMaxDocument* pDoc = GetDocument();
	// Look the status for sm if there is a sm
	if(pDoc->m_nStatusSM == MAX_SM_FILED || pDoc->m_nStatusSM == MAX_SMA_FILED)
	{
		CStringArray modes;
		for(unsigned int i= 0; i < pDoc->m_sm.GetNumValues(); i++)
		{
			unsigned int iNumParams = pDoc->m_sm.GetNumParams(i);
			if(iNumParams <= 0) continue;	// There is no params
			const MsdFile::value_t &sParams = pDoc->m_sm.GetValue(i);

			const CString sValueName = sParams[0];

			if(stricmp(sValueName, "NOTES")==0)
			{
				if( iNumParams < 7 )
				{
					printf("In file, there is no enough fields on #NOTES tag.");
					continue;
				}

				CString sStepsType = sParams[1];
				CString sDescription = sParams[2];
				CString sDifficulty = sParams[3];
				CString sMeter = sParams[4];

				TrimLeft(sStepsType);
				TrimRight(sStepsType);
				TrimLeft(sDescription);
				TrimRight(sDescription);
				TrimLeft(sDifficulty);
				TrimRight(sDifficulty);

				int nDificuilty = atoi(sMeter);
				sMeter.Format("%d", nDificuilty);

				CString str;
				str += sStepsType + " | " + sDescription + " | "  + sDifficulty + " (" + sMeter + ")";
				modes.push_back(str);
			}
		}

		if(modes.empty())
		{
			message_box( GTK_DIALOG_MODAL,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "There is no note data on this SM. Loaded defaults");
		}
		else
		{
			CMaxSMConvChoose* dlg = new CMaxSMConvChoose();
			dlg->m_vnGroups = modes;
			if(dlg->DoModal() == GTK_RESPONSE_ACCEPT)
			{
				// NOTE: SM support only if this is a plus (stops in beats)
				if(pDoc->m_nStatusSM == MAX_SM_FILED)
					pDoc->Convert_SM_SKSFv2(&pDoc->song, &pDoc->m_sm, modes[dlg->m_nSel], false);
				else if(pDoc->m_nStatusSM == MAX_SMA_FILED)
					pDoc->Convert_SMA_SKSFv2(&pDoc->song, &pDoc->m_sm, modes[dlg->m_nSel]);
			}
			else
			{
				message_box( GTK_DIALOG_MODAL,
                            GTK_MESSAGE_WARNING,
                            GTK_BUTTONS_CLOSE,
                            "No selected steps. Loaded defaults");
			}
			delete dlg;
		}

		pDoc->m_nStatusSM = MAX_SM_LOADED;
		pDoc->Fix();
		pDoc->InitAux();
		pDoc->Actualizate();
	}

	// Initial view
	m_vnGroupDisp.push_back(0);
	m_vnModDisp.push_back(0);
	m_vnModDisp.push_back(1);
	m_vnModDisp.push_back(4);
	m_vnModDisp.push_back(5);

	gtk_widget_set_app_paintable(m_gtkDrawingArea, TRUE);
	ActualizateScrolls();
	OnActualizate();

	while (gtk_events_pending ())
	  gtk_main_iteration ();
}

void CMaxView::ActualizateScrolls(void)
{
	CMaxDocument* pDoc = GetDocument();
	if (!pDoc)
		return;

	int nSizeGroup = m_vnGroupDisp.size();
	int nSizeMod = m_vnModDisp.size();
	int nLimit = 0, nMaxLimit = 0;

	for(int k = 0; k < int(m_vnGroupDisp.size()); k++)
	{
		nMaxLimit = (int)(MAXEDIT_VIEW_PIXELS_PER_BEAT*fVel*
			pDoc->hGroupAux[m_vnGroupDisp[k]].fLimitArrows);
		if(nLimit < nMaxLimit) nLimit = nMaxLimit;
	}

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;

	int nXR = nDistBetGroups*nSizeGroup+MAXEDIT_VIEW_PIXELS_PADDING
				+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_XFREE*nSizeMod;

	gtk_widget_set_size_request(
		m_gtkDrawingArea,
		nXR+MAXEDIT_VIEW_MOD_XFREE,
		nLimit+MAXEDIT_VIEW_PIXELS_YFREE);
}

void CMaxView::RestoreDraw(void)
{
	if(m_sra != NULL && m_cra != NULL)
	{
		gint width = cairo_image_surface_get_width(m_sra);
		gint height = cairo_image_surface_get_height(m_sra);

		m_bRestoreDraw = true;

		gtk_widget_queue_draw_area(m_gtkDrawingArea, m_xa, m_ya, width, height);
		while (gtk_events_pending ())
			gtk_main_iteration ();
		m_nTest = 0;
	}
}

void CMaxView::SaveDraw(int x, int y, int width, int height)
{
	if(m_cra != NULL && m_sra != NULL)
	{
		cairo_destroy(m_cra);
		cairo_surface_destroy(m_sra);
		m_cra = NULL;
		m_sra = NULL;
	}

	m_xa = x;
	m_ya = y;

	m_sra = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
	m_cra = cairo_create (m_sra);

	GdkPixbuf* pb = gdk_pixbuf_get_from_drawable(NULL,
		GDK_DRAWABLE(m_gtkDrawingArea->window), NULL,
		x, y, 0, 0, width, height);

	gdk_cairo_set_source_pixbuf(m_cra, pb, 0, 0);

	cairo_rectangle (m_cra, 0, 0, width, height);
	cairo_fill (m_cra);

	m_nTest++;
}

void CMaxView::SelDraw(int x, int y, int width, int height)
{
	m_xsel = x;
	m_ysel = y;
	m_wsel = width;
	m_hsel = height;
	m_bSelDraw = true;
	gtk_widget_queue_draw_area(m_gtkDrawingArea, x, y, width, height);
	while (gtk_events_pending ())
	  gtk_main_iteration ();
}

void CMaxView::DelDraw(int x, int y, int width, int height)
{
	m_xsel = x;
	m_ysel = y;
	m_wsel = width;
	m_hsel = height;
	m_bDelDraw = true;
	gtk_widget_queue_draw_area(m_gtkDrawingArea, x, y, width, height);
	while (gtk_events_pending ())
	  gtk_main_iteration ();
}

void CMaxView::ArrowPreDraw(int nX, int nY)
{
	m_nX = nX;
	m_nY = nY;
	int nMinX = min(m_nX, m_nBackX),
		nMinY = min(m_nY, m_nBackY),
		nMaxX = max(m_nX, m_nBackX),
		nMaxY = max(m_nY, m_nBackY);
	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;
	float fBeat = Arrow2Beat(nMaxY, m_vnGroupDisp.at(m_nGroup));
	float fy = fBeat*fVel;
	float fBackBeat = Arrow2Beat(nMinY, m_vnGroupDisp.at(m_nGroup));
	float fBacky = fBackBeat*fVel;
	int clipX, clipY, clipW, clipH;
	clipX = nMinX*MAXEDIT_VIEW_PIXELS_WIDTH+MAXEDIT_VIEW_PIXELS_PADDING+nDistBetGroups*m_nGroup;
	clipY = int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT);
	clipW = ((nMaxX-nMinX)+1)*MAXEDIT_VIEW_PIXELS_WIDTH;
	clipH = int((fy-fBacky)*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT;
	m_bArrowPreDraw = true;
	SaveDraw(clipX, clipY, clipW, clipH);
	gtk_widget_queue_draw_area(m_gtkDrawingArea, clipX, clipY, clipW, clipH);
	while (gtk_events_pending ())
	  gtk_main_iteration ();
}

void CMaxView::ModPreDraw(int nY, int nRefMod)
{
	m_nX = 0;
	m_nY = nY;
	int nMinY = min(m_nY, m_nBackY),
		nMaxY = max(m_nY, m_nBackY);
	int nMod = m_vnModDisp.at(nRefMod);
	int nExt = GetModExt(nMod);

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;

	float fBeat = Arrow2Beat(nMaxY, 0);
	float fy = fBeat*fVel;
	float fBackBeat = Arrow2Beat(nExt!=0?nMinY:m_nY, 0);
	float fBacky = fBackBeat*fVel;

	int clipX, clipY, clipW, clipH;
	clipX = 0;
	clipY = int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT);
	clipW = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod+MAXEDIT_VIEW_MOD_WIDTH;

	if(nExt == 0)
		clipH = MAXEDIT_VIEW_PIXELS_PER_BEAT;
	else
		clipH = int((fy-fBacky)*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT;
	m_bModPreDraw = true;
	SaveDraw(clipX, clipY, clipW, clipH);
	gtk_widget_queue_draw_area(m_gtkDrawingArea, clipX, clipY, clipW, clipH);
	while (gtk_events_pending ())
	  gtk_main_iteration ();
}

void CMaxView::ArrowPreDrawExpose(cairo_t* cr, int nX, int nY, int nBackX, int nBackY, int nRefGroup)
{
	int nGroup = m_vnGroupDisp.at(nRefGroup);
	int nMinX = min(nX, nBackX),
		nMinY = min(nY, nBackY),
		nMaxX = max(nX, nBackX),
		nMaxY = max(nY, nBackY);

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;

	cairo_save(cr);

	// Translate accord to current group
	cairo_translate(cr, nDistBetGroups*nRefGroup+MAXEDIT_VIEW_PIXELS_PADDING+0.5, 0.5);

	for(int y = nMinY; y <= nMaxY; y++)
	{
		float fBeat = Arrow2Beat(y, nGroup);
		float fBeatNext = Arrow2Beat(y+1, nGroup);
		float fy = fBeat*fVel;
		float fny = fBeatNext*fVel;
		for(int x = nMinX; x <= nMaxX; x++)
		{
			int nArrow = 0;
			if(m_nEdit == 1)
			{
				nArrow = m_nAddType;
			}
			else if(m_nEdit == 0)
			{
				nArrow = 3;
				if(y == nMinY) nArrow = 2;
				if(y == nMaxY) nArrow = 4;
			}

			// Draw Normal
			DrawArrow(cr, x, fy, fny, nGroup, nArrow);
		}
	}
	cairo_restore(cr);
}

void CMaxView::ModPreDrawExpose(cairo_t* cr, int nY, int nBackY, int nRefGroup)
{
	int nMod = m_vnModDisp.at(nRefGroup);
	int nMinY = min(nY, nBackY),
		nMaxY = max(nY, nBackY);

	cairo_save(cr);

	switch(nMod)
	{
	case MAXSTEP_BEATSPLIT:
		// Draw change
		DrawMod(cr, "BS:", &nY, &nY, 1, nRefGroup);
		break;
	case MAXSTEP_CHANGEBPM:
		// Draw changes
		DrawMod(cr, "BPM:", &nY, &nY, 1, nRefGroup);
		break;
	case MAXSTEP_CHANGEVEL:
		// Draw changes
		DrawModExt(cr, "IVel:", "EVel", &nMinY, &nMaxY, &nMinY, &nMaxY, 1, nRefGroup);
		break;
	case MAXSTEP_JUMP:
		// Draw changes
		DrawModExt(cr, "Jump", "End Jump", &nMinY, &nMaxY, &nMinY, &nMaxY, 1, nRefGroup);
		break;
	case MAXSTEP_TICKCOUNT:
		// Draw changes
		DrawMod(cr, "TC:", &nY, &nY, 1, nRefGroup);
		break;
	case MAXSTEP_STOP:
		// Draw changes
		DrawModExt(cr, "Stop", "End Stop", &nMinY, &nMaxY, &nMinY, &nMaxY, 1, nRefGroup);
		break;
	default:
		break;
	}

	cairo_restore(cr);
}

void CMaxView::DrawArrow(cairo_t* cr, int x, float fy, float fny, int nGroup, int nArrow)
{
	if(nArrow == 1)
		DrawImageFromIM(cr, x%5+MAXEDIT_IM_BASE_TAP,
			x*MAXEDIT_VIEW_PIXELS_WIDTH,
			int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT), MAXEDIT_VIEW_PIXELS_WIDTH,
			MAXEDIT_VIEW_PIXELS_PER_BEAT);
	if(nArrow == 2)
	{
		int nY = int(MAXEDIT_VIEW_PIXELS_PER_BEAT*(fny-fy));
		if(nY > 0)
		DrawImageFromIM(cr, x%5+MAXEDIT_IM_BASE_LONG,
			x*MAXEDIT_VIEW_PIXELS_WIDTH,
			int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT/2,
			MAXEDIT_VIEW_PIXELS_WIDTH,
			nY);
		DrawImageFromIM(cr, x%5+MAXEDIT_IM_BASE_TAP,
			x*MAXEDIT_VIEW_PIXELS_WIDTH,
			int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT), MAXEDIT_VIEW_PIXELS_WIDTH,
			MAXEDIT_VIEW_PIXELS_PER_BEAT);
	}
	if(nArrow == 3)
	{
		int nY = int(MAXEDIT_VIEW_PIXELS_PER_BEAT*(fny-fy));
		if(nY > 0)
			DrawImageFromIM(cr, x%5+MAXEDIT_IM_BASE_LONG,
				x*MAXEDIT_VIEW_PIXELS_WIDTH,
				int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT), MAXEDIT_VIEW_PIXELS_WIDTH,
				int(MAXEDIT_VIEW_PIXELS_PER_BEAT*(fny-fy)));
	}
	if(nArrow == 4)
		DrawImageFromIM(cr, x%5+MAXEDIT_IM_BASE_LONG_BOTTOM,
			x*MAXEDIT_VIEW_PIXELS_WIDTH,
			int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT), MAXEDIT_VIEW_PIXELS_WIDTH,
			MAXEDIT_VIEW_PIXELS_PER_BEAT);
	if(nArrow >= 5)
		DrawImageFromIM(cr, nArrow-5+MAXEDIT_IM_BASE_ITEMS,
			x*MAXEDIT_VIEW_PIXELS_WIDTH,
			int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT), MAXEDIT_VIEW_PIXELS_WIDTH,
			MAXEDIT_VIEW_PIXELS_PER_BEAT);
}

void CMaxView::DrawSelection(cairo_t* cr, int nX, int nY, int nBackX, int nBackY, int nGroup, int nFamily)
{
	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;
	if(nFamily == 1)
	{
		int nMinX = min(nX, nBackX),
			nMinY = min(nY, nBackY),
			nMaxX = max(nX, nBackX),
			nMaxY = max(nY, nBackY);
		float fBeat = Arrow2Beat(nMaxY, m_vnGroupDisp.at(nGroup));
		float fy = fBeat*fVel;
		float fBackBeat = Arrow2Beat(nMinY, m_vnGroupDisp.at(nGroup));
		float fBacky = fBackBeat*fVel;
		int clipX, clipY, clipW, clipH;
		clipX = nMinX*MAXEDIT_VIEW_PIXELS_WIDTH+MAXEDIT_VIEW_PIXELS_PADDING+nDistBetGroups*nGroup;
		clipY = int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT);
		clipW = ((nMaxX-nMinX)+1)*MAXEDIT_VIEW_PIXELS_WIDTH;
		clipH = int((fy-fBacky)*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT;

		cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
		cairo_rectangle(cr, double(clipX)+0.5, double(clipY)+0.5, double(clipW), double(clipH));
		cairo_fill(cr);
	}
	if(nFamily == 2)
	{
		int nMinY = min(nY, nBackY),
			nMaxY = max(nY, nBackY);
		float fBeat = Arrow2Beat(nMaxY, m_vnModDisp.at(nGroup));
		float fy = fBeat*fVel;
		float fBackBeat = Arrow2Beat(nMinY, m_vnModDisp.at(nGroup));
		float fBacky = fBackBeat*fVel;
		int clipX, clipY, clipW, clipH;
		clipX = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*m_nGroup;
		clipY = int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT),
		clipW = MAXEDIT_VIEW_MOD_WIDTH;
		clipH = int((fy-fBacky)*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT;

		cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
		cairo_rectangle(cr, double(clipX)+0.5, double(clipY)+0.5, double(clipW), double(clipH));
		cairo_fill(cr);
	}
}

void CMaxView::GetSelectionCoords(int nX, int nY, int nBackX, int nBackY, int nGroup, int nFamily, int *x, int *y, int *w, int *h)
{
	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;
	if(nFamily == 1)
	{
		int nMinX = min(nX, nBackX),
			nMinY = min(nY, nBackY),
			nMaxX = max(nX, nBackX),
			nMaxY = max(nY, nBackY);
		float fBeat = Arrow2Beat(nMaxY, m_vnGroupDisp.at(nGroup));
		float fy = fBeat*fVel;
		float fBackBeat = Arrow2Beat(nMinY, m_vnGroupDisp.at(nGroup));
		float fBacky = fBackBeat*fVel;
		(*x) = nMinX*MAXEDIT_VIEW_PIXELS_WIDTH+MAXEDIT_VIEW_PIXELS_PADDING+nDistBetGroups*nGroup;
		(*y) = int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT);
		(*w) = ((nMaxX-nMinX)+1)*MAXEDIT_VIEW_PIXELS_WIDTH;
		(*h) = int((fy-fBacky)*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT;
	}
	if(nFamily == 2)
	{
		int nMinY = min(nY, nBackY),
			nMaxY = max(nY, nBackY);
		float fBeat = Arrow2Beat(nMaxY, m_vnModDisp.at(nGroup));
		float fy = fBeat*fVel;
		float fBackBeat = Arrow2Beat(nMinY, m_vnModDisp.at(nGroup));
		float fBacky = fBackBeat*fVel;
		(*x) = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*m_nGroup;
		(*y) = int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT),
		(*w) = MAXEDIT_VIEW_MOD_WIDTH;
		(*h) = int((fy-fBacky)*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT;
	}
}

void CMaxView::RestoreDrawExpose(cairo_t* cr)
{
	if(m_cra != NULL && m_sra != NULL)
	{
		cairo_set_source_surface(cr, m_sra, m_xa, m_ya);
		cairo_paint(cr);
	}
}

void CMaxView::SelDrawExpose(cairo_t* cr)
{
	cairo_set_source_rgba(cr, 0.5, 0.5, 0.5, 0.5);
	cairo_rectangle(cr, double(m_xsel)+0.5, double(m_ysel)+0.5, double(m_wsel), double(m_hsel));
	cairo_fill(cr);
}

void CMaxView::DelDrawExpose(cairo_t* cr)
{
	cairo_set_source_rgba(cr, 1, 1, 1, 0.85);
	cairo_rectangle(cr, double(m_xsel)+0.5, double(m_ysel)+0.5, double(m_wsel), double(m_hsel));
	cairo_fill(cr);
}

void CMaxView::DrawMod(cairo_t* cr, LPCSTR cFormat, int* nPlace, int* data, int nSize, int nRefMod)
{
	if(nSize<0) return;
	if(nPlace==NULL) return;

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;

	static const double dashmod[] = {2.0};
	static cairo_pattern_t* penmod = cairo_pattern_create_rgba(0.8, 0.8, 0.0, 1.0);
	double penmod_width = 1;
	cairo_select_font_face (cr, "Arial",
		CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_font_face_t* font;
	font = cairo_get_font_face(cr);

	cairo_set_source(cr, penmod);
	cairo_set_line_width(cr, penmod_width);

	for(int k = 0; k < nSize; k++)
	{
		int nW = nPlace[k];
		int nData = data[k];

		// Doesn't matter the group
		float fBeat = Arrow2Beat(nW, 0);
		float fy = fBeat*fVel;

		int x = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod,
			y = 0, sx = MAXEDIT_VIEW_MOD_WIDTH, sy = MAXEDIT_VIEW_PIXELS_PER_BEAT;

		CStdStringA str;
		str.Format(cFormat, nData);

		// ************* Drawing Dashed line

		cairo_save(cr);

		cairo_translate(cr,
				0.5, int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT)+0.5);

		cairo_set_dash(cr, dashmod, 1, 0.0);
		cairo_move_to(cr, MAXEDIT_VIEW_PIXELS_PADDING, 0);
		cairo_line_to(cr, x+sx, 0);

		cairo_stroke(cr);

		cairo_set_dash(cr, NULL, 0, 0.0);


		// ********** Drawing text in clip rectangle
		cairo_save(cr);

		cairo_rectangle(cr, x, y, sx, sy);

		cairo_clip (cr);
		cairo_new_path (cr);

		cairo_move_to(cr, x, MAXEDIT_SHOW_MOD_YPADDING);

		cairo_set_font_face(cr, font);
		cairo_set_font_size(cr, MAXEDIT_SHOW_MOD_SIZE);
		cairo_show_text(cr, str);

		cairo_restore(cr);
		// ********** Clip text

		cairo_restore(cr);
	}
}

void CMaxView::DrawMod(cairo_t* cr, LPCSTR cFormat, int* nPlace, float* data, int nSize, int nRefMod)
{
	if(nSize<0) return;
	if(nPlace==NULL) return;

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;

	static const double dashmod[] = {2.0};
	static cairo_pattern_t* penmod = cairo_pattern_create_rgba(0.8, 0.8, 0.0, 1.0);
	double penmod_width = 1;
	cairo_select_font_face (cr, "Arial",
		CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_font_face_t* font;
	font = cairo_get_font_face(cr);

	cairo_set_source(cr, penmod);
	cairo_set_line_width(cr, penmod_width);

	for(int k = 0; k < nSize; k++)
	{
		int nW = nPlace[k];
		float nData = data[k];

		// Doesn't matter the group
		float fBeat = Arrow2Beat(nW, 0);
		float fy = fBeat*fVel;

		int x = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod,
			y = 0, sx = MAXEDIT_VIEW_MOD_WIDTH, sy = MAXEDIT_VIEW_PIXELS_PER_BEAT;

		CStdStringA str;
		str.Format(cFormat, nData);

		// ************* Drawing Dashed line

		cairo_save(cr);

		cairo_translate(cr,
				0.5, int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT)+0.5);

		cairo_set_dash(cr, dashmod, 1, 0.0);
		cairo_move_to(cr, MAXEDIT_VIEW_PIXELS_PADDING, 0);
		cairo_line_to(cr, x+sx, 0);

		cairo_stroke(cr);

		cairo_set_dash(cr, NULL, 0, 0.0);


		// ********** Drawing text in clip rectangle
		cairo_save(cr);

		cairo_rectangle(cr, x, y, sx, sy);

		cairo_clip (cr);
		cairo_new_path (cr);

		cairo_move_to(cr, x, MAXEDIT_SHOW_MOD_YPADDING);

		cairo_set_font_face(cr, font);
		cairo_set_font_size(cr, MAXEDIT_SHOW_MOD_SIZE);
		cairo_show_text(cr, str);

		cairo_restore(cr);
		// ********** Clip text

		cairo_restore(cr);
	}
}

void CMaxView::DrawModExt(cairo_t* cr, LPCSTR cIFormat, LPCSTR cEFormat, int* nI, int* nE, int* Idata, int* Edata, int nSize, int nRefMod)
{
	if(nSize<0) return;
	if(nI==NULL||nE==NULL) return;

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;

	static const double dashmod[] = {2.0};
	static cairo_pattern_t* penmod = cairo_pattern_create_rgba(0.8, 0.8, 0.0, 1.0);
	double penmod_width = 1;
	cairo_select_font_face (cr, "Arial",
		CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_font_face_t* font;
	font = cairo_get_font_face(cr);

	cairo_set_source(cr, penmod);
	cairo_set_line_width(cr, penmod_width);

	for(int k = 0; k < nSize; k++)
	{
		int i = nI[k];
		int e = nE[k];
		int nIData = Idata[k];
		int nEData = Edata[k];

		// Doesn't matter the group
		float fBeat = Arrow2Beat(e, 0);
		float fy = fBeat*fVel;
		float fBackBeat = Arrow2Beat(i, 0);
		float fBacky = fBackBeat*fVel;
		int x, y, sx, sy;
		CStdStringA strI;
		strI.Format(cIFormat, nIData);
		CStdStringA strE;
		strE.Format(cEFormat, nEData);

		// ************* Drawing Dashed line (1/2)

		x = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod;
		y = 0; sx = MAXEDIT_VIEW_MOD_WIDTH; sy = MAXEDIT_VIEW_PIXELS_PER_BEAT;

		cairo_save(cr);

		cairo_translate(cr,
				0.5, int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT)+0.5);

		cairo_set_dash(cr, dashmod, 1, 0.0);
		cairo_move_to(cr, MAXEDIT_VIEW_PIXELS_PADDING, 0);
		cairo_line_to(cr, x+sx, 0);

		cairo_stroke(cr);

		cairo_set_dash(cr, NULL, 0, 0.0);

		// ********** Drawing text in clip rectangle (1/2)
		cairo_save(cr);

		cairo_rectangle(cr, x, y, sx, sy);

		cairo_clip (cr);
		cairo_new_path (cr);

		cairo_move_to(cr, x, MAXEDIT_SHOW_MOD_YPADDING);

		cairo_set_font_face(cr, font);
		cairo_set_font_size(cr, MAXEDIT_SHOW_MOD_SIZE);
		cairo_show_text(cr, strI);

		cairo_restore(cr);

		// Finish (1/2)
		cairo_restore(cr);

		// ************* Drawing Dashed line (2/2)

		x = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod;
		y = 0; sx = MAXEDIT_VIEW_MOD_WIDTH; sy = MAXEDIT_VIEW_PIXELS_PER_BEAT;

		cairo_save(cr);

		cairo_translate(cr,
				0.5, int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT)+0.5);

		cairo_set_dash(cr, dashmod, 1, 0.0);
		cairo_move_to(cr, MAXEDIT_VIEW_PIXELS_PADDING, 0);
		cairo_line_to(cr, x+sx, 0);

		cairo_stroke(cr);

		cairo_set_dash(cr, NULL, 0, 0.0);

		// ********** Drawing text in clip rectangle (2/2)
		cairo_save(cr);

		cairo_rectangle(cr, x, y-sy, sx, sy);

		cairo_clip (cr);
		cairo_new_path (cr);

		cairo_move_to(cr, x, 0);

		cairo_set_font_face(cr, font);
		cairo_set_font_size(cr, MAXEDIT_SHOW_MOD_SIZE);
		cairo_show_text(cr, strE);

		cairo_restore(cr);

		// Finish (2/2)
		cairo_restore(cr);

		// Drawing a single line to make extents
		cairo_save(cr);

		cairo_translate(cr,
				x+0.5, 0.5);

		cairo_set_dash(cr, dashmod, 1, 0.0);
		cairo_move_to(cr, MAXEDIT_VIEW_MOD_DISTEXTLINE, fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT);
		cairo_line_to(cr, MAXEDIT_VIEW_MOD_DISTEXTLINE, fy*MAXEDIT_VIEW_PIXELS_PER_BEAT);

		cairo_stroke(cr);

		cairo_restore(cr);
	}
}

void CMaxView::DrawModExt(cairo_t* cr, LPCSTR cIFormat, LPCSTR cEFormat, int* nI, int* nE, float* Idata, float* Edata, int nSize, int nRefMod)
{
	if(nSize<0) return;
	if(nI==NULL||nE==NULL) return;

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;

	static const double dashmod[] = {2.0};
	static cairo_pattern_t* penmod = cairo_pattern_create_rgba(0.8, 0.8, 0.0, 1.0);
	double penmod_width = 1;
	cairo_select_font_face (cr, "Arial",
		CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_font_face_t* font;
	font = cairo_get_font_face(cr);

	cairo_set_source(cr, penmod);
	cairo_set_line_width(cr, penmod_width);

	for(int k = 0; k < nSize; k++)
	{
		int i = nI[k];
		int e = nE[k];
		float nIData = Idata[k];
		float nEData = Edata[k];

		// Doesn't matter the group
		float fBeat = Arrow2Beat(e, 0);
		float fy = fBeat*fVel;
		float fBackBeat = Arrow2Beat(i, 0);
		float fBacky = fBackBeat*fVel;
		int x, y, sx, sy;
		CStdStringA strI;
		strI.Format(cIFormat, nIData);
		CStdStringA strE;
		strE.Format(cEFormat, nEData);

		// ************* Drawing Dashed line (1/2)

		x = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod;
		y = 0; sx = MAXEDIT_VIEW_MOD_WIDTH; sy = MAXEDIT_VIEW_PIXELS_PER_BEAT;

		cairo_save(cr);

		cairo_translate(cr,
				0.5, int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT)+0.5);

		cairo_set_dash(cr, dashmod, 1, 0.0);
		cairo_move_to(cr, MAXEDIT_VIEW_PIXELS_PADDING, 0);
		cairo_line_to(cr, x+sx, 0);

		cairo_stroke(cr);

		cairo_set_dash(cr, NULL, 0, 0.0);

		// ********** Drawing text in clip rectangle (1/2)
		cairo_save(cr);

		cairo_rectangle(cr, x, y, sx, sy);

		cairo_clip (cr);
		cairo_new_path (cr);

		cairo_move_to(cr, x, MAXEDIT_SHOW_MOD_YPADDING);

		cairo_set_font_face(cr, font);
		cairo_set_font_size(cr, MAXEDIT_SHOW_MOD_SIZE);
		cairo_show_text(cr, strI);

		cairo_restore(cr);

		// Finish (1/2)
		cairo_restore(cr);

		// ************* Drawing Dashed line (2/2)

		x = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
			+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod;
		y = 0; sx = MAXEDIT_VIEW_MOD_WIDTH; sy = MAXEDIT_VIEW_PIXELS_PER_BEAT;

		cairo_save(cr);

		cairo_translate(cr,
				0.5, int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT)+0.5);

		cairo_set_dash(cr, dashmod, 1, 0.0);
		cairo_move_to(cr, MAXEDIT_VIEW_PIXELS_PADDING, 0);
		cairo_line_to(cr, x+sx, 0);

		cairo_stroke(cr);

		cairo_set_dash(cr, NULL, 0, 0.0);

		// ********** Drawing text in clip rectangle (2/2)
		cairo_save(cr);

		cairo_rectangle(cr, x, y-sy, sx, sy);

		cairo_clip (cr);
		cairo_new_path (cr);

		cairo_move_to(cr, x, 0);

		cairo_set_font_face(cr, font);
		cairo_set_font_size(cr, MAXEDIT_SHOW_MOD_SIZE);
		cairo_show_text(cr, strE);

		cairo_restore(cr);

		// Finish (2/2)
		cairo_restore(cr);

		// Drawing a single line to make extents
		cairo_save(cr);

		cairo_translate(cr,
				x+0.5, 0.5);

		cairo_set_dash(cr, dashmod, 1, 0.0);
		cairo_move_to(cr, MAXEDIT_VIEW_MOD_DISTEXTLINE, fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT);
		cairo_line_to(cr, MAXEDIT_VIEW_MOD_DISTEXTLINE, fy*MAXEDIT_VIEW_PIXELS_PER_BEAT);

		cairo_stroke(cr);

		cairo_restore(cr);
	}
}

gboolean CMaxView::OnExpose(cairo_t* cr)
{
	if(!m_bInitialDraw) m_bInitialDraw = true;
	CMaxDocument* pDoc = GetDocument();
	if (!pDoc)
		return FALSE;

	if(m_bSelDraw)
	{
		RestoreDrawExpose(cr);
		SelDrawExpose(cr);
		m_bSelDraw = false;
	}
	else if(m_bDelDraw)
	{
		RestoreDrawExpose(cr);
		DelDrawExpose(cr);
		m_bDelDraw = false;
	}
	else if(m_bArrowPreDraw)
	{
		RestoreDrawExpose(cr);
		ArrowPreDrawExpose(cr, m_nX, m_nY, m_nBackX, m_nBackY, m_nGroup);
		m_bArrowPreDraw = false;
	}
	else if(m_bModPreDraw)
	{
		RestoreDrawExpose(cr);
		ModPreDrawExpose(cr, m_nY, m_nBackY, m_nGroup);
		m_bModPreDraw = false;
	}
	else if(m_bRestoreDraw)
	{
		RestoreDrawExpose(cr);
		m_bRestoreDraw = false;
	}
	else
	{
		cairo_set_source_rgb(cr, 1, 1, 1);
		cairo_paint(cr);

		cairo_pattern_t* pennormal;
		pennormal = cairo_pattern_create_rgb(0.0, 0.0, 0.0);
		double pennormal_width = 1;
		cairo_pattern_t* penbeat;
		penbeat = cairo_pattern_create_rgb(0.0, 0.0, 1.0);
		//double penbeat_width = 1;
		cairo_pattern_t* penmeasure;
		penmeasure = cairo_pattern_create_rgb(1.0, 0.0, 0.0);
		//double penmeasure_width = 1;

		cairo_select_font_face (cr, "Arial",
			CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		cairo_font_face_t* font;
		font = cairo_get_font_face(cr);

		int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;

		int nMaxLimit = 0;

		for(int k = 0; k < int(m_vnGroupDisp.size()); k++)
		{
			int nGroup = m_vnGroupDisp[k];
			int nLimit = int(ceil(Arrow2Beat(-1,nGroup)*fVel))*MAXEDIT_VIEW_PIXELS_PER_BEAT;
			int nGuide = Vel2GuideMaster(fVel);

			nMaxLimit = max(nMaxLimit, nLimit);

			// Save transformations
			cairo_save(cr);

			// Translate accord to current group
			cairo_translate(cr, nDistBetGroups*k+MAXEDIT_VIEW_PIXELS_PADDING+0.5, 0.5);

			// Draw vertical guides
			for(int x = 0; x <= pDoc->m_nArrowsPerLine; x++)
			{
				cairo_set_source(cr, pennormal);

				cairo_move_to(cr,
					x*MAXEDIT_VIEW_PIXELS_WIDTH,
					0);
				cairo_line_to(cr,
					x*MAXEDIT_VIEW_PIXELS_WIDTH,
					nLimit);

				cairo_set_line_width(cr, pennormal_width);
				cairo_stroke (cr);
			}

			// Draw horizontal guides
			for(int y = 0; y <=
				int(ceil(Arrow2Beat(-1,nGroup)*pow(2.0f, float(nGuide))));
				y++)
			{
				int n = SetGuide(y, nGuide, cr);
				int k = int(float(y)/pow(2.0f, float(nGuide)));
				DWORD b = DWORD(float(y)/pow(2.0f, float(nGuide))*
					fVel*MAXEDIT_VIEW_PIXELS_PER_BEAT);

				cairo_move_to(cr,
					0,
					b);
				cairo_line_to(cr,
					pDoc->m_nArrowsPerLine*MAXEDIT_VIEW_PIXELS_WIDTH,
					b);

				cairo_stroke(cr);

				if(n <= 0)
				{
					cairo_set_source(cr, penbeat);
					CString str;
					str.Format(_T("%d"), k);
					cairo_set_font_face(cr, font);
					cairo_set_font_size(cr, MAXEDIT_SHOW_BEAT_SIZE);
					cairo_move_to(cr,
						pDoc->m_nArrowsPerLine*MAXEDIT_VIEW_PIXELS_WIDTH+
							MAXEDIT_SHOW_BEAT_PADDING,
						b+MAXEDIT_SHOW_BEAT_YPADDING);
					cairo_show_text(cr, str);
				}

				if(n < 0)
				{
					cairo_set_source(cr, penmeasure);
					CString str;
					str.Format(_T("%d"), k/4);
					cairo_set_font_face(cr, font);
					cairo_set_font_size(cr, MAXEDIT_SHOW_MEASURE_SIZE);
					cairo_move_to(cr,
						pDoc->m_nArrowsPerLine*MAXEDIT_VIEW_PIXELS_WIDTH+
							MAXEDIT_SHOW_MEASURE_PADDING,
						b+MAXEDIT_SHOW_MEASURE_YPADDING);
					cairo_show_text(cr, str);
				}

			}

			// Draw items and arrows
			for(int x = 0; x < pDoc->m_nArrowsPerLine; x++)
			{
				bool bDrawingHold = false;
				for(int y = 0; y < pDoc->song.hStepGroup[nGroup].hGroup.nSizeArrows; y++)
				{
					float fBeat = Arrow2Beat(y, nGroup);
					float fBeatNext = Arrow2Beat(y+1, nGroup);
					float fy = fBeat*fVel;
					float fny = fBeatNext*fVel;
					int nArrow = pDoc->GetArrow(x, y, nGroup);

					if(nArrow != 3)
					{
						bDrawingHold = false;
					}

					if(nArrow >= 2 && nArrow <= 3 && !bDrawingHold)
					{
						bDrawingHold = true;
						int j = (y+1);
						int nArrowj = 0;
						int nStatusj = -1;
						for(j = y+1; j < pDoc->song.hStepGroup[nGroup].hGroup.nSizeArrows; j++)
						{
							nArrowj = pDoc->GetArrow(x, j, nGroup);
							fny = Arrow2Beat(j, nGroup)*fVel;
							DrawArrow(cr, x, fy, fny, nGroup, nArrow);
							if(nArrowj != 3) break;
							nArrow = nArrowj;
							fy = fny;
						}

						//fBeatNext = Arrow2Beat(j, nGroup);
					}


					// Draw Normal
					else if(!bDrawingHold) DrawArrow(cr, x, fy, fny, nGroup, nArrow);
				}
			}

			cairo_restore(cr);

			if(m_nEdit == 2 && m_bSelected)
			{
				DrawSelection(cr, m_nX, m_nY, m_nBackX, m_nBackY, m_nGroup, m_nFamily);
			}
		}



		//static const double dashmod[] = {2.0};
		static cairo_pattern_t* penmod = cairo_pattern_create_rgba(0.0, 0.0, 1.0, 1.0);
		//double penmod_width = 1;

		for(int k = 0; k < int(m_vnModDisp.size()); k++)
		{
			int nMod = m_vnModDisp.at(k);
			cairo_save(cr);

			// Translate accord to current group
			cairo_translate(cr,
				nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
				+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*k+0.5, 0.5);

			// Draw vertical guides in mod

			cairo_set_source(cr, penmod);
			cairo_set_line_width(cr, 1);

			cairo_move_to(cr, 0, 0);
			cairo_line_to(cr, 0, nMaxLimit);
			cairo_move_to(cr, MAXEDIT_VIEW_MOD_WIDTH, 0);
			cairo_line_to(cr, MAXEDIT_VIEW_MOD_WIDTH, nMaxLimit);

			cairo_stroke(cr);

			cairo_restore(cr);

			int zero = 0;

			switch(nMod)
			{
			case MAXSTEP_BEATSPLIT:
				// Draw first one
				DrawMod(cr, "BS: %d", &zero, &pDoc->song.hStep.nBeatSplit, 1, k);
				// Draw changes
				DrawMod(cr, "BS: %d", pDoc->song.nWBeatSplit, pDoc->song.nBeatSplit,
					pDoc->song.hStep.nSizeBeatSplit, k);
				break;
			case MAXSTEP_CHANGEBPM:
				// Draw first one
				DrawMod(cr, "BPM: %g", &zero, &pDoc->song.hStep.fBPM, 1, k);
				// Draw changes
				DrawMod(cr, "BPM: %g", pDoc->song.nWChangeBPM, pDoc->song.fChangeBPM,
					pDoc->song.hStep.nSizeChangeBPM, k);
				break;
			case MAXSTEP_CHANGEVEL:
				// Draw first one
				DrawMod(cr, "Vel: %g", &zero, &pDoc->song.hStep.fVel, 1, k);
				// Draw changes
				DrawModExt(cr, "IVel: %g", "EVel: %g",
					pDoc->song.nIChangeVel, pDoc->song.nEChangeVel,
					pDoc->song.fIChangeVel, pDoc->song.fChangeVel,
					pDoc->song.hStep.nSizeChangeVel, k);
				break;
			case MAXSTEP_JUMP:
				// Draw changes
				DrawModExt(cr, "Jump", "End Jump",
					pDoc->song.nIJumps, pDoc->song.nEJumps,
					pDoc->song.nIJumps, pDoc->song.nEJumps,
					pDoc->song.hStep.nSizeJumps, k);
				break;
			case MAXSTEP_TICKCOUNT:
				// Draw first one
				DrawMod(cr, "TC: %d", &zero, &pDoc->song.hStep.nTickCount, 1, k);
				// Draw changes
				DrawMod(cr, "TC: %d", pDoc->song.nWTickCount, pDoc->song.nTickCount,
					pDoc->song.hStep.nSizeTickCount, k);
				break;
			case MAXSTEP_STOP:
				// Draw changes
				DrawModExt(cr, "Stop", "End Stop",
					pDoc->song.nIStops, pDoc->song.nEStops,
					pDoc->song.nIStops, pDoc->song.nEStops,
					pDoc->song.hStep.nSizeStops, k);
				break;
			default:
				break;
			}
		}

	}

	return TRUE;
}

DWORD CMaxView::Arrow2Pixel(int nArrow, int nGroup)
{
    CMaxDocument* pDoc = GetDocument();
	if(nArrow < 0)
		nArrow = pDoc->song.hStepGroup[nGroup].hGroup.nSizeArrows-1;

	float beat = Arrow2Beat(nArrow, nGroup);

	return Beat2Pixel(beat);
}

DWORD CMaxView::Beat2Pixel(float fBeat)
{
	if(fBeat < 0)
		return 0;

	return DWORD(fBeat*fVel*MAXEDIT_VIEW_PIXELS_PER_BEAT);
}

float CMaxView::Arrow2Beat(int nArrow, int nGroup)
{
	if(nArrow < 0)
		nArrow = GetDocument()->
			song.hStepGroup[nGroup].hGroup.nSizeArrows-1;

	return GetDocument()->BeatArrowEdit(nArrow);
}

static const double guide1[] = {10.0, 10.0};
static const double guide2[] = {5.0, 5.0};
static const double guide3[] = {5.0, 5.0};
static const double guide4[] = {4.0, 1.0, 1.0, 1.0};

static const int nguide1 = 2;
static const int nguide2 = 2;
static const int nguide3 = 2;
static const int nguide4 = 4;

int CMaxView::SetGuide(int y, int nGuide, cairo_t* cr)
{
	while(float(y/2) == (float(y)/2) && nGuide > 0)
	{
		nGuide--;
		y /= 2;
	}

	if(nGuide == 0 && float(y/4) == (float(y)/4))
		nGuide = -1;

	double c = 0.0;
	if(nGuide >= 3)
		c = 0.5*(2-exp(-float(nGuide-3)));
	cairo_set_source_rgba(cr, c, c, c, 1.0);

	int nGuidex = min(nGuide, 4);

	switch(nGuidex)
	{
	case -1:
		cairo_set_dash(cr, NULL, 0, 0);
		cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, 1.0);
		cairo_set_line_width(cr, 2.0);
		break;
	case 0:
		cairo_set_dash(cr, NULL, 0, 0);
		cairo_set_source_rgba(cr, 0.0, 0.0, 1.0, 1.0);
		cairo_set_line_width(cr, 1.0);
		break;
	case 1:
		cairo_set_dash(cr, guide1, nguide1, 0);
		cairo_set_line_width(cr, 1.0);
		break;
	case 2:
		cairo_set_dash(cr, guide2, nguide2, 0);
		cairo_set_line_width(cr, 1.0);
		break;
	case 3:
		cairo_set_dash(cr, guide3, nguide3, 0);
		cairo_set_line_width(cr, 1.0);
		break;
	case 4:
		cairo_set_dash(cr, guide4, nguide4, 0);
		cairo_set_line_width(cr, 1.0);
		break;
	default:
		cairo_set_dash(cr, NULL, 0, 0);
		cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);
		cairo_set_line_width(cr, 1.0);
		break;
	}
	return nGuide;
}

int CMaxView::Vel2GuideMaster(float fVel)
{
	if(fVel<0) return 0;

	int a = 0;
	a = int(log(fVel)/log(2.0f));

	return a;
}

int CMaxView::GetMaxSizeArrows(void)
{
	int a = 0;
	for(int k = 0; k < GetDocument()->song.hStep.nSizeArrowGroups; k++)
	{
		if(a < GetDocument()->song.hStepGroup[k].hGroup.nSizeArrows)
			a = GetDocument()->song.hStepGroup[k].hGroup.nSizeArrows;
	}
	return a;
}

int CMaxView::GetModExt(int nMod)
{
	switch(nMod)
	{
	case MAXSTEP_TICKCOUNT:
	case MAXSTEP_CHANGEBPM:
	case MAXSTEP_BEATSPLIT:
		return 0;
		break;
	case MAXSTEP_STOP:
	case MAXSTEP_JUMP:
	case MAXSTEP_CHANGEVEL:
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

void CMaxView::DrawImageFromIM(cairo_t* cr, int nIndex, int nX, int nY, int sX, int sY)
{
	if(nIndex < 0) return;
	if(nIndex >= int(sim.size())) return;
	cairo_surface_t* surf = sim.at(nIndex);
	cairo_surface_t* temp = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, sX, sY);
	cairo_t* ctemp = cairo_create(temp);
	int img_height = cairo_image_surface_get_height(surf);
    int img_width = cairo_image_surface_get_width(surf);
	double width_ratio = double(sX) / double(img_width);
	double height_ratio = double(sY) / double(img_height);

	cairo_translate(ctemp, 0.5, 0.5);
	cairo_scale(ctemp, width_ratio, height_ratio);
	cairo_set_source_surface(ctemp, surf, 0, 0);
	cairo_move_to(ctemp, 0, 0);
	cairo_paint(ctemp);

	cairo_save(cr);
	cairo_translate(cr, double(nX), double(nY));
	cairo_set_source_surface(cr, temp, 0, 0);
	cairo_move_to(cr, 0, 0);
	cairo_paint(cr);
	cairo_restore(cr);

	cairo_destroy(ctemp);
	cairo_surface_destroy(temp);
}

int CMaxView::Beat2Arrow(float fBeat)
{
	return GetDocument()->ArrowBeatEdit(fBeat);
}

int CMaxView::Click2Arrow(int x, int y, int* nArrow, int* nLine, int* nRefGroup)
{
	(*nArrow) = 0;
	(*nLine) = 0;
	(*nRefGroup) = 0;

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
		MAXEDIT_DIST_SHOWGROUPS;

	int refGroup = (x-MAXEDIT_VIEW_PIXELS_PADDING)/nDistBetGroups;
	if(refGroup >= int(m_vnGroupDisp.size())) return 0;

	int nGroup = m_vnGroupDisp.at(refGroup);
	if(nGroup >= GetDocument()->song.hStep.nSizeArrowGroups)
		return 0;

	int xf = x-MAXEDIT_VIEW_PIXELS_PADDING-refGroup*nDistBetGroups;
	if(xf < 0 || xf >= ((refGroup+1)*nDistBetGroups)) return 0;
	int line = xf/MAXEDIT_VIEW_PIXELS_WIDTH;
	if(line < 0 || line >= GetDocument()->m_nArrowsPerLine) return 0;

	float fBeat = float(y)/fVel/MAXEDIT_VIEW_PIXELS_PER_BEAT;
	int arrow = Beat2Arrow(fBeat);
	if(arrow >= GetDocument()->song.hStepGroup[nGroup].hGroup.nSizeArrows)
		return 0;

	(*nArrow) = arrow;
	(*nLine) = line;
	(*nRefGroup) = refGroup;
	return 1;
}

int CMaxView::Click2ArrowAsociated(int x, int y, int nRefGroup, int* nArrow, int* nLine)
{
	(*nArrow) = 0;
	(*nLine) = 0;

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
		MAXEDIT_DIST_SHOWGROUPS;

	if(nRefGroup >= int(m_vnGroupDisp.size())) return 0;

	int nGroup = m_vnGroupDisp.at(nRefGroup);
	if(nGroup >= GetDocument()->song.hStep.nSizeArrowGroups)
		return 0;

	int xf = x-MAXEDIT_VIEW_PIXELS_PADDING-nRefGroup*nDistBetGroups;
	int selX = int(xf/MAXEDIT_VIEW_PIXELS_WIDTH);
	int line = clamp(selX,
					 0,
					 GetDocument()->m_nArrowsPerLine-1);

	float fBeat = float(y)/fVel/MAXEDIT_VIEW_PIXELS_PER_BEAT;
	int selY = Beat2Arrow(fBeat);
	int arrow = clamp(selY,
					  0,
					  GetDocument()->song.hStepGroup[nGroup].hGroup.nSizeArrows-1);

	(*nArrow) = arrow;
	(*nLine) = line;
	return 1;
}

int CMaxView::Click2Mod(int x, int y, int* nArrow, int* nLine, int* nRefGroup)
{
	(*nArrow) = 0;
	(*nLine) = 0;	// Not necesary for this
	(*nRefGroup) = 0;

	int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
		MAXEDIT_DIST_SHOWGROUPS;
	int nSizeDispGroups = m_vnGroupDisp.size();

	int nRelX = (x-MAXEDIT_VIEW_PIXELS_PADDING-nSizeDispGroups*nDistBetGroups
		-MAXEDIT_VIEW_MOD_PADDING);

	int refGroup = nRelX/MAXEDIT_VIEW_MOD_DIST;
	if(refGroup < 0 || refGroup >= int(m_vnModDisp.size()) ||
		nRelX < 0) return 0;

	int xf = nRelX-refGroup*MAXEDIT_VIEW_MOD_DIST;
	if(xf >= MAXEDIT_VIEW_MOD_WIDTH) return 0;

	float fBeat = float(y)/fVel/MAXEDIT_VIEW_PIXELS_PER_BEAT;
	int arrow = Beat2Arrow(fBeat);
	if(arrow >= GetMaxSizeArrows())
		return 0;

	(*nArrow) = arrow;
	(*nRefGroup) = refGroup;
	return 1;
}

int CMaxView::Click2ModAsociated(int x, int y, int nRefGroup, int* nArrow, int* nLine)
{
	// ITS VERY EASY, ONLY DETERMINATES Y TO ARROWS
	(*nArrow) = 0;
	(*nLine) = 0;	// Not necesary for this

	float fBeat = float(y)/fVel/MAXEDIT_VIEW_PIXELS_PER_BEAT;
	int arrow = Beat2Arrow(fBeat);
	if(arrow >= GetMaxSizeArrows())
		arrow = GetMaxSizeArrows();

	(*nArrow) = arrow;
	return 1;
}

int CMaxView::Click2Family(int x, int y, int* nX, int* nY, int* nGroup, int* nFamily)
{
	(*nFamily) = 0;
	int c = Click2Arrow(x, y, nY, nX, nGroup);
	if(c) (*nFamily) = 1;
	else
	{
		c = Click2Mod(x, y, nY, nX, nGroup);
		if(c) (*nFamily) = 2;
		else return 0;
	}
	return 1;
}

int CMaxView::Click2FamilyAsociated(int x, int y, int nGroup, int nFamily,  int* nX, int* nY)
{
	if(nFamily == 1)
	{
		if(Click2ArrowAsociated(x, y, nGroup, nY, nX)) return 1;
	}
	if(nFamily == 2)
	{
		if(Click2ModAsociated(x, y, nGroup, nY, nX)) return 1;
	}
	return 0;
}

bool CMaxView::ClickInSelection(int mx, int my)
{
	if(m_bSelected && m_nEdit == 2)
	{
		int x, y, h, w;
		GetSelectionCoords(m_nX, m_nY, m_nBackX, m_nBackY, m_nGroup, m_nFamily, &x, &y, &w, &h);
		if(x < mx && mx < (x+w) && y < my && my < (y+h)) return true;
	}
	return false;
}

int CMaxView::AddArrows(void)
{
	CMaxDocument *pDoc = GetDocument();
	int nMinX = min(m_nX, m_nBackX),
		nMinY = min(m_nY, m_nBackY),
		nMaxX = max(m_nX, m_nBackX),
		nMaxY = max(m_nY, m_nBackY);
	int nGroup = m_vnGroupDisp.at(m_nGroup);
	int nArrow = m_nAddType;
	int nPar = m_nEdit == 3?0:m_nAddPar;

	// This is an action, register it to UNDO
	RegArrows();

	// Do the action
	for(int y = nMinY; y <= nMaxY; y++)
	{
		for(int x = nMinX; x <= nMaxX; x++)
		{
			pDoc->SetArrow(x, y, nGroup, nArrow, nPar);
		}
	}

	return 1;
}

int CMaxView::AddFArrows(void)
{
	int nMinX = min(m_nX, m_nBackX),
		nMinY = min(m_nY, m_nBackY),
		nMaxX = max(m_nX, m_nBackX),
		nMaxY = max(m_nY, m_nBackY);
	int nGroup = m_vnGroupDisp.at(m_nGroup);
	int nArrow = 0;
	int nPar = m_nEdit == 3?0:m_nAddPar;
	for(int y = nMinY; y <= nMaxY; y++)
	{
		if(y == nMinY) nArrow = 2;
		else if(y == nMaxY) nArrow = 4;
		else nArrow = 3;
		for(int x = nMinX; x <= nMaxX; x++)
		{
			int nArrowG = GetDocument()->GetArrow(x, y, nGroup);
			if ((nArrowG == 3 && y == nMinY) ||
				(nArrowG == 3 && y == nMaxY)) continue;
			if ((nArrowG == 4 && y == nMinY) ||
				(nArrowG == 2 && y == nMaxY)) nArrow = 3;
			GetDocument()->SetArrow(x, y, nGroup, nArrow, nPar);
		}
	}
	return 1;
}

int CMaxView::DelArrows(void)
{
	int nMinX = min(m_nX, m_nBackX),
		nMinY = min(m_nY, m_nBackY),
		nMaxX = max(m_nX, m_nBackX),
		nMaxY = max(m_nY, m_nBackY);
	int nGroup = m_vnGroupDisp.at(m_nGroup);
	RegArrows();
	int nArrow = 0;
	int nPar = 0;
	for(int y = nMinY; y <= nMaxY; y++)
	{
		for(int x = nMinX; x <= nMaxX; x++)
		{
			GetDocument()->SetArrow(x, y, nGroup, nArrow, nPar);
		}
	}
	return 1;
}

gint CMaxView::DoModModal(int nMod, int nVer, int** nInt, float** fFloat)
{
	switch(nMod)
	{
	case MAXSTEP_BEATSPLIT:
		{
			CBeatSplitDlg* dlg = new CBeatSplitDlg();
			dlg->m_nBeatSplit = nVer==-1?1:(*nInt)[1];
			gint c = dlg->DoModal();
			if(c == GTK_RESPONSE_ACCEPT)
			{
				(*nInt)[1] = dlg->m_nBeatSplit;
			}
			delete dlg;
			return c;
		}

		break;
	case MAXSTEP_CHANGEBPM:
		{
			CChangeBPMDlg* dlg = new CChangeBPMDlg();
			dlg->m_fChangeBPM = nVer==-1?120:(*fFloat)[0];
			gint c = dlg->DoModal();
			if(c == GTK_RESPONSE_ACCEPT)
			{
				(*fFloat)[0] = dlg->m_fChangeBPM;
			}
			delete dlg;
			return c;
		}
		break;
	case MAXSTEP_CHANGEVEL:
		{
			CChangeVelDlg* dlg = new CChangeVelDlg();
			dlg->m_fChangeVel = nVer==-1?1:(*fFloat)[0];
			dlg->m_fIChangeVel = nVer==-1?1:(*fFloat)[1];
			gint c = dlg->DoModal();
			if(c == GTK_RESPONSE_ACCEPT)
			{
				(*fFloat)[0] = dlg->m_fChangeVel;
				(*fFloat)[1] = dlg->m_fIChangeVel;
			}
			delete dlg;
			return c;
		}
		break;
	case MAXSTEP_JUMP:
		return GTK_RESPONSE_ACCEPT;
		break;
	case MAXSTEP_TICKCOUNT:
		{
			CTickCountDlg* dlg = new CTickCountDlg();
			dlg->m_nTickCount = nVer==-1?1:(*nInt)[1];
			gint c = dlg->DoModal();
			if(c == GTK_RESPONSE_ACCEPT)
			{
				(*nInt)[1] = dlg->m_nTickCount;
			}
			delete dlg;
			return c;
		}
		break;
	case MAXSTEP_STOP:
		return GTK_RESPONSE_ACCEPT;
		break;
	default:
		// USER please, refer to python coding
		return GTK_RESPONSE_CANCEL;
		break;
	}
	return GTK_RESPONSE_CANCEL;
}

int CMaxView::AddMod(void)
{
	CMaxDocument* pDoc = GetDocument();
	int nMinY = min(m_nY, m_nBackY),
		nMaxY = max(m_nY, m_nBackY);
	int nMod = m_vnModDisp.at(m_nGroup);

	int nVer = VerificateMod(nMod, m_nY, m_nBackY);

	int nSize, nSizeInt, nSizeFloat;
	int *nInt = NULL;
	float *fFloat = NULL;
	int nIW, nEW;
	pDoc->GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);
	pDoc->GetModWhere(nMod, &nIW, &nEW);
	if((nIW == nEW && m_nY == 0) || (nIW != nEW && m_nBackY == 0 && m_nY == 0))	// Editing primary
	{
		int nRGroup = m_nRGroup;
		int nRFamily = m_nRFamily;
		m_nRGroup = m_nGroup;
		m_nRFamily = 2;
		m_bRPressed = true;
		OnEditModPrimary();
		m_nRGroup = nRGroup;
		m_nRFamily = nRFamily;
		return 1;
	}
	if(nSizeInt > 0 && nVer == -1) nInt = (int*)malloc(nSizeInt*sizeof(int));
	if(nSizeFloat > 0 && nVer == -1) fFloat = (float*)malloc(nSizeFloat*sizeof(float));
	if(nVer != -1) pDoc->GetMod(nMod, nVer, &nInt, &fFloat);

	if(nIW == nEW) nInt[nIW] = m_nY;
	else
	{
		if(nVer != -1)
		{
			nInt[nIW] = min(nMinY, pDoc->song.nIChangeVel[nVer]);
			nInt[nEW] = max(nMaxY, pDoc->song.nEChangeVel[nVer]);
		}
		else
		{
			nInt[nIW] = nMinY;
			nInt[nEW] = nMaxY;
		}
	}

	if(DoModModal(nMod, nVer, &nInt, &fFloat) == GTK_RESPONSE_ACCEPT)
	{
		if(nIW == nEW) pDoc->RegMods(0, m_nY, 0, m_nY, 2, nMod);
		else RegMods();
		if(nVer == -1) pDoc->AddMod(nMod, nInt, nSizeInt, fFloat, nSizeFloat);
		else pDoc->EditMod(nMod, nVer, nInt, nSizeInt, fFloat, nSizeFloat);
	}
	else return 0;

	if(nSizeInt > 0 && nVer == -1) free(nInt);
	if(nSizeFloat > 0 && nVer == -1) free(fFloat);
	if(nVer != -1) pDoc->FreeMod(&nInt, &fFloat);

	if(nIW != nEW && nVer >= 0)
	{
		while(1)
		{
			int y = VerificateMod(nMod, nMinY, nMaxY, nVer);
			if(y < 0) break;
			pDoc->DelMod(nMod, y);
		}
	}
	return 1;
}

int CMaxView::DelMod(void)
{
	int nMinY = min(m_nY, m_nBackY),
		nMaxY = max(m_nY, m_nBackY);
	int nMod = m_vnModDisp.at(m_nGroup);

	RegMods();

	while(1)
	{
		int nInd = VerificateMod(nMod, nMinY, nMaxY);
		if(nInd == -1) break;
		GetDocument()->DelMod(nMod, nInd);
	}

	return 1;
}

int CMaxView::VerificateMod(int nMod, int y, int by)
{
	return GetDocument()->VerificateMod(nMod, y, by);
}

int CMaxView::VerificateMod(int nMod, int y, int by, int i)
{
	return GetDocument()->VerificateMod(nMod, y, by, i);
}

void CMaxView::RegArrows(void)
{
	CMaxDocument *pDoc = GetDocument();
	pDoc->RegArrows(m_nX, m_nY, m_nBackX, m_nBackY, m_nFamily, m_vnGroupDisp.at(m_nGroup));
}

void CMaxView::RegMods(void)
{
	CMaxDocument *pDoc = GetDocument();
	pDoc->RegMods(m_nX, m_nY, m_nBackX, m_nBackY, m_nFamily, m_vnModDisp.at(m_nGroup));
}

void CMaxView::Deletendisplace(void)
{
	if(m_nFamily == 1)
	{
		// Perform copy from displacement to erased
		CMaxDocument* pDoc = GetDocument();
		pDoc->RegSong(&pDoc->song);
		int nMinX = min(m_nX, m_nBackX),
			nMinY = min(m_nY, m_nBackY),
			nMaxX = max(m_nX, m_nBackX),
			nMaxY = max(m_nY, m_nBackY);
		int nGroup = m_vnGroupDisp.at(m_nGroup);
		int nW = nMaxX - nMinX + 1;
		int nH = nMaxY - nMinY + 1;
		for(int y = 0; y < (pDoc->song.hStepGroup[nGroup].hGroup.nSizeArrows-nH); y++)
		{
			if((y+nMaxY+1) < pDoc->song.hStepGroup[nGroup].hGroup.nSizeArrows)
			{
				memcpy(pDoc->song.hStepGroup[nGroup].nArrows+(y+nMinY)*pDoc->m_nArrowsPerLine,
					pDoc->song.hStepGroup[nGroup].nArrows+(y+nMaxY+1)*pDoc->m_nArrowsPerLine,
					pDoc->m_nArrowsPerLine*sizeof(int));
				memcpy(pDoc->song.hStepGroup[nGroup].nPArrows+(y+nMinY)*pDoc->m_nArrowsPerLine,
					pDoc->song.hStepGroup[nGroup].nPArrows+(y+nMaxY+1)*pDoc->m_nArrowsPerLine,
					pDoc->m_nArrowsPerLine*sizeof(int));
			}
			else
			{
				memset(pDoc->song.hStepGroup[nGroup].nArrows+(y+nMinY)*pDoc->m_nArrowsPerLine,
					0,
					pDoc->m_nArrowsPerLine*sizeof(int));
				memset(pDoc->song.hStepGroup[nGroup].nPArrows+(y+nMinY)*pDoc->m_nArrowsPerLine,
					0,
					pDoc->m_nArrowsPerLine*sizeof(int));
			}
		}
	}
	if(m_nFamily == 2)
	{
		DelMod();
		CMaxDocument* pDoc = GetDocument();
		int nMinY = min(m_nY, m_nBackY),
			nMaxY = max(m_nY, m_nBackY);
		int nH = nMaxY - nMinY + 1;
		int nMod = m_vnModDisp.at(m_nGroup);
		int nSize, nSizeInt, nSizeFloat;
		int *nInt;
		float *fFloat;
		int nIW, nEW;
		pDoc->GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);
		pDoc->GetModWhere(nMod, &nIW, &nEW);
		for(int y = 0; y < nSize; y++)
		{
			pDoc->GetMod(nMod, y, &nInt, &fFloat);
			if(nInt[nIW] >= nMaxY || nInt[nEW] >= nMaxY)
			{
				nInt[nIW] -= nH;
				if(nIW != nEW) nInt[nEW] -= nH;
				pDoc->EditMod(nMod, y, nInt, nSizeInt, fFloat, nSizeFloat);
			}
			pDoc->FreeMod(&nInt, &fFloat);
		}
	}
}

void CMaxView::OnActionStepSize(void)
{
	CSetSizeDlg *dlg = new CSetSizeDlg();
	dlg->m_vnGroups.clear();
	for(int i = 0; i < GetDocument()->song.hStep.nSizeArrowGroups; i++)
	{
		dlg->m_vnGroups.push_back(
			GetDocument()->song.hStepGroup[0].hGroup.nSizeArrows);
	}
	dlg->m_vnActive.resize(dlg->m_vnGroups.size(), 0);
	for(guint i = 0; i < m_vnGroupDisp.size(); i++)
	{
		dlg->m_vnActive[m_vnGroupDisp[i]] = 1;
	}

	dlg->m_vnMods.resize(MAXSTEP_SIZE, 0);
	for(guint i = 0; i < m_vnModDisp.size(); i++)
	{
		dlg->m_vnMods[m_vnModDisp[i]] = 1;
	}

	if(dlg->DoModal() != GTK_RESPONSE_ACCEPT) return;
	m_vnGroupDisp.clear();
	for(int i = 0; i < GetDocument()->song.hStep.nSizeArrowGroups; i++)
	{
		GetDocument()->ResizeSteps(i, dlg->m_vnGroups[i]);
		if(dlg->m_vnActive[i]) m_vnGroupDisp.push_back(i);
	}

	m_vnModDisp.clear();
	for(guint i = 0; i < dlg->m_vnMods.size(); i++)
	{
		if(dlg->m_vnMods[i]) m_vnModDisp.push_back(i);
	}

	Redraw();
	ActualizateScrolls();
	delete dlg;
}

void CMaxView::OnActionStepPreferences(void)
{
	CMaxDocument* pDoc = GetDocument();
	if(pDoc == NULL)
		return;
	// Initialize dialog and show
	CStepOptionsDlg *dlg = new CStepOptionsDlg();
	dlg->m_vfAttrib.clear();
	dlg->m_vGroup.clear();
	for(int i = 0; i < pDoc->song.hStep.nSizeArrowGroups; i++)
	{
		STEPOPT_GROUP group;
		group.nNoMiss = pDoc->song.hStepGroup[i].hGroup.nNoMiss;
		group.nNoCount = pDoc->song.hStepGroup[i].hGroup.nNoCount;
		group.nIsVanishSup = pDoc->song.hStepGroup[i].hGroup.nIsVanishSup;
		group.nIsVanishInf = pDoc->song.hStepGroup[i].hGroup.nIsVanishInf;
		group.nIsSkin = pDoc->song.hStepGroup[i].hGroup.nIsSkin;
		group.fVanishSup = pDoc->song.hStepGroup[i].hGroup.fVanishSup;
		group.fVanishInf = pDoc->song.hStepGroup[i].hGroup.fVanishInf;
		group.nSkin = pDoc->song.hStepGroup[i].hGroup.nSkin;
		group.nGroup = i;
		dlg->m_vGroup.push_back(
			group);
	}
	for(int i = 0; i < pDoc->song.hStep.nSizeAttrib; i++)
	{
		dlg->m_vfAttrib.push_back(pDoc->song.fAttrib[i]);
	}
	dlg->m_nDelay = pDoc->song.hStep.nOffset;
	dlg->m_nLevel = pDoc->song.hStep.nDificulty;
	dlg->m_nRuntime = pDoc->song.hStep.dwLSong;
	dlg->m_nArrowsPerLine = pDoc->song.hStep.nArrowsPerLine;

	// Show dialog, user will modificate
	if(dlg->DoModal() != GTK_RESPONSE_ACCEPT) return;

	pDoc->RegSong(&pDoc->song);

	// ** Coordination phase

	// Actualizate attribs
	pDoc->song.hStep.nSizeAttrib = (int)dlg->m_vfAttrib.size();
	pDoc->Reinit();
	for(guint i = 0; i < dlg->m_vfAttrib.size(); i++)
	{
		pDoc->song.fAttrib[i] = dlg->m_vfAttrib[i];
	}

	// Get the maximum group
	int nMaxGroup = 0;
	for(guint j = 0; j < dlg->m_vGroup.size(); j++)
	{
		if(nMaxGroup < dlg->m_vGroup[j].nGroup)
			nMaxGroup = dlg->m_vGroup[j].nGroup;
	}

	// Rezise groups if there is more groups than existent
	if(nMaxGroup >= pDoc->song.hStep.nSizeArrowGroups)
		pDoc->InitArrowGruops(pDoc->song.hStep.nSizeArrowGroups, nMaxGroup+1);

	// Assign info
	for(guint j = 0; j < dlg->m_vGroup.size(); j++)
	{
		STEPOPT_GROUP group = dlg->m_vGroup[j];
		int i = group.nGroup;
		pDoc->song.hStepGroup[i].hGroup.nNoMiss = group.nNoMiss;
		pDoc->song.hStepGroup[i].hGroup.nNoCount = group.nNoCount;
		pDoc->song.hStepGroup[i].hGroup.nIsVanishSup = group.nIsVanishSup;
		pDoc->song.hStepGroup[i].hGroup.nIsVanishInf = group.nIsVanishInf;
		pDoc->song.hStepGroup[i].hGroup.nIsSkin = group.nIsSkin;
		pDoc->song.hStepGroup[i].hGroup.fVanishSup = group.fVanishSup;
		pDoc->song.hStepGroup[i].hGroup.fVanishInf = group.fVanishInf;
		pDoc->song.hStepGroup[i].hGroup.nSkin = group.nSkin;
	}
	pDoc->song.hStep.nOffset = dlg->m_nDelay;
	pDoc->song.hStep.nDificulty = dlg->m_nLevel;
	pDoc->song.hStep.dwLSong = dlg->m_nRuntime;

	// Find groups deleted
	vector<int> vnDelGroups;
	for(int i = 0; i < pDoc->song.hStep.nSizeArrowGroups; i++)
	{
		int nConfirm = 1;
		for(guint j = 0; j < dlg->m_vGroup.size(); j++)
		{
			if(dlg->m_vGroup[j].nGroup == i)
				nConfirm = 0;
		}
		if(nConfirm)
			vnDelGroups.push_back(i);
	}

	// Delete groups
	for(guint j = 0; j < vnDelGroups.size(); j++)
	{
		int nDel = vnDelGroups[j];
		pDoc->DelGroup(nDel);
		for(guint i = 0; i < vnDelGroups.size(); i++)
		{
			if(vnDelGroups[i] > nDel)
				vnDelGroups[i]--;
		}
	}

	// Take care of views
	vector<int>::iterator it;
	for(it=m_vnGroupDisp.begin(); it < m_vnGroupDisp.end(); it++)
	{
		if((*it) >= pDoc->song.hStep.nSizeArrowGroups)
			m_vnGroupDisp.erase(it);
	}

	// Actualizate arrows per line
	if(pDoc->song.hStep.nArrowsPerLine > dlg->m_nArrowsPerLine)	// Shrink
	{
		for(int i = 0; i < pDoc->song.hStep.nSizeArrowGroups; i++)
		{
			for(int j = 0; j < pDoc->song.hStepGroup[i].hGroup.nSizeArrows; j++)
			{
				memcpy(pDoc->song.hStepGroup[i].nArrows+j*dlg->m_nArrowsPerLine,
					   pDoc->song.hStepGroup[i].nArrows+j*pDoc->song.hStep.nArrowsPerLine,
					   sizeof(int)*dlg->m_nArrowsPerLine);
				memcpy(pDoc->song.hStepGroup[i].nPArrows+j*dlg->m_nArrowsPerLine,
					   pDoc->song.hStepGroup[i].nPArrows+j*pDoc->song.hStep.nArrowsPerLine,
					   sizeof(int)*dlg->m_nArrowsPerLine);
			}
		}
		pDoc->song.hStep.nArrowsPerLine = dlg->m_nArrowsPerLine;
		pDoc->Reinit();
	}
	else if(pDoc->song.hStep.nArrowsPerLine < dlg->m_nArrowsPerLine) // Grow
	{
		int nPrev = pDoc->song.hStep.nArrowsPerLine;
		pDoc->song.hStep.nArrowsPerLine = dlg->m_nArrowsPerLine;
		pDoc->Reinit();
		for(int i = 0; i < pDoc->song.hStep.nSizeArrowGroups; i++)
		{
			for(int j = pDoc->song.hStepGroup[i].hGroup.nSizeArrows-1; j >= 0; j--)
			{
				memcpy(pDoc->song.hStepGroup[i].nArrows+j*dlg->m_nArrowsPerLine,
					   pDoc->song.hStepGroup[i].nArrows+j*nPrev,
					   sizeof(int)*nPrev);
				memcpy(pDoc->song.hStepGroup[i].nPArrows+j*dlg->m_nArrowsPerLine,
					   pDoc->song.hStepGroup[i].nPArrows+j*nPrev,
					   sizeof(int)*nPrev);
				memset(pDoc->song.hStepGroup[i].nArrows+j*dlg->m_nArrowsPerLine+nPrev,
					   0,
					   sizeof(int)*(dlg->m_nArrowsPerLine-nPrev));
				memset(pDoc->song.hStepGroup[i].nPArrows+j*dlg->m_nArrowsPerLine+nPrev,
					   0,
					   sizeof(int)*(dlg->m_nArrowsPerLine-nPrev));
			}
		}
	}

	ActualizateScrolls();
	Redraw();
}

void CMaxView::Redraw(void)
{
	m_bPreviewDraw = false;
	m_bRestoreDraw = false;
	m_bRestoreDrawMouse = false;
	gtk_widget_queue_draw(m_gtkDrawingArea);
}

void CMaxView::ActualizateStepRadio(void)
{
	gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON( gMain->action_step[m_nEdit] ), true);
}

// Signals

void CMaxView::OnActualizate(void)
{
	ActualizateStepRadio();
}

void CMaxView::OnLButtonUp(int x, int y)
{
	if(m_bPressed)
	{
		int nX, nY;

		Click2FamilyAsociated(x, y, m_nGroup, m_nFamily, &nX, &nY);

		if(m_nFamily == 1)
		{
			m_nX = nX;
			m_nY = nY;
			if(m_nEdit != 2)
			{
				if(m_nEdit == 1) AddArrows();
				else if(m_nEdit == 0) AddFArrows();
				else DelArrows();
			}
			else
			{
				m_bSelected = true;
			}
		}
		if(m_nFamily == 2)
		{
			m_nX = nX;
			m_nY = nY;
			if(m_nEdit != 2)
			{
				if(m_nEdit != 3) AddMod();
				else DelMod();
			}
			else
			{
				m_bSelected = true;
			}
		}

		m_bPressed = false;

		m_bRestoreDrawMouse = false;
		ActualizateScrolls();
		Redraw();
	}
}

void CMaxView::OnRButtonUp(int x, int y, GdkEventButton* event)
{
	if(!m_bPressed)
	{
		int nX, nY, nGroup, nFamily = 0;
		if(ClickInSelection(x, y))
		{
			if(m_nFamily == 1) gtk_menu_popup(GTK_MENU(gMain->menu_selection), NULL, NULL, NULL, NULL, event->button, event->time);
			if(m_nFamily == 2) gtk_menu_popup(GTK_MENU(gMain->menu_selection_mod), NULL, NULL, NULL, NULL, event->button, event->time);
		}
		else if(Click2Family(x, y, &nX, &nY, &nGroup, &nFamily))
		{
			m_bRPressed = true;
			m_nRX = nX;
			m_nRY = nY;
			m_nRGroup = nGroup;
			m_nRFamily = nFamily;
			if(nFamily == 1) gtk_menu_popup(GTK_MENU(gMain->menu_edit), NULL, NULL, NULL, NULL, event->button, event->time);
			if(m_nFamily == 2) gtk_menu_popup(GTK_MENU(gMain->menu_edit_mod), NULL, NULL, NULL, NULL, event->button, event->time);
		}
		else gtk_menu_popup(GTK_MENU(gMain->menu_outside), NULL, NULL, NULL, NULL, event->button, event->time);
	}
}

void CMaxView::OnLButtonDown(int x, int y)
{
	if(!m_bPressed)
	{
		int nX, nY, nGroup, nFamily = 0;
		m_bSelected = false;
		if(Click2Family(x, y, &nX, &nY, &nGroup, &nFamily))
		{
			m_nBackX = nX;
			m_nBackY = nY;
			m_nGroup = nGroup;
			m_nFamily = nFamily;
			m_bPressed = true;
		}
	}
}

void CMaxView::OnRButtonDown(int x, int y)
{
}

void CMaxView::OnEnterNotify(void)
{
}

void CMaxView::OnLeaveNotify(void)
{
	m_bPressed = false;
	if(m_bRestoreDrawMouse) {RestoreDraw(); m_bRestoreDrawMouse = false;}
}

void CMaxView::OnMouseMove(int x, int y)
{
	while (gtk_events_pending ())// && !m_bInitialDraw)
		gtk_main_iteration ();
	// Virtual
	static int nBackX = 0, nBackY = 0, nBackGroup = 0, nBackFamily = 0;

	if(!m_bPreviewDraw && !m_bDrawing)
	{
		m_bDrawing = true;
		int nDistBetGroups = MAXEDIT_VIEW_PIXELS_WIDTH*GetDocument()->m_nArrowsPerLine+
			MAXEDIT_DIST_SHOWGROUPS;
		int nX = 0, nY = 0, nGroup = 0, nFamily = 0;

		if(!m_bPressed)
		{
			Click2Family(x, y, &nX, &nY, &nGroup, &nFamily);

			if(nFamily != nBackFamily || nBackX != nX || nBackY != nY || nBackGroup != nGroup)
			{
				if(m_bRestoreDrawMouse) {RestoreDraw(); m_bRestoreDrawMouse = false;}
				if(nFamily == 1)
				{
					float fBeat = Arrow2Beat(nY, m_vnGroupDisp.at(nGroup));
					float fy = fBeat*fVel;

					SaveDraw(nX*MAXEDIT_VIEW_PIXELS_WIDTH+MAXEDIT_VIEW_PIXELS_PADDING+nDistBetGroups*nGroup,
							int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT),
							MAXEDIT_VIEW_PIXELS_WIDTH,
							MAXEDIT_VIEW_PIXELS_PER_BEAT);
					SelDraw(nX*MAXEDIT_VIEW_PIXELS_WIDTH+MAXEDIT_VIEW_PIXELS_PADDING+nDistBetGroups*nGroup,
							int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT),
							MAXEDIT_VIEW_PIXELS_WIDTH,
							MAXEDIT_VIEW_PIXELS_PER_BEAT);
					m_bRestoreDrawMouse = true;
				}
				if(nFamily == 2)
				{
					float fBeat = Arrow2Beat(nY, 0);
					float fy = fBeat*fVel;

					int nRefMod = nGroup;
					int x = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
							+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod,
						y = int(fy*MAXEDIT_VIEW_PIXELS_PER_BEAT),
						sx = MAXEDIT_VIEW_MOD_WIDTH,
						sy = MAXEDIT_VIEW_PIXELS_PER_BEAT;

					SaveDraw(x, y, sx, sy);
					SelDraw(x, y, sx, sy);
					m_bRestoreDrawMouse = true;
				}
			}
		}
		if(m_nFamily == 1 && m_bPressed)
		{
			nGroup = m_nGroup;
			Click2FamilyAsociated(x, y, m_nGroup, m_nFamily, &nX, &nY);
			if(nBackX != nX || nBackY != nY)
			{
				int nMinX = min(nX, m_nBackX),
					nMinY = min(nY, m_nBackY),
					nMaxX = max(nX, m_nBackX),
					nMaxY = max(nY, m_nBackY);
				if(m_bRestoreDrawMouse) {RestoreDraw(); m_bRestoreDrawMouse = false;}
				if(m_nEdit == 0 || m_nEdit == 1)
				{
					ArrowPreDraw(nX, nY);
					m_bRestoreDrawMouse = true;
				}
				if(m_nEdit == 2 || m_nEdit == 3)
				{
					float fBeat = Arrow2Beat(nMaxY, m_vnGroupDisp.at(m_nGroup));
					float fy = fBeat*fVel;
					float fBackBeat = Arrow2Beat(nMinY, m_vnGroupDisp.at(m_nGroup));
					float fBacky = fBackBeat*fVel;
					int clipX, clipY, clipW, clipH;
					clipX = nMinX*MAXEDIT_VIEW_PIXELS_WIDTH+MAXEDIT_VIEW_PIXELS_PADDING+nDistBetGroups*m_nGroup;
					clipY = int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT);
					clipW = ((nMaxX-nMinX)+1)*MAXEDIT_VIEW_PIXELS_WIDTH;
					clipH = int((fy-fBacky)*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT;
					SaveDraw(clipX, clipY, clipW, clipH);
					if(m_nEdit == 2)
						SelDraw(clipX, clipY, clipW, clipH);
					else
						DelDraw(clipX, clipY, clipW, clipH);
					m_bRestoreDrawMouse = true;
				}
			}
		}
		if(m_nFamily == 2 && m_bPressed)
		{
			nGroup = m_nGroup;
			Click2FamilyAsociated(x, y, m_nGroup, m_nFamily, &nX, &nY);
			if(nBackX != nX || nBackY != nY)
			{
				int nMinY = min(nY, m_nBackY),
					nMaxY = max(nY, m_nBackY);
				if(m_bRestoreDrawMouse) {RestoreDraw(); m_bRestoreDrawMouse = false;}
				if(m_nEdit == 0 || m_nEdit == 1)
				{
					ModPreDraw(nY, m_nGroup);
					m_bRestoreDrawMouse = true;
				}
				if(m_nEdit == 2 || m_nEdit == 3)
				{
					int nRefMod = m_nGroup;
					float fBeat = Arrow2Beat(nMaxY, 0);
					float fy = fBeat*fVel;
					float fBackBeat = Arrow2Beat(nMinY, 0);
					float fBacky = fBackBeat*fVel;
					int clipX, clipY, clipW, clipH;
					clipX = nDistBetGroups*m_vnGroupDisp.size()+MAXEDIT_VIEW_PIXELS_PADDING
							+MAXEDIT_VIEW_MOD_PADDING+MAXEDIT_VIEW_MOD_DIST*nRefMod;
					clipY = int(fBacky*MAXEDIT_VIEW_PIXELS_PER_BEAT);
					clipW = MAXEDIT_VIEW_MOD_WIDTH;
					clipH = int((fy-fBacky)*MAXEDIT_VIEW_PIXELS_PER_BEAT)+MAXEDIT_VIEW_PIXELS_PER_BEAT;
					SaveDraw(clipX, clipY, clipW, clipH);
					if(m_nEdit == 2)
						SelDraw(clipX, clipY, clipW, clipH);
					else
						DelDraw(clipX, clipY, clipW, clipH);
					m_bRestoreDrawMouse = true;
				}
			}
		}

		nBackFamily = nFamily;
		nBackX = nX; nBackY = nY; nBackGroup = nGroup;
		m_bDrawing = false;
	}
}

void CMaxView::OnStepLongStep(GtkToggleAction *action)
{
	if(gtk_toggle_action_get_active(action))
	{
		m_nEdit = 0;
		if(m_bInitialDraw) Redraw();
	}
}

void CMaxView::OnStepSingleStep(GtkToggleAction *action)
{
	if(gtk_toggle_action_get_active(action))
	{
		m_nEdit = 1;
		if(m_bInitialDraw) Redraw();
	}
}

void CMaxView::OnStepSelectStep(GtkToggleAction *action)
{
	if(gtk_toggle_action_get_active(action))
	{
		m_nEdit = 2;
		if(m_bInitialDraw) Redraw();
	}
}

void CMaxView::OnStepEraseStep(GtkToggleAction *action)
{
	if(gtk_toggle_action_get_active(action))
	{
		m_nEdit = 3;
		if(m_bInitialDraw) Redraw();
	}
}

void CMaxView::OnDelete(void)
{
	if(m_nEdit == 2 && m_bSelected)
	{
		if(m_nFamily == 1)
		{
			DelArrows();
		}
		if(m_nFamily == 2)
		{
			DelMod();
		}
		ActualizateScrolls();
		Redraw();
	}
}

void                GtkClipboardGetMaxStep              (GtkClipboard *clipboard,
                                                         GtkSelectionData *selection_data,
                                                         guint info,
                                                         gpointer user_data_or_owner)
{
	ClipData *d = (ClipData*)user_data_or_owner;
	gchar *data = (gchar*)d->ptrData;
	GdkAtom atom = gdk_atom_intern("MaxEditSteps", FALSE);
	gtk_selection_data_set(selection_data, atom, 8, (guchar*)data, d->ndata);
}

void                GtkClipboardClearMaxStep            (GtkClipboard *clipboard,
                                                         gpointer user_data_or_owner)
{
	if(user_data_or_owner != NULL)
	{
		ClipData* clipdata = (ClipData*)user_data_or_owner;
		if(clipdata->ptrData != NULL) free(clipdata->ptrData);
		free(user_data_or_owner);
	}
}

void                GtkClipboardReceivedMaxStep         (GtkClipboard *clipboard,
                                                         GtkSelectionData *selection_data,
                                                         gpointer data_to_send)
{
	ClipData *d = (ClipData*)data_to_send;
	const guchar *data = gtk_selection_data_get_data(selection_data);
	gint ndata = gtk_selection_data_get_length(selection_data);
	if(ndata >= 0)
	{
		guchar *foo = (guchar*)malloc(ndata*sizeof(guchar));
		memcpy(foo, data, ndata*sizeof(guchar));
		d->ptrData = (void*)foo;
		d->ndata = (size_t)ndata;
	}
	else
	{
		d->ptrData = NULL;
		d->ndata = 0;
	}

}

void CMaxView::PutClipData(ClipData* clipdata)
{
	CMaxDocument* pDoc = GetDocument();
	if(m_nFamily == 1)	// Steps
	{
		int nGroup = m_vnGroupDisp.at(m_nGroup);
		int nMinX = min(m_nX, m_nBackX),
			nMinY = min(m_nY, m_nBackY),
			nMaxX = max(m_nX, m_nBackX),
			nMaxY = max(m_nY, m_nBackY);
		MAXSTEP_CLIPBOARD c;
		c.hStep.nW = nMaxX - nMinX + 1;
		c.hStep.nH = nMaxY - nMinY + 1;
		if(c.hStep.nW*c.hStep.nH <= 0) return;
		c.nArrows = (int*)malloc(c.hStep.nW*c.hStep.nH*sizeof(int));
		c.nPArrows = (int*)malloc(c.hStep.nW*c.hStep.nH*sizeof(int));
		for(int y = 0; y < c.hStep.nH; y++)
		{
			memcpy(c.nArrows+y*c.hStep.nW,
				pDoc->song.hStepGroup[nGroup].nArrows+(nMinY+y)*pDoc->m_nArrowsPerLine+nMinX,
				c.hStep.nW*sizeof(int));
			memcpy(c.nPArrows+y*c.hStep.nW,
				pDoc->song.hStepGroup[nGroup].nPArrows+(nMinY+y)*pDoc->m_nArrowsPerLine+nMinX,
				c.hStep.nW*sizeof(int));
		}

		// Proceed to write it to raw data
		char nIdent = 's';	// Identificator to steps
		stringstream ss;
		ss.write((char*)&nIdent, sizeof(char));
		ss.write((char*)&c.hStep, sizeof(MAXSTEP_CLIPBOARD_HEADER));
		ss.write((char*)c.nArrows, c.hStep.nW*c.hStep.nH*sizeof(int));
		ss.write((char*)c.nPArrows, c.hStep.nW*c.hStep.nH*sizeof(int));
		size_t nLen2 = (size_t)ss.tellp();
		char* buf = (char*)malloc(nLen2);
		memcpy(buf, ss.str().c_str(), nLen2);
		clipdata->ptrData = (void*)buf;
		clipdata->ndata = nLen2;
	}
	if(m_nFamily == 2)	// Mods
	{
		int nMod = m_vnModDisp.at(m_nGroup);
		int nMinY = min(m_nY, m_nBackY),
			nMaxY = max(m_nY, m_nBackY);
		MAXMOD_CLIPBOARD c;
		vector<STEPMODINFO*> smiVector;
		int nSize;
		int *nInt;
		float *fFloat;
		int nIW, nEW;
		pDoc->GetParMod(nMod, &nSize, &c.hMod.nSizeInt, &c.hMod.nSizeFloat);
		pDoc->GetModWhere(nMod, &nIW, &nEW);

		for(int y = 0; y < nSize; y++)
		{
			pDoc->GetMod(nMod, y, &nInt, &fFloat);
			if(INSIDE_OF_OUTSIDE(nInt, nMinY, nMaxY, nIW, nEW))
			{
				STEPMODINFO* elem;
				elem = (STEPMODINFO*)malloc(sizeof(STEPMODINFO));
				if(c.hMod.nSizeInt > 0)
				{
					elem->nModInt = (int*)malloc(c.hMod.nSizeInt*sizeof(int));
					memcpy(elem->nModInt, nInt, c.hMod.nSizeInt*sizeof(int));
				}
				if(c.hMod.nSizeFloat > 0)
				{
					elem->fModFloat = (float*)malloc(c.hMod.nSizeFloat*sizeof(float));
					memcpy(elem->fModFloat, fFloat, c.hMod.nSizeFloat*sizeof(float));
				}
				smiVector.push_back(elem);
			}
			pDoc->FreeMod(&nInt, &fFloat);
		}

		c.hMod.nRef = nMinY;
		c.hMod.nType = nMod;
		c.hMod.nSize = (int)smiVector.size();

		// Proceed to write it to raw data
		char nIdent = 'm';	// Identificator to mods
		stringstream ss;
		ss.write((char*)&nIdent, sizeof(char));
		ss.write((char*)&c.hMod, sizeof(MAXMOD_CLIPBOARD_HEADER));
		for(int y = 0; y < c.hMod.nSize; y++)
		{
			if(c.hMod.nSizeInt > 0) ss.write((char*)smiVector[y]->nModInt, c.hMod.nSizeInt*sizeof(int));
			if(c.hMod.nSizeFloat > 0) ss.write((char*)smiVector[y]->fModFloat, c.hMod.nSizeFloat*sizeof(float));
			if(c.hMod.nSizeInt > 0) free(smiVector[y]->nModInt);
			if(c.hMod.nSizeFloat > 0) free(smiVector[y]->fModFloat);
			free(smiVector[y]);
		}
		size_t nLen2 = (size_t)ss.tellp();
		char* buf = (char*)malloc(nLen2);
		memcpy(buf, ss.str().c_str(), nLen2);
		clipdata->ptrData = (void*)buf;
		clipdata->ndata = nLen2;
	}
}

void CMaxView::GetClipData(ClipData* clipdata)
{
	CMaxDocument* pDoc = GetDocument();
	if(m_nFamily == 1)	// Steps
	{
		int nGroup = m_vnGroupDisp.at(m_nGroup);
		int nMinX = min(m_nX, m_nBackX),
			nMinY = min(m_nY, m_nBackY),
			nMaxX = max(m_nX, m_nBackX),
			nMaxY = max(m_nY, m_nBackY);
		int nW = pDoc->m_nArrowsPerLine - nMinX,
			nH = pDoc->song.hStepGroup[nGroup].hGroup.nSizeArrows - nMinY;
		MAXSTEP_CLIPBOARD c;
		char* buf = (char*)clipdata->ptrData;
		stringstream ss;
		ss.write(buf, clipdata->ndata);
		char nIdent;
		ss.read((char*)&nIdent, sizeof(char));
		if(nIdent != 's') return;
		ss.read((char*)&c.hStep, sizeof(MAXSTEP_CLIPBOARD_HEADER));
		if((c.hStep.nW*c.hStep.nH) <= 0) return;
		c.nArrows = (int*)malloc(c.hStep.nW*c.hStep.nH*sizeof(int));
		c.nPArrows = (int*)malloc(c.hStep.nW*c.hStep.nH*sizeof(int));
		ss.read((char*)c.nArrows, c.hStep.nW*c.hStep.nH*sizeof(int));
		ss.read((char*)c.nPArrows, c.hStep.nW*c.hStep.nH*sizeof(int));

		nW = min(nW, c.hStep.nW);
		nH = min(nH, c.hStep.nH);

		int nX_a = m_nX, nY_a = m_nY, nBackX_a = m_nBackX, nBackY_a = m_nBackY, nFamily_a = m_nFamily, nGroup_a = m_nGroup;
		m_nX = nMinX+nW-1; m_nY = nMinY+nH-1; m_nBackX = nMinX; m_nBackY = nMinY; m_nFamily = 1;
		RegArrows();
		m_nX = nX_a; m_nY = nY_a; m_nBackX = nBackX_a; m_nBackY = nBackY_a; m_nFamily = nFamily_a; m_nGroup = nGroup_a;

		for(int y = 0; y < nH; y++)
		{
			memcpy(pDoc->song.hStepGroup[nGroup].nArrows+(nMinY+y)*pDoc->m_nArrowsPerLine+nMinX,
				c.nArrows+y*c.hStep.nW,
				nW*sizeof(int));
			memcpy(pDoc->song.hStepGroup[nGroup].nPArrows+(nMinY+y)*pDoc->m_nArrowsPerLine+nMinX,
				c.nPArrows+y*c.hStep.nW,
				nW*sizeof(int));
		}
	}
	if(m_nFamily == 2)	// Mods
	{
		int nMod = m_vnModDisp.at(m_nGroup);
		int nMinY,
			nMaxY,
			nRef = min(m_nY, m_nBackY);

		MAXMOD_CLIPBOARD c;
		char* buf = (char*)clipdata->ptrData;
		stringstream ss;
		ss.write(buf, clipdata->ndata);
		char nIdent;
		ss.read((char*)&nIdent, sizeof(char));
		if(nIdent != 'm') return;
		ss.read((char*)&c.hMod, sizeof(MAXMOD_CLIPBOARD_HEADER));
		if(nMod != c.hMod.nType || c.hMod.nSize <= 0) return;
		c.hModInfo = (STEPMODINFO*)malloc(c.hMod.nSize*sizeof(STEPMODINFO));
		for(int y = 0; y < c.hMod.nSize; y++)
		{
			if(c.hMod.nSizeInt > 0) c.hModInfo[y].nModInt = (int*)malloc(c.hMod.nSizeInt*sizeof(int));
			if(c.hMod.nSizeFloat > 0) c.hModInfo[y].fModFloat = (float*)malloc(c.hMod.nSizeFloat*sizeof(float));
			if(c.hMod.nSizeInt > 0) ss.read((char*)c.hModInfo[y].nModInt, c.hMod.nSizeInt*sizeof(int));
			if(c.hMod.nSizeFloat > 0) ss.read((char*)c.hModInfo[y].fModFloat, c.hMod.nSizeFloat*sizeof(float));
		}

		int nSize, nSizeInt, nSizeFloat;
		int nIW, nEW;

		pDoc->GetModWhere(nMod, &nIW, &nEW);
		pDoc->GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);
		// Modify nMinY and nMaxY
		nMinY = nRef + c.hModInfo[0].nModInt[nIW] - c.hMod.nRef;
		nMaxY = nRef + c.hModInfo[c.hMod.nSize-1].nModInt[nEW] - c.hMod.nRef;

		// Do backup in UNDO
		int nX_a = m_nX, nY_a = m_nY, nBackX_a = m_nBackX, nBackY_a = m_nBackY, nFamily_a = m_nFamily, nGroup_a = m_nGroup;
		m_nY = nMaxY; m_nBackY = nMinY; m_nFamily = 2;
		RegMods();
		m_nX = nX_a; m_nY = nY_a; m_nBackX = nBackX_a; m_nBackY = nBackY_a; m_nFamily = nFamily_a; m_nGroup = nGroup_a;

		// Erase mods between nMinY and nMaxY
		while(1)
		{
			int y = VerificateMod(nMod, nMinY, nMaxY);
			if(y == -1) break;
			pDoc->DelMod(nMod, y);
		}

		// Get Size again
		pDoc->GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);

		for(int i = 0; i < c.hMod.nSize; i++)
		{
			bool bInc = true;
			int *nIntI = NULL;
			float* fFloatF = NULL;

			if(nSizeInt > 0)
			{
				nIntI = (int*)malloc(nSizeInt*sizeof(int));
				memcpy(nIntI, c.hModInfo[i].nModInt, nSizeInt*sizeof(int));
			}
			if(nSizeFloat > 0)
			{
				fFloatF = (float*)malloc(nSizeFloat*sizeof(float));
				memcpy(fFloatF, c.hModInfo[i].fModFloat, nSizeFloat*sizeof(float));
			}

			if((nRef + c.hModInfo[i].nModInt[nIW] - c.hMod.nRef) >= 0)
			{
				nIntI[nIW] = nRef + c.hModInfo[i].nModInt[nIW] - c.hMod.nRef;
				nIntI[nEW] = nRef + c.hModInfo[i].nModInt[nEW] - c.hMod.nRef;
				pDoc->AddMod(nMod, nIntI, nSizeInt, fFloatF, nSizeFloat);
			}
			if(nSizeInt > 0) free(c.hModInfo[i].nModInt);
			if(nSizeFloat > 0) free(c.hModInfo[i].fModFloat);
			if(nSizeInt > 0) free(nIntI);
			if(nSizeFloat > 0) free(fFloatF);
		}
		free(c.hModInfo);
	}
}

void CMaxView::GetClipData(int nX, int nY, int nGroup, int nFamily, ClipData* clipdata)
{
	int nX_a = m_nX, nY_a = m_nY, nBackX_a = m_nBackX, nBackY_a = m_nBackY, nFamily_a = m_nFamily, nGroup_a = m_nGroup;
	m_nX = nX; m_nY = nY; m_nBackX = nX; m_nBackY = nY; m_nFamily = nFamily; m_nGroup = nGroup;
	GetClipData(clipdata);
	m_nX = nX_a; m_nY = nY_a; m_nBackX = nBackX_a; m_nBackY = nBackY_a; m_nFamily = nFamily_a; m_nGroup = nGroup_a;
}

void CMaxView::PutClipData(int nX, int nY, int nGroup, int nFamily, ClipData* clipdata)
{
	int nX_a = m_nX, nY_a = m_nY, nBackX_a = m_nBackX, nBackY_a = m_nBackY, nFamily_a = m_nFamily, nGroup_a = m_nGroup;
	m_nX = nX; m_nY = nY; m_nBackX = nX; m_nBackY = nY; m_nFamily = nFamily; m_nGroup = nGroup;
	PutClipData(clipdata);
	m_nX = nX_a; m_nY = nY_a; m_nBackX = nBackX_a; m_nBackY = nBackY_a; m_nFamily = nFamily_a; m_nGroup = nGroup_a;
}

void CMaxView::OnCut(void)
{
	OnCopy();
	OnDelete();
}

void CMaxView::OnCopy(void)
{
	if(m_nEdit == 2 && m_bSelected)
	{
		GtkClipboard* clip = gMain->main_clip;
		GtkTargetEntry target;
		target.target = (char*)malloc(20);
		strcpy(target.target, "MaxEditSteps");
		target.flags = 0;
		target.info = 0;

		ClipData* clipdata;
		clipdata = (ClipData*)malloc(sizeof(ClipData));
		clipdata->ptrData = NULL;
		clipdata->ndata = 0;

		PutClipData(clipdata);

		gboolean c = gtk_clipboard_set_with_data(clip, &target, 1, GtkClipboardGetMaxStep, GtkClipboardClearMaxStep, clipdata);
		gtk_clipboard_store(clip);
	}
}

void CMaxView::OnPaste(void)
{
	if(m_nEdit == 2 && m_bSelected)
	{
		CMaxDocument *pDoc = GetDocument();
		GtkClipboard* clip = gMain->main_clip;
		ClipData *cdata = (ClipData*)malloc(sizeof(ClipData));

		GdkAtom atom = gdk_atom_intern("MaxEditSteps", FALSE);
		GtkSelectionData *seldata = gtk_clipboard_wait_for_contents(clip, atom);
		GtkClipboardReceivedMaxStep(clip, seldata, cdata);

		// ** DO SOMETHING WITH THIS SHIT
		GetClipData(cdata);

		if(cdata->ptrData!=NULL) free(cdata->ptrData);
		free(cdata);

		Redraw();
	}
}

void CMaxView::OnUndo(void)
{
	GetDocument()->Undo();
	ActualizateScrolls();
	Redraw();
}

void CMaxView::OnRedo(void)
{
	GetDocument()->Redo();
	ActualizateScrolls();
	Redraw();
}

void CMaxView::OnModPrimary(void)
{
}

void CMaxView::OnCutR(void)
{
	OnCopyR();
	OnDeleteR();
}

void CMaxView::OnCopyR(void)
{
	if(m_bRPressed)
	{
		GtkClipboard* clip = gMain->main_clip;
		GtkTargetEntry target;
		target.target = (char*)malloc(20);
		strcpy(target.target, "MaxEditSteps");
		target.flags = 0;
		target.info = 0;

		ClipData* clipdata;
		clipdata = (ClipData*)malloc(sizeof(ClipData));
		clipdata->ptrData = NULL;
		clipdata->ndata = 0;

		PutClipData(m_nRX, m_nRY, m_nRGroup, m_nRFamily, clipdata);

		gboolean c = gtk_clipboard_set_with_data(clip, &target, 1, GtkClipboardGetMaxStep, GtkClipboardClearMaxStep, clipdata);
		gtk_clipboard_store(clip);
	}
}

void CMaxView::OnPasteR(void)
{
	if(m_bRPressed)
	{
		CMaxDocument *pDoc = GetDocument();
		GtkClipboard* clip = gMain->main_clip;
		ClipData *cdata = (ClipData*)malloc(sizeof(ClipData));

		GdkAtom atom = gdk_atom_intern("MaxEditSteps", FALSE);
		GtkSelectionData *seldata = gtk_clipboard_wait_for_contents(clip, atom);
		GtkClipboardReceivedMaxStep(clip, seldata, cdata);

		// ** DO SOMETHING WITH THIS SHIT
		GetClipData(m_nRX, m_nRY, m_nRGroup, m_nRFamily, cdata);

		if(cdata->ptrData!=NULL) free(cdata->ptrData);
		free(cdata);

		Redraw();
	}
}

void CMaxView::OnDeleteR(void)
{
	if(m_bRPressed)
	{
		int nX_a = m_nX, nY_a = m_nY, nBackX_a = m_nBackX, nBackY_a = m_nBackY, nFamily_a = m_nFamily, nGroup_a = m_nGroup;
		m_nX = m_nRX; m_nY = m_nRY; m_nBackX = m_nRX; m_nBackY = m_nRY; m_nFamily = m_nRFamily; m_nGroup = m_nRGroup;
		if(m_nFamily == 1)
		{
			DelArrows();
		}
		if(m_nFamily == 2)
		{
			DelMod();
		}
		m_nX = nX_a; m_nY = nY_a; m_nBackX = nBackX_a; m_nBackY = nBackY_a; m_nFamily = nFamily_a; m_nGroup = nGroup_a;
		ActualizateScrolls();
		Redraw();
	}
}

void CMaxView::OnDeletendisplaceR(void)
{
	if(m_bRPressed)
	{
		int nX_a = m_nX, nY_a = m_nY, nBackX_a = m_nBackX, nBackY_a = m_nBackY, nFamily_a = m_nFamily, nGroup_a = m_nGroup;
		m_nX = m_nRX; m_nY = m_nRY; m_nBackX = m_nRX; m_nBackY = m_nRY; m_nFamily = m_nRFamily; m_nGroup = m_nRGroup;
		Deletendisplace();
		m_nX = nX_a; m_nY = nY_a; m_nBackX = nBackX_a; m_nBackY = nBackY_a; m_nFamily = nFamily_a; m_nGroup = nGroup_a;
		ActualizateScrolls();
		Redraw();
	}
}

void CMaxView::OnDeletendisplace(void)
{
	if(m_nEdit == 2 && m_bSelected)
	{
		Deletendisplace();
		ActualizateScrolls();
		Redraw();
	}
}

void CMaxView::OnDeleteGroup(void)
{
	if(m_bRPressed && m_nRFamily == 1)
	{
		CMaxDocument* pDoc = GetDocument();
		int nGroup = m_vnGroupDisp.at(m_nRGroup);
		pDoc->DelGroup(nGroup);
		m_vnGroupDisp.erase(m_vnGroupDisp.begin()+m_nRGroup);
		ActualizateScrolls();
		Redraw();
	}
}

void CMaxView::OnAddGroup(void)
{
	CMaxDocument* pDoc = GetDocument();
	pDoc->AddGroup();
	m_vnGroupDisp.push_back(pDoc->song.hStep.nSizeArrowGroups-1);
	ActualizateScrolls();
	Redraw();
}

void CMaxView::OnIncrStep(void)
{
	if(m_bRPressed && m_nRFamily == 1)
	{
		CMaxDocument* pDoc = GetDocument();
		CIncrStepDlg *dlg = new CIncrStepDlg();
		int nGroup = m_vnGroupDisp.at(m_nRGroup);
		dlg->m_nSize = 0;

		if(dlg->DoModal() != GTK_RESPONSE_ACCEPT) return;
		if(dlg->m_nSize <= 0) return;

		pDoc->ResizeSteps(nGroup, pDoc->song.hStepGroup[nGroup].hGroup.nSizeArrows+dlg->m_nSize);
		delete dlg;
		ActualizateScrolls();
		Redraw();
	}
}

void CMaxView::OnMirrorH(void)
{
	if(m_nEdit == 2 && m_bSelected)
	{
		CMaxDocument* pDoc = GetDocument();
		const int nSwitchH[] = {1,0,2,4,3};
		const int nSizeSwitchH = sizeof(nSwitchH)/sizeof(int);
		int nMinX = min(m_nX, m_nBackX),
			nMinY = min(m_nY, m_nBackY),
			nMaxX = max(m_nX, m_nBackX),
			nMaxY = max(m_nY, m_nBackY);
		int nGroup = m_vnGroupDisp.at(m_nGroup);
		int nW = nMaxX - nMinX + 1;
		int nH = nMaxY - nMinY + 1;
		int* nArrows = (int*)malloc(pDoc->m_nArrowsPerLine*sizeof(int));
		int* nPArrows = (int*)malloc(pDoc->m_nArrowsPerLine*sizeof(int));
		for(int y = nMinY; y <= nMaxY; y++)
		{
			for(int x = nMinX; x <= nMaxX; x++)
			{
				nArrows[nMaxX-x] = pDoc->GetArrow(x, y, nGroup);
				nPArrows[nMaxX-x] = pDoc->GetArrowPar(x, y, nGroup);
			}
			memcpy(pDoc->song.hStepGroup[nGroup].nArrows+y*pDoc->m_nArrowsPerLine, nArrows, pDoc->m_nArrowsPerLine*sizeof(int));
			memcpy(pDoc->song.hStepGroup[nGroup].nPArrows+y*pDoc->m_nArrowsPerLine, nPArrows, pDoc->m_nArrowsPerLine*sizeof(int));
		}
		free(nArrows);
		free(nPArrows);
		Redraw();
	}
}

void CMaxView::OnMirrorV(void)
{
	if(m_nEdit == 2 && m_bSelected)
	{
		CMaxDocument* pDoc = GetDocument();
		const int nSwitchH[] = {1,0,2,4,3};
		const int nSizeSwitchH = sizeof(nSwitchH)/sizeof(int);
		int nMinX = min(m_nX, m_nBackX),
			nMinY = min(m_nY, m_nBackY),
			nMaxX = max(m_nX, m_nBackX),
			nMaxY = max(m_nY, m_nBackY);
		int nGroup = m_vnGroupDisp.at(m_nGroup);
		int nW = nMaxX - nMinX + 1;
		int nH = nMaxY - nMinY + 1;
		int* nArrows = (int*)malloc(pDoc->m_nArrowsPerLine*sizeof(int));
		int* nPArrows = (int*)malloc(pDoc->m_nArrowsPerLine*sizeof(int));
		for(int y = nMinY; y <= nMaxY; y++)
		{
			for(int x = 0; x < pDoc->m_nArrowsPerLine; x++)
			{
				nArrows[(x/nSizeSwitchH)*nSizeSwitchH+nSwitchH[x%nSizeSwitchH]] = pDoc->GetArrow(x, y, nGroup);
				nPArrows[(x/nSizeSwitchH)*nSizeSwitchH+nSwitchH[x%nSizeSwitchH]] = pDoc->GetArrowPar(x, y, nGroup);
			}
			memcpy(pDoc->song.hStepGroup[nGroup].nArrows+y*pDoc->m_nArrowsPerLine, nArrows, pDoc->m_nArrowsPerLine*sizeof(int));
			memcpy(pDoc->song.hStepGroup[nGroup].nPArrows+y*pDoc->m_nArrowsPerLine, nPArrows, pDoc->m_nArrowsPerLine*sizeof(int));
		}
		free(nArrows);
		free(nPArrows);
		Redraw();
	}
}

void CMaxView::OnEditModPrimary(void)
{
	if(m_bRPressed && m_nRFamily == 2)
	{
		CMaxDocument* pDoc = GetDocument();

		int nMod = m_vnModDisp.at(m_nRGroup);

		int nSize, nSizeInt, nSizeFloat;
		int *nInt = NULL;
		float *fFloat = NULL;
		int nIW, nEW;
		pDoc->GetParMod(nMod, &nSize, &nSizeInt, &nSizeFloat);
		pDoc->GetModWhere(nMod, &nIW, &nEW);

		if(nSizeInt > 0) nInt = (int*)malloc(nSizeInt*sizeof(int));
		if(nSizeFloat > 0) fFloat = (float*)malloc(nSizeFloat*sizeof(float));

		switch(nMod)
		{
		case MAXSTEP_BEATSPLIT:
			nInt[1] = pDoc->song.hStep.nBeatSplit;
			break;
		case MAXSTEP_CHANGEBPM:
			fFloat[0] = pDoc->song.hStep.fBPM;
			break;
		case MAXSTEP_CHANGEVEL:
			fFloat[0] = pDoc->song.hStep.fVel;
			fFloat[1] = pDoc->song.hStep.fVel;
			break;
		case MAXSTEP_TICKCOUNT:
			nInt[1] = pDoc->song.hStep.nTickCount;
			break;
		default:
			if(nSizeInt > 0) free(nInt);
			if(nSizeFloat > 0) free(fFloat);
			return;
			break;
		}

		if(DoModModal(nMod, 0, &nInt, &fFloat) == GTK_RESPONSE_ACCEPT)
		{
			switch(nMod)
			{
			case MAXSTEP_BEATSPLIT:
				pDoc->song.hStep.nBeatSplit = nInt[1];
				break;
			case MAXSTEP_CHANGEBPM:
				pDoc->song.hStep.fBPM = fFloat[0];
				break;
			case MAXSTEP_CHANGEVEL:
				pDoc->song.hStep.fVel = fFloat[0];
				break;
			case MAXSTEP_TICKCOUNT:
				pDoc->song.hStep.nTickCount = nInt[1];
				break;
			}
		}

		if(nSizeInt > 0) free(nInt);
		if(nSizeFloat > 0) free(fFloat);
	}
}

void CMaxView::OnStepType(void)
{
	CTypeNoteDlg* dlg = new CTypeNoteDlg;
	dlg->m_nSel = m_nAddType;
	dlg->m_nValue = m_nAddPar;
	dlg->m_nArg1 = 0;
	dlg->m_nArg2 = 0;
	dlg->m_nType = 0;
	if(m_nAddType == 21)
	{
		// THE PACKING FOR THIS VERSION IS 5-5-6 COMPLETING 16 BITS
		// Previous version is 4-4-8
		dlg->m_nArg1 = m_nAddPar & 0x1F;
		dlg->m_nArg2 = (m_nAddPar >> 5) & 0x1F;
		dlg->m_nType = (m_nAddPar >> 10) & 0x3F;
	}
	if(dlg->DoModal() == GTK_RESPONSE_ACCEPT)
	{
		m_nAddType = dlg->m_nSel;
		if(m_nAddType == 21)
		{
			// THE PACKING FOR THIS VERSION IS 5-5-6 COMPLETING 16 BITS
			// Previous version is 4-4-8
			m_nAddPar = (dlg->m_nArg1 & 0x1F) |
						((dlg->m_nArg2 & 0x1F) << 5) |
						((dlg->m_nType & 0x3F) << 10);
		}
		else
		{
			m_nAddPar = dlg->m_nValue;
		}
	}
	delete dlg;
}

void CMaxView::OnStepSelectionSetType(void)
{
	if(m_nEdit == 2 && m_bSelected && m_nFamily == 1)
	{
		CMaxDocument* pDoc = GetDocument();
		m_nAddType = pDoc->GetArrow(m_nX, m_nY, m_vnGroupDisp.at(m_nGroup));
		m_nAddPar = pDoc->GetArrowPar(m_nX, m_nY, m_vnGroupDisp.at(m_nGroup));
	}
}

void CMaxView::OnStepPreview(void)
{
}

void CMaxView::OnSongSplash(void)
{
	GtkWidget *dialog;
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new ("Open Splash",
                                          GTK_WINDOW(gMain->main_window),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "PNG archives");
    gtk_file_filter_add_pattern(filter, "*.png");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	TCHAR tchCur[MAX_PATH];
	char chCur[MAX_PATH];
	GetMaxCurrentDirectory(tchCur);
	Max_strcpy(chCur, tchCur);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), chCur);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

		char *folder = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (dialog));
		Max_strcpy(tchCur, folder);
		SetMaxCurrentDirectory(tchCur);
		g_free(folder);

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		char core[MAX_PATH];
		Max_strcpy_utf8(core,MAX_PATH,filename,0);
		g_free (filename);

		m_strSplash.Format(_T("%s"), core);
    }

	gtk_widget_destroy (dialog);
}

void CMaxView::OnSongMusic(void)
{
	GtkWidget *dialog;
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new ("Open Music",
                                          GTK_WINDOW(gMain->main_window),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "MP3 archives");
    gtk_file_filter_add_pattern(filter, "*.mp3");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	TCHAR tchCur[MAX_PATH];
	char chCur[MAX_PATH];
	GetMaxCurrentDirectory(tchCur);
	Max_strcpy(chCur, tchCur);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), chCur);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

		char *folder = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (dialog));
		Max_strcpy(tchCur, folder);
		SetMaxCurrentDirectory(tchCur);
		g_free(folder);

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		char core[MAX_PATH];
		Max_strcpy_utf8(core,MAX_PATH,filename,0);
		g_free (filename);

		m_strMusic.Format(_T("%s"), core);
    }

	gtk_widget_destroy (dialog);
}

void CMaxView::OnSongBGA(void)
{
	GtkWidget *dialog;
    GtkFileFilter *filter;

    dialog = gtk_file_chooser_dialog_new ("Open BGA",
                                          GTK_WINDOW(gMain->main_window),
                                          GTK_FILE_CHOOSER_ACTION_OPEN,
                                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                          GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                          NULL);

    filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "AVI archives");
    gtk_file_filter_add_pattern(filter, "*.avi");
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog), filter);

	TCHAR tchCur[MAX_PATH];
	char chCur[MAX_PATH];
	GetMaxCurrentDirectory(tchCur);
	Max_strcpy(chCur, tchCur);
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (dialog), chCur);

    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;

		char *folder = gtk_file_chooser_get_current_folder(GTK_FILE_CHOOSER (dialog));
		Max_strcpy(tchCur, folder);
		SetMaxCurrentDirectory(tchCur);
		g_free(folder);

        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));

		char core[MAX_PATH];
		Max_strcpy_utf8(core,MAX_PATH,filename,0);
		g_free (filename);

		m_strBGA.Format(_T("%s"), core);
    }

	gtk_widget_destroy (dialog);
}

void CMaxView::OnSongOptions(void)
{
	CComandOptionsDlg* dlg = new CComandOptionsDlg;
	memcpy(&dlg->m_arg, &arg, sizeof(ARGSTEP));
	if(dlg->DoModal() == GTK_RESPONSE_ACCEPT)
	{
		memcpy(&arg, &dlg->m_arg, sizeof(ARGSTEP));
		fVel = arg.fUserVel;
	}

	delete dlg;

	ActualizateScrolls();
	Redraw();
}

void CMaxView::OnSongFullscreen(GtkToggleAction *toolbutton)
{
	m_bFull = gtk_toggle_action_get_active(toolbutton)?true:false;
}

void CMaxView::OnStepInfo(void)
{
}

void CMaxView::OnSetCurStepType(int nStep)
{
	if(nStep < 2 && nStep > 4)
	{
		m_nEdit = 0;
	}
	else
	{
		m_nAddType = nStep;
		m_nEdit = 1;
	}
	ActualizateStepRadio();
}

#include "GLFont2.h"
bool g_bSimulate = false;
CFont2 *fmtArial;
DWORD dwLastTickFPS = 0;
int nFrames = 0;

void CMaxView::OnStepSimulate(void)
{
	static gint func_ref = 0;
	if(!g_bSimulate)
	{
		// Initialization
		memset(&piuconfig, 0, sizeof(PIUCONFIG));
		for(int x = 0; x < 10; x++) for(int y = 0; y < 4; y++) piuconfig.nWhereButtons[x][y] = -2;

		// Asignation
		for(int x = 0; x < 10; x++)
		{
			piuconfig.nWhereButtons[x][0] = -1; // All first set to keyboard
			piuconfig.nWhereButtons[x][1] = -1; // All first set to keyboard
		}

		piuconfig.nButtons[0][0] = SDLK_z; // DL 1
		piuconfig.nButtons[1][0] = SDLK_q; // UL 1
		piuconfig.nButtons[2][0] = SDLK_s; // CE 1
		piuconfig.nButtons[3][0] = SDLK_e; // UR 1
		piuconfig.nButtons[4][0] = SDLK_c; // DL 1
		piuconfig.nButtons[5][0] = SDLK_v; // DL 1
		piuconfig.nButtons[6][0] = SDLK_r; // UL 1
		piuconfig.nButtons[7][0] = SDLK_g; // CE 1
		piuconfig.nButtons[8][0] = SDLK_y; // UR 1
		piuconfig.nButtons[9][0] = SDLK_n; // DL 1

		piuconfig.nButtons[0][1] = SDLK_KP_1; // DL 2
		piuconfig.nButtons[1][1] = SDLK_KP_7; // UL 2
		piuconfig.nButtons[2][1] = SDLK_KP_5; // CE 2
		piuconfig.nButtons[3][1] = SDLK_KP_9; // UR 2
		piuconfig.nButtons[4][1] = SDLK_KP_3; // DL 2
		piuconfig.nButtons[5][1] = SDLK_KP_1; // DL 2
		piuconfig.nButtons[6][1] = SDLK_KP_7; // UL 2
		piuconfig.nButtons[7][1] = SDLK_KP_5; // CE 2
		piuconfig.nButtons[8][1] = SDLK_KP_9; // UR 2
		piuconfig.nButtons[9][1] = SDLK_KP_3; // DL 2

		TCHAR chCore[MAX_PATH], chCur[MAX_PATH];
		GetMaxProcessDirectory(chCore);
		GetMaxCurrentDirectory(chCur);
		SetMaxCurrentDirectory(chCore);
		if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0 )
		{
			printf("Unable to initialize SDL: %s\n", SDL_GetError());
			return;
		}

		g_gmMain = new CGraphMain();
		g_gmMain->Set(1);

		bool bFull = m_bFull;
		int nResWidth = 800, nResHeight = 600;
		if(bFull) _printf(_T("Colocando el modo de video a %dx%d FullScreen\n"), nResWidth, nResHeight);
		else _printf(_T("Colocando el modo de video a %dx%d No FullScreen\n"), nResWidth, nResHeight);

		g_gmMain->Initialize(nResWidth, nResHeight, 0, bFull);
		g_gmMain->Clear(0.0, 0.0, 0.0, 1.0);
		g_gmMain->Frustum(-40.0,40.0,-30.0,30.0,50.0,100.0);
		g_gtNoTex = new CGraphTexture();
		fmtArial = new CFont2();
		fmtArial->LoadFont(_T("Arial.fnt"));

		_printf(_T("Inicializando Sistema de sonido\n"));
		g_ssMain = new CSoundSys();
		if(!g_ssMain->Initialize()) return;

		for(int x = 0; x < MAX_CHANNELS; x++)
		{
			channels[x] = new CSoundChannel();
			g_ssMain->GetChannel(x, channels[x]);
		}

		_printf(_T("Inicializando Motor de entrada\n"));
		if(!CStdInput::LaunchInput())
		{
			printf("Unable to create Input \n");
			return;
		}

		gdouble d = gtk_adjustment_get_value(m_gtkVAdj);

		// ***************************************************
		// StepSmu Init
		CMaxStepSmu::LoadResources(m_strMusic, m_strBGA, m_strSplash);
		g_gmMain->BeginScene();
		CMaxStepSmu::DrawSplash();
		g_gmMain->EndScene();
		g_gmMain->SwapBuffers();

		g_stepsmu = new CMaxStepSmu();
		g_stepsmu->pDoc = GetDocument();
		g_stepsmu->SetArg(&arg);
		if(d != 0.0)
		{
			float fBeatEdit = float(d)/MAXEDIT_VIEW_PIXELS_PER_BEAT/fVel;
			__int64 nTime = g_stepsmu->BeatToTime(GetDocument()->BeatArrowTime(fBeatEdit));
			g_stepsmu->m_nTime = nTime+GetDocument()->song.hStep.nOffset;
		}

		g_stepsmu->StepCompile();

		if(d != 0.0)
		{
			float fBeatEdit = float(d)/MAXEDIT_VIEW_PIXELS_PER_BEAT/fVel;
			__int64 nTime = g_stepsmu->BeatToTime(GetDocument()->BeatArrowTime(fBeatEdit));
			g_stepsmu->m_nTime = nTime+GetDocument()->song.hStep.nOffset;
			CMaxStepSmu::LaunchSong((unsigned int)nTime);
		}
		else
		{
			CMaxStepSmu::LaunchSong();
		}
		// ***************************************************

		dwLastTickFPS = dwTick = dwLastTick = SDL_GetTicks();
		nFrames = 0;
		// Init the loop context acording to drawing and signals
		func_ref = g_timeout_add(1000/60, (GSourceFunc) Draw_MainView, (gpointer)gMain->main_window);

		g_bSimulate = true;
		SetMaxCurrentDirectory(chCur);
	}
	else
	{
		// ***************************************************
		// StepSmu delete
		g_stepsmu->StepDestroy();
		CMaxStepSmu::DelResources();
		delete g_stepsmu;
		// ***************************************************

		fmtArial->Delete();
		delete fmtArial;

		for(int x = 0; x < MAX_CHANNELS; x++)
		{
			delete channels[x];
		}

		g_ssMain->Destroy();
		g_gmMain->Destroy();
		delete g_ssMain;
		delete g_gmMain;

		CStdInput::StopInput();

		SDL_Quit();
		g_bSimulate = false;
		g_source_remove(func_ref);
	}

}

int ProcEvent(int nKey, int nEvent)
{
	if(nKey == KEY_QUIT) return 0;
	g_stepsmu->Event_View(nEvent, nKey);
	return 1;
}

gboolean Draw_MainView(GtkWidget *window)
{
	static bool bDrawing = false;
	if(bDrawing || !g_bSimulate) return TRUE;
	bDrawing = true;
	int nKey, nEvent;
	while(CStdInput::PollEvent(&nKey, &nEvent))
	{
		if(!ProcEvent(nKey, nEvent))
		{
			g_mvView.at(gMain->m_uCurView)->OnStepSimulate();
			bDrawing = false;
			return TRUE;
		}
	}

	dwTick = SDL_GetTicks();

     nFrames++;
     if ( dwTick - dwLastTickFPS >= 1000 ){ // If last prinf() was more than 1 sec ago
         // printf and reset timer
         printf("%d frame/s \n", nFrames);
         nFrames = 0;
         dwLastTickFPS += 1000;
     }



	g_gmMain->Clear(0.0, 0.0, 0.0, 1.0);

	if(!g_gmMain->BeginScene()) return TRUE;

	CMaxStepSmu::UpdateSong();
	CMaxStepSmu::DrawBGA();

	g_stepsmu->Draw();

	g_gmMain->EndScene();
	g_gmMain->SwapBuffers();
	dwLastTick = dwTick;

	bDrawing = false;
	return TRUE;
}
