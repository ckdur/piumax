#include "AccReader.h"

CAccReader::CAccReader(void)
{
	m_nSizeAttrib=0;
}

CAccReader::~CAccReader(void)
{
	ResizeAttribs(0);
}

int CAccReader::AddAttrib(LPCTSTR strAttrib, LPCTSTR strFormat, bool bIsMultiData, int nDependence /*= -1*/)
{
	if(nDependence >= m_nSizeAttrib) return 0;
	if(!ResizeAttribs(m_nSizeAttrib+1)) return 0;
	m_strAttrib[m_nSizeAttrib-1] = CString(strAttrib);
	m_strFormat[m_nSizeAttrib-1] = CString(strFormat);
	m_bIsMultiData[m_nSizeAttrib-1] = bIsMultiData;
	if(nDependence >= 0)
	{
		// ** Some dependency has this attrib
		// ** asegurate that is a valid dependency
		// ** if the nDependent attrib is dependent, use dependency of this
		if(m_nDependence[nDependence] >= 0)
			m_nDependence[m_nSizeAttrib-1] = m_nDependence[nDependence];
		else m_nDependence[m_nSizeAttrib-1] = nDependence;
	}
	return 1;
}

int CAccReader::DeleteAttrib(int nIndex)
{
	if(nIndex <= m_nSizeAttrib) return 0;
	m_nSizeAttrib--;
	vector< CString >::iterator it1;
	vector< int >::iterator it2;
	vector< bool >::iterator it3;
	vector< vector< vector< vector< CString > > > >::iterator it4;
	it1 = m_strAttrib.begin()+(size_t)nIndex;
	m_strAttrib.erase(it1);
	it1 = m_strFormat.begin()+(size_t)nIndex;
	m_strFormat.erase(it1);
	it2 = m_nDependence.begin()+(size_t)nIndex;
	m_nDependence.erase(it2);
	it3 = m_bIsMultiData.begin()+(size_t)nIndex;
	m_bIsMultiData.erase(it3);
	it4 = m_strData.begin()+(size_t)nIndex;
	m_strData.erase(it4);
	return 1;
}

// Resizes the attribs in the stack
int CAccReader::ResizeAttribs(int nNewSize)
{
	if(nNewSize<0) return 0;
	if(m_nSizeAttrib == nNewSize) return 1;

	m_strAttrib.resize(nNewSize, CString());
	m_strFormat.resize(nNewSize, CString());
	m_nDependence.resize(nNewSize, -1);
	m_bIsMultiData.resize(nNewSize, false);
	m_strData.resize(nNewSize, vector< vector< vector< CString > > >(0));

	m_nSizeAttrib = nNewSize;
	return 1;
}

int CAccReader::ReadLine(CString* strPar, FILE* hFile)
{
	(*strPar) = _T("");
	TCHAR ch[1000];	// Used for reading
	bool bDone = false;
	do
	{
		bDone = (_fgets(ch, 1000, hFile) == NULL);
		if(bDone)
		{
			if((*strPar).IsEmpty()) return 0;
			else break;
		}
		size_t f = _tcslen(ch);
		if(f == 0) return 0;
		if(ch[f-1] == _T('\n')) break;
	} while(!bDone);
	(*strPar) = ch;
	return 1;
}

int CAccReader::Read(LPCTSTR strName)
{
	if(m_nSizeAttrib<=0) return 0;

	// Open the file
	FILE* hFile = _tfopen(strName, _T("r+"));
	if(hFile == NULL)
    {
        _printf(_T("File not found: ") STROP _T("\n"), strName);
        return 0;
    }
	CString strPar;	// Used for complete parameters
	CString strArg;	// Used for extract formated data
	if(!ReadLine(&strPar, hFile))
	{
		fclose(hFile);
		return 0;
	}
	bool bCont = false;
	do
	{
		bCont = false;
		// NO ENDLINES
		strPar.Remove(_T('\n'));
		strPar.Remove(_T('\r'));
		int nCode = 0;

		// the "//" is comment, ignore it
		if((nCode = strPar.Find(_T("//"))) != -1)
		{
			strPar = strPar.Left(nCode);
		}
comparision:
		for(int i = 0; i < m_nSizeAttrib; i++)
		{
			// Search for comparision
			// #ATTRIBi:

			nCode = strPar.Find(_T('#'));
			// No numeral? pass this line
			if(nCode == -1) break;

			int nCode2 = strPar.Find(_T(':'));
			// No two points? pass this line
			if(nCode2 == -1) break;

			// strComp = "#ATTRIB"
			CString strComp = strPar.Mid(nCode, nCode2-nCode);
			// strComp2 = "#[ATTRIBi]"
			CString strComp2;
			strComp2 = _T("#") + m_strAttrib[i];

			// Is our attrib?
			if(strComp == strComp2)
			{
				// Add element data for this attrib
				AppendData(i);
				// If the attrib is multidata, will explore
				// the other lines while find another '#'
				if(m_bIsMultiData[i])
				{
					// Looping data
					while(ReadLine(&strPar, hFile))
					{
						strPar.Remove(_T('\n'));
						strPar.Remove(_T('\r'));
						if((nCode = strPar.Find(_T("//"))) != -1)
						{
							strPar = strPar.Left(nCode);
						}
						nCode = strPar.Find(_T('#'));
						nCode2 = strPar.Find(_T(':'));
						// If we can find both '#' and ':', recognize again
						// End of looping data
						if(nCode != -1 && nCode2 != -1) goto comparision;
						// Now, strPar is the argument, format it!
						FormatIt(i, strPar);
					}
				}
				else
				{
					strArg = strPar.Mid(nCode2+1, strPar.GetLength()-nCode2-1);
					// Now, strArg is the argument, format it!
					FormatIt(i, strArg);
				}
			}
		}
		strPar = _T("");
		if(!bCont)
		{
			bCont = ReadLine(&strPar, hFile)?true:false;
		}
	} while(bCont);
	fclose(hFile);
	FitData();
	return 1;
}

// This function add's new data to stack
// If depends of another attrib, this won't append
// The depend-on data will be added automatically
int CAccReader::AppendData(int nAttrib)
{
	// Attrib cannot be negative numbers
	if(nAttrib < 0) return 0;
	// Attrib must be here
	if(nAttrib >= m_nSizeAttrib) return 0;
	// Attrib is independent?
	if(m_nDependence[nAttrib] == -1)
		m_strData[nAttrib].push_back(vector<  vector< CString > >(0) );
	else
	{
		// Attrib is totally dependent, find our dependency
		// And resize it for compatibility
		m_strData[nAttrib].resize(
			m_strData[m_nDependence[nAttrib]].size(),
									vector< vector< CString > >(0));
	}
	return m_strData[nAttrib].size();
}

// This function formats a single line and append it at end of element
// FORMAT:
// #######
// DATA:
// ##data1##data2##data3##..##datan(##)
// Where # can be any wchar indicating separation of data
// # CANNOT BE " OR #
// Example:
// FORMAT = :
// DATA = 500:1:700:ASDG
int CAccReader::FormatIt(int nAttrib, CString strData)
{
	// No target element, no format
	int nElem = m_strData[nAttrib].size()-1;
	if(nElem <= -1) return -1;

	// No data, no format
	if(strData.IsEmpty()) return -1;

	vector< CString > strList(0);	// Here we store data temporally

	// Append one element
	int nDataComp = m_strData[nAttrib][nElem].size()-1;
	m_strData[nAttrib][nElem].push_back( vector< CString >(0) );
	int nData = m_strData[nAttrib][nElem].size()-1;
	// Prevention, maybe append the element is failed
	if(nDataComp >= nData || nData <= -1) return -1;

	// If this Attrib dont have any format
	// We'll append all the data without format
	if(m_strFormat[nAttrib].IsEmpty())
	{
		int nPrevItem = m_strData[nAttrib][nElem][nData].size()-1;
		m_strData[nAttrib][nElem][nData].push_back( strData );
		int nLastItem = m_strData[nAttrib][nElem][nData].size()-1;
		// Prevention, maybe append the item is failed
		if(nPrevItem >= nLastItem || nLastItem <= -1) return -1;
		return nLastItem;
	}
	// We will explore char by char, finding some match
	// for formatting

	int a1 = 0, a2 = 0;
	bool bChar = false;
	for(int i = 0; i < strData.GetLength(); i++)
	{
		// If is someone
		// ..."TEXT DATA"...
		// ACC stores it as new subdata, ignoring format
		// NOTE: if we find some '\"', will be replazced for '"'
		// and isn't used for format
		bool bMatch = false;
		bool bAnaText = false;
		if(strData.GetAt(i) == _T('\"')&& (i == 0 || strData.GetAt(i-1) != _T('\\')))
		{
			if(!bChar)
			{
				bChar = true;
				a1 = i+1;
			}
			else
			{
				bChar = false;
				a2 = i;
				bMatch = true;
				bAnaText = true;
			}
		}
		// Inside of "Data", skip this arg
		if(bChar) continue;
		// ** Analize for matching
		for(int j = 0; j < m_strFormat[nAttrib].GetLength(); j++)
		{
			// If we found some match, stop comprobating
			if(strData.GetAt(i) == m_strFormat[nAttrib].GetAt(j))
			{
				a2 = i;
				bMatch = true;
				break;
			}
		}
		// If no found match at end, asume it at finish
		if(i == (strData.GetLength()-1) && !bMatch)
		{
			a2 = i+1;
			bMatch = true;
		}
		// Really has |(a1|d|a|t|a|)|#a2|
		// MINIMUM FOR STORAGE: |Da1|#a2|
		if(bMatch)
		{
			if(bAnaText && a2>=a1)
			{
				strList.push_back(strData.Mid(a1, a2-a1));
				strList.back().Replace(_T("\\n"),_T("\n"));
				strList.back().Replace(_T("\\t"),_T("\t"));
			}
			else if(a2>a1)
				strList.push_back(strData.Mid(a1, a2-a1));
			a1 = a2+1;
		}
	}
	// Finished the data format, we can store it
	m_strData[nAttrib][nElem][nData] = strList;
	return 0;
}

void CAccReader::FitData(void)
{
	for(size_t nAttrib = 0; nAttrib < m_strData.size(); nAttrib++)
	{
		if(m_nDependence[nAttrib] != -1)
			m_strData[nAttrib].resize(
			m_strData[m_nDependence[nAttrib]].size(),
									vector< vector< CString > >(0));
	}
}

int CAccReader::ResetData(void)
{
	for(size_t x = 0; x < m_strData.size(); x++)
	{
		m_strData[x].clear();
	}
	return 1;
}

// ** Helpers
bool tx_double(LPCTSTR str, double* d)
{
	if(_sscanf(str, _T("%lg"), d)!=1) return false;
	return true;
}

bool tx_float(LPCTSTR str, float* d)
{
	if(_sscanf(str, _T("%g"), d)!=1) return false;
	return true;
}

bool tx_int(LPCTSTR str, int* d)
{
	if(_sscanf(str, _T("%d"), d)!=1) return false;
	return true;
}

bool tx_int_cat(LPCTSTR str, int* d, TCHAR** cat, size_t scat)
{
	if(tx_int(str, d)) return true;
	for(int s = 0; s < (int)(scat); s++) if(_tcscmp(*(cat+s), str) == 0)
	{
		(*d) = s;
		return true;
	}
	(*d) = (int)scat;
	return false;
}

bool tx_uint_cat(LPCTSTR str, unsigned int* d, TCHAR** cat, size_t scat)
{
	if(tx_uint(str, d)) return true;
	for(unsigned int s = 0; s < scat; s++) if(_tcscmp(*(cat+s), str) == 0)
	{
		(*d) = s;
		return true;
	}
	(*d) = scat;
	return false;
}

bool tx_uint(LPCTSTR str, unsigned int* d)
{
	if(_sscanf(str, _T("%u"), d)!=1) return false;
	return true;
}

bool tx_int64(LPCTSTR str, __int64* d)
{
	if(_sscanf(str, _T("%lld"), d)!=1) return false;
	return true;
}

bool tx_bool(LPCTSTR str, bool* d)
{
	int b = 0;
	if(!tx_int(str,	&b))
	{
		if(_tcscmp(str, _T("true")) == 0) b = 1;
		else if(_tcscmp(str, _T("false")) == 0) b = 0;
		else return false;
	}
	(*d) = b?true:false;
	return true;
}
