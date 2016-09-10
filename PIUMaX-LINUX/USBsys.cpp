#include "USBsys.h"
#include "UsbRes.h"
#include "cmnpiu.h"
#include "cmnpiu_ex.h"
#include "game1.h"
#include "game2.h"
#include "Game3.h"
#include "Game4.h"

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <linux/limits.h>
#include <sys/mount.h>
#include <glob.h>
#include <unistd.h>

#define PROC_USB_DEV "/proc/scsi/usb-storage"
#define SYS_SCSI_HOST "/sys/class/scsi_host/host%s/device"
#define SYS_SCSI_DEV "/sys/class/scsi_disk/%s:*"
#define SYS_SCSI_TGT "%s/device/block/*"
#define SYS_MOUNT_PARTS "%s/%s*"

CMovieStream2* movie_USBintro;
CMovieStream2* movie_USBloop;
CMovieStream2* movie_USBexit;
CUsbRes* usbres;
TCHAR chUsb[32][260];
int nUsb = 0;
GLuint movie_texUsb;

bool bSaveUsb = false;
__int64 nCountUsb = 0;
__int64 nTimeExitUsb = 0;
int nItemDown[2];
int nItemUp[2];
int nChoosesUsb[2];
__int64 nFirstEnum[2];
bool bFirstEnum[2];
int nSizeItem = 1;
__int64 nRec[2];
bool bRec[2];
/////////////
int bUsb[2];
bool bSaveUsbActive[2];
TCHAR strSave[2][260];
TCHAR chNameProfUsb[2][9];
/////////////

TCHAR chItem[7][9];
TCHAR chSaveItem[7][260];
PLAYER* plItem[7];
int nPlayedGame1[7];
int nPlayedGame2[7];
int nPlayedGame3[7];
int nPlayedGame4[7];
int nPlayedGame5[7];
int nRewardGame1[7];
int nRewardGame2[7];
int nRewardGame3[7];
int nRewardGame4[7];
int nRewardGame5[7];
int nSizeItemUsb[7];
int nItemUsb[7][12];
CString strLastPlayed[7];
GLuint uTexSplash[7];
GLuint uTexPlayerSplash[2];

FMOD_SOUND *sound_insertusb;

__int64 nTimeChoosesUsb[2][2];

#ifndef _WIN32
char chUsbDev[32][260];
bool bUsbMounted = false;
void LinuxUSBmount(void)
{
    if(bUsbMounted) return;
    bUsbMounted = true;
    DIR *dp = NULL;
  struct dirent *dt = NULL;

  if((dp = opendir(PROC_USB_DEV)) == NULL) {
    fprintf(stderr, "Can not open %s: %s\n", PROC_USB_DEV, strerror(errno));
    return;
  }

  /* find usb storage device hosts which appear as scsi hosts */
  /* for the sake of example this one hasn't been done with glob(3) */
  while((dt = readdir(dp)) != NULL) {
    int scsi_dev = 0;
    /* skip '.' and '..', possibly others too */
    if((scsi_dev = atoi(basename(dt->d_name))) < 1) continue;

    char buf[PATH_MAX];
    char pat[PATH_MAX];

    snprintf(buf, PATH_MAX, SYS_SCSI_HOST, basename(dt->d_name));
    snprintf(pat, PATH_MAX, SYS_SCSI_DEV, basename(dt->d_name));

    glob_t hosts;
    size_t count;

    /* find SCSI host device paths */
    glob(pat, 0, 0, &hosts);
    if(hosts.gl_pathc > 0) {
      char **p;
      int n;
      for(p = hosts.gl_pathv, n = hosts.gl_pathc; n; p++, n--) {
        char tgtbuf[PATH_MAX + NAME_MAX];

        snprintf(tgtbuf, PATH_MAX + NAME_MAX, SYS_SCSI_TGT, *p);

        /* find SCSI disk device paths */
        glob_t devs;
        glob(tgtbuf, 0, 0, &devs);
        if(devs.gl_pathc > 0) {
          char **ptr;
          int c;
          for(ptr = devs.gl_pathv, c = devs.gl_pathc; c; ptr++, c--) {
            char tgts[PATH_MAX + NAME_MAX];

            snprintf(tgts, PATH_MAX + NAME_MAX, SYS_MOUNT_PARTS, *ptr, basename(*ptr));

            glob_t parts;
            glob(tgts, 0, 0, &parts);
            if(parts.gl_pathc > 0) {
                char **ptra;
                int k;
                for(ptra = parts.gl_pathv, k = parts.gl_pathc; k; ptra++, k--) {
                    printf("We would now call mount(2) for /dev/%s.\n",
                   basename(*ptra));
                   char tag[PATH_MAX + NAME_MAX];
                   char to_mnt[PATH_MAX + NAME_MAX];
                   snprintf(tag, PATH_MAX + NAME_MAX, "/dev/%s", basename(*ptra));
                   snprintf(to_mnt, PATH_MAX + NAME_MAX, "/mnt/%d", nUsb);
                   int nRet;
                   nRet = mount(tag, to_mnt, "vfat", MS_MGC_VAL | MS_NOSUID, "");
                   if(nRet < 0)
                   {
                        printf("Cannot mount for /dev/%s.\n",
                        basename(*ptra));
                   }
                   else{
                        printf("Mounted! for /dev/%s\n",
                        basename(*ptra));
                        _sprintf(chUsb[nUsb], _T("/mnt/%d"), nUsb);
                        nUsb++;
                   }
                }
            }

          }
        }
        globfree(&devs);
      }
      globfree(&hosts);
    }
  }
}

void LinuxUSBumount(void)
{
    if(!bUsbMounted) return;
    bUsbMounted = false;
    for(int k = 0; k < nUsb; k++)
    {
        char chUsbAnsi[MAX_PATH];
        UnicodeToAnsi(chUsb[k], chUsbAnsi);
        umount(chUsbAnsi);
    }
}
#endif

void ShowProfile(int nPlayerShow, int nItemShow)
{
	glPushMatrix();
	glTranslated((nPlayerShow?20.0:-20.0), 0.0, 0.0);
	double dPGame1 = double(nPlayedGame1[nItemShow])/double(nSizeTotalGame1Ex?nSizeTotalGame1Ex:1)*100;
	double dPGame2 = double(nPlayedGame2[nItemShow])/double(nSizeTotalGame2Ex?nSizeTotalGame2Ex:1)*100;
	double dPGame3 = double(nPlayedGame3[nItemShow])/double(nSizeTotalGame3Ex?nSizeTotalGame3Ex:1)*100;
	double dPGame4 = double(nPlayedGame4[nItemShow])/double(nSizeTotalGame4Ex?nSizeTotalGame4Ex:1)*100;
	double dPGame5 = double(nPlayedGame5[nItemShow])/double(nSizeTotalGame5Ex?nSizeTotalGame5Ex:1)*100;
	double dRGame1 = double(nRewardGame1[nItemShow])/double(nSizeTotalGame1Ex?nSizeTotalGame1Ex:1)*100;
	double dRGame2 = double(nRewardGame2[nItemShow])/double(nSizeTotalGame2Ex?nSizeTotalGame2Ex:1)*100;
	double dRGame3 = double(nRewardGame3[nItemShow])/double(nSizeTotalGame3Ex?nSizeTotalGame3Ex:1)*100;
	double dRGame4 = double(nRewardGame4[nItemShow])/double(nSizeTotalGame4Ex?nSizeTotalGame4Ex:1)*100;
	double dRGame5 = double(nRewardGame5[nItemShow])/double(nSizeTotalGame5Ex?nSizeTotalGame5Ex:1)*100;
	CString str;
	str.Format(_T("%.1f%%"), dPGame1);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+2.51953, 7.06250, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.1f%%"), dPGame2);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+2.51953, 4.75000, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.1f%%"), dPGame3);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+2.51953, 2.43750, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.1f%%"), dPGame4);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+2.51953, 0.12500, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.1f%%"), dPGame5);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+2.51953, -2.18750, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);

	str.Format(_T("%.1f%%"), dRGame1);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+8.55469, 7.06250, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.1f%%"), dRGame2);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+8.55469, 4.75000, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.1f%%"), dRGame3);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+8.55469, 2.43750, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.1f%%"), dRGame4);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+8.55469, 0.12500, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);
	str.Format(_T("%.1f%%"), dRGame5);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+8.55469, -2.18750, 0.0);
	font_Micro->glDrawText(0.84375, -2.56348, -0.84375, 2.56348, 0.0, 0.0, -50.0, 1.0, 1.0, str, DT_CENTER|DT_VCENTER);

	str.Format(_T("%.8d"), plItem[nItemShow]->nMileage);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+5.77148, 15.34375, 0.0);
	font_Micro->glDrawText(0.78125, -5.12696, -0.78125, 5.12696, 0.0, 0.0, -50.0, 1.0, 1.3, str, DT_CENTER|DT_VCENTER);

	str.Format(_T("%.2d"), plItem[nItemShow]->nLevel);
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+0.7539, -3.76562, 0.0);
	font_Micro->glDrawText(0.57812, -2.9961, -0.57812, 2.9961, 0.0, 0.0, -50.0, 1.0, 1.15625, str, DT_CENTER|DT_VCENTER);

	str.Format(_T("%d / %d"), plItem[nItemShow]->nExp-int(pow(2.0, double(plItem[nItemShow]->nLevel-1))),
		int(pow(2.0, double(plItem[nItemShow]->nLevel))-pow(2.0, double(plItem[nItemShow]->nLevel-1))));
	double dApt = (double(plItem[nItemShow]->nExp)-pow(2.0, double(plItem[nItemShow]->nLevel-1)))
		/(pow(2.0, double(plItem[nItemShow]->nLevel))-pow(2.0, double(plItem[nItemShow]->nLevel-1)));
	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)+0.087890625, -5.53125, 0.0);
	if(plItem[nItemShow]->nExp >= 1)
	{
		glBindTexture(GL_TEXTURE_2D, usbres->skin[19]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-11.513671875				  ,  1.03125, -50.0);
		glTexCoord2d(dApt, 1.0); glVertex3d(-11.513671875+dApt*23.02734375,  1.03125, -50.0);
		glTexCoord2d(dApt, 0.0); glVertex3d(-11.513671875+dApt*23.02734375, -1.03125, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-11.513671875				  , -1.03125, -50.0);
		glEnd();
	}
	font_Micro->glDrawText(1.03125, -11.513671875, -1.03125, 11.513671875, 0.0, 0.0, -50.0, 1.0, 1.15625, str, DT_CENTER|DT_VCENTER);

	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0)-10.535161, -11.8125, 0.0);
#define VEL_SHOWTEXT_USB 0.005
#define TIME_WAITTEXTNAME_USB 1500
	__int64 nTimeTextName = nCountUsb-nRec[nPlayerShow]-TIME_WAITTEXTNAME_USB;
	if(nTimeTextName < 0) nTimeTextName = 0;
	double dSizeTex = font_Micro->CalcSingleLine(strLastPlayed[nItemShow], 1.15625);
	DWORD t1 = DWORD(double(21.09376/VEL_SHOWTEXT_USB));
	DWORD t2 = DWORD(double(dSizeTex/VEL_SHOWTEXT_USB));
	DWORD t = t1+t2;
	DWORD nShowText = (DWORD(nTimeTextName)+t1)%t;
	double kx = -VEL_SHOWTEXT_USB*double(nShowText)+21.09376;
	font_Micro->glDrawText(1.1875, 0, -1.1875, 21.09376, kx, 0.0, -50.0, 1.0, 1.15625, strLastPlayed[nItemShow], DT_VCENTER);

	glLoadIdentity();
	glTranslated((nPlayerShow?20.0:-20.0), 22.5, 0.0);
	str = ( plItem[nItemShow]->chName);
	font_Micro->glDrawText(1.0625, -2.28516, -1.0625, 13.65234, 5.68359, 0.0, -50.0, 1.0, 2.125, str, DT_CENTER|DT_VCENTER);
	for(int x = 0; x < nSizeItemUsb[nItemShow]; x++)
	{
		glLoadIdentity();
		glTranslated((nPlayerShow?20.0:-20.0)-10.327+double(x%6)*4.1308, -20.0-4.746-double(x/6)*2.2558, 0.0);
		glBindTexture(GL_TEXTURE_2D, usbres->skin[20+nItemUsb[nItemShow][x]]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-2.0654,  1.1279, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 2.0654,  1.1279, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 2.0654, -1.1279, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-2.0654, -1.1279, -50.0);
		glEnd();
	}
	glPopMatrix();
}

void GenShowProfiles()
{
	for(int x = 0; x < 7; x++)
	{
		nPlayedGame1[x] = 0;
		nPlayedGame2[x] = 0;
		nPlayedGame3[x] = 0;
		nPlayedGame4[x] = 0;
		nPlayedGame5[x] = 0;
		nRewardGame1[x] = 0;
		nRewardGame2[x] = 0;
		nRewardGame3[x] = 0;
		nRewardGame4[x] = 0;
		nRewardGame5[x] = 0;
		nSizeItemUsb[x] = 0;
	}
	for(int x = 0; x < nSizeItem; x++)
	{
		switch(nInterAfterUsb)
		{
		case INT_GAME_1:
			strLastPlayed[x].Format(_T("%ls - %ls"), GetNameTotalSongsGame1(plItem[x]->nLastArcade[0]), GetArtistTotalSongsGame1(plItem[x]->nLastArcade[0]));
			break;
		case INT_GAME_2:
			strLastPlayed[x] = mission->m_strName[plItem[x]->nLastMission];
			break;
		case INT_GAME_3:
			strLastPlayed[x].Format(_T("%ls - %ls"), GetNameTotalSongsGame3(plItem[x]->nLastSpecial[0]), GetArtistTotalSongsGame3(plItem[x]->nLastSpecial[0]));
			break;
		case INT_GAME_4:
			strLastPlayed[x].Format(_T("%ls - %ls"), GetNameTotalSongsGame4(plItem[x]->nLastBrainShower[0]), GetArtistTotalSongsGame4(plItem[x]->nLastBrainShower[0]));
			break;
		default:
			strLastPlayed[x] = _T("");
			break;
		}
		strLastPlayed[x];
		for(int i = 0; i < (nSizeTotalGame1*10); i++)
		{
			if(plItem[x]->nArcadeRecords[i][0]) nPlayedGame1[x]++;
		}
		for(int i = 0; i < (nSizeTotalGame2); i++)
		{
			if(plItem[x]->nMission[i]) nPlayedGame2[x]++;
		}
		for(int i = 0; i < (nSizeTotalGame3*5); i++)
		{
			if(plItem[x]->nSpecialRecords[i][0]) nPlayedGame3[x]++;
		}
		for(int i = 0; i < (nSizeTotalGame4); i++)
		{
			if(plItem[x]->nBrainShowerRecords[i][0]) nPlayedGame4[x]++;
		}
		for(int i = 0; i < (nSizeTotalGame5); i++)
		{
			if(plItem[x]->nAprovedTraining[i]) nPlayedGame5[x]++;
		}

		for(int i = 0; i < (nSizeTotalGame1*10); i++)
		{
			if(plItem[x]->nLockedSongNew[i] == 0 && IsAvalibleModeTotal(i/10, i%10)) nRewardGame1[x]++;
		}
		for(int i = 0; i < (nSizeTotalGame2); i++)
		{
			if(plItem[x]->nUnlockedMission[i]) nRewardGame2[x]++;
		}
		for(int i = 0; i < (nSizeTotalGame3*5); i++)
		{
			if(plItem[x]->nLockedSpecNew[i] == 0 && IsAvalibleModeTotalGame3(i/5, i%5)) nRewardGame3[x]++;
		}
		for(int i = 0; i < (nSizeTotalGame4); i++)
		{
			if(plItem[x]->nLockedBrainShower[i] == 0) nRewardGame4[x]++;
		}
		nRewardGame5[x] = 0;

		for(int y = (MAX_SKINS+MAX_CHARACTERS); y < 100; y++)
		{
			if(plItem[x]->nLockedAsp[y])
			{
				nSizeItemUsb[x]++;
				nItemUsb[x][nSizeItemUsb[x]-1] = (y-(MAX_SKINS+MAX_CHARACTERS))*2+plItem[x]->nLockedAsp[y];
			}
		}
	}
}

void EnumUSBDevices()
{
#ifdef _WIN32
	TCHAR chDrives[32][8];
	int nDrives = 0;
	nUsb = 0;
	DWORD dwDrives = GetLogicalDrives();
	for(int x = 2; x < 32; x++)	// Comienza desde 2 para que no considere los floppy [A: y B:]
	{
		DWORD dwComp = (1<<x);
		if(dwDrives & dwComp)
		{
			_sprintf(chDrives[nDrives], 8, _T("%c:" DIRDIV), ('A'+char(x)));
			nDrives++;
		}
	}
	for(int x = 0; x < nDrives; x++)
	{
		if(GetDriveType(chDrives[x]) == DRIVE_REMOVABLE)
		{
			_strcpy(chUsb[nUsb], chDrives[x]);
			nUsb++;
		}
	}
#else

    // There is no need to be specific-ports, windows version doesn't care
    if(bUsbMounted) LinuxUSBumount();
	LinuxUSBmount();
#endif // WIN32
}

void EnumUSBProfiles()
{
	int nCountNewProfiles = 0;
	nSizeItem = 0;
	EnumUSBDevices();
	// ** Crear perfil de la maquina
	_strcpy(chItem[0], _T("[MACHINE]"));
	_strcpy(chSaveItem[nSizeItem], _T("\0"));
	ConvertCurMachineProfile(&plItem[0]);
	nSizeItem = 1;
	int nLimit = min(2, nUsb);	// Escanear maximo 2 USB's
	TCHAR chFile[260];
	FILE* hFileAcc;
	FILE* hFilePlay;
	for(int x = 0; x < nLimit; x++)
	{
		_sprintf(chFile, _T("%lsMAXsave/MAXpsave.acc"), chUsb[x]);
		hFileAcc = _fopen(chFile, _T("r"));
		if(hFileAcc == NULL) // No existen los perfiles
		{
			TCHAR chDirCre[MAX_PATH];
			_sprintf(chDirCre, _T("%lsMAXsave"), chUsb[x]);
			char ch[MAX_PATH];
			Max_strcpy(ch, chDirCre);
			_mkdir(ch);
			hFileAcc = _fopen(chFile, _T("w+"));
			if(hFileAcc != NULL)	// Error al crear perfiles
			{
				_fputs(_T("*Este es el archivo que enumera la cantidad de perfiles guardadas en esta USB\n"), hFileAcc);
				_fputs(_T("*Favor, no modificar el contenido de este archivo\n"), hFileAcc);
				_fputs(_T("profile1\n"), hFileAcc);
				_fputs(_T("profile2\n"), hFileAcc);
				_fputs(_T("profile3\n"), hFileAcc);
				fclose(hFileAcc);
				PLAYER* pl = new PLAYER();
				GetDefPlayerFromFile(&pl);
				struct tm * tmToday;
				time_t tToday;
				TCHAR chNamesU[3][12];
				// Get current date and use it to format the USB profile
				time(&tToday);
				tmToday = localtime(&tToday);
				_sprintf(pl->chName, _T("max%.3d%.2d%.2d%.2d"),
					tmToday->tm_yday,tmToday->tm_hour,tmToday->tm_min,nCountNewProfiles);
				_strcpy(chNamesU[0], pl->chName);
				nCountNewProfiles++;
				_sprintf(chFile, _T("%lsMAXsave/profile1.bin"), chUsb[x]);
				_strcpy(chSaveItem[nSizeItem], chFile);
				hFilePlay = _fopen(chFile, _T("w+b")/*b*/);
				if(hFilePlay != NULL)
				{
					fwrite(pl, sizeof(PLAYER), 1, hFilePlay);
					fclose(hFilePlay);
				}
				_sprintf(pl->chName, _T("max%.3d%.2d%.2d%.2d"),
					tmToday->tm_yday,tmToday->tm_hour,tmToday->tm_min,nCountNewProfiles);
				_strcpy(chNamesU[1], pl->chName);
				nCountNewProfiles++;
				_sprintf(chFile, _T("%lsMAXsave/profile2.bin"), chUsb[x]);
				_strcpy(chSaveItem[nSizeItem+1], chFile);
				hFilePlay = _fopen(chFile, _T("w+b")/*b*/);
				if(hFilePlay != NULL)
				{
					fwrite(pl, sizeof(PLAYER), 1, hFilePlay);
					fclose(hFilePlay);
				}
				_sprintf(pl->chName, _T("max%.3d%.2d%.2d%.2d"),
					tmToday->tm_yday,tmToday->tm_hour,tmToday->tm_min,nCountNewProfiles);
				_strcpy(chNamesU[2], pl->chName);
				nCountNewProfiles++;
				_sprintf(chFile, _T("%lsMAXsave/profile3.bin"), chUsb[x]);
				_strcpy(chSaveItem[nSizeItem+2], chFile);
				hFilePlay = _fopen(chFile, _T("w+b")/*b*/);
				if(hFilePlay != NULL)
				{
					fwrite(pl, sizeof(PLAYER), 1, hFilePlay);
					fclose(hFilePlay);
				}
				for(int i = 0; i < 3; i++)
				{
					uTexSplash[nSizeItem] = uTexSplash[0];
					//_sprintf(pl->chName, 12, _T("profile%d"), i+1);
					memcpy(plItem[nSizeItem], pl, sizeof(PLAYER));
					_strcpy(chItem[nSizeItem], chNamesU[i]);
					nSizeItem++;
				}
				delete pl;
			}
		}
		else
		{
			TCHAR ch[260];
			CString str;
			int nCaretProf = 0;
			while(_fgets(ch, 260, hFileAcc) != NULL && nCaretProf < 3)
			{
				str = ( ch);
				str.Remove(_T('\n')); str.Remove(_T('\r'));
				if(str.Find(_T('*')) == -1)
				{
					//_sprintf(chFile, _T("%lsMAXsave/%ls.bin"), chUsb[x], str);
					//hFilePlay = _fopen(chFile, _T("r")/*b*/);
					str.Format(_T("%lsMAXsave/%ls.bin"), chUsb[x], str.c_str());
					hFilePlay = _fopen(str, _T("rb")/*b*/);
					if(hFilePlay != NULL)
					{
						fread(plItem[nSizeItem], sizeof(PLAYER), 1, hFilePlay);
						_strcpy(chItem[nSizeItem], plItem[nSizeItem]->chName);
						_strcpy(chSaveItem[nSizeItem], str);
						fclose(hFilePlay);
						str = ( ch);
						str.Remove(_T('\n')); str.Remove(_T('\r'));
						LPCTSTR chIso = str;
						str.Format(_T("%lsMAXsave/%ls.png"), chUsb[x], str);
						uTexSplash[nSizeItem] = GenTexImage(str);
						if(uTexSplash[nSizeItem] == 0) uTexSplash[nSizeItem] = uTexSplash[0];
						nSizeItem++;
						nCaretProf++;
					}
				}
			}
			fclose(hFileAcc);
		}
	}
	dwTick = dwLastTick = SDL_GetTicks();
}

#define TIME_EXITSAVE_USB 5000
#define TIME_EXITNOR_USB 30000

int DrawUSBSystem()
{
	nCountUsb += dwTick-dwLastTick;
	DWORD dwTimeIntro = movie_USBintro->bLoad?movie_USBintro->dwDur:0;
#define CHOOSES_USB 2
#define TIME_EXIT_USB 1000
	if((nChoosesUsb[0] >= CHOOSES_USB || !np[0]) &&
		(nChoosesUsb[1] >= CHOOSES_USB || !np[1]))
	{
		__int64 nTrans4 = nCountUsb - max((!np[0])?0:nTimeChoosesUsb[0][CHOOSES_USB-1],(!np[1])?0:nTimeChoosesUsb[1][CHOOSES_USB-1]);
		if(nTrans4 > TIME_EXIT_USB) return 0;
		if(movie_USBexit->bLoad)
		{
			movie_USBexit->Update(dwWait);
			movie_USBexit->GrabAVIFrame();
			glBindTexture(GL_TEXTURE_2D, movie_USBexit->tex);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
			glEnd();
		}
	}
	else
	{
		if(nCountUsb < dwTimeIntro)
		{
			if(movie_USBintro->bLoad)
			{
				movie_USBintro->Update(dwWait);
				movie_USBintro->GrabAVIFrame();
				glBindTexture(GL_TEXTURE_2D, movie_USBintro->tex);

				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
				glEnd();
			}
		}
		else if(movie_USBloop->bLoad)
		{
			movie_USBloop->Update(dwWait);
			movie_USBloop->GrabAVIFrame();
			glBindTexture(GL_TEXTURE_2D, movie_USBloop->tex);

			glBegin(GL_QUADS);
  			glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
			glEnd();
		}
	}
#define TIME_WAIT_INTRO_USB	1000
#define TIME_INTRO_USB 1600
#define TIME_EXPLOTION_UPDOWN_USB 500
#define TIME_APPEAR_INSTRUCTION_USB	300
#define TIME_EXPLOTION_INSTRUCTION_USB 500
#define TIME_TRANSI_EXIT_INTRO_USB 300
	double dApt = 0.0;
	if(nCountUsb > TIME_WAIT_INTRO_USB && nCountUsb < TIME_INTRO_USB)
	{
		dApt = (double(nCountUsb)-TIME_WAIT_INTRO_USB)/(TIME_INTRO_USB-TIME_WAIT_INTRO_USB);
		glBindTexture(GL_TEXTURE_2D, usbres->skin[0]->GetCurrentTex());
		glPushMatrix();
		glScaled(1.0+0.5*(1.0-dApt), 1.0+0.5*(1.0-dApt), 1.0);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
		glPopMatrix();
	}
	if(nCountUsb >= TIME_INTRO_USB)
	{
		if((nChoosesUsb[0] >= CHOOSES_USB || !np[0]) &&
		(nChoosesUsb[1] >= CHOOSES_USB || !np[1]))
		{
			__int64 nTrans4 = nCountUsb - max((!np[0])?0:nTimeChoosesUsb[0][CHOOSES_USB-1],(!np[1])?0:nTimeChoosesUsb[1][CHOOSES_USB-1]);
			if(nTrans4 <= TIME_TRANSI_EXIT_INTRO_USB)
			{
				dApt = (double(nTrans4))/(TIME_TRANSI_EXIT_INTRO_USB);
				glBindTexture(GL_TEXTURE_2D, usbres->skin[0]->GetCurrentTex());
				glPushMatrix();
				glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
				glEnd();
				glPopMatrix();
			}
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, usbres->skin[0]->GetCurrentTex());
			glBegin(GL_QUADS);
  			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
		}
		if(nCountUsb >= (TIME_INTRO_USB+TIME_APPEAR_INSTRUCTION_USB)
			&& nCountUsb < (TIME_INTRO_USB+TIME_APPEAR_INSTRUCTION_USB+TIME_EXPLOTION_INSTRUCTION_USB))
		{
			dApt = (double(nCountUsb)-TIME_INTRO_USB-TIME_APPEAR_INSTRUCTION_USB)/TIME_EXPLOTION_INSTRUCTION_USB;
			for(int x = 0; x < 2; x++) if(np[x] && !bFirstEnum[x])
			{
				glBindTexture(GL_TEXTURE_2D, usbres->skin[13]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated(x?20.0:-20.0, 0.0, 0.0);
				glScaled(1.0-0.2*(1-dApt), 1.0-0.2*(1-dApt), 1.0);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  11.25, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  11.25, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -11.25, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -11.25, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
		}
		if(nCountUsb >= (TIME_INTRO_USB+TIME_APPEAR_INSTRUCTION_USB+TIME_EXPLOTION_INSTRUCTION_USB))
		{
			for(int x = 0; x < 2; x++) if(np[x] && !bFirstEnum[x])
			{
				glBindTexture(GL_TEXTURE_2D, usbres->skin[13]->GetCurrentTex());
				glPushMatrix();
				glTranslated(x?20.0:-20.0, 0.0, 0.0);
				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  11.25, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  11.25, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -11.25, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -11.25, -50.0);
				glEnd();
				glPopMatrix();
			}
		}
#define TIME_TRANSI_ITEM_USB 300
#define TIME_START_ITEM_SIDE_USB 300
#define TIME_TRANSI_ITEM_SIDE_USB 300
#define TIME_START_NAME_USB 300
#define TIME_TRANSI_NAME_USB 300
#define TIME_START_SPLASH_USB 300
#define TIME_TRANSI_SPLASH_USB 300
#define TIME_START_ENUM_USB 300
#define TIME_TRANSI_ENUM_USB 300

#define TIME_GLOUTH_ITEM_USB 300
#define TIME_GLOUTH_ITEM_SIDE_USB 300
#define TIME_GLOUTH_NAME_USB 300
		for(int x = 0; x < 2; x++) if(bFirstEnum[x] && np[x])
		{
			if(nChoosesUsb[x] < CHOOSES_USB)
			{
				__int64 nTrans1 = nCountUsb-nFirstEnum[x];
				if(nTrans1 >= TIME_START_ITEM_SIDE_USB && nTrans1 < (TIME_START_ITEM_SIDE_USB+TIME_TRANSI_ITEM_SIDE_USB))
				{
					dApt = pow((double(nTrans1)-TIME_START_ITEM_SIDE_USB)/TIME_TRANSI_ITEM_SIDE_USB, 0.5);
					glBindTexture(GL_TEXTURE_2D, usbres->skin[2]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, -24.6875+9.375*(1-dApt), 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
					glEnd();
					glPopMatrix();
				}
				if(nTrans1 >= (TIME_START_ITEM_SIDE_USB+TIME_TRANSI_ITEM_SIDE_USB))
				{
					glBindTexture(GL_TEXTURE_2D, usbres->skin[2]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, -24.6875, 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
					glEnd();
					glPopMatrix();
					if(bRec[x])
					{
						__int64 nTrans2 = nCountUsb-nRec[x];
						if(nTrans2 <= TIME_GLOUTH_ITEM_SIDE_USB)
						{
							dApt = pow(double(nTrans2)/TIME_GLOUTH_ITEM_SIDE_USB, 0.5);
							glBindTexture(GL_TEXTURE_2D, usbres->skin[10]->GetCurrentTex());
							glPushMatrix();
							glTranslated((x?1:-1)*20.0, -24.6875, 0.0);
							glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBegin(GL_QUADS);
  							glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
					}
				}
				if(nTrans1 >= TIME_START_NAME_USB && nTrans1 < (TIME_START_NAME_USB+TIME_TRANSI_NAME_USB))
				{
					dApt = pow((double(nTrans1)-TIME_START_NAME_USB)/TIME_TRANSI_NAME_USB, 0.5);
					glBindTexture(GL_TEXTURE_2D, usbres->skin[3]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, 22.34375-4.6875*(1-dApt), 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
					glEnd();
					glPopMatrix();
				}
				if(nTrans1 >= (TIME_START_NAME_USB+TIME_TRANSI_NAME_USB))
				{
					glBindTexture(GL_TEXTURE_2D, usbres->skin[3]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, 22.34375, 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
					glEnd();
					glPopMatrix();
					if(bRec[x])
					{
						__int64 nTrans2 = nCountUsb-nRec[x];
						if(nTrans2 <= TIME_GLOUTH_NAME_USB)
						{
							dApt = pow(double(nTrans2)/TIME_GLOUTH_NAME_USB, 0.5);
							glBindTexture(GL_TEXTURE_2D, usbres->skin[11]->GetCurrentTex());
							glPushMatrix();
							glTranslated((x?1:-1)*20.0, 22.34375, 0.0);
							glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBegin(GL_QUADS);
  							glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
					}
				}
				if(nTrans1 < TIME_TRANSI_ITEM_USB)
				{
					dApt = double(nTrans1)/TIME_TRANSI_ITEM_USB;
					glBindTexture(GL_TEXTURE_2D, usbres->skin[1]->GetCurrentTex());
					glPushMatrix();
					glColor4d(1.0, 1.0, 1.0, dApt);
					glTranslated((x?1:-1)*40.0*(1-dApt)+(x?1:-1)*20.0, 0.0, 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
				if(nTrans1 >= TIME_TRANSI_ITEM_USB)
				{
					glBindTexture(GL_TEXTURE_2D, usbres->skin[1]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0+(x?1:-1)*20.0,  20.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0+(x?1:-1)*20.0,  20.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0+(x?1:-1)*20.0, -20.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0+(x?1:-1)*20.0, -20.0, -50.0);
					glEnd();
					if(bRec[x])
					{
						__int64 nTrans2 = nCountUsb-nRec[x];
						if(nTrans2 <= TIME_GLOUTH_ITEM_USB)
						{
							dApt = pow(double(nTrans2)/TIME_GLOUTH_ITEM_USB, 0.5);
							glBindTexture(GL_TEXTURE_2D, usbres->skin[9]->GetCurrentTex());
							glPushMatrix();
							glTranslated((x?1:-1)*20.0, 0.0, 0.0);
							glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
							glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
							glBegin(GL_QUADS);
  							glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
							glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
							glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
							glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
							glEnd();
							glColor4d(1.0, 1.0, 1.0, 1.0);
							glPopMatrix();
						}
					}
				}
				if(nTrans1 >= TIME_START_SPLASH_USB && nTrans1 < (TIME_START_SPLASH_USB+TIME_TRANSI_SPLASH_USB))
				{
					dApt = pow((double(nTrans1)-TIME_START_SPLASH_USB)/TIME_TRANSI_SPLASH_USB, 0.5);
					glPushMatrix();
					glTranslated((x?10.56640625:-29.43359375), 20.0, 0);
					glScaled(1.0-0.2*(1-dApt),1.0-0.2*(1-dApt),1.0);
					glColor4d(1.0, 1.0, 1.0, dApt);
					glBindTexture(GL_TEXTURE_2D, uTexSplash[nItemUp[x]]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-3.8635,  3.8635, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 3.8635,  3.8635, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 3.8635, -3.8635, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-3.8635, -3.8635, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, usbres->skin[4]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-4.6875,  4.6875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 4.6875,  4.6875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 4.6875, -4.6875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-4.6875, -4.6875, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
				if(nTrans1 >= (TIME_START_SPLASH_USB+TIME_TRANSI_SPLASH_USB))
				{
					glPushMatrix();
					glTranslated((x?10.56640625:-29.43359375), 20.0, 0);
					glBindTexture(GL_TEXTURE_2D, uTexSplash[nItemUp[x]]);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-3.8635,  3.8635, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 3.8635,  3.8635, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 3.8635, -3.8635, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-3.8635, -3.8635, -50.0);
					glEnd();
					glBindTexture(GL_TEXTURE_2D, usbres->skin[4]->GetCurrentTex());
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-4.6875,  4.6875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 4.6875,  4.6875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 4.6875, -4.6875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-4.6875, -4.6875, -50.0);
					glEnd();
					glPopMatrix();
					ShowProfile(x, nItemUp[x]);
				}
				for(int i = 0; i < nSizeItem; i++)
				{
					if(nTrans1 >= (TIME_START_ENUM_USB+TIME_TRANSI_ENUM_USB*i) && nTrans1 < (TIME_START_ENUM_USB+TIME_TRANSI_ENUM_USB*(i+1)))
					{
						dApt = pow((double(nTrans1)-(TIME_START_ENUM_USB+TIME_TRANSI_ENUM_USB*i))/TIME_TRANSI_ENUM_USB, 0.5);
						glBindTexture(GL_TEXTURE_2D, usbres->skin[6]->GetCurrentTex());
						glPushMatrix();
						glTranslated(0, -double(i)*2.5+7.5, 0);
						glScaled(1.0-0.2*(1-dApt),1.0-0.2*(1-dApt),1.0);
						glColor4d(1.0, 1.0, 1.0, dApt);
						glBegin(GL_QUADS);
  						glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  1.25, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  1.25, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -1.25, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -1.25, -50.0);
						glEnd();
						CString str;
						if(i != 0) str = ( plItem[i]->chName);
						else str = _T("[Machine]");
						font_Micro->glDrawText(1.25, -5.0, -1.25, 5.0, 0.0, 0.0, -50.0, 1.0, 1.2, str, DT_CENTER|DT_VCENTER);
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
					if(nTrans1 >= (TIME_START_ENUM_USB+TIME_TRANSI_ENUM_USB*(i+1)))
					{
						glBindTexture(GL_TEXTURE_2D, usbres->skin[6]->GetCurrentTex());
						glPushMatrix();
						glTranslated(0, -double(i)*2.5+7.5, 0);
						glBegin(GL_QUADS);
  						glTexCoord2d(0.0, 1.0); glVertex3d(-5.0,  1.25, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 5.0,  1.25, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 5.0, -1.25, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-5.0, -1.25, -50.0);
						glEnd();
						CString str;
						if(i != 0) str = ( plItem[i]->chName);
						else str = _T("[Machine]");
						font_Micro->glDrawText(1.25, -5.0, -1.25, 5.0, 0.0, 0.0, -50.0, 1.0, 1.2, str, DT_CENTER|DT_VCENTER);
						glPopMatrix();
					}
				}
				if(nChoosesUsb[x] > 0 && nChoosesUsb[x] < CHOOSES_USB)
				{
					__int64 nTrans3 = nCountUsb - nTimeChoosesUsb[x][nChoosesUsb[x]-1];
					dApt = pow(double(nTrans3%TIME_GLOUTH_ITEM_SIDE_USB)/TIME_GLOUTH_ITEM_SIDE_USB, 0.5);
					glBindTexture(GL_TEXTURE_2D, usbres->skin[10]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, -24.6875, 0.0);
					glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
					glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
					glEnd();
					glLoadIdentity();
					dApt = pow(double(nTrans3%TIME_GLOUTH_NAME_USB)/TIME_GLOUTH_NAME_USB, 0.5);
					glBindTexture(GL_TEXTURE_2D, usbres->skin[11]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, 22.34375, 0.0);
					glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
					glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
					glEnd();
					glLoadIdentity();
					dApt = pow(double(nTrans3%TIME_GLOUTH_ITEM_USB)/TIME_GLOUTH_ITEM_USB, 0.5);
					glBindTexture(GL_TEXTURE_2D, usbres->skin[9]->GetCurrentTex());
					glTranslated((x?1:-1)*20.0, 0.0, 0.0);
					glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
					glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
					glBegin(GL_QUADS);
					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
			}
			if(nChoosesUsb[x] >= CHOOSES_USB)
			{
#define TIME_MAXTRANSI_ITEM_USB 300
				__int64 nTrans5 = nCountUsb - nTimeChoosesUsb[x][CHOOSES_USB-1];
				if(nTrans5 < TIME_TRANSI_ITEM_SIDE_USB)
				{
					dApt = pow((double(nTrans5))/TIME_TRANSI_ITEM_SIDE_USB, 0.5);
					glBindTexture(GL_TEXTURE_2D, usbres->skin[2]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, -24.6875+9.375*(dApt), 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
					glEnd();
					glPopMatrix();
				}
				if(nTrans5 < TIME_TRANSI_NAME_USB)
				{
					dApt = pow((double(nTrans5))/TIME_TRANSI_NAME_USB, 0.5);
					glBindTexture(GL_TEXTURE_2D, usbres->skin[3]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, 22.34375-4.6875*(dApt), 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
					glEnd();
					glPopMatrix();
				}
				if(nTrans5 < TIME_MAXTRANSI_ITEM_USB)
				{
					glBindTexture(GL_TEXTURE_2D, usbres->skin[1]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?1:-1)*20.0, 0.0, 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
					glEnd();
					glPopMatrix();
				}
				if(nTrans5 > TIME_MAXTRANSI_ITEM_USB && nTrans5 < (TIME_MAXTRANSI_ITEM_USB+TIME_TRANSI_ITEM_USB))
				{
					dApt = (double(nTrans5)-TIME_MAXTRANSI_ITEM_USB)/TIME_TRANSI_ITEM_USB;
					glBindTexture(GL_TEXTURE_2D, usbres->skin[1]->GetCurrentTex());
					glPushMatrix();
					glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
					glTranslated((x?1:-1)*40.0*(dApt)+(x?1:-1)*20.0, 0.0, 0.0);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
				if(nTrans5 < TIME_TRANSI_SPLASH_USB)
				{
					dApt = pow((double(nTrans5))/TIME_TRANSI_SPLASH_USB, 0.5);
					glBindTexture(GL_TEXTURE_2D, usbres->skin[4]->GetCurrentTex());
					glPushMatrix();
					glTranslated((x?10.56640625:-29.43359375), 20.0, 0);
					glScaled(1.0-0.2*(dApt),1.0-0.2*(dApt),1.0);
					glColor4d(1.0, 1.0, 1.0, 1-dApt);
					glBegin(GL_QUADS);
  					glTexCoord2d(0.0, 1.0); glVertex3d(-4.6875,  4.6875, -50.0);
					glTexCoord2d(1.0, 1.0); glVertex3d( 4.6875,  4.6875, -50.0);
					glTexCoord2d(1.0, 0.0); glVertex3d( 4.6875, -4.6875, -50.0);
					glTexCoord2d(0.0, 0.0); glVertex3d(-4.6875, -4.6875, -50.0);
					glEnd();
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glPopMatrix();
				}
			}
		}
		if(nCountUsb <= (TIME_INTRO_USB+TIME_EXPLOTION_UPDOWN_USB))
		{
			dApt = (double(nCountUsb)-TIME_INTRO_USB)/TIME_EXPLOTION_UPDOWN_USB;
			glBindTexture(GL_TEXTURE_2D, usbres->skin[0]->GetCurrentTex());
			glPushMatrix();
			glColor4d(1.0, 1.0, 1.0, (1.0-dApt));
			glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
			glBegin(GL_QUADS);
  			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
		for(int x = 0; x < 2; x++)
		{
			if(x == 0)
			{
				if(bArrows[0] && np[0] && bFirstEnum[x])
				{
					bRec[0] = true;
					nRec[0] = nCountUsb;
					nItemDown[x] = nItemUp[x];
					nItemUp[x]--;
					if(nItemUp[x] < 0) nItemUp[x] = nSizeItem-1;
					nChoosesUsb[x] = 0;
					bArrows[0] = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
				}
				if(bArrows[4] && np[0] && bFirstEnum[x])
				{
					bRec[0] = true;
					nRec[0] = nCountUsb;
					nItemDown[x] = nItemUp[x];
					nItemUp[x]++;
					if(nItemUp[x] >= nSizeItem) nItemUp[x] = 0;
					nChoosesUsb[x] = 0;
					bArrows[4] = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
				}
				if(bArrows[2] && !np[0] /*Restricciones de las interfaces*/
				&& nInterAfterUsb != INT_GAME_3 && nCredit && nInterAfterUsb != INT_GAME_2)
				{
					PlusStagePlayer(0);
					nCredit--;
					bArrows[2] = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)4, sound_pulse, 0, &channels[4]);
				}
				if(bArrows[2] && np[0] && !bFirstEnum[x])
				{
					EnumUSBProfiles();
					GenShowProfiles();
					bFirstEnum[0] = true;
					nFirstEnum[0] = nCountUsb;
					nItemUp[x] = 0;
					nItemDown[x] = -1;
					bArrows[2] = false;
				}
				if(bArrows[2] && np[0] && bFirstEnum[x])
				{
					nChoosesUsb[x]++;
					if(nChoosesUsb[x] == 1) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
					if(nChoosesUsb[x] == 2) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					if(nChoosesUsb[x] <= CHOOSES_USB)
						nTimeChoosesUsb[x][nChoosesUsb[x]-1] = nCountUsb;
					bArrows[2] = false;
				}
			}
			if(x == 1)
			{
				if(bArrows[5] && np[1] && bFirstEnum[x])
				{
					bRec[1] = true;
					nRec[1] = nCountUsb;
					nItemDown[x] = nItemUp[x];
					nItemUp[x]--;
					if(nItemUp[x] < 0) nItemUp[x] = nSizeItem-1;
					nChoosesUsb[x] = 0;
					bArrows[5] = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
				}
				if(bArrows[9] && np[1] && bFirstEnum[x])
				{
					bRec[1] = true;
					nRec[1] = nCountUsb;
					nItemDown[x] = nItemUp[x];
					nItemUp[x]++;
					if(nItemUp[x] >= nSizeItem) nItemUp[x] = 0;
					nChoosesUsb[x] = 0;
					bArrows[9] = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)5, sound_change1, 0, &channels[5]);
				}
				if(bArrows[7] && !np[1] /*Restricciones de las interfaces*/
				&& nInterAfterUsb != INT_GAME_3 && nCredit && nInterAfterUsb != INT_GAME_2)
				{
					PlusStagePlayer(1);
					nCredit--;
					bArrows[7] = false;
					FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)4, sound_pulse, 0, &channels[4]);
				}
				if(bArrows[7] && np[1] && !bFirstEnum[x])
				{
					EnumUSBProfiles();
					GenShowProfiles();
					bFirstEnum[x] = true;
					nFirstEnum[x] = nCountUsb;
					nItemUp[x] = 0;
					nItemDown[x] = -1;
					bArrows[7] = false;
				}
				if(bArrows[7] && np[1] && bFirstEnum[x])
				{
					nChoosesUsb[x]++;
					if(nChoosesUsb[x] == 1) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)2, sound_preStart, 0, &channels[2]);
					if(nChoosesUsb[x] == 2) FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)3, sound_start, 0, &channels[3]);
					if(nChoosesUsb[x] <= CHOOSES_USB)
						nTimeChoosesUsb[x][nChoosesUsb[x]-1] = nCountUsb;
					bArrows[7] = false;
				}
			}
		}

	}
	return 1;
}

int DrawUSBSaveSystem()
{
#define TIME_EXITSAVE_OUT_USB 1000
	nCountUsb += dwTick-dwLastTick;
	DWORD dwTimeIntro = movie_USBintro->bLoad?movie_USBintro->dwDur:0;
	if(nCountUsb >= nTimeExitUsb)
	{
		if(movie_USBexit->bLoad)
		{
			movie_USBexit->Update(dwWait);
			movie_USBexit->GrabAVIFrame();
			glBindTexture(GL_TEXTURE_2D, movie_USBexit->tex);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
			glEnd();
		}
	}
	else
	{
		if(nCountUsb < dwTimeIntro)
		{
			if(movie_USBintro->bLoad)
			{
				movie_USBintro->Update(dwWait);
				movie_USBintro->GrabAVIFrame();
				glBindTexture(GL_TEXTURE_2D, movie_USBintro->tex);

				/*glBindTexture(GL_TEXTURE_2D, movie_texUsb);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, movie_USBintro->nwidth, movie_USBintro->nheight, GL_RGB, GL_UNSIGNED_BYTE, movie_USBintro->data);*/

				glBegin(GL_QUADS);
  				glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
				glEnd();
			}
		}
		else if(movie_USBloop->bLoad)
		{
			movie_USBloop->Update(dwWait);
			movie_USBloop->GrabAVIFrame();
			glBindTexture(GL_TEXTURE_2D, movie_USBloop->tex);

			glBegin(GL_QUADS);
  			glTexCoord2d(0.0, 1.0); glVertex3d(-80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 80.0, -60.0, -100.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 80.0,  60.0, -100.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-80.0,  60.0, -100.0);
			glEnd();
		}
	}

	double dApt = 0.0;
	if(nCountUsb > TIME_WAIT_INTRO_USB && nCountUsb < TIME_INTRO_USB)
	{
		dApt = (double(nCountUsb)-TIME_WAIT_INTRO_USB)/(TIME_INTRO_USB-TIME_WAIT_INTRO_USB);
		glBindTexture(GL_TEXTURE_2D, usbres->skin[0]->GetCurrentTex());
		glPushMatrix();
		glScaled(1.0+0.5*(1.0-dApt), 1.0+0.5*(1.0-dApt), 1.0);
		glBegin(GL_QUADS);
  		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
		glPopMatrix();
	}
	if(nCountUsb >= TIME_INTRO_USB && nCountUsb < nTimeExitUsb)
	{
		glBindTexture(GL_TEXTURE_2D, usbres->skin[0]->GetCurrentTex());
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
		glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
		glEnd();
		for(int x = 0; x < 2; x++) if(!np[x] && bSaveUsbActive[x])
		{
			__int64 nTrans1 = nCountUsb-TIME_INTRO_USB;
			if(nTrans1 >= TIME_START_ITEM_SIDE_USB && nTrans1 < (TIME_START_ITEM_SIDE_USB+TIME_TRANSI_ITEM_SIDE_USB))
			{
				dApt = pow((double(nTrans1)-TIME_START_ITEM_SIDE_USB)/TIME_TRANSI_ITEM_SIDE_USB, 0.5);
				glBindTexture(GL_TEXTURE_2D, usbres->skin[2]->GetCurrentTex());
				glPushMatrix();
				glTranslated((x?1:-1)*20.0, -24.6875+9.375*(1-dApt), 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTrans1 >= (TIME_START_ITEM_SIDE_USB+TIME_TRANSI_ITEM_SIDE_USB))
			{
				glBindTexture(GL_TEXTURE_2D, usbres->skin[2]->GetCurrentTex());
				glPushMatrix();
				glTranslated((x?1:-1)*20.0, -24.6875, 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
				glEnd();
				glPopMatrix();
				/*if(bRec[x])
				{
					__int64 nTrans2 = nCountUsb-nRec[x];
					if(nTrans2 <= TIME_GLOUTH_ITEM_SIDE_USB)
					{
						dApt = pow(double(nTrans2)/TIME_GLOUTH_ITEM_SIDE_USB, 0.5);
						glBindTexture(GL_TEXTURE_2D, usbres->skin[10]->GetCurrentTex());
						glPushMatrix();
						glTranslated((x?1:-1)*20.0, -24.6875, 0.0);
						glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
						glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
				}*/
			}
			if(nTrans1 >= TIME_START_NAME_USB && nTrans1 < (TIME_START_NAME_USB+TIME_TRANSI_NAME_USB))
			{
				dApt = pow((double(nTrans1)-TIME_START_NAME_USB)/TIME_TRANSI_NAME_USB, 0.5);
				glBindTexture(GL_TEXTURE_2D, usbres->skin[3]->GetCurrentTex());
				glPushMatrix();
				glTranslated((x?1:-1)*20.0, 22.34375-4.6875*(1-dApt), 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTrans1 >= (TIME_START_NAME_USB+TIME_TRANSI_NAME_USB))
			{
				glBindTexture(GL_TEXTURE_2D, usbres->skin[3]->GetCurrentTex());
				glPushMatrix();
				glTranslated((x?1:-1)*20.0, 22.34375, 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
				glEnd();
				glPopMatrix();
				/*if(bRec[x])
				{
					__int64 nTrans2 = nCountUsb-nRec[x];
					if(nTrans2 <= TIME_GLOUTH_NAME_USB)
					{
						dApt = pow(double(nTrans2)/TIME_GLOUTH_NAME_USB, 0.5);
						glBindTexture(GL_TEXTURE_2D, usbres->skin[11]->GetCurrentTex());
						glPushMatrix();
						glTranslated((x?1:-1)*20.0, 22.34375, 0.0);
						glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
						glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
				}*/
			}
			if(nTrans1 < TIME_TRANSI_ITEM_USB)
			{
				dApt = double(nTrans1)/TIME_TRANSI_ITEM_USB;
				glBindTexture(GL_TEXTURE_2D, usbres->skin[1]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, dApt);
				glTranslated((x?1:-1)*40.0*(1-dApt)+(x?1:-1)*20.0, 0.0, 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTrans1 >= TIME_TRANSI_ITEM_USB)
			{
				glBindTexture(GL_TEXTURE_2D, usbres->skin[1]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0+(x?1:-1)*20.0,  20.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0+(x?1:-1)*20.0,  20.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0+(x?1:-1)*20.0, -20.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0+(x?1:-1)*20.0, -20.0, -50.0);
				glEnd();
				/*if(bRec[x])
				{
					__int64 nTrans2 = nCountUsb-nRec[x];
					if(nTrans2 <= TIME_GLOUTH_ITEM_USB)
					{
						dApt = pow(double(nTrans2)/TIME_GLOUTH_ITEM_USB, 0.5);
						glBindTexture(GL_TEXTURE_2D, usbres->skin[9]->GetCurrentTex());
						glPushMatrix();
						glTranslated((x?1:-1)*20.0, 0.0, 0.0);
						glScaled(1.0+0.2*dApt,1.0+0.2*dApt,1.0);
						glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
						glBegin(GL_QUADS);
						glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
						glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
						glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
						glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
						glEnd();
						glColor4d(1.0, 1.0, 1.0, 1.0);
						glPopMatrix();
					}
				}*/
			}
			if(nTrans1 >= TIME_START_SPLASH_USB && nTrans1 < (TIME_START_SPLASH_USB+TIME_TRANSI_SPLASH_USB))
			{
				dApt = pow((double(nTrans1)-TIME_START_SPLASH_USB)/TIME_TRANSI_SPLASH_USB, 0.5);
				glPushMatrix();
				glTranslated((x?10.56640625:-29.43359375), 20.0, 0);
				glScaled(1.0-0.2*(1-dApt),1.0-0.2*(1-dApt),1.0);
				glColor4d(1.0, 1.0, 1.0, dApt);
				glBindTexture(GL_TEXTURE_2D, uTexPlayerSplash[x]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-3.8635,  3.8635, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 3.8635,  3.8635, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 3.8635, -3.8635, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-3.8635, -3.8635, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, usbres->skin[4]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-4.6875,  4.6875, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 4.6875,  4.6875, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 4.6875, -4.6875, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-4.6875, -4.6875, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTrans1 >= (TIME_START_SPLASH_USB+TIME_TRANSI_SPLASH_USB))
			{
				glPushMatrix();
				glTranslated((x?10.56640625:-29.43359375), 20.0, 0);
				glBindTexture(GL_TEXTURE_2D, uTexPlayerSplash[x]);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-3.8635,  3.8635, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 3.8635,  3.8635, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 3.8635, -3.8635, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-3.8635, -3.8635, -50.0);
				glEnd();
				glBindTexture(GL_TEXTURE_2D, usbres->skin[4]->GetCurrentTex());
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-4.6875,  4.6875, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 4.6875,  4.6875, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 4.6875, -4.6875, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-4.6875, -4.6875, -50.0);
				glEnd();
				glPopMatrix();
				ShowProfile(x, x);
			}
		}
		if(nCountUsb <= (TIME_INTRO_USB+TIME_EXPLOTION_UPDOWN_USB))
		{
			dApt = (double(nCountUsb-TIME_INTRO_USB))/TIME_EXPLOTION_UPDOWN_USB;
			glBindTexture(GL_TEXTURE_2D, usbres->skin[0]->GetCurrentTex());
			glPushMatrix();
			glColor4d(1.0, 1.0, 1.0, (1.0-dApt));
			glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
			glBegin(GL_QUADS);
  			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
			glColor4d(1.0, 1.0, 1.0, 1.0);
			glPopMatrix();
		}
	}

	if(nCountUsb >= nTimeExitUsb && nCountUsb < (nTimeExitUsb+TIME_EXITSAVE_OUT_USB))
	{
		__int64 nTrans5 = nCountUsb - (nTimeExitUsb);
		for(int x = 0; x < 2; x++) if(bSaveUsbActive[x])
		{
			if(nTrans5 < TIME_TRANSI_ITEM_SIDE_USB)
			{
				dApt = pow((double(nTrans5))/TIME_TRANSI_ITEM_SIDE_USB, 0.5);
				glBindTexture(GL_TEXTURE_2D, usbres->skin[2]->GetCurrentTex());
				glPushMatrix();
				glTranslated((x?1:-1)*20.0, -24.6875+9.375*(dApt), 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  4.6875, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  4.6875, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -4.6875, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -4.6875, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTrans5 < TIME_TRANSI_NAME_USB)
			{
				dApt = pow((double(nTrans5))/TIME_TRANSI_NAME_USB, 0.5);
				glBindTexture(GL_TEXTURE_2D, usbres->skin[3]->GetCurrentTex());
				glPushMatrix();
				glTranslated((x?1:-1)*20.0, 22.34375-4.6875*(dApt), 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  2.34375, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  2.34375, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -2.34375, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -2.34375, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTrans5 < TIME_MAXTRANSI_ITEM_USB)
			{
				glBindTexture(GL_TEXTURE_2D, usbres->skin[1]->GetCurrentTex());
				glPushMatrix();
				glTranslated((x?1:-1)*20.0, 0.0, 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
				glEnd();
				glPopMatrix();
			}
			if(nTrans5 > TIME_MAXTRANSI_ITEM_USB && nTrans5 < (TIME_MAXTRANSI_ITEM_USB+TIME_TRANSI_ITEM_USB))
			{
				dApt = (double(nTrans5)-TIME_MAXTRANSI_ITEM_USB)/TIME_TRANSI_ITEM_USB;
				glBindTexture(GL_TEXTURE_2D, usbres->skin[1]->GetCurrentTex());
				glPushMatrix();
				glColor4d(1.0, 1.0, 1.0, 1.0-dApt);
				glTranslated((x?1:-1)*40.0*(dApt)+(x?1:-1)*20.0, 0.0, 0.0);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-15.0,  20.0, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 15.0,  20.0, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 15.0, -20.0, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-15.0, -20.0, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
			if(nTrans5 < TIME_TRANSI_SPLASH_USB)
			{
				dApt = pow((double(nTrans5))/TIME_TRANSI_SPLASH_USB, 0.5);
				glBindTexture(GL_TEXTURE_2D, usbres->skin[4]->GetCurrentTex());
				glPushMatrix();
				glTranslated((x?10.56640625:-29.43359375), 20.0, 0);
				glScaled(1.0-0.2*(dApt),1.0-0.2*(dApt),1.0);
				glColor4d(1.0, 1.0, 1.0, 1-dApt);
				glBegin(GL_QUADS);
				glTexCoord2d(0.0, 1.0); glVertex3d(-4.6875,  4.6875, -50.0);
				glTexCoord2d(1.0, 1.0); glVertex3d( 4.6875,  4.6875, -50.0);
				glTexCoord2d(1.0, 0.0); glVertex3d( 4.6875, -4.6875, -50.0);
				glTexCoord2d(0.0, 0.0); glVertex3d(-4.6875, -4.6875, -50.0);
				glEnd();
				glColor4d(1.0, 1.0, 1.0, 1.0);
				glPopMatrix();
			}
		}
		if(nTrans5 <= TIME_TRANSI_EXIT_INTRO_USB)
		{
			dApt = (double(nTrans5))/(TIME_TRANSI_EXIT_INTRO_USB);
			glBindTexture(GL_TEXTURE_2D, usbres->skin[0]->GetCurrentTex());
			glPushMatrix();
			glScaled(1.0+0.5*(dApt), 1.0+0.5*(dApt), 1.0);
			glBegin(GL_QUADS);
			glTexCoord2d(0.0, 1.0); glVertex3d(-40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 1.0); glVertex3d( 40.0,  30.0, -50.0);
			glTexCoord2d(1.0, 0.0); glVertex3d( 40.0, -30.0, -50.0);
			glTexCoord2d(0.0, 0.0); glVertex3d(-40.0, -30.0, -50.0);
			glEnd();
			glPopMatrix();
		}
	}
	if(nCountUsb >= (nTimeExitUsb+TIME_EXITSAVE_OUT_USB))
	{
		return 0;
	}
	return 1;
}

////////////////////////////////////////////////
// ** Funciones principales

int CreateContextUsb()
{
	for(int x = 0; x < 7; x++) plItem[x] = new PLAYER();
	uTexSplash[0] = GenTexImage(_T("USB/USB_NO.png"));

	FMOD_System_CreateStream(sys, "USB/InsertUSB.mp3", FMOD_HARDWARE | FMOD_2D, 0, &sound_insertusb);

	return 1;
}

void DestroyContextUsb()
{
	//delete usbres;
	for(int x = 0; x < 7; x++) delete plItem[x];
	glDeleteTextures(7, uTexSplash);
	FMOD_Sound_Release(sound_insertusb);
}

void ActualizateContextUsb()
{
	movie_USBintro = new CMovieStream2();
	movie_USBloop = new CMovieStream2();
	movie_USBexit = new CMovieStream2();
	movie_USBintro->OpenAVI(_T("USB/USB_Intro.avi"));
	movie_USBloop->OpenAVI(_T("USB/usb_loop.avi"));
	movie_USBloop->bLoop = true;
	movie_USBexit->OpenAVI(_T("USB/usb_exit.avi"));
	usbres = new CUsbRes();
	usbres->LoadSkin();
	for(int x = 0; x < 2; x++)
	{
		nItemDown[x] = -1;
		nItemUp[x] = 0;
		nFirstEnum[x] = 0;
		bFirstEnum[x] = false;
		nRec[x] = 0;
		bRec[x] = false;
		nTimeChoosesUsb[0][x] = 0;
		nTimeChoosesUsb[1][x] = 0;
	}
	nUsb = 0;
	nSizeItem = 2;
	nChoosesUsb[0] = 0;
	nChoosesUsb[1] = 0;
	nItemUp[0] = 0;
	nItemUp[1] = 0;

	if(bSaveUsb)
	{
		for(int x = 0; x < 2; x++)
		{
			for(int y = (MAX_SKINS+MAX_CHARACTERS); y < 100; y++)
			{
				if(player_sesion[x]->nLockedAsp[y] == 1)
					player_sesion[x]->nLockedAsp[y] = 0;
			}
			memcpy(plItem[x], player_sesion[x], sizeof(PLAYER));
			if(!np[x] && bUsb[x] && bSaveUsbActive[x])
			{
				FILE* hFile = _fopen(strSave[x], _T("w+b"));
				if(hFile == NULL)
				{
					_printf(_T("El perfil no se ha podido guardar\n"));
				}
				else
				{
					fwrite(player_sesion[x], sizeof(PLAYER), 1, hFile);
					fclose(hFile);
				}
			}
			if(!np[x] && !bUsb[x] && bSaveUsbActive[x])
			{
				machine_sesion->nMileage = player_sesion[x]->nMileage;
				machine_sesion->nExp = player_sesion[x]->nExp;
				machine_sesion->nLevel = player_sesion[x]->nLevel;
				for(int z = 0; z < 10000; z++)
				{
					machine_sesion->nLockedSongNew[z] = player_sesion[x]->nLockedSongNew[z];
					if(z < 5000) machine_sesion->nLockedSpecNew[z] = player_sesion[x]->nLockedSpecNew[z];
					if(z < 1000)
					{
						machine_sesion->nLockedBrainShower[z] = player_sesion[x]->nLockedBrainShower[z];
						machine_sesion->nMission[z] = player_sesion[x]->nMission[z];
						machine_sesion->nUnlockedMission[z] = player_sesion[x]->nUnlockedMission[z];
						machine_sesion->nUnlockedWay[z] = player_sesion[x]->nUnlockedWay[z];
					}
					if(z < 100)
					{
						machine_sesion->nLockedAsp[z] = player_sesion[x]->nLockedAsp[z];
					}
					if(z < 20)
					{
						machine_sesion->nLockedChar[z] = player_sesion[x]->nLockedChar[z];
					}
				}
			}
		}
		SaveMachineProfile();
		GenShowProfiles();
	}
	else
	{
		bSaveUsbActive[0] = false;
		bSaveUsbActive[1] = false;
		FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)30, sound_insertusb, 0, &channels[30]);
	}
	nCountUsb = 0;
	nTimeExitUsb = bSaveUsb?TIME_EXITSAVE_USB:TIME_EXITNOR_USB;
	dwTick = dwLastTick = SDL_GetTicks();
}

int FrameUsb()
{
	usbres->Update(dwWait);
	if(!bSaveUsb)
	{
		if(DrawUSBSystem())
		{
			return INT_USB;
		}
		else
		{
			CString str1, str2;
			for(int x = 0; x < 2; x++)
			{
				str1 = ( chSaveItem[nItemUp[x]]);
				str2 = ( player_sesion[x]->chName);
				if(np[x] && (str1 != str2 || nItemUp[x] == 0))
				{
					memcpy(player_sesion[x], plItem[nItemUp[x]], sizeof(PLAYER));
					_strcpy(strSave[x], chSaveItem[nItemUp[x]]);
					if(nItemUp[x] != 0) _strcpy(chNameProfUsb[x], chItem[nItemUp[x]]);
					else _strcpy(chNameProfUsb[x], _T("\0"));
					if(nItemUp[x] > 0) bUsb[x] = nItemUp[x];
					else
					{
						bUsb[x] = 0;
					}
					bSaveUsbActive[x] = true;
					ShowSesionTray(x);
				}
				if(np[x]) uTexPlayerSplash[x] = uTexSplash[nItemUp[x]];
			}
			for(int x = 1; x < 7; x++)
			{
				if(uTexSplash[x] != uTexSplash[0] && x != nItemUp[0] && x != nItemUp[1]/* && bCheck*/)
					glDeleteTextures(1, &uTexSplash[x]);
			}
#ifndef _WIN32
			LinuxUSBumount();
#endif
		}
	}
	else
	{
		if(DrawUSBSaveSystem())
		{
			return INT_USB;
		}
		else
		{
			for(int x = 0; x < 2; x++) if(!np[x])
			{
				memset(player_sesion[x], 0, sizeof(PLAYER));
				_strcpy(strSave[x], _T("\0"));
				_strcpy(chNameProfUsb[x], _T("\0"));
				bUsb[x] = 0;
				bSaveUsbActive[x] = false;
				HideSesionTray(x);
				if(uTexPlayerSplash[x] != uTexSplash[0]) glDeleteTextures(1, &uTexPlayerSplash[x]);
			}
#ifndef _WIN32
			LinuxUSBumount();
#endif
		}
	}
	delete usbres;
	if(movie_USBintro->bLoad) movie_USBintro->CloseAVI();
	if(movie_USBloop->bLoad) movie_USBloop->CloseAVI();
	if(movie_USBexit->bLoad) movie_USBexit->CloseAVI();
	delete movie_USBintro;
	delete movie_USBloop;
	delete movie_USBexit;
	return 0;
}

int EventUsb(int nEvent, int nKey)
{
	if(nEvent == EV_KEYDOWN)
	{
		if(nKey == EV_P1_ARROW1) bArrows[0] = true;
		if(nKey == EV_P1_ARROW7) bArrows[1] = true;
		if(nKey == EV_P1_CENTER) bArrows[2] = true;
		if(nKey == EV_P1_ARROW9) bArrows[3] = true;
		if(nKey == EV_P1_ARROW3) bArrows[4] = true;
		if(nKey == EV_P2_ARROW1) bArrows[5] = true;
		if(nKey == EV_P2_ARROW7) bArrows[6] = true;
		if(nKey == EV_P2_CENTER) bArrows[7] = true;
		if(nKey == EV_P2_ARROW9) bArrows[8] = true;
		if(nKey == EV_P2_ARROW3) bArrows[9] = true;
	}
	if(nEvent == EV_KEYUP)
	{
		if(nKey == EV_P1_ARROW1) bArrows[0] = false;
		if(nKey == EV_P1_ARROW7) bArrows[1] = false;
		if(nKey == EV_P1_CENTER) bArrows[2] = false;
		if(nKey == EV_P1_ARROW9) bArrows[3] = false;
		if(nKey == EV_P1_ARROW3) bArrows[4] = false;
		if(nKey == EV_P2_ARROW1) bArrows[5] = false;
		if(nKey == EV_P2_ARROW7) bArrows[6] = false;
		if(nKey == EV_P2_CENTER) bArrows[7] = false;
		if(nKey == EV_P2_ARROW9) bArrows[8] = false;
		if(nKey == EV_P2_ARROW3) bArrows[9] = false;
	}
	return 1;
}
