#ifndef PIUMAX_OGLSPRITE
#define PIUMAX_OGLSPRITE

#include <GL/glew.h>
#include <SDL/SDL.h>
#include <math.h>

class COGLSprite
{
public:
	COGLSprite(void);
	~COGLSprite(void);
	int InitImage(LPCTSTR chName, int nSize);
	int m_nSize;
	void Update(DWORD dwMiliseconds);
	DWORD m_dwNext;
	int m_dwDuration;
	void SetDuration(DWORD dwDuration);
	int m_dwFrame;
	GLuint GetCurrentTex();
	GLuint* texture;
	void DeleteImage();
};

#endif
