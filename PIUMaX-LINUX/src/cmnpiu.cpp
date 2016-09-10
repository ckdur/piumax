#include "stdafx.h"
#include "cmnpiu.h"

int nTotalCredit = 0;
png_uint_32 imgWidth;
png_uint_32 imgHeight;
PLAYER* player_sesion[2];
MACHINE_PROFILE* machine_sesion;
FMOD_CHANNEL *channels[MAX_CHANNELS];
CPortBuf* portLights;

__int64 nGeneralCount;

__int64 nTimeShowSesionTray[2];
__int64 nTimeShowSesionTrayMileage[2];
__int64 nTimeShowSesionTrayExp[2];
bool bShowSesionTray[2];
bool bShowSesionTrayMileage[2];
bool bShowSesionTrayExp[2];

// ** HERE BEGINS LIBUSB DEFS
#ifdef _WIN32
#include <lusb0_usb.h>
#else
#include <usb.h>
#endif

InputHandler_PIUIO* g_ihPIUIO;

/* static struct to ensure the USB subsystem is initialized on start */
struct USBInit
{
	USBInit() { usb_init(); usb_find_busses(); usb_find_devices(); }
};

static struct USBInit g_USBInit;

static struct usb_device *FindDevice( int iVendorID, int iProductID )
{
	for( usb_bus *bus = usb_get_busses(); bus; bus = bus->next )
		for( struct usb_device *dev = bus->devices; dev; dev = dev->next )
			if( iVendorID == dev->descriptor.idVendor && iProductID == dev->descriptor.idProduct )
				return dev;

	printf( "FindDevice(): no match for VID 0x%04x, PID 0x%04x.", iVendorID, iProductID );
	return NULL;
}

bool USBDriver_Impl::DeviceExists( uint16_t iVendorID, uint16_t iProductID )
{
	return FindDevice(iVendorID, iProductID) != NULL;
}

USBDriver_Impl::USBDriver_Impl()
{
	m_pHandle = NULL;
}

USBDriver_Impl::~USBDriver_Impl()
{
	Close();
}

bool USBDriver_Impl::Open( int iVendorID, int iProductID )
{
	Close();

	if( usb_find_busses() < 0 )
	{
		printf( "Libusb: usb_find_busses: %s", usb_strerror() );
		return false;
	}

	if( usb_find_devices() < 0 )
	{
		printf( "Libusb: usb_find_devices: %s", usb_strerror() );
		return false;
	}

	struct usb_device *dev = FindDevice( iVendorID, iProductID );

	if( dev == NULL )
	{
		printf( "Libusb: no match for %04x, %04x.", iVendorID, iProductID );
		return false;
	}

	m_pHandle = usb_open( dev );

	if( m_pHandle == NULL )
	{
		printf( "Libusb: usb_open: %s", usb_strerror() );
		return false;
	}

#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
	// The device may be claimed by a kernel driver. Attempt to reclaim it.

	for( unsigned iface = 0; iface < dev->config->bNumInterfaces; iface++ )
	{
		int iResult = usb_detach_kernel_driver_np( m_pHandle, iface );

		// no attached driver, no error -- ignore these
		if( iResult == -ENODATA || iResult == 0 )
			continue;

		printf( "usb_detach_kernel_driver_np: %s\n", usb_strerror() );


#ifdef LIBUSB_HAS_GET_DRIVER_NP
		// on EPERM, a driver exists and we can't detach - report which one
		if( iResult == -EPERM )
		{
			char szDriverName[16];
			strcpy( szDriverName, "(unknown)" );
			usb_get_driver_np(m_pHandle, iface, szDriverName, 16);

			printf( "(cannot detach kernel driver \"%s\")", szDriverName );
		}
#endif	// LIBUSB_HAS_GET_DRIVER_NP

		Close();
		return false;
	}
#endif	// LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP

	if ( !SetConfiguration(dev->config->bConfigurationValue) )
	{
		printf( "Libusb: usb_set_configuration: %s", usb_strerror() );
		Close();
		return false;
	}

	// attempt to claim all interfaces for this device
	for( unsigned i = 0; i < dev->config->bNumInterfaces; i++ )
	{
		if ( !ClaimInterface(i) )
		{
			printf( "Libusb: usb_claim_interface(%i): %s", i, usb_strerror() );
			Close();
			return false;
		}
	}

	return true;
}

void USBDriver_Impl::Close()
{
	// never opened
	if( m_pHandle == NULL )
		return;

	usb_set_altinterface( m_pHandle, 0 );
	usb_reset( m_pHandle );
	usb_close( m_pHandle );
	m_pHandle = NULL;
}

int USBDriver_Impl::ControlMessage( int iType, int iRequest, int iValue, int iIndex, char *pData, int iSize, int iTimeout )
{
	return usb_control_msg( m_pHandle, iType, iRequest, iValue, iIndex, pData, iSize, iTimeout );
}

int USBDriver_Impl::BulkRead( int iEndpoint, char *pData, int iSize, int iTimeout )
{
	return usb_bulk_read( m_pHandle, iEndpoint, pData, iSize, iTimeout );
}

int USBDriver_Impl::BulkWrite( int iEndpoint, char *pData, int iSize, int iTimeout )
{
	return usb_bulk_write( m_pHandle, iEndpoint, pData, iSize, iTimeout );
}

int USBDriver_Impl::InterruptRead( int iEndpoint, char *pData, int iSize, int iTimeout )
{
	return usb_interrupt_read( m_pHandle, iEndpoint, pData, iSize, iTimeout );
}

int USBDriver_Impl::InterruptWrite( int iEndpoint,char *pData, int iSize, int iTimeout )
{
	return usb_interrupt_write( m_pHandle, iEndpoint, pData, iSize, iTimeout );
}

bool USBDriver_Impl::SetConfiguration( int iConfig )
{
	return usb_set_configuration( m_pHandle, iConfig ) == 0;
}

bool USBDriver_Impl::ClaimInterface( int iInterface )
{
	return usb_claim_interface( m_pHandle, iInterface ) == 0;
}

bool USBDriver_Impl::ReleaseInterface( int iInterface )
{
	return usb_release_interface( m_pHandle, iInterface ) == 0;
}

const char* USBDriver_Impl::GetError() const
{
	return usb_strerror();
}

USBDriver::USBDriver()
{
	m_pDriver = NULL;
}

USBDriver::~USBDriver()
{
	delete m_pDriver;
}

bool USBDriver::OpenInternal( uint16_t iVendorID, uint16_t iProductID )
{
	Close();

	/* see if this device actually exists before trying to open it */
	if( !USBDriver_Impl::DeviceExists(iVendorID, iProductID) )
	{
		printf( "USBDriver::OpenInternal(0x%04x, 0x%04x): device does not exist\n", iVendorID, iProductID );
		return false;
	}

	m_pDriver = new USBDriver_Impl();

	/* if !m_pDriver, this build cannot support USB drivers. */
	if( m_pDriver == NULL )
	{
		printf( "USBDriver::OpenInternal(): Create failed. (No driver impl?" );
		return false;
	}

	return m_pDriver->Open( iVendorID, iProductID );
}

bool USBDriver::Open()
{
	return false;
}

void USBDriver::Close()
{
	if( m_pDriver )
		m_pDriver->Close();
}

const short PIUIO_VENDOR_ID	= 0x0547;
const short PIUIO_PRODUCT_ID	= 0x1002;

/* proprietary (arbitrary?) request PIUIO requires to handle I/O */
const short PIUIO_CTL_REQ = 0xAE;

/* timeout value for read/writes, in microseconds (so, 10 ms) */
const int REQ_TIMEOUT = 10000;

bool PIUIO::DeviceMatches( int iVID, int iPID )
{
	return iVID == PIUIO_VENDOR_ID && iPID == PIUIO_PRODUCT_ID;
}

bool PIUIO::Open()
{
	return OpenInternal( PIUIO_VENDOR_ID, PIUIO_PRODUCT_ID );
}

bool PIUIO::Read( uint32_t *pData )
{
	/* XXX: magic number left over from the ITG disassembly */
	int iExpected = 8;

	int iResult = m_pDriver->ControlMessage(
		USB_DIR_IN | USB_TYPE_VENDOR, PIUIO_CTL_REQ,
		0, 0, (char*)pData, iExpected, REQ_TIMEOUT );

	return iResult == iExpected;
}

bool PIUIO::Write( const uint32_t iData )
{
	/* XXX: magic number left over from the ITG disassembly */
	int iExpected = 8;

	int iResult = m_pDriver->ControlMessage(
		USB_DIR_OUT | USB_TYPE_VENDOR, PIUIO_CTL_REQ,
		0, 0, (char*)&iData, iExpected, REQ_TIMEOUT );

	return iResult == iExpected;
}

bool PIUIO::BulkReadWrite( uint32_t pData[8] )
{
	/* XXX: magic number left over from the ITG disassembly */
	int iExpected = 32;

	// this is caught by the r16 kernel hack, using '10011' as
	// a sentinel. the rest of the USB parameters aren't used.
	int iResult = m_pDriver->ControlMessage( 0, 0, 0, 0,
		(char*)pData, iExpected, 10011 );

	return iResult == iExpected;
}

bool InputHandler_PIUIO::s_bInitialized = false;

// simple helper function to automatically reopen PIUIO if a USB error occurs
static void Reconnect( PIUIO &board )
{
	printf( "PIUIO connection lost! Retrying..." );

	while( !board.Open() )
	{
		board.Close();
	}

	printf( "PIUIO reconnected." );
}

InputHandler_PIUIO::InputHandler_PIUIO()
{
    m_iLightData = 0x0;
    m_iChanged = 0;
	m_bFoundDevice = false;

	/* if a handler has already been created (e.g. by ScreenArcadeStart)
	 * and it has claimed the board, don't try to claim it again. */
	if( s_bInitialized )
	{
		printf( "InputHandler_PIUIO: Redundant driver loaded. Disabling..." );
		return;
	}

	// attempt to connect to the I/O board
	if( !Board.Open() )
	{
		printf( "InputHandler_PIUIO: Could not establish a connection with the I/O device." );
		return;
	}

	// set the relevant global flags (static flag, input type)
	m_bFoundDevice = true;
	s_bInitialized = true;
	m_bShutdown = false;

    m_iInputField = 0;
	m_iLastInputField = 0;

    bool bIsKernel = false;
#ifdef LINUX
    FILE *file;
    file = fopen("/rootfs/stats/patch/modules/usbcore.ko", "rb");
	if( file != NULL )
	{
        fclose(file);
        bIsKernel = true;
	}
#endif

	m_InputType = bIsKernel ? INPUT_KERNEL : INPUT_NORMAL;

	//SetLightsMappings();
}

InputHandler_PIUIO::~InputHandler_PIUIO()
{
	// reset all lights and unclaim the device
	if( m_bFoundDevice )
	{
		Board.Write( 0 );	// it's okay if this fails
		Board.Close();

		s_bInitialized = false;
	}
}

//void InputHandler_PIUIO::SetLightsMappings()
//{
	/*uint32_t iCabinetLights[NUM_CABINET_LIGHTS] =
	{
        // UL, UR, LL, LR marquee lights
		(1 << 23), (1 << 26), (1 << 25), (1 << 24),

		// selection buttons (not used), bass lights
		0, 0, (1 << 10), (1 << 10)
	};

	uint32_t iGameLights[MAX_GAME_CONTROLLERS][MAX_GAME_BUTTONS] =
	{
		// Left, Right, Up, Down
		{ (1 << 20), (1 << 21), (1 << 18), (1 << 19) },	// Player 1
		{ (1 << 4), (1 << 5), (1 << 2), (1 << 3) }	// Player 2
	};*/

	/* The coin counter moves halfway if we send bit 4, then the rest of
	 * the way when we send bit 5. If bit 5 is sent without bit 4 prior,
	 * the coin counter doesn't do anything, so we just keep it on and
	 * use bit 4 to pulse. */
	//uint32_t iCoinTriggers[2] = { (1 << 27), (1 << 28) };
//}

void InputHandler_PIUIO::InputThreadMain()
{
    UpdateLights();
    HandleInput();

    /* export the I/O values to the helper, for LUA binding */
    //MK6Helper::Import( m_iInputData, m_iLightData );
}
void InputHandler_PIUIO::HandleInput()
{
    if(!m_bFoundDevice) return;
	// reset our reading data
	m_iInputField = 0x0;
	memset(&m_iInputData, 0, sizeof(m_iInputData)/sizeof(uint32_t));

	/* PIU02 only reports one set of sensors (with four total sets) on each
	 * I/O request. In order to report all sensors for all arrows, we must:
	 *
	 * 1. Specify the requested set at bits 15-16 and 31-32 of the output
	 * 2. Write lights data and sensor selection to PIUIO
	 * 3. Read from PIUIO and save that set of sensor data
	 * 4. Repeat 2-3 until all four sets of sensor data are obtained
	 * 5. Bitwise OR the sensor data together to produce the final input
	 *
	 * The R16 kernel hack simply does all of this in kernel space, instead
	 * of alternating between kernel space and user space on each call.
	 * We pass it an 8-member uint32_t array with the lights data and get
	 * input in its place. (Why 8? I have no clue. We just RE'd it...)
	 */

	switch( m_InputType )
	{
	case INPUT_NORMAL:
		/* Normal input: write light data (with requested sensor set);
		 * perform one Write/Read cycle to get input; invert. */
		{
			for ( uint32_t i = 0; i < 4; ++i )
			{
				// write a set of sensors to request
				m_iLightData &= 0xFFFCFFFC;
				m_iLightData |= (i | (i << 16));

				// request this set of sensors
				while( !Board.Write(m_iLightData) )
					Reconnect( Board );

                m_iInputData[i] = 0x0;
				// read from this set of sensors
				while( !Board.Read(&m_iInputData[i]) )
					Reconnect( Board );

                m_iInputData[i] = ~m_iInputData[i];
			}
		}
		break;
	case INPUT_KERNEL:
		/* Kernel input: write light data (with desired sensor set)
		 * in array members 0, 2, 4, 6; call BulkReadWrite; invert
		 * input and copy it to our central data array. */
		{
            memset(&m_iBulkReadData, 0, sizeof(m_iBulkReadData)/sizeof(uint32_t));

			m_iLightData &= 0xFFFCFFFC;

			for( uint32_t i = 0; i < 4; ++i )
				m_iBulkReadData[i*2] = m_iLightData | (i | (i << 16));

			Board.BulkReadWrite( m_iBulkReadData );

			/* PIUIO opens high, so invert the input data. */
			for ( uint32_t i = 0; i < 4; ++i )
				m_iInputData[i] = ~m_iBulkReadData[i*2];
		}
		break;
	}

	// combine the read data into a single field
	for( int i = 0; i < 4; ++i )
		m_iInputField |= m_iInputData[i];

	// generate our input events bit field (1 = change, 0 = no change)
	m_iChanged = m_iInputField ^ m_iLastInputField;
	m_iLastInputField = m_iInputField;
}

void InputHandler_PIUIO::UpdateLights()
{
	// NO LIGHTS UPDATING
}
// HERE ENDS LIBUSB

void SetCurDir()
{
	TCHAR chCore[MAX_PATH];
	GetMaxProcessDirectory(chCore);
	SetMaxCurrentDirectory(chCore);
	_printf(_T("Set dir to process: %ls\n"), chCore);
}

void ClearDraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

int GetPowerOf2(int nSize)	// Devuelve el numero elevado al 2 mas cercano al numero especificado
{
	int nTest = 0, power = 0;
	while(nTest < nSize)
	{
		power++;
		nTest = (int)pow((double)2, power);
	}
	return nTest;
}

int ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return 0;
    }
	else return 1;
}

void GetSeccion(void *pSrcData, int nWidth, int nHeight, int nChan, int nx, int ny, int cx, int cy, void *pDstData)
{
	char *b = (char *)pSrcData;
	char *c = (char *)pDstData;
	for(int y = ny; y < (ny+cy); y++)
	{
		memcpy(c+(y-ny)*cx*nChan, b+(y*nWidth+nx)*nChan, nChan*cx*sizeof(char));
	}
	/*for(int x = 0; x < cx; x++)
	{
		for(int y = 0; y < cy; y++)
		{
			int ox = x+nx;
			int oy = y+ny;
			for(int a = 0; a < nChan; a++)
			{
				if(oy >= nHeight || ox >= nWidth)
				{
					c[(x+y*cx)*nChan+a] = 0;
				}
				else c[(x+y*cx)*nChan+a] = b[(oy*nWidth+ox)*nChan+a];
			}
		}
	}*/
	return;
}
#ifdef UNICODE
/*int UnicodeToAnsi(LPCWSTR pszW, LPSTR pszA)
{
    unsigned long cCharacters;
    if (NULL == pszA || NULL == pszW)
    {
        return 0;
    }
    cCharacters = (unsigned long)wcslen(pszW);
    if (0 >= wcstombs(pszA, pszW, cCharacters))
    {
        return 0;
    }
    *(pszA+cCharacters) = 0;
    return 1;
}*/
#endif // UNICODE

int SaveMachineProfile()
{
#ifdef PIUMAX_FOR_HOS
	FILE* hFileBin = _fopen(_T("/SAVE/PIUMAX/MAXmsave.bin"), _T("w+b"));
#else
    FILE* hFileBin = _fopen(_T("MAXmsave.bin"), _T("w+b"));
#endif
	if(hFileBin == NULL)
	{
        printf("Cannot save MAXmsave.bin");
        return 0;
	}
	fwrite(machine_sesion, sizeof(MACHINE_PROFILE), 1, hFileBin);
	fclose(hFileBin);
	return 1;
}

int AdaptMachineProfile(int nMode, int nPlayerd)
{
	if(nMode)	// Escribir datos de la máquina al perfil
	{
		for(int z = 0; z < 10000; z++)
		{
			if(machine_sesion->nLockedSongNew[z])
				player_sesion[nPlayerd]->nLockedSongNew[z] = machine_sesion->nLockedSongNew[z];
			if(z < 5000) if(machine_sesion->nLockedSpecNew[z])
				player_sesion[nPlayerd]->nLockedSpecNew[z] = machine_sesion->nLockedSpecNew[z];
			if(z < 1000)
			{
				if(machine_sesion->nLockedBrainShower[z])
					player_sesion[nPlayerd]->nLockedBrainShower[z] = machine_sesion->nLockedBrainShower[z];
				if(machine_sesion->nMission[z])
					player_sesion[nPlayerd]->nMission[z] = machine_sesion->nMission[z];
				if(machine_sesion->nUnlockedMission[z])
					player_sesion[nPlayerd]->nUnlockedMission[z] = machine_sesion->nUnlockedMission[z];
			}
			if(z < 100)
			{
				if(machine_sesion->nLockedAsp[z])
					player_sesion[nPlayerd]->nLockedAsp[z] = machine_sesion->nLockedAsp[z];
			}
			if(z < 20)
			{
				if(machine_sesion->nLockedChar[z])
					player_sesion[nPlayerd]->nLockedChar[z] = machine_sesion->nLockedChar[z];
			}
		}
	}
	else		// Escribir datos del perfil a la máquina
	{
	}
	return 1;
}

void ConvertCurMachineProfile(PLAYER** pl)
{
	memset((*pl), 0, sizeof(PLAYER));
	(*pl)->nLastMission = 0;
	(*pl)->nMileage = machine_sesion->nMileage;
	(*pl)->nLevel = machine_sesion->nLevel;
	(*pl)->nExp = machine_sesion->nExp;
	for(int z = 0; z < 10000; z++)
	{
		(*pl)->nLockedSongNew[z] = machine_sesion->nLockedSongNew[z];
		if(z < 5000) (*pl)->nLockedSpecNew[z] = machine_sesion->nLockedSpecNew[z];
		if(z < 1000)
		{
			(*pl)->nBgaSongNew[z] = 1;
			(*pl)->nBgaSpecNew[z] = 1;
			(*pl)->nLockedBrainShower[z] = machine_sesion->nLockedBrainShower[z];
			(*pl)->nMission[z] = machine_sesion->nMission[z];
			(*pl)->nUnlockedMission[z] = machine_sesion->nUnlockedMission[z];
			(*pl)->nUnlockedWay[z] = machine_sesion->nUnlockedWay[z];
		}
		if(z < 100)
		{
			(*pl)->nLockedAsp[z] = machine_sesion->nLockedAsp[z];
		}
		(*pl)->nLockedAsp[ITEM_BGAOFF] = 0;
		if(z < 20)
		{
			(*pl)->nLockedChar[z] = machine_sesion->nLockedChar[z];
		}
	}
}

void userReadData(png_structp pngPtr, png_bytep data, png_size_t length)
{
	png_voidp a = png_get_io_ptr(pngPtr);
	fread((char*)data, length, 1, (FILE*)a);
}

GLuint GenTexImage(LPCTSTR chName)
{
	GLuint texture;

	png_byte pngsig[PNGSIGSIZE];
    int is_png = 0;
	FILE *hFile = _fopen(chName, _T("rb"));
	if(hFile == NULL)
	{
        _printf(_T("Can't open file: %ls\n"), chName);
        return 0;
	}
	fread(pngsig, 1, PNGSIGSIZE, hFile);
	is_png = !png_sig_cmp(pngsig, 0, PNGSIGSIZE);
	if(!is_png)
	{
		fclose(hFile);
		return 0;
	}

	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr)
	{
		fclose(hFile);
		return 0;
	}
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		fclose(hFile);
		return 0;
	}
	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &infoPtr,(png_infopp)0);
		fclose(hFile);
		return 0;
	}

	png_set_read_fn(pngPtr, (voidp)hFile, userReadData);
	png_set_sig_bytes(pngPtr, PNGSIGSIZE);
    png_read_info(pngPtr, infoPtr);

	imgWidth =  png_get_image_width(pngPtr, infoPtr);
    imgHeight = png_get_image_height(pngPtr, infoPtr);
    png_uint_32 bitdepth   = png_get_bit_depth(pngPtr, infoPtr);
    png_uint_32 channels   = png_get_channels(pngPtr, infoPtr);
    png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);

	switch (color_type)
	{
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            break;
        case PNG_COLOR_TYPE_GRAY:
            if (bitdepth < 8)
				png_set_expand_gray_1_2_4_to_8(pngPtr);
            break;
    }
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(pngPtr);
    if (bitdepth == 16)
        png_set_strip_16(pngPtr);

	png_bytep* rowPtrs = new png_bytep[imgHeight];
	const char* data = new char[imgWidth * imgHeight * bitdepth * channels / 8];
    const unsigned int stride = imgWidth * bitdepth * channels / 8;
    for (size_t i = 0; i < imgHeight; i++)
	{
        unsigned int q = (imgHeight - i - 1) * stride;
        rowPtrs[i] = (png_bytep)data + q;
    }

	png_read_image(pngPtr, rowPtrs);
	png_read_end(pngPtr, NULL);
	png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
	fclose(hFile);
	delete rowPtrs;

	int mode = GL_RGBA;
	if(channels == 3) mode = GL_RGB;
	if((channels != 3 && channels != 4) || bitdepth != 8)
	{
		delete data;
		return 0;
	}

	glGenTextures(1, &texture);
	int nRx = GetPowerOf2(imgWidth);
	int nRy = GetPowerOf2(imgHeight);
	bool bIsResize = (nRx != imgWidth || nRy != imgHeight);
	char *rdata;
	if(bIsResize) rdata = new char[nRx*nRy*channels*bitdepth/8];
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	if(bIsResize)
		gluScaleImage(mode, imgWidth, imgHeight, GL_UNSIGNED_BYTE, data, nRx, nRy, GL_UNSIGNED_BYTE, rdata);
	if(bIsResize) gluBuild2DMipmaps(GL_TEXTURE_2D, mode, nRx, nRy, mode,GL_UNSIGNED_BYTE, rdata);
	else gluBuild2DMipmaps(GL_TEXTURE_2D, mode, nRx, nRy, mode,GL_UNSIGNED_BYTE, data);
	// Ahora vamos a almacenar las variables de acceso a las texturas
	if(bIsResize) delete rdata;
	delete data;
	return texture;
}

void GetDefPlayerFromFile(PLAYER** pl)
{
#ifdef PIUMAX_FOR_HOS
	FILE* hFilePlay = _fopen(_T("/SAVE/PIUMAX/MAXsave.bin"), _T("rb"));
#else
    FILE* hFilePlay = _fopen(_T("MAXsave.bin"), _T("rb"));
#endif
	if(hFilePlay == NULL) ConvertCurMachineProfile(&(*pl));
	else
	{
		fread((*pl), sizeof(PLAYER), 1, hFilePlay);
		fclose(hFilePlay);
	}
}

#ifndef ANUCREATOR

void ConfigGetDefaults(int nCaret)
{
	if(nCaret == 0)
	{
		piuconfig.nLevelMach = 1;
		piuconfig.nBarLevel = 2;
		piuconfig.nStageBreak = 1;
		piuconfig.nLevelJud = 2;
		piuconfig.nTimePlay = 0;
		piuconfig.nCoinsCred1 = 5;
		piuconfig.nCoinsCred2 = 5;
		piuconfig.nModeMach = 0;
		piuconfig.nStageLimit = 0;
		for(int x = 0; x < 10; x++)
		{
			for(int y = 0; y < 4; y++)
			{
				piuconfig.nWhereButtons[x][y] = y==0?-1:-2;
				piuconfig.nButtons[x][y] = 0;
			}
		}
		piuconfig.nServiceButton[0] = -2;
		piuconfig.nServiceButton[1] = 0;
		piuconfig.nServiceButton[2] = 0;
		piuconfig.nConfigButton[0] = -2;
		piuconfig.nConfigButton[1] = 0;
		piuconfig.nConfigButton[2] = 0;
		piuconfig.nCoinButton1[0] = -2;
		piuconfig.nCoinButton1[1] = 0;
		piuconfig.nCoinButton1[2] = 0;
		piuconfig.nCoinButton2[0] = -2;
		piuconfig.nCoinButton2[1] = 0;
		piuconfig.nCoinButton2[2] = 0;
		piuconfig.nButtons[0][0] = SDLK_z;
		piuconfig.nButtons[1][0] = SDLK_q;
		piuconfig.nButtons[2][0] = SDLK_s;
		piuconfig.nButtons[3][0] = SDLK_e;
		piuconfig.nButtons[4][0] = SDLK_c;
		piuconfig.nButtons[5][0] = SDLK_v;
		piuconfig.nButtons[6][0] = SDLK_r;
		piuconfig.nButtons[7][0] = SDLK_g;
		piuconfig.nButtons[8][0] = SDLK_y;
		piuconfig.nButtons[9][0] = SDLK_n;
		piuconfig.nMachineOffset = 0;
	}
	if(nCaret == 1)
	{
		piuconfig.nLevelMach = 1;
		piuconfig.nBarLevel = 2;
		piuconfig.nStageBreak = 2;
		piuconfig.nLevelJud = 2;
		piuconfig.nTimePlay = 1;
	}
	if(nCaret == 2)
	{
		piuconfig.nCoinsCred1 = 5;
		piuconfig.nCoinsCred2 = 5;
		piuconfig.nModeMach = 0;
		piuconfig.nStageLimit = 0;
		piuconfig.nSoundPrev = 1;
	}
}

void PlusTotalCredit()
{
#ifdef PIUMAX_FOR_HOS
	FILE* hFile = _fopen(_T("/SAVE/PIUMAX/credit.bin"), _T("w+b"));
#else
    FILE* hFile = _fopen(_T("credit.bin"), _T("w+b"));
#endif
	if(hFile == NULL) return;
	nTotalCredit++;
	fwrite(&nTotalCredit, sizeof(int), 1, hFile);
	fclose(hFile);
}

void PlusCredit(int nSet)
{
	if(nSet == 0 || nSet == 2)
	{
		nSubCredit1++;
		if(nSubCredit1 >= piuconfig.nCoinsCred1)
		{
			nSubCredit1 = 0;
			nCredit++;
			if(nSet == 0) PlusTotalCredit();
			_printf(_T("**Consola**: Creditos = %d\n"), nCredit);
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)0, sound_credit, 0, &channels[0]);
			FMOD_Channel_SetPosition(channels[0], 0, FMOD_TIMEUNIT_MS);
		}
		else
		{
			_printf(_T("**Consola**: SubCreditos1 = %d\n"), nSubCredit1);
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)0, sound_subcred, 0, &channels[0]);
			FMOD_Channel_SetPosition(channels[0], 0, FMOD_TIMEUNIT_MS);
		}
	}
	else
	{
		nSubCredit2++;
		if(nSubCredit2 >= piuconfig.nCoinsCred2)
		{
			nSubCredit2 = 0;
			nCredit++;
			PlusTotalCredit();
			_printf(_T("**Consola**: Creditos = %d\n"), nCredit);
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)0, sound_credit, 0, &channels[0]);
			FMOD_Channel_SetPosition(channels[0], 0, FMOD_TIMEUNIT_MS);
		}
		else
		{
			_printf(_T("**Consola**: SubCreditos2 = %d\n"), nSubCredit2);
			FMOD_System_PlaySound(sys, (FMOD_CHANNELINDEX)0, sound_subcred, 0, &channels[0]);
			FMOD_Channel_SetPosition(channels[0], 0, FMOD_TIMEUNIT_MS);
		}
	}
}

// ** GenDefMach Generador del perfil de la máquina
// ** Carga el archivo "def_mach.acc"
// ** y lo guarda en un binario en "MAXmsave.bin" en el directorio del programa

int GenDefPlayer()
{
	PLAYER* pl = new PLAYER();
#ifdef PIUMAX_FOR_HOS
	FILE* hFilePlay = _fopen(_T("/SAVE/PIUMAX/MAXsave.bin"), _T("wb"));
#else
    FILE* hFilePlay = _fopen(_T("MAXsave.bin"), _T("wb"));
#endif
	if(hFilePlay == NULL) return 0;
	ConvertCurMachineProfile(&pl);
	fwrite(pl, sizeof(PLAYER), 1, hFilePlay);
	fclose(hFilePlay);
	delete pl;
	return 1;
}

int LoadMachineProfile()
{
#ifdef PIUMAX_FOR_HOS
	FILE* hFileBin = _fopen(_T("/SAVE/PIUMAX/MAXmsave.bin"), _T("rb"));
#else
    FILE* hFileBin = _fopen(_T("MAXmsave.bin"), _T("rb"));
#endif
	if(hFileBin == NULL) {
        _printf(_T("Cannot load MAXmsave.bin\n"));
        return 0;
	}
	fread(machine_sesion, sizeof(MACHINE_PROFILE), 1, hFileBin);
	fclose(hFileBin);
	return 1;
}

int GenDefMach()
{
	FILE* hFileAcc = _fopen(_T("def_mach.acc"), _T("r"));
	if(hFileAcc == NULL)
	{
        _printf(_T("Cannot load def_mach.acc\n"));
        return 0;
	}
	TCHAR ch[500];

	// ** Inicializar los datos
	for(int z = 0; z < 10000; z++)
	{
		machine_sesion->nLockedSongNew[z] = 0;
		_strcpy(machine_sesion->chArcadeRecords[z], _T(""));
		machine_sesion->nArcadeRecords[z] = 0;
		if(z < 5000)
		{
			machine_sesion->nLockedSpecNew[z] = 0;
			_strcpy(machine_sesion->chSpecialRecords[z], _T(""));
			machine_sesion->nSpecialRecords[z] = 0;
		}
		if(z < 1000)
		{
			machine_sesion->nLockedBrainShower[z] = 0;
			machine_sesion->nMission[z] = 0;
			machine_sesion->nUnlockedMission[z] = 0;
			machine_sesion->nInfoAdventure[z] = 0;
			_strcpy(machine_sesion->chBrainShowerRecords[z], _T(""));
			machine_sesion->nBrainShowerRecords[z] = 0;
			machine_sesion->nBrainShowerRecordsEx[z] = 0;
		}
		if(z < 100) machine_sesion->nLockedAsp[z] = 0;
		if(z < 20)
		{
			machine_sesion->nBufRecords[z] = 0;
			_strcpy(machine_sesion->chRecords[z], _T(""));
			machine_sesion->nRecords[z] = 0;
			machine_sesion->nLockedChar[z] = 0;
		}
	}
	///////////////////////////
	int nSizeSongLockNew = 0;
	int nSizeSongLockRetro = 0;
	int nSizeSpecLockNew = 0;
	int nSizeSpecLockRetro = 0;
	int nSizeBrainShowerLock = 0;
	int nSizeMissionUnlocked = 0;
	int nSizeWayUnlocked = 0;
	int nSizeCharLock = 0;
	int nSizeAspLock = 0;
	while(_fgets(ch, 500, hFileAcc) != NULL)
	{
		CString str;
		str = ( ch);
		str.Remove(_T('\n')); str.Remove(_T('\r'));
		if(str.Find(_T('*')) == -1)	// No es un comentario
		{
			if(str.Find(_T("#SIZE_SONGLOCKNEW")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_SONGLOCKNEW %d"), &nSizeSongLockNew) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SIZE_SONGLOCKRETRO")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_SONGLOCKRETRO %d"), &nSizeSongLockRetro) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SIZE_SPECLOCKNEW")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_SPECLOCKNEW %d"), &nSizeSpecLockNew) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SIZE_SPECLOCKRETRO")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_SPECLOCKRETRO %d"), &nSizeSpecLockRetro) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SIZE_BRAINSHOWERLOCK")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_BRAINSHOWERLOCK %d"), &nSizeBrainShowerLock) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SIZE_MISSIONUNLOCKED")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_MISSIONUNLOCKED %d"), &nSizeMissionUnlocked) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SIZE_WAYUNLOCKED")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_WAYUNLOCKED %d"), &nSizeWayUnlocked) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SIZE_CHARLOCK")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_CHARLOCK %d"), &nSizeCharLock) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SIZE_ASPLOCK")) != -1)	// Channel
			{
				if(_sscanf(str, _T("#SIZE_ASPLOCK %d"), &nSizeAspLock) != 1) {fclose(hFileAcc); return 0;}
			}
			if(str.Find(_T("#SONGLOCKNEW")) != -1)	// Channel
			{
				for(int x = 0; x < nSizeSongLockNew; x++)
				{
					if(_fgets(ch, 500, hFileAcc) != NULL)
					{
						//CString str;
						str = ( ch);
						str.Remove(_T('\n')); str.Remove(_T('\r'));
						//if(str.Find(_T('*')) != -1) continue;
						int ns[11];
						if(_sscanf(str, _T("%d:\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d"), &ns[0], &ns[1], &ns[2], &ns[3], &ns[4], &ns[5], &ns[6], &ns[7], &ns[8], &ns[9], &ns[10]) < 1) {fclose(hFileAcc); return 0;}
						for(int y = 0; y < 10; y++)
						{
							machine_sesion->nLockedSongNew[ns[0]*10+y] = ns[y+1];
						}
					}
				}
			}
			if(str.Find(_T("#SPECLOCKNEW")) != -1)	// Channel
			{
				for(int x = 0; x < nSizeSpecLockNew; x++)
				{
					if(_fgets(ch, 500, hFileAcc) != NULL)
					{
						CString str;
						str = ( ch);
						str.Remove(_T('\n')); str.Remove(_T('\r'));
						int ns[6];
						if(_sscanf(str, _T("%d:\t%d\t%d\t%d\t%d\t%d"), &ns[0], &ns[1], &ns[2], &ns[3], &ns[4], &ns[5]) != 6) {fclose(hFileAcc); return 0;}
						for(int y = 0; y < 5; y++)
						{
							machine_sesion->nLockedSpecNew[ns[0]*5+y] = ns[y+1];
						}
					}
				}
			}
			if(str.Find(_T("#BRAINSHOWERLOCK")) != -1)	// Channel
			{
				for(int x = 0; x < nSizeBrainShowerLock; x++)
				{
					if(_fgets(ch, 500, hFileAcc) != NULL)
					{
						CString str;
						str = ( ch);
						str.Remove(_T('\n')); str.Remove(_T('\r'));
						int ns[2];
						if(_sscanf(str, _T("%d:\t%d"), &ns[0], &ns[1]) != 2) {fclose(hFileAcc); return 0;}
						machine_sesion->nLockedBrainShower[ns[0]] = ns[1];
					}
				}
			}
			if(str.Find(_T("#MISSIONUNLOCKED")) != -1)	// Channel
			{
				for(int x = 0; x < nSizeMissionUnlocked; x++)
				{
					if(_fgets(ch, 500, hFileAcc) != NULL)
					{
						CString str;
						str = ( ch);
						str.Remove(_T('\n')); str.Remove(_T('\r'));
						int ns;
						if(_sscanf(str, _T("%d"), &ns) != 1) {fclose(hFileAcc); return 0;}
						machine_sesion->nUnlockedMission[ns] = 1;
					}
				}
			}
			if(str.Find(_T("#WAYUNLOCKED")) != -1)	// Channel
			{
				for(int x = 0; x < nSizeWayUnlocked; x++)
				{
					if(_fgets(ch, 500, hFileAcc) != NULL)
					{
						CString str;
						str = ( ch);
						str.Remove(_T('\n')); str.Remove(_T('\r'));
						int ns;
						if(_sscanf(str, _T("%d"), &ns) != 1) {fclose(hFileAcc); return 0;}
						machine_sesion->nUnlockedWay[ns] = 1;
					}
				}
			}
			if(str.Find(_T("#CHARLOCK")) != -1)	// Channel
			{
				for(int x = 0; x < nSizeCharLock; x++)
				{
					if(_fgets(ch, 500, hFileAcc) != NULL)
					{
						CString str;
						str = ( ch);
						str.Remove(_T('\n')); str.Remove(_T('\r'));
						int ns[2];
						if(_sscanf(str, _T("%d:\t%d"), &ns[0], &ns[1]) != 2) {fclose(hFileAcc); return 0;}
						machine_sesion->nLockedChar[ns[0]] = ns[1];
					}
				}
			}
			if(str.Find(_T("#SIZE_ASPLOCK")) != -1)	// Channel
			{
				for(int x = 0; x < nSizeAspLock; x++)
				{
					if(_fgets(ch, 500, hFileAcc) != NULL)
					{
						CString str;
						str = ( ch);
						str.Remove(_T('\n')); str.Remove(_T('\r'));
						int ns[2];
						if(_sscanf(str, _T("%d:\t%d"), &ns[0], &ns[1]) != 2) {fclose(hFileAcc); return 0;}
						machine_sesion->nLockedAsp[ns[0]] = ns[1];
					}
				}
			}
		}
	}
	fclose(hFileAcc);
#ifdef PIUMAX_FOR_HOS
	FILE* hFileBin = _fopen(_T("/SAVE/PIUMAX/MAXmsave.bin"), _T("w+b"));
#else
    FILE* hFileBin = _fopen(_T("MAXmsave.bin"), _T("w+b"));
#endif
	if(hFileBin == NULL) {
        _printf(_T("Cannot save MAXmsave.bin\n"));
        return 0;
	}
	machine_sesion->nSizeSongLockNew = nSizeSongLockNew;
	machine_sesion->nSizeSongLockRetro = nSizeSongLockRetro;
	machine_sesion->nSizeSpecLockNew = nSizeSpecLockNew;
	machine_sesion->nSizeSpecLockRetro = nSizeSpecLockRetro;
	machine_sesion->nSizeBrainShowerLock = nSizeBrainShowerLock;
	machine_sesion->nSizeMissionUnlocked = nSizeMissionUnlocked;
	machine_sesion->nSizeCharLock = nSizeCharLock;
	machine_sesion->nSizeAspLock = nSizeAspLock;
	fwrite(machine_sesion, sizeof(MACHINE_PROFILE), 1, hFileBin);
	fclose(hFileBin);
	return 1;
}

void PlusStagePlayer(int nPlay)
{
	switch(piuconfig.nStageLimit)
	{
		case 0: np[nPlay] = 8; break;
		case 1: np[nPlay] = 6; break;
		case 2: np[nPlay] = 4; break;
		case 3: np[nPlay] = 2; break;
		default: np[nPlay] = 8; break;
	}
}

#define MAX_EXP 536870911
#define MAX_LVL 30

void PlusExpPlayer(int nPlayer, int nLvl, double dApt)
{
	if(nLvl <= 0) return;
	if(nPlayer > 1 || nPlayer < 0) return;
	if(player_sesion[nPlayer]->nLevel <= 0) player_sesion[nPlayer]->nLevel = 1;
	if(player_sesion[nPlayer]->nExp <= 0) player_sesion[nPlayer]->nExp = 1;
	double dPlusExp = double(nLvl)*2*dApt*player_sesion[nPlayer]->nLevel;
	if(nLvl <= (player_sesion[nPlayer]->nLevel-3))
		dPlusExp = 0;
	if(nLvl > (player_sesion[nPlayer]->nLevel-3) && nLvl < player_sesion[nPlayer]->nLevel)
		dPlusExp = dPlusExp/double(player_sesion[nPlayer]->nLevel-nLvl);
	if(nLvl > player_sesion[nPlayer]->nLevel)
		dPlusExp = dPlusExp*double(nLvl-player_sesion[nPlayer]->nLevel)*(nLvl > (player_sesion[nPlayer]->nLevel+3)?5:1);
	player_sesion[nPlayer]->nExp += int(dPlusExp);
	if(player_sesion[nPlayer]->nExp > MAX_EXP) player_sesion[nPlayer]->nExp = MAX_EXP;
	player_sesion[nPlayer]->nLevel = BYTE(log10(double(player_sesion[nPlayer]->nExp))/log10(2.0)+1);
}

void ShowSesionTray(int nPlayer)
{
	if(bUsb[nPlayer])
	{
		bShowSesionTray[nPlayer] = true;
		nTimeShowSesionTray[nPlayer] = nGeneralCount;
	}
}

void ShowSesionTrayMileage(int nPlayer)
{
	bShowSesionTrayMileage[nPlayer] = true;
	nTimeShowSesionTrayMileage[nPlayer] = nGeneralCount;
}

void ShowSesionTrayExp(int nPlayer)
{
	bShowSesionTrayExp[nPlayer] = true;
	nTimeShowSesionTrayExp[nPlayer] = nGeneralCount;
}

void HideSesionTray(int nPlayer)
{
	bShowSesionTray[nPlayer] = false;
}

void HideSesionTrayMileage(int nPlayer)
{
	bShowSesionTrayMileage[nPlayer] = false;
}

void HideSesionTrayExp(int nPlayer)
{
	bShowSesionTrayExp[nPlayer] = false;
}

void SetVolume(FMOD_BOOL bIsZero)
{
	for(int x = 1; x < MAX_CHANNELS; x++)
	{
		FMOD_System_GetChannel(sys, x, &channels[x]);
		FMOD_Channel_SetMute(channels[x], bIsZero);
	}
}
#endif

///////////////////////////////////////
