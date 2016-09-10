/*
 * The original MSD format is simply:
 * 
 * #PARAM0:PARAM1:PARAM2:PARAM3;
 * #NEXTPARAM0:PARAM1:PARAM2:PARAM3;
 * 
 * (The first field is typically an identifier, but doesn't have to be.)
 *
 * The semicolon is not optional, though if we hit a # on a new line, eg:
 * #VALUE:PARAM1
 * #VALUE2:PARAM2
 * we'll recover.
 *
 * TODO: Normal text fields need some way of escaping.  We need to be able to escape
 * colons and "//".  Also, we should escape #s, so if we really want to put a # at the
 * beginning of a line, we can. 
 */

/*
* This is a modified version of this MSD format reader
* Modificated by: CK-Dur
* This is for import .sm to .sksf2 for PIUMaX Project
*/

//#include "global.h"	// Maybe reemplaced by stdafx.h
#include "MsdFile.h"
//#include "RageLog.h"	// There is no log
//#include "RageUtil.h"	// Maybe reemplaced by basutil.h
//#include "RageFile.h"	// Reemplace all reading-writing operations by std-C++

void MsdFile::AddParam( char *buf, int len )
{
	values.back().params.push_back(CString(buf, len));
}

void MsdFile::AddValue() /* (no extra charge) */
{
	values.push_back(value_t());
	values.back().params.reserve( 32 );
}

void MsdFile::ReadBuf( char *buf, int len )
{
	values.reserve( 64 );

	int value_start = -1;

	bool ReadingValue=false;
	int i = 0;
	while(i < len)
	{
		if( i+1 < len && buf[i] == '/' && buf[i+1] == '/' )
		{
			/* //; erase with spaces until newline */
			do {
				buf[i] = ' ';
				i++;
			} while(i < len && buf[i] != '\n');

			continue;
		}

		if(ReadingValue && buf[i] == '#') {
			/* Unfortunately, many of these files are missing ;'s.
			 * If we get a # when we thought we were inside a value, assume we
			 * missed the ;.  Back up and end the value. */
			/* Make sure this # is the first non-whitespace character on the line. */
			bool FirstChar = true;
			int j;
			for(j = i-1; j >= 0 && !strchr("\r\n", buf[j]); --j)
			{
				if(buf[j] == ' ' || buf[j] == '\t')
					continue;

				FirstChar = false;
				break;
			}

			if(!FirstChar) {
				/* Oops, we're not; handle this like a regular character. */
				i++;
				continue;
			}

			/* Skip newlines and whitespace before adding the value. */
			while( j >= 1 && strchr("\r\n", buf[j-1]) )
				--j;

			AddParam(buf+value_start, j - value_start);
			ReadingValue=false;
		}

		/* # starts a new value. */
		if(!ReadingValue && buf[i] == '#') {
			AddValue();
			ReadingValue=true;
		}

		if(!ReadingValue)
		{
			i++;
			continue; /* nothing else is meaningful outside of a value */
		}

		/* : and ; end the current param, if any. */
		if(value_start != -1 && (buf[i] == ':' || buf[i] == ';'))
			AddParam(buf+value_start, i - value_start);

		/* # and : begin new params. */
		if(buf[i] == '#' || buf[i] == ':')
		{
			i++; /* skip */
			value_start = i;
			continue;
		}

		/* ; ends the current value. */
		if(buf[i] == ';')
			ReadingValue=false;

		i++;
	}
	
	/* Add any unterminated value at the very end. */
	if(ReadingValue)
		AddParam(buf+value_start, i - value_start);
}

// returns true if successful, false otherwise
bool MsdFile::ReadFile( CString sNewPath )
{
	error = "";

	//PIUMaXreplaced 
	ifstream f;

	/* Open a file. */

	f.open(sNewPath, ios::in | ios::binary);
	
	if( !f.is_open())
	{
		error = _T("No file opened");
		return false;
	}

	// allocate a string to hold the file
	
	CString FileString;

	streampos begin, end;
	begin = f.tellg();
	f.seekg(0, ifstream::end);
	end = f.tellg();
	f.seekg(0, ifstream::beg);
	

	int size = int(end-begin);
	FileString.reserve( size );

	int iBytesRead = -1;
	char buf[4096];
	int ret = 0;
	while(1)
	{
		int ToRead = min(sizeof(buf), size-ret);

		int nret = int(f.tellg());
		f.read( buf, ToRead );
		int got = int(f.tellg())-nret;
		if( got == 0 )
			break;
		if( got < 0 ) 
		{
			iBytesRead = -1;
			break;
		}

		FileString.append( buf, got );

		ret += got;
		iBytesRead = ret;
	}

	f.close();
	
	
	if( iBytesRead == -1 )
	{
		error = _T("Can't read file in specific bytes");
		return false;
	}

	// END OF MODIFICATION

	ReadBuf( (char*) FileString.c_str(), iBytesRead );

	return true;
}

CString MsdFile::GetParam(unsigned val, unsigned par) const
{
	if(val >= GetNumValues()) return "";
	if(par >= GetNumParams(val)) return "";

	return values[val].params[par];
}

/*
 * (c) 2001-2004 Chris Danford, Glenn Maynard
 *
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
