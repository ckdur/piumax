
#include "GraphMain.h"

CGraphMain::CGraphMain(void)
{
	m_nGestor = 0;	// NoDraw
	m_nWidth = 0;
	m_nHeight = 0;
	m_nBits = 0;
	m_bFull = true;
	m_bWindow = false;
	m_nCurrentMatrix = 0;
	m_nCurPrimitiveType = 0;
	m_nStackVertex = 0;
	m_bFlipY = false;
#ifdef PIUMAX_DX
	m_d3d = NULL;
	m_d3ddev = NULL;
	for(int x = 0; x < 10; x++) m_d3dmat[x] = NULL;
	size_vertex_buffer = 0;
	vertex_buffer.clear();
	size_vertex = 0;
	size_texcoord = 0;
	dcx = 0; dcy = 0; dcz = 0; dcrhw = 0;
	dctu = 0; dctv = 0; dctw = 0;
	dr = 0; dg = 0; db = 0; da = 1;
#endif // PIUMAX_DX
}

CGraphMain::~CGraphMain(void)
{
	Destroy();
}

int CGraphMain::Initialize(int nWidth, int nHeight, int nBits, bool bFullscreen)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_nBits = nBits;
	m_bFull = bFullscreen;
	if(!m_bWindow) if(!InitializeWindow())
	{
		MAIN_CRASH(_T("ERROR: Tratando de inicializar ventana / consola"), 0x32);
		return 0;
	}
	if(m_nGestor == 1)
	{
#ifndef PIUMAX_OGL
		MAIN_CRASH(_T("Tratando de inicializar Gestor OpenGL cuando no existe para este build"), 0x30);
		return 0;
#else
		_printf(_T("Gestor de graficos OpenGL, iniciando... "));
		glViewport(0, 0, m_nWidth, m_nHeight);
		glEnable(GL_TEXTURE_2D);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifndef PIUMAX_DX
		MAIN_CRASH(_T("Tratando de inicializar Gestor DirectX cuando no existe para este build"), 0x31);
		return 0;
#else

		_printf(_T("Gestor de graficos Direct3D 9, iniciando... \n"));

		m_d3d = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D interface

		if(m_d3d == NULL)
		{
			_printf(_T("(ERROR): GraphMain::Initialize, cannot create Direct3D interface\n"));
			return 0;
		}

		D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information

		ZeroMemory(&d3dpp, sizeof(d3dpp));    // clear out the struct for use
		d3dpp.Windowed = bFullscreen?FALSE:TRUE;    // program windowed, not fullscreen
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;    // discard old frames
#ifdef _XBOX
		d3dpp.hDeviceWindow = NULL;
#else
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		if( SDL_GetWindowWMInfo(window,  &info) < 0 )
		{
			DeleteWindow();
			return 0;
		}
		d3dpp.hDeviceWindow = info.info.win.window;
		//d3dpp.hDeviceWindow = GetActiveWindow(); //info.window;
#endif

		d3dpp.EnableAutoDepthStencil = 1;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;    // set the back buffer format to 32-bit
		d3dpp.BackBufferWidth = nWidth;    // set the width of the buffer
		d3dpp.BackBufferHeight = nHeight;    // set the height of the buffer

		// create a device class using this information and information from the d3dpp stuct
		HRESULT hr;
		
		hr = m_d3d->CreateDevice(D3DADAPTER_DEFAULT,
						  D3DDEVTYPE_HAL,
						  d3dpp.hDeviceWindow,
						  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
						  &d3dpp,
						  &m_d3ddev);
		if(FAILED(hr))
		{
			_printf(_T("(ERROR): GraphMain::Initialize, cannot create Direct3D Device\n"));
			return 0;
		}

		// Set Viewport
		D3DVIEWPORT9 pViewPort;
		pViewPort.X = 0;
		pViewPort.Y = 0;
		pViewPort.Width = m_nWidth;
		pViewPort.Height = m_nHeight;
		pViewPort.MinZ = -50.0f;
		pViewPort.MaxZ = -100.0f;
		m_d3ddev->SetViewport(&pViewPort);

		// Clear color & depth still no used
		m_d3ddev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		m_d3ddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		
		m_d3ddev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

		m_d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		m_d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//maximum ambient light (NEW)
		m_d3ddev->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
		//lighting disabled (NEW)
		m_d3ddev->SetRenderState(D3DRS_LIGHTING,false);
		//don't cull backside (NEW)
		m_d3ddev->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);//D3DCULL_NONE);

		for(int x = 0; x < 10; x++)
		{
			D3DXCreateMatrixStack(0, &m_d3dmat[x]);
			m_d3dmat[x]->LoadIdentity();
		}

		m_d3ddev->SetFVF(tri_fvf);

		for(unsigned i = 0;i < 8;++i)
		 {
		 m_d3ddev->SetSamplerState(i,
											D3DSAMP_MINFILTER,
											D3DTEXF_LINEAR);
		 m_d3ddev->SetSamplerState(i,
											D3DSAMP_MAGFILTER,
											D3DTEXF_LINEAR);
		 m_d3ddev->SetSamplerState(i,
											D3DSAMP_MIPFILTER,
											D3DTEXF_ANISOTROPIC);
		 m_d3ddev->SetSamplerState(i,
											D3DSAMP_MAXANISOTROPY,
											0);
		 m_d3ddev->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		 m_d3ddev->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		 }


#endif // PIUMAX_DX
	}
	return 1;
}

void CGraphMain::Destroy(void)
{
	if(m_nGestor == 1)
	{
#ifndef PIUMAX_OGL
		MAIN_CRASH(_T("Tratando de destruir Gestor OpenGL cuando no existe para este build"), 0x30);
		return;
#else
		DeleteWindow();
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifndef PIUMAX_DX
		MAIN_CRASH(_T("Tratando de destruir Gestor DirectX cuando no existe para este build"), 0x31);
		return;
#else

		vertex_buffer.clear();

		if(m_d3ddev)
		{
			m_d3ddev->Release();
			m_d3ddev = NULL;
		}
		if(m_d3d)
		{
			m_d3d->Release();
			m_d3d = NULL;
		}
#endif // PIUMAX_DX
	}
	if(!m_bWindow) DeleteWindow();
}


int CGraphMain::Set(int nGestor)
{
	if(nGestor == 1)
	{
#ifndef PIUMAX_OGL
		_printf(_T("This PIU MaX build cannot use OpenGL\nError to set Graph Gestor"));
		return 0;
#endif // PIUMAX_OGL
	}
	if(nGestor == 2)
	{
#ifndef PIUMAX_DX
		_printf(_T("This PIU MaX build cannot use DirectX\nError to set Graph Gestor"));
		return 0;
#endif // PIUMAX_DX
	}
	m_nGestor = nGestor;
	return 1;
}

int CGraphMain::InitializeWindow(void)
{
	Uint32 uflags = 0;
		uflags |= (m_bFull?SDL_WINDOW_FULLSCREEN :0);

#if defined(PIUMAX_OGL)
	if(m_nGestor==1)
	{
		uflags |= SDL_WINDOW_OPENGL;
		_printf(_T("Initializing with OPENGL\n"));
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
#if defined(PIUMAX_DX)
	if(m_nGestor==2)
	{
		uflags |= SDL_SWSURFACE;
		_printf(_T("Initializing with DIRECTX\n"));
	}
#endif

	window = SDL_CreateWindow(
		MAIN_TITLE, SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , m_nWidth, m_nHeight, 
		uflags);

#if defined(PIUMAX_OGL)
	// Create an OpenGL context associated with the window.
	glcontext = SDL_GL_CreateContext(window);
#endif

	m_bWindow = true;
	return 1;
}

void CGraphMain::DeleteWindow(void)
{
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	m_bWindow = false;
}


// ** Comienzo de las funciones especificas

/*
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
#endif // PIUMAX_DX
	}
*/

void CGraphMain::MatrixMode(int nMode)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		GLenum mode = GL_MODELVIEW;
		switch(nMode)
		{
			case PMG_MODELVIEW: mode = GL_MODELVIEW; break;
			case PMG_PROJECTION: mode = GL_PROJECTION; break;
			default: mode = GL_TEXTURE; break;
		}
		m_nCurrentMatrix = nMode;
		glMatrixMode(mode);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		m_nCurrentMatrix = nMode;
#endif // PIUMAX_DX
	}
}


void CGraphMain::LoadIdentity(void)
{
		if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glLoadIdentity();
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX* view_matrix;

		m_d3dmat[m_nCurrentMatrix]->LoadIdentity();
		view_matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;

		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,view_matrix);
#endif // PIUMAX_DX
	}
}

void CGraphMain::LookAt(double eyex, double eyey, double eyez,
						double centerx, double centery, double centerz,
						double upx, double upy, double upz)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		gluLookAt(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXVECTOR3 eye_vector,lookat_vector,up_vector;
		D3DXMATRIX *view_matrix = NULL, *matrix;

		view_matrix = new D3DXMATRIX();

		eye_vector=D3DXVECTOR3((FLOAT)eyex, (FLOAT)eyey, (FLOAT)eyez);
		lookat_vector=D3DXVECTOR3((FLOAT)centerx, (FLOAT)centery, (FLOAT)centerz );
		up_vector=D3DXVECTOR3((FLOAT)upx, (FLOAT)upy, (FLOAT)upz);

		D3DXMatrixLookAtRH(view_matrix,
						  &eye_vector,
						  &lookat_vector,
						  &up_vector);

		m_d3dmat[m_nCurrentMatrix]->MultMatrix(view_matrix);
		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;

		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);

		delete view_matrix;
#endif // PIUMAX_DX
	}
}

void CGraphMain::Frustum(double left, double right, double bottom, double top, double znear, double zfar)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glFrustum(left, right, bottom, top, znear, zfar);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX *view_matrix = NULL, *matrix;

		view_matrix = new D3DXMATRIX();

		D3DXMatrixPerspectiveOffCenterRH(view_matrix, (FLOAT)left, (FLOAT)right, (FLOAT)bottom, (FLOAT)top, (FLOAT)znear, (FLOAT)zfar);

		m_d3dmat[m_nCurrentMatrix]->MultMatrix(view_matrix);
		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;

		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);

		D3DVIEWPORT9 pViewPort;
		pViewPort.X = 0;
		pViewPort.Y = 0;
		pViewPort.Width = m_nWidth;
		pViewPort.Height = m_nHeight;
		pViewPort.MinZ = -float(znear);
		pViewPort.MaxZ = -float(zfar);
		m_d3ddev->SetViewport(&pViewPort);

		delete view_matrix;
#endif // PIUMAX_DX
	}
}


void CGraphMain::Ortho(double left, double right, double bottom, double top, double znear, double zfar)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glOrtho(left, right, bottom, top, znear, zfar);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX *view_matrix = NULL, *matrix;

		view_matrix = new D3DXMATRIX();

		D3DXMatrixOrthoOffCenterRH(view_matrix, (FLOAT)left, (FLOAT)right, (FLOAT)bottom, (FLOAT)top, (FLOAT)znear, (FLOAT)zfar);

		m_d3dmat[m_nCurrentMatrix]->MultMatrix(view_matrix);
		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;
		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);

		D3DVIEWPORT9 pViewPort;
		pViewPort.X = 0;
		pViewPort.Y = 0;
		pViewPort.Width = m_nWidth;
		pViewPort.Height = m_nHeight;
		pViewPort.MinZ = -float(znear);
		pViewPort.MaxZ = -float(zfar);
		m_d3ddev->SetViewport(&pViewPort);

		delete view_matrix;
#endif // PIUMAX_DX
	}
}

void CGraphMain::Perspective(double fovy, double aspect, double znear, double zfar)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		gluPerspective(fovy, aspect, znear, zfar);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX *view_matrix = NULL, *matrix;

		view_matrix = new D3DXMATRIX();

		D3DXMatrixPerspectiveFovRH(view_matrix, (FLOAT)fovy, (FLOAT)aspect, (FLOAT)znear, (FLOAT)zfar);

		m_d3dmat[m_nCurrentMatrix]->MultMatrix(view_matrix);
		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;
		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);

		D3DVIEWPORT9 pViewPort;
		pViewPort.X = 0;
		pViewPort.Y = 0;
		pViewPort.Width = m_nWidth;
		pViewPort.Height = m_nHeight;
		pViewPort.MinZ = -float(znear);
		pViewPort.MaxZ = -float(zfar);
		m_d3ddev->SetViewport(&pViewPort);

		delete view_matrix;
#endif // PIUMAX_DX
	}
}

void CGraphMain::Translate(double dx, double dy, double dz)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glTranslated(dx, dy, dz);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX *matrix;

		m_d3dmat[m_nCurrentMatrix]->TranslateLocal((FLOAT)dx, (FLOAT)dy, (FLOAT)dz);
		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;
		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);
#endif // PIUMAX_DX
	}
}

void CGraphMain::Rotate(double dtheta, double dx, double dy, double dz)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glRotated(dtheta, dx, dy, dz);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX *matrix;
		D3DXVECTOR3 arb_vector = D3DXVECTOR3((FLOAT)dx, (FLOAT)dy, (FLOAT)dz);

		m_d3dmat[m_nCurrentMatrix]->RotateAxisLocal(&arb_vector, (FLOAT)(dtheta*M_PI/180.0));
		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;
		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);
#endif // PIUMAX_DX
	}
}

void CGraphMain::Scale(double dx, double dy, double dz)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glScaled(dx, dy, dz);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX *matrix;

		m_d3dmat[m_nCurrentMatrix]->ScaleLocal((FLOAT)dx, (FLOAT)dy, (FLOAT)dz);
		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;
		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);
#endif // PIUMAX_DX
	}
}

int CGraphMain::PushMatrix(void)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glPushMatrix();
		GLenum code = glGetError();
		if(code != GL_NO_ERROR) return 0;
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX *matrix;
		HRESULT hr = m_d3dmat[m_nCurrentMatrix]->Push();
		if(FAILED(hr)) return 0;

		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;
		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);
#endif // PIUMAX_DX
	}
	return 1;
}

int CGraphMain::PopMatrix(void)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glPopMatrix();
		GLenum code = glGetError();
		if(code != GL_NO_ERROR) return 0;
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DXMATRIX *matrix;
		HRESULT hr = m_d3dmat[m_nCurrentMatrix]->Pop();
		if(FAILED(hr)) return 0;

		matrix = m_d3dmat[m_nCurrentMatrix]->GetTop();

		D3DTRANSFORMSTATETYPE mode = D3DTS_VIEW;
		switch(m_nCurrentMatrix)
		{
			case PMG_MODELVIEW: mode = D3DTS_VIEW; break;
			case PMG_PROJECTION: mode = D3DTS_PROJECTION; break;
			case PMG_TEXTURE: mode = D3DTS_TEXTURE0; break;
			case PMG_TEXTURE1: mode = D3DTS_TEXTURE1; break;
			case PMG_TEXTURE2: mode = D3DTS_TEXTURE2; break;
			case PMG_TEXTURE3: mode = D3DTS_TEXTURE3; break;
			case PMG_TEXTURE4: mode = D3DTS_TEXTURE4; break;
			case PMG_TEXTURE5: mode = D3DTS_TEXTURE5; break;
			case PMG_TEXTURE6: mode = D3DTS_TEXTURE6; break;
			case PMG_TEXTURE7: mode = D3DTS_TEXTURE7; break;
		}

		m_d3ddev->SetTransform(mode,matrix);
#endif // PIUMAX_DX
	}
	return 1;
}


int CGraphMain::Clear(double r, double g, double b, double a)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glClearColor((GLclampf)r, (GLclampf)g, (GLclampf)b, (GLclampf)a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		D3DCOLOR clearcolor = D3DCOLOR_RGBA((BYTE)(r*255), (BYTE)(g*255), (BYTE)(b*255), (BYTE)(a*255));
		HRESULT hr=m_d3ddev->Clear(0,
                          NULL,
                          D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                          clearcolor,
                          1.0f,
                          0 );
		if(FAILED(hr)) return 0;
#endif // PIUMAX_DX
	}
	return 1;
}

void CGraphMain::SwapBuffers(void)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		SDL_GL_SwapWindow(window);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		HRESULT hr = m_d3ddev->Present(NULL,
                            NULL,
                            NULL,
                            NULL );

		if(FAILED(hr)) _printf(_T("Fallo al renderizar en DirectX\n"));
#endif // PIUMAX_DX
	}
}

int CGraphMain::BeginScene(void)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		// *************** NO CODE FOR OPENGL
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		HRESULT hr=m_d3ddev->BeginScene();
		if(FAILED(hr)) return 0;

#endif // PIUMAX_DX
	}
	return 1;
}

void CGraphMain::EndScene(void)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		// *************** NO CODE FOR OPENGL
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		m_d3ddev->EndScene();
#endif // PIUMAX_DX
	}
}

int CGraphMain::Begin(int nEnum)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		GLenum codex = GL_POINTS;
		m_nCurPrimitiveType = nEnum;
		switch(nEnum)
		{
			case PMG_POINTS: codex = GL_POINTS; break;
			case PMG_LINES: codex = GL_LINES; break;
			case PMG_LINE_STRIP: codex = GL_LINE_STRIP; break;
			case PMG_LINE_LOOP: codex = GL_LINE_LOOP; break;
			case PMG_TRIANGLES: codex = GL_TRIANGLES; break;
			case PMG_TRIANGLE_STRIP: codex = GL_TRIANGLE_STRIP; break;
			case PMG_TRIANGLE_FAN: codex = GL_TRIANGLE_FAN; break;
			case PMG_QUADS: codex = GL_QUADS; break;
			case PMG_QUAD_STRIP: codex = GL_QUAD_STRIP; break;
			case PMG_POLYGON: codex = GL_POLYGON; break;
		}
		glBegin(codex);
		GLenum code = glGetError();
		if(code != GL_NO_ERROR) return 0;
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		if(m_nStackVertex) return 0;
		m_nStackVertex = 1;
		m_nCurPrimitiveType = nEnum;
		if(size_vertex_buffer != 0)
		{
			size_vertex_buffer = 0;
			vertex_buffer.clear();
			size_vertex = 0;
			size_texcoord = 0;
		}
#endif // PIUMAX_DX
	}
	return 1;
}

int CGraphMain::End(void)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glEnd();
		GLenum code = glGetError();
		if(code != GL_NO_ERROR) return 0;
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		if(!m_nStackVertex) return 0;
		if(size_vertex_buffer == 0) return 0;

		D3DPRIMITIVETYPE prim_type = D3DPT_POINTLIST;
		UINT nElem = 0;
		switch(m_nCurPrimitiveType)
		{
			case PMG_POINTS: prim_type = D3DPT_POINTLIST; nElem = (UINT)(size_vertex_buffer); break;
			case PMG_LINES: prim_type = D3DPT_LINELIST; nElem = (UINT)(size_vertex_buffer/2); break;
			case PMG_LINE_STRIP: prim_type = D3DPT_LINESTRIP; nElem = (UINT)(max(size_vertex_buffer-1, 0)); break;
			case PMG_LINE_LOOP: prim_type = D3DPT_LINESTRIP; nElem = (UINT)(max(size_vertex_buffer-1, 0)); break;
			case PMG_TRIANGLES: prim_type = D3DPT_TRIANGLELIST; nElem = (UINT)(size_vertex_buffer/3); break;
			case PMG_TRIANGLE_STRIP: prim_type = D3DPT_TRIANGLESTRIP; nElem = (UINT)(max(size_vertex_buffer-2, 0)); break;
			case PMG_TRIANGLE_FAN: prim_type = D3DPT_TRIANGLEFAN; nElem = (UINT)(max(size_vertex_buffer-2, 0)); break;
			case PMG_QUADS: prim_type = D3DPT_TRIANGLEFAN; nElem = (UINT)(max(size_vertex_buffer-2, 0)); break;
			case PMG_QUAD_STRIP: prim_type = D3DPT_TRIANGLEFAN; nElem = (UINT)(max(size_vertex_buffer-2, 0)); break;
			case PMG_POLYGON: prim_type = D3DPT_TRIANGLEFAN; nElem = (UINT)(max(size_vertex_buffer-2, 0)); break;
		}

		// Initialize a vertex buffer
		LPDIRECT3DVERTEXBUFFER9 d3dbuf = NULL;
		HRESULT hr = m_d3ddev->CreateVertexBuffer(size_vertex_buffer*sizeof(tri_vertex),
									 D3DUSAGE_WRITEONLY,//Usage
                                     tri_fvf,           //FVF
                                     D3DPOOL_MANAGED,
									 &d3dbuf,
									 NULL);
		if(FAILED(hr))
		{
			if(d3dbuf) d3dbuf->Release();
			d3dbuf = NULL;
			return 0;
		}

		void* vb_vert;
		hr = d3dbuf->Lock(0, 0, &vb_vert, 0);
		if(FAILED(hr))
		{
			if(d3dbuf) d3dbuf->Release();
			d3dbuf = NULL;
			return 0;
		}

		memcpy(vb_vert, vertex_buffer.data(), size_vertex_buffer*sizeof(tri_vertex));
		d3dbuf->Unlock();

		hr = m_d3ddev->SetStreamSource(0,     //StreamNumber
									 d3dbuf,			  //StreamData
									 0,                   //OffsetInBytes
									 sizeof(tri_vertex)); //Stride
		if(FAILED(hr))
		{
			if(d3dbuf) d3dbuf->Release();
			d3dbuf = NULL;
		}

		hr = m_d3ddev->DrawPrimitive(prim_type, //PrimitiveType
										0,           //StartVertex
										nElem);      //PrimitiveCount
		if(FAILED(hr))
		{
			if(d3dbuf) d3dbuf->Release();
			d3dbuf = NULL;
		}

		if(d3dbuf) d3dbuf->Release();
		d3dbuf = NULL;

		if(size_vertex_buffer != 0)
		{
			size_vertex_buffer = 0;
			vertex_buffer.clear();
			size_vertex = 0;
			size_texcoord = 0;
		}

		m_nStackVertex = 0;
		m_nCurPrimitiveType = -1;

#endif // PIUMAX_DX
	}
	return 1;
}

int CGraphMain::Vertex3d(double dx, double dy, double dz)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glVertex3d(dx, dy, dz);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		if(!m_nStackVertex) return 0;
		size_vertex++;
		if(size_vertex>size_vertex_buffer)
		{
			size_vertex_buffer=size_vertex;

			vertex_buffer.resize(size_vertex);
		}
		tri_vertex vadd;
		vadd.x =(float)(dcx= dx);
		vadd.y =(float)(dcy= dy);
		vadd.z =(float)(dcz= dz);
		vadd.w =(float)(dcrhw= 1.0);
		vadd.tu =(float)dctu;
		vadd.tv =(float)dctv;
		vadd.tw =(float)dctw;
		vadd.colour =D3DCOLOR_RGBA((BYTE)(dr*255), (BYTE)(dg*255), (BYTE)(db*255), (BYTE)(da*255));
		vertex_buffer[size_vertex_buffer-1] = vadd;
#endif // PIUMAX_DX
	}
	return 1;
}

int CGraphMain::TexCoord2d(double du, double dv)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glTexCoord2d(du, dv);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
        double u=du, v=dv, w=0.0;
        if(m_bFlipY) v=1-dv;
		if(!m_nStackVertex) return 0;
		v=1-v;	// Force the flip-Y
		size_texcoord++;
		if(size_texcoord>size_vertex_buffer)
		{
			size_vertex_buffer=size_texcoord;

			vertex_buffer.resize(size_texcoord);
		}
		tri_vertex vadd;
		vadd.x =(float)dcx;
		vadd.y =(float)dcy;
		vadd.z =(float)dcz;
		vadd.w =(float)(dcrhw= 1.0);
		vadd.tu =(float)(dctu= du);
		vadd.tv =(float)(dctv= dv);
		vadd.tw =(float)(dctw= 0.0);
		vadd.colour =D3DCOLOR_RGBA((BYTE)(dr*255), (BYTE)(dg*255), (BYTE)(db*255), (BYTE)(da*255));
		vertex_buffer[size_vertex_buffer-1] = vadd;
#endif // PIUMAX_DX
	}
	return 1;
}

int CGraphMain::TexCoord3d(double du, double dv, double dw)
{
	double u=du, v=dv, w=dw;
	if(m_bFlipY) v=1-dv;
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glTexCoord3d(u, v, w);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		if(!m_nStackVertex) return 0;
		v=1-v;	// Force the flip-Y
		size_texcoord++;
		if(size_texcoord>size_vertex_buffer)
		{
			size_vertex_buffer=size_texcoord;

			vertex_buffer.resize(size_texcoord);
		}
		tri_vertex vadd;
		vadd.x =(float)dcx;
		vadd.y =(float)dcy;
		vadd.z =(float)dcz;
		vadd.w =(float)(dcrhw= 1.0);
		vadd.tu =(float)(dctu= u);
		vadd.tv =(float)(dctv= v);
		vadd.tw =(float)(dctw= w);
		vadd.colour =D3DCOLOR_RGBA((BYTE)(dr*255), (BYTE)(dg*255), (BYTE)(db*255), (BYTE)(da*255));
		vertex_buffer[size_vertex_buffer-1] = vadd;
#endif // PIUMAX_DX
	}
	return 1;
}

void CGraphMain::Color4d(double r, double g, double b, double a)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		glColor4d(r, g, b, a);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		dr = r; dg = g; db = b; da = a;
#endif // PIUMAX_DX
	}
}

int CGraphMain::CreateTexture(int nMipMapLevels, int nFormat, int nWidth, int nHeight, CGraphTexture* m_gtArray)
{
	m_gtArray->m_nGestor = m_nGestor;
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL

		glGenTextures(1, &(*m_gtArray).m_uId);
		if((*m_gtArray).m_uId == 0) return 0;

#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX


		m_d3ddev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		m_d3ddev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);

		D3DFORMAT format = D3DFMT_A8R8G8B8;
		GLint int_format = 3;
		switch(nFormat)
		{
			case PMT_RGBA: format = D3DFMT_A8R8G8B8; int_format = 4; break;
			case PMT_RGBX: format = D3DFMT_X8R8G8B8; int_format = 4; break;
			case PMT_RGB: format = D3DFMT_R8G8B8; int_format = 3; break;
			case PMT_BGRA: format = D3DFMT_A8B8G8R8; int_format = 4; break;
			case PMT_BGRX: format = D3DFMT_X8B8G8R8; int_format = 4; break;
			case PMT_BGR: format = D3DFMT_R8G8B8; int_format = 3; break;
			default: return 0; break;
		}
		HRESULT hr = m_d3ddev->CreateTexture(nWidth,
			nHeight,
			max(nMipMapLevels, 0),
			D3DUSAGE_DYNAMIC,
			format,
			D3DPOOL_DEFAULT,
			&m_gtArray->m_tex,
			NULL);
		if(FAILED(hr))
		{
			CString strErr = DXGetErrorDescription(hr);
			_printf(strErr);
			return 0;
		}
#endif // PIUMAX_DX
	}
	return 1;
}


int CGraphMain::BindTexture(CGraphTexture* gtTex)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		if(gtTex == NULL) glBindTexture(GL_TEXTURE_2D, 0);
		else glBindTexture(GL_TEXTURE_2D, gtTex->m_uId);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		if(gtTex != NULL) m_d3ddev->SetTexture(0, gtTex->m_tex);
#endif // PIUMAX_DX
	}
	return 1;
}

int CGraphMain::BlendFunc(int nSrc, int nDst)
{
	if(m_nGestor == 1)
	{
#ifdef PIUMAX_OGL
		GLenum src, dst;
		src = pmgGlTraduceBlend(nSrc);
		dst = pmgGlTraduceBlend(nDst);
		glBlendFunc(src, dst);
#endif // PIUMAX_OGL
	}
	if(m_nGestor == 2)
	{
#ifdef PIUMAX_DX
		DWORD src, dst;
		src = pmgDXTraduceBlend(nSrc);
		dst = pmgDXTraduceBlend(nSrc);
		m_d3ddev->SetRenderState(D3DRS_SRCBLEND, src);
		m_d3ddev->SetRenderState(D3DRS_DESTBLEND, dst);
#endif // PIUMAX_DX
	}
	return 1;
}

#ifdef PIUMAX_OGL
GLenum CGraphMain::pmgGlTraduceBlend(int arg)
{
	GLenum format;
	switch(arg)
	{
		case PMG_ZERO: format = GL_ZERO; break;
		case PMG_ONE: format = GL_ONE; break;
		case PMG_SRC_COLOR: format = GL_SRC_COLOR; break;
		case PMG_ONE_MINUS_SRC_COLOR: format = GL_ONE_MINUS_SRC_COLOR; break;
		case PMG_DST_COLOR: format = GL_DST_COLOR; break;
		case PMG_ONE_MINUS_DST_COLOR: format = GL_ONE_MINUS_DST_COLOR; break;
		case PMG_SRC_ALPHA: format = GL_SRC_ALPHA; break;
		case PMG_ONE_MINUS_SRC_ALPHA: format = GL_ONE_MINUS_SRC_ALPHA; break;
		case PMG_DST_ALPHA: format = GL_DST_ALPHA; break;
		case PMG_ONE_MINUS_DST_ALPHA: format = GL_ONE_MINUS_DST_ALPHA; break;
		default: return 0; break;
	}
	return format;
}
#endif // PIUMAX_OGL

#ifdef PIUMAX_DX
DWORD CGraphMain::pmgDXTraduceBlend(int arg)
{
	DWORD format;
	switch(arg)
	{
		case PMG_ZERO: format = D3DBLEND_ZERO; break;
		case PMG_ONE: format = D3DBLEND_ONE; break;
		case PMG_SRC_COLOR: format = D3DBLEND_SRCCOLOR; break;
		case PMG_ONE_MINUS_SRC_COLOR: format = D3DBLEND_INVSRCCOLOR; break;
		case PMG_DST_COLOR: format = D3DBLEND_DESTCOLOR; break;
		case PMG_ONE_MINUS_DST_COLOR: format = D3DBLEND_INVDESTCOLOR; break;
		case PMG_SRC_ALPHA: format = D3DBLEND_SRCALPHA; break;
		case PMG_ONE_MINUS_SRC_ALPHA: format = D3DBLEND_INVSRCALPHA; break;
		case PMG_DST_ALPHA: format = D3DBLEND_DESTALPHA; break;
		case PMG_ONE_MINUS_DST_ALPHA: format = D3DBLEND_INVDESTALPHA; break;
		default: return 0; break;
	}
	return format;
}
#endif // PIUMAX_DX
