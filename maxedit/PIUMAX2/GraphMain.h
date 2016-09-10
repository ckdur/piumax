#pragma once

#include "stdafx.h"
#include "defcomp.h"
#include "GraphTexture.h"

struct tri_vertex{
    float x, y, z, w;
    DWORD colour;
	float tu,tv,tw;
};

#if defined(PIUMAX_DX)
struct primitive{
	LPDIRECT3DVERTEXBUFFER9 d3dbuf;
	D3DPRIMITIVETYPE prim_type;
	UINT nElem;
};
#endif

class CGraphMain
{
public:
	CGraphMain(void);
	~CGraphMain(void);

	// Variables
	int m_nGestor;
	int m_nWidth;
	int m_nHeight;
	int m_nBits;
	bool m_bFull;
	bool m_bWindow; // Window / Context initializated

	// System-based variables
	int m_nCurrentMatrix;
	int m_nCurPrimitiveType;
	int m_nStackVertex;

	// Texture specific variables
	bool m_bFlipY;

	// sdl WINDOW
	SDL_Window *window;

#if defined(PIUMAX_DX)
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_d3ddev;
	LPD3DXMATRIXSTACK m_d3dmat[10];

	// ** Vertex Buffer
	int size_vertex_buffer;
	vector< tri_vertex > vertex_buffer;

	// ** Vertex & Other count
	int size_vertex;
	int size_texcoord;
	double dcx, dcy, dcz, dcrhw;
	double dctu, dctv, dctw;
	double dr, dg, db, da;
#endif // PIUMAX_DX

	// Funciones de inicialización
	int Initialize(int nWidth, int nHeight, int nBits, bool bFullscreen);
	int Set(int nGestor);
	int InitializeWindow(void);
	void DeleteWindow(void);
	void Destroy(void);
	void LookAt(double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz);
	void Frustum(double left, double right, double bottom, double top, double znear, double zfar);
	void MatrixMode(int nMode);
	void LoadIdentity(void);
	void Ortho(double left, double right, double bottom, double top, double znear, double zfar);
	void Perspective(double fovy, double aspect, double znear, double zfar);
	int Clear(double r, double g, double b, double a);
	void SwapBuffers(void);
	int BeginScene(void);
	void EndScene(void);
	int Begin(int nEnum);
	int End(void);
	void Translate(double dx, double dy, double dz);
	void Rotate(double dtheta, double dx, double dy, double dz);
	void Scale(double dx, double dy, double dz);
	int PushMatrix(void);
	int PopMatrix(void);
	int Vertex3d(double dx, double dy, double dz);
	int TexCoord2d(double du, double dv);
	int TexCoord3d(double du, double dv, double dw);
	void Color4d(double r, double g, double b, double a);
	int CreateTexture(int nMipMapLevels, int nFormat, int nWidth, int nHeight, CGraphTexture* m_gtArray);
	int BindTexture(CGraphTexture* gtTex);
	int BlendFunc(int nSrc, int nDst);

private:
#ifdef PIUMAX_OGL
	GLenum pmgGlTraduceBlend(int arg);
	SDL_GLContext glcontext;
#endif // PIUMAX_OGL

#ifdef PIUMAX_DX
	DWORD pmgDXTraduceBlend(int arg);
#endif // PIUMAX_DX
};

enum PMG_MATRICES
{
	PMG_MODELVIEW,
	PMG_PROJECTION,
	PMG_TEXTURE,
	PMG_TEXTURE1,
	PMG_TEXTURE2,
	PMG_TEXTURE3,
	PMG_TEXTURE4,
	PMG_TEXTURE5,
	PMG_TEXTURE6,
	PMG_TEXTURE7
};

enum PMG_PRIMITIVETYPES
{
	PMG_POINTS, 
	PMG_LINES,
	PMG_LINE_STRIP,
	PMG_LINE_LOOP,
	PMG_TRIANGLES,
	PMG_TRIANGLE_STRIP,
	PMG_TRIANGLE_FAN,
	PMG_QUADS,
	PMG_QUAD_STRIP,
	PMG_POLYGON
};

enum PMG_BLEND
{
	PMG_ZERO,
	PMG_ONE,
	PMG_SRC_COLOR,
	PMG_ONE_MINUS_SRC_COLOR,
	PMG_DST_COLOR,
	PMG_ONE_MINUS_DST_COLOR,
	PMG_SRC_ALPHA,
	PMG_ONE_MINUS_SRC_ALPHA,
	PMG_DST_ALPHA,
	PMG_ONE_MINUS_DST_ALPHA
};

#if defined(PIUMAX_DX)
const DWORD tri_fvf=D3DFVF_XYZW|D3DFVF_DIFFUSE|D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE3(0);
#endif