#ifndef MAXEDIT_VIEW
#define MAXEDIT_VIEW

#include "stdafx.h"
#include "main_document.h"
#include "argstep.h"
#include "MaxStepSmu.h"

#define MAXEDIT_VIEW_PIXELS_PER_BEAT	24
#define MAXEDIT_VIEW_PIXELS_WIDTH		24
#define MAXEDIT_VIEW_PIXELS_PADDING		14
#define MAXEDIT_VIEW_PIXELS_YFREE		150
#define MAXEDIT_SHOW_MEASURE_PADDING		18
#define MAXEDIT_SHOW_MEASURE_YPADDING		8
#define MAXEDIT_SHOW_MEASURE_SIZE		13
#define MAXEDIT_SHOW_BEAT_PADDING		1
#define MAXEDIT_SHOW_BEAT_YPADDING		8
#define MAXEDIT_SHOW_BEAT_SIZE		13
#define MAXEDIT_DIST_SHOWGROUPS		24

#define MAXEDIT_VIEW_MOD_PADDING		14
#define MAXEDIT_VIEW_MOD_WIDTH		50
#define MAXEDIT_VIEW_MOD_HEIGHT		14
#define MAXEDIT_VIEW_MOD_XFREE		50
#define MAXEDIT_VIEW_MOD_DIST		60
#define MAXEDIT_VIEW_MOD_DISTEXTLINE		30
#define MAXEDIT_SHOW_MOD_SIZE		10
#define MAXEDIT_SHOW_MOD_YPADDING		10

#define MAXEDIT_IM_BASE_TAP	0
#define MAXEDIT_IM_BASE_LONG	5
#define MAXEDIT_IM_BASE_LONG_BOTTOM	10
#define MAXEDIT_IM_BASE_ITEMS	15

struct _ClipData
{
	void* ptrData;
	size_t ndata;
};

typedef _ClipData ClipData;

class CMaxView
{
public:
    CMaxView();
    ~CMaxView();

    uint16_t m_uDoc;
protected:

public:
    GtkWidget * m_gtkScrolledWindow;
	GtkWidget * m_gtkDrawingArea;
	GtkAdjustment * m_gtkHAdj;
	GtkAdjustment * m_gtkVAdj;

	cairo_t* m_cra;
	cairo_surface_t *m_sra;
	gint m_xa;
	gint m_ya;
	bool m_bRestoreDraw;

	gint m_xsel;
	gint m_ysel;
	gint m_wsel;
	gint m_hsel;
	bool m_bSelDraw;
	bool m_bDelDraw;
	bool m_bArrowPreDraw;
	bool m_bDrawing;
	bool m_bSelected;	// Is something selected in Edit 2?

	bool m_bRestoreDrawMouse;
	bool m_bModPreDraw;

	int m_nTest;

	// Custom func
public:

	// Functional
	void Init(void);
	void ActualizateScrolls(void);
	void Redraw(void);
	void ActualizateStepRadio(void);
	int SetGuide(int y, int nGuide, cairo_t* cr);
	void DrawImageFromIM(cairo_t* cr, int nIndex, int nX, int nY, int sX, int sY);
	void RestoreDraw(void);
	void SaveDraw(int x, int y, int width, int height);
	void SelDraw(int x, int y, int width, int height);
	void DelDraw(int x, int y, int width, int height);
	void ArrowPreDraw(int nX, int nY);
	void ModPreDraw(int nY, int nRefMod);
	void RestoreDrawExpose(cairo_t* cr);
	void SelDrawExpose(cairo_t* cr);
	void DelDrawExpose(cairo_t* cr);
	void ArrowPreDrawExpose(cairo_t* cr, int nX, int nY, int nBackX, int nBackY, int nRefGroup);
	void ModPreDrawExpose(cairo_t* cr, int nY, int nBackY, int nRefGroup);
	void DrawArrow(cairo_t* cr, int x, float fy, float fny, int nGroup, int nArrow);
	void DrawSelection(cairo_t* cr, int nX, int nY, int nBackX, int nBackY, int nGroup, int nFamily);
	void DrawMod(cairo_t* cr, LPCSTR cFormat, int* nPlace, int* data, int nSize, int nRefMod);
	void DrawModExt(cairo_t* cr, LPCSTR cIFormat, LPCSTR cEFormat, int* nI, int* nE, int* Idata, int* Edata, int nSize, int nRefMod);
	void DrawMod(cairo_t* cr, LPCSTR cFormat, int* nPlace, float* data, int nSize, int nRefMod);
	void DrawModExt(cairo_t* cr, LPCSTR cIFormat, LPCSTR cEFormat, int* nI, int* nE, float* Idata, float* Edata, int nSize, int nRefMod);

	// Actions (require Undo)
	void PutClipData(ClipData* clipdata);
	void GetClipData(ClipData* clipdata);
	int AddMod(void);
	int DelMod(void);
	int AddArrows(void);
	int AddFArrows(void);
	int DelArrows(void);

	void GetClipData(int nX, int nY, int nGroup, int nFamily, ClipData* clipdata); // Not necesary to undo
	void PutClipData(int nX, int nY, int nGroup, int nFamily, ClipData* clipdata); // Not necesary to undo

	// Utils
	DWORD Arrow2Pixel(int nArrow, int nGroup);
	float Arrow2Beat(int nArrow, int nGroup);

	DWORD Beat2Pixel(float fBeat);
	int Beat2Arrow(float fBeat);
	int Vel2GuideMaster(float fVel);
	int GetMaxSizeArrows(void);
	int GetModExt(int nMod);

	int Click2Arrow(int x, int y, int* nArrow, int* nLine, int* nRefGroup);
	int Click2ArrowAsociated(int x, int y, int nRefGroup, int* nArrow, int* nLine);
	int Click2Mod(int x, int y, int* nArrow, int* nLine, int* nRefGroup);
	int Click2ModAsociated(int x, int y, int nRefGroup, int* nArrow, int* nLine);
	int Click2Family(int x, int y, int* nX, int* nY, int* nGroup, int* nFamily);
	int Click2FamilyAsociated(int x, int y, int nGroup, int nFamily,  int* nX, int* nY);

	bool ClickInSelection(int mx, int my);
	void GetSelectionCoords(int nX, int nY, int nBackX, int nBackY, int nGroup, int nFamily, int *x, int *y, int *w, int *h);

	int VerificateMod(int nMod, int y, int by);
	int VerificateMod(int nMod, int y, int by, int i);

	gint DoModModal(int nMod, int nVer, int** nInt, float** fFloat);

	void RegArrows(void);
	void RegMods(void);
	void Deletendisplace(void);

	// Custom var
public:
	bool m_bInitialDraw;
	bool m_bSimulation;
	float fVel;
	int m_nBackX;		// Position (accord family)
	int m_nBackY;		// Position (accord family)
	int m_nX;		// Position (accord family)
	int m_nY;		// Position (accord family)
	int m_nGroup;		// Group (accord family, arrows-> group, mod-> mod)
	int m_nFamily;		// Family (0-nothing, 1-arrows, 2-mod)
	int m_nRX;		// Position (accord family)
	int m_nRY;		// Position (accord family)
	int m_nRGroup;		// Group (accord family, arrows-> group, mod-> mod)
	int m_nRFamily;		// Family (0-nothing, 1-arrows, 2-mod)
	bool m_bRPressed;
	bool m_bPressed;
	bool m_bPreviewDraw;
	bool m_bFull;	// Fullscreen for StepSmu
	CString m_strMusic;
	CString m_strSplash;
	CString m_strBGA;
	CString m_strFile;
	CString m_strLights;
	ARGSTEP arg;
	int m_nEdit;
	int m_nAddType;
	int m_nAddPar;
	vector<int> m_vnGroupDisp;
	vector<int> m_vnModDisp;

	// signals
	gboolean OnExpose(cairo_t* cr);
	void OnLButtonUp(int x, int y);
	void OnRButtonUp(int x, int y, GdkEventButton* event);
	void OnLButtonDown(int x, int y);
	void OnRButtonDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnActionStepSize(void);
	void OnActionStepPreferences(void);
	void OnStepLongStep(GtkToggleAction *action);
	void OnStepSingleStep(GtkToggleAction *action);
	void OnStepSelectStep(GtkToggleAction *action);
	void OnStepEraseStep(GtkToggleAction *action);
	void OnEnterNotify(void);
	void OnLeaveNotify(void);
	void OnActualizate(void);
	void OnDelete(void);
	void OnCut(void);
	void OnCopy(void);
	void OnPaste(void);
	void OnUndo(void);
	void OnRedo(void);
	void OnModPrimary(void);

	void OnCutR(void);
	void OnCopyR(void);
	void OnPasteR(void);
	void OnDeleteR(void);

	void OnDeletendisplaceR(void);
	void OnDeletendisplace(void);
	void OnDeleteGroup(void);
	void OnAddGroup(void);
	void OnIncrStep(void);
	void OnMirrorH(void);
	void OnMirrorV(void);
	void OnEditModPrimary(void);

	void OnStepType(void);
	void OnStepSelectionSetType(void);
	void OnStepPreview(void);
	void OnStepSimulate(void);
	void OnSongSplash(void);
	void OnSongMusic(void);
	void OnSongBGA(void);
	void OnSongOptions(void);
	void OnSongFullscreen(GtkToggleAction *toolbutton);
	void OnStepInfo(void);

	void OnSetCurStepType(int nStep);

	CMaxDocument* GetDocument(void);
};

extern CMaxStepSmu *g_stepsmu;

gboolean Draw_MainView(GtkWidget *window);
extern bool g_bSimulate;


#endif // MAXEDIT_VIEW
