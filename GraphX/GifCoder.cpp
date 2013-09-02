// GifCoder.cpp: implementation of the CGifCoder class.
//

#include "stdafx.h"
#include "GifCoder.h"

// GifCoder.cpp: implementation of the CGifCoder class.
//
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CGifCoder::CGifCoder()
{
	for (int i= 0; i< 256; ++i)
	{
		images[i].PicBuf= NULL;
		images[i].ColTable= NULL;
	}
	m_nFrames= 0;
}

CGifCoder::~CGifCoder()
{
	if (m_nFrames> 0)
		for (int i= 0; i< m_nFrames; ++i)
		{
			delete[] images[i].ColTable;
			delete[] images[i].PicBuf;
		}
}

int CGifCoder::ReadGif ()
{
	struct Tgifgce {
		byte transpcolflag:1;
		byte userinputflag:1;
		byte dispmeth:3;
		byte res:3;
		byte delaytimelo;
		byte delaytimehi;
		byte transpcolindex;
	} gifgce= {0,0,0,0,0,0,0};
	int i;
	int m_TrIndex;
	TColTable ColTable, LocTable;


	memcpy (&dscgif, m_Buf+ m_BufPos, 13);
	m_BufPos+= 13;
	if (m_BufPos> m_FileLength)
		return ERROR_FILE_FORMAT;
	if (strncmp (dscgif.header, "GIF8", 4)!=0)
		return ERROR_FILE_FORMAT;

////////////////////////// AD - for interlace ///////////////////////////////
	ColTable.colres = short ((dscgif.pflds & 0x07) +1);
	ColTable.sogct = short (1 << ColTable.colres);


// Delete old data -------------------------------------------------------------
	if (m_nFrames> 0)
		for (i= 0; i< m_nFrames; ++i)
		{
			delete[] images[i].ColTable;
			images[i].ColTable= NULL;
			delete[] images[i].PicBuf;
			images[i].PicBuf= NULL;
		}

	m_nFrames= 0;

// Global colour map?
	if (dscgif.pflds & 0x80)
	{
		memcpy (ColTable.paleta, m_Buf+m_BufPos, 3*ColTable.sogct);
		m_BufPos+= 3*ColTable.sogct;
		if (m_BufPos> m_FileLength)
			return ERROR_FILE_FORMAT;
	}

// Setting bgrnd index ---------------------------------------------------------
	i= dscgif.bcindx;
	m_BgColor= RGB(ColTable.paleta[i].r,ColTable.paleta[i].g,ColTable.paleta[i].b);


	byte ch;
	m_TrIndex= -1;
	while (m_BufPos< m_FileLength && m_nFrames< m_pLFO->PageNumber)
	{
		ch= m_Buf[m_BufPos++];
		if (m_BufPos> m_FileLength)
			return ERROR_FILE_FORMAT;

		if (ch == '!')                     // extension
		{
			byte count;
			byte fc;

			fc= m_Buf[m_BufPos++];
			if (m_BufPos> m_FileLength)
				break;


		//////////////// AD - for transparency //////////////////////////
			if (fc == 0xF9)
			{
				count= m_Buf[m_BufPos++];
				if (m_BufPos> m_FileLength)
					break;
				memcpy (&gifgce, m_Buf+m_BufPos, 4);
				m_BufPos+= 4;
				if (m_BufPos> m_FileLength)
					break;

				if (gifgce.transpcolflag)
					m_TrIndex = gifgce.transpcolindex;
				else
					m_TrIndex = -1;
			}

			while (m_BufPos< m_FileLength && m_Buf[m_BufPos++])
				m_BufPos+= m_Buf[m_BufPos-1];
		}
		else if (ch == ',')                 // image
		{
			memcpy (&image, m_Buf+m_BufPos, 9);
			m_BufPos+= 9;
			if (m_BufPos> m_FileLength)
				return ERROR_FILE_FORMAT;

			// Local colour map?
			if (image.pf & 0x80)
			{
				LocTable.sogct = short (1 << ((image.pf & 0x07) +1));
				memcpy (LocTable.paleta, m_Buf+m_BufPos, 3*LocTable.sogct);
				m_BufPos+= 3*LocTable.sogct;
				if (m_BufPos> m_FileLength)
					return ERROR_FILE_FORMAT;
			}

/*
			if ((image.pf & 0x80) || (dscgif.pflds & 0x80))
			{
				BOOL has_white = false;

				for (i=0; i < LocTable.sogct; i++)
					if (RGB (LocTable.paleta[i].r, LocTable.paleta[i].g, LocTable.paleta[i].b)== 0xFFFFFF)
						has_white = true;

			// Make transparency colour black...
				//if (m_TrIndex != -1)
				//	r[m_TrIndex] = g[m_TrIndex] = b[m_TrIndex] = 0;

			// Fill in with white // AD
				if (m_TrIndex!= -1)
				{
					has_white = true;
					while (i < 256)
					{
						LocTable.paleta[i].r= LocTable.paleta[i].g= LocTable.paleta[i].b= 255;
						i++;
					}
				}

			// Force last colour to white...   // AD
				if ((m_TrIndex != -1) && !has_white)
					LocTable.paleta[255].r= LocTable.paleta[255].g= LocTable.paleta[255].b= 255;

			}
*/		

			int badcode;
			
			m_IsInterlaced = image.pf & 0x40;
			m_istep = 8;
			if (m_IsInterlaced)
				m_iypos = 0;
			else
				m_iypos= image.h-1;
			m_ipass = 0;


			images[m_nFrames].x= image.l;
			images[m_nFrames].y= image.t;
			images[m_nFrames].cx= image.w;
			images[m_nFrames].cy= image.h;
			images[m_nFrames].delay= gifgce.delaytimehi*256+ gifgce.delaytimelo;
			images[m_nFrames].nRes= ColTable.colres;

			images[m_nFrames].TrIndex= m_TrIndex;
			images[m_nFrames].RemoveMeth= gifgce.dispmeth;

		// Setting color table ---------------------------------------------------------
			if ((images[m_nFrames].ColTable= new COLORREF[256])== NULL)
				return ERROR_MEMORY_TOO_LOW;
			if (image.pf & 0x80)			// have local table
				for (i= 0; i< 256; ++i)
					images[m_nFrames].ColTable[i]= RGB(LocTable.paleta[i].r,LocTable.paleta[i].g,LocTable.paleta[i].b);
			else
				for (i= 0; i< 256; ++i)
					images[m_nFrames].ColTable[i]= RGB(ColTable.paleta[i].r,ColTable.paleta[i].g,ColTable.paleta[i].b);

			if ((m_PicBuf= images[m_nFrames].PicBuf= new byte[(image.w+4)*image.h])== NULL)
				return ERROR_MEMORY_TOO_LOW;

			m_nRowsPassed= 0;
			int nRes;
			if ((nRes= Decoder(image.w , badcode))< 0)
				return nRes;
			++m_nFrames;
		}
		else if (ch== ';')			// end identificator -----------------------------
		{
			return SUCCESS;
		}
			
	}

	return SUCCESS;
}



















/* get_next_code()
 * - gets the next code from the GIF file.  Returns the code, or else
 * a negative number in case of file errors...
 */
short CGifCoder::GetNextCode()
{
	int i;
	byte x;;
	UINT ret;
	static byte b1;                           /* Current byte */
	static byte byte_buff[257];               /* Current block */
	static byte *pbytes;                      /* Pointer to next byte in block */

	if (nbits_left == 0)
	{
		if (navail_bytes <= 0)
		{

			/* Out of bytes in current block, so read next block
			 */
			pbytes = byte_buff;
			if ((navail_bytes = GetByte()) < 0)
				return navail_bytes;
			else if (navail_bytes)
			{
				for (i = 0; i < navail_bytes; ++i)
				{
					if ((x = GetByte()) < 0)
						return x;
					byte_buff[i] = x;
				}
			}
		}
		b1 = *pbytes++;
		nbits_left = 8;
		--navail_bytes;
	}

	ret = b1 >> (8 - nbits_left);
	while (curr_size > nbits_left)
	{
		if (navail_bytes <= 0)
		{

			/* Out of bytes in current block, so read next block
			 */
			pbytes = byte_buff;
			if ((navail_bytes = GetByte()) < 0)
				return navail_bytes;
			else if (navail_bytes)
			{
				for (i = 0; i < navail_bytes; ++i)
				{
					if ((x = GetByte()) < 0)
						return x;
					byte_buff[i] = x;
				}
			}
		}
		b1 = *pbytes++;
		ret |= b1 << nbits_left;
		nbits_left += 8;
		--navail_bytes;
	}
	nbits_left= short(nbits_left-curr_size);
	ret &= CodeMask[curr_size];
	return ((SHORT)(ret));
}



/* SHORT decoder(linewidth)
 *    SHORT linewidth;               * Pixels per line of image *
 *
 * - This function decodes an LZW image, according to the method used
 * in the GIF spec.  Every *linewidth* "characters" (ie. pixels) decoded
 * will generate a call to out_line(), which is a user specific function
 * to display a line of pixels.  The function gets it's codes from
 * get_next_code() which is responsible for reading blocks of data and
 * seperating them into the proper size codes.  Finally, get_byte() is
 * the global routine to read the next byte from the GIF file.
 *
 * It is generally a good idea to have linewidth correspond to the actual
 * width of a line (as specified in the Image header) to make your own
 * code a bit simpler, but it isn't absolutely necessary.
 *
 * Returns: 0 if successful, else negative.  (See ERRS.H)
 *
 */

SHORT CGifCoder::Decoder(SHORT linewidth, INT& bad_code_count)
{
	register byte *sp, *bufptr;
	byte *buf= NULL;
	register SHORT code, fc, oc, bufcnt;
	SHORT c, size, ret;
	short slot;                          /* Last read code */
	short top_slot;                      /* Highest code for current size */
	short newcodes;                      /* First available code */
	short ending;                        /* Value for a ending code */
	short clear;                         /* Value for a clear code */
	byte stack[MAX_CODES + 1];            /* Stack for storing pixels */
	byte suffix[MAX_CODES + 1];           /* Suffix table */
	USHORT prefix[MAX_CODES + 1];           /* Prefix linked list */


	/* Initialize for decoding a new image...
	 */
	try
	{
	bad_code_count = 0;
	if ((size = GetByte()) < 0)
		return ERROR_FILE_FORMAT;
	if (size < 2 || 9 < size)
		return ERROR_FILE_FORMAT;

	curr_size = short (size + 1);
	top_slot = short (1 << curr_size);
	clear = short (1 << size);
	ending = short (clear + 1);
	slot = newcodes = short (ending + 1);
	navail_bytes = nbits_left = 0;

	/* Initialize in case they forgot to put in a clear code.
	 * (This shouldn't happen, but we'll try and decode it anyway...)
	 */
	oc = fc = 0;

   /* Allocate space for the decode buffer
    */
	if ((buf = new byte[linewidth + 1]) == NULL)
		return ERROR_MEMORY_TOO_LOW;

   /* Set up the stack pointer and decode buffer pointer
    */

	sp = stack;
	bufptr = buf;
	bufcnt = linewidth;

   /* This is the main loop.  For each code we get we pass through the
    * linked list of prefix codes, pushing the corresponding "character" for
	 * each code onto the stack.  When the list reaches a single "character"
	 * we push that on the stack too, and then start unstacking each
    * character for output in the correct order.  Special handling is
	 * included for the clear code, and the whole thing ends when we get
    * an ending code.
    */
	while ((c = GetNextCode()) != ending)
	{
      /* If we had a file error, return without completing the decode
       */
	if (c < 0)
	{
		delete[] buf;
		return 0;
	}

      /* If the code is a clear code, reinitialize all necessary items.
		 */
	if (c == clear)
	{
		curr_size = short (size + 1);
		slot = newcodes;
		top_slot = short (1 << curr_size);

         /* Continue reading codes until we get a non-clear code
          * (Another unlikely, but possible case...)
          */
		while ((c = GetNextCode()) == clear)
            ;

			/* If we get an ending code immediately after a clear code
          * (Yet another unlikely case), then break out of the loop.
          */
		if (c == ending)
			break;

         /* Finally, if the code is beyond the range of already set codes,
          * (This one had better NOT happen...  I have no idea what will
			 * result from this, but I doubt it will look good...) then set it
          * to color zero.
          */
		if (c >= slot)
			c = 0;

		oc = fc = c;

         /* And let us not forget to put the char into the buffer... And
			 * if, on the off chance, we were exactly one pixel from the end
          * of the line, we have to send the buffer to the out_line()
          * routine...
          */
		*bufptr++ = (byte)c;
		if (--bufcnt == 0)
		{
			if ((ret = (short)OutLine(buf, linewidth)) < 0)
			{
				delete[] buf;
				return ret;
			}
            bufptr = buf;
            bufcnt = linewidth;
		}
	}
	else
	{

         /* In this case, it's not a clear code or an ending code, so
          * it must be a code code...  So we can now decode the code into
			 * a stack of character codes. (Clear as mud, right?)
          */
		code = c;

         /* Here we go again with one of those off chances...  If, on the
          * off chance, the code we got is beyond the range of those already
			 * set up (Another thing which had better NOT happen...) we trick
          * the decoder into thinking it actually got the last code read.
          * (Hmmn... I'm not sure why this works...  But it does...)
          */
		if (code >= slot)
        {
			if (code > slot)
				++bad_code_count;
			code = oc;
			*sp++ = (byte)fc;
		}

			/* Here we scan back along the linked list of prefixes, pushing
          * helpless characters (ie. suffixes) onto the stack as we do so.
          */
		while (code >= newcodes)
        {
			*sp++ = suffix[code];
			code = prefix[code];
		}

         /* Push the last character on the stack, and set up the new
          * prefix and suffix, and if the required slot number is greater
          * than that allowed by the current bit size, increase the bit
          * size.  (NOTE - If we are all full, we *don't* save the new
          * suffix and prefix...  I'm not certain if this is correct...
          * it might be more proper to overwrite the last code...
          */
		*sp++ = (byte)code;
		if (slot < top_slot)
		{
			fc= code;
			suffix[slot] = (byte)fc;
			prefix[slot++] = oc;
			oc = c;
		}
		if (slot >= top_slot)
			if (curr_size < 12)
			{
				top_slot <<= 1;
				++curr_size;
			} 

         /* Now that we've pushed the decoded string (in reverse order)
          * onto the stack, lets pop it off and put it into our decode
          * buffer...  And when the decode buffer is full, write another
          * line...
          */
		while (sp > stack)
        {
	        *bufptr++ = *(--sp);
		    if (--bufcnt == 0)
			{
				if ((ret = (short)OutLine(buf, linewidth)) < 0)
				{
					delete[] buf;
					return ret;
				}
				bufptr = buf;
				bufcnt = linewidth;
			}
		}
	}
	}
	ret = 0;
	if (bufcnt != linewidth)
		ret = (short)OutLine(buf, (linewidth - bufcnt));
	}
	catch (int)
	{
		if (buf)
			delete[] buf;
		return ERROR_FILE_FORMAT;		
	}
	if (buf)
		delete[] buf;
	return ret;
}










// ************************************************************************************
//
// Description	    : 
//
// Function name	: CGifCoder::OutLine
// Return type		: int 
// Argument         : byte *pixels
// Argument         : int LineLen
//
int CGifCoder::OutLine(byte *pixels, int LineLen)
{
	int pos;
	if (m_IsInterlaced)
	{
		int y= image.h- m_iypos-1;
		if (LineLen> image.w || y< 0 || y>= image.h)
			return ERROR_FILE_FORMAT;

		pos= y*((LineLen+3)/4*4);
		memcpy (m_PicBuf+pos, pixels, LineLen);

		if ((m_iypos += m_istep) >= image.h)
			do
			{
				if (m_ipass++ > 0)
					m_istep/= 2;
				m_iypos= m_istep / 2;
			}
			while (m_iypos > image.h);
	}
	else
	{
		if (LineLen> image.w || m_iypos< 0 || m_iypos>= image.h)
			return ERROR_FILE_FORMAT;
		pos= m_iypos*((LineLen+3)/4*4);
		memcpy (m_PicBuf+pos, pixels, LineLen);
		--m_iypos;
	}
	
	++m_nRowsPassed;
	
	// Call callback function
	if (m_pfnCallback && m_nFrames== m_pLFO->PageNumber-1)
		m_pfnCallback (&m_FileInfo, m_pBitmap, NULL, 0, m_nRowsPassed, 0, m_pUserData);


	return SUCCESS;
}











// ************************************************************************************
//
// Description	    : 
//
// Function name	: CGifCoder::OpenFile
// Return type		: int - number of frames (0 - if error)
// Argument         : LPCTSTR fname
//
int CGifCoder::OpenPicture(LPCTSTR fname)
{
// Read file to memory buffer --------------------------------------------------
	CFile f;
	if (!f.Open (fname, CFile::modeRead | CFile::typeBinary))
		return ERROR_FILE_OPEN;

	try
	{
		m_FileLength= f.GetLength();
		m_Buf= new byte[m_FileLength];
		f.Read (m_Buf, m_FileLength);
		f.Close();
	}
	catch (CFileException* pErr)
	{
		pErr->Delete();
		delete[] m_Buf;
		return ERROR_FILE_READ;
	}


// Decode GIF ------------------------------------------------------------------
	m_BufPos= 0;
	int res= ReadGif();
	delete[] m_Buf;

	if (res== SUCCESS && m_nFrames)
		return SUCCESS;

	for (int i= 0; i< m_nFrames; ++i)
	{
		delete[] images[i].ColTable;
		images[i].ColTable= NULL;
		delete[] images[i].PicBuf;
		images[i].PicBuf= NULL;
	}
	m_nFrames= 0;

	if (res== SUCCESS)
		res= ERROR_FILE_FORMAT;
	return res;
}




// ************************************************************************************
//
// Description	    : Opens gif picture from memory buffer
//
// Function name	: CGifCoder::OpenPictureMemory
// Return type		: int - number of frames (0 - if error)
//
// Argument         : BYTE *buf		- bufer
// Argument         : DWORD len		- bufer length
//
int CGifCoder::OpenPictureMemory(BYTE *buf, DWORD len)
{
	m_BufPos= 0;
	m_Buf= buf;
	m_FileLength= len;

// Decode GIF ------------------------------------------------------------------
	int res= ReadGif();

	if (res== SUCCESS && m_nFrames)
		return SUCCESS;
	for (int i= 0; i< m_nFrames; ++i)
	{
		delete[] images[i].ColTable;
		images[i].ColTable= NULL;
		delete[] images[i].PicBuf;
		images[i].PicBuf= NULL;
	}
	m_nFrames= 0;

	if (res== SUCCESS)
		res= ERROR_FILE_FORMAT;
	return res;
}




inline byte CGifCoder::GetByte()
{
	if (m_BufPos< m_FileLength)
		return m_Buf[m_BufPos++];
	else
	{
		throw -1;
		return 0;
	}
}



void CGifCoder::GetTotalSize(int &cx, int &cy)
{
	cx= dscgif.scrwidth;
	cy= dscgif.scrheight;
}




// ************************************************************************************
//
// Description	    : 
//
// Function name	: CGifCoder::LoadBitmap
// Return type		: L_INT 
// Argument         : L_CHAR L_FAR* pszFile
// Argument         : pBITMAPHANDLE pBitmap
// Argument         : L_INT nBitsPerPixel
// Argument         : L_INT nOrder
// Argument         : pLOADFILEOPTION pLoadOptions
//
L_INT CGifCoder::LoadBitmap(L_CHAR L_FAR* pszFile, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT /*nOrder*/, pLOADFILEOPTION pLoadOptions)
{
	try
	{
		int nErr;

	// Load picture in our format -------------------------------------------------
		m_pfnCallback= NULL;
		LOADFILEOPTION lfo;
		if (pLoadOptions== NULL)
		{
			lfo.PageNumber= 1;
			m_pLFO= &lfo;
		}
		else
			m_pLFO= pLoadOptions;

		nErr= OpenPicture (pszFile);
		if (nErr!= SUCCESS)
			throw nErr;

		CreateLTBitmap (pBitmap, nBitsPerPixel);
	}
	catch (int nErr)
	{
		return nErr;
	}

	return SUCCESS;
}










// ************************************************************************************
//
// Description	    : Loads memory buffer to bitmap
//
// Function name	: CGifCoder::LoadBitmapMemory
// Return type		: L_INT 
// Argument         : L_CHAR L_FAR *pBuffer
// Argument         : pBITMAPHANDLE pBitmap
// Argument         : L_INT nBitsPerPixel
// Argument         : L_INT nOrder
// Argument         : L_INT32 nBufferSize
// Argument         : pLOADFILEOPTION pLoadOptions
// Argument         : pFILEINFO pFileInfo
//
L_INT CGifCoder::LoadBitmapMemory(L_CHAR L_FAR *pBuffer, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT /*nOrder*/, L_INT32 nBufferSize, pLOADFILEOPTION pLoadOptions, pFILEINFO /*pFileInfo*/)
{
	try
	{
		int nErr;

	// Load picture in our format -------------------------------------------------
		m_pfnCallback= NULL;
		LOADFILEOPTION lfo;
		if (pLoadOptions== NULL)
		{
			lfo.PageNumber= 1;
			m_pLFO= &lfo;
		}
		else
			m_pLFO= pLoadOptions;

		nErr= OpenPictureMemory ((BYTE*)pBuffer, nBufferSize);
		if (nErr!= SUCCESS)
			throw nErr;

		CreateLTBitmap (pBitmap, nBitsPerPixel);
	}
	catch (int nErr)
	{
		return nErr;
	}

	return SUCCESS;
}





// ************************************************************************************
//
// Description	    : Loads file from disk using a callback function
//
// Function name	: CGifCoder::LoadFile
// Return type		: L_INT 
// Argument         : L_CHAR* pszFile
// Argument         : pBITMAPHANDLE pBitmap
// Argument         : L_INT nBitsPerPixel
// Argument         : L_INT nOrder
// Argument         : L_UINT uFlags
// Argument         : FILEREADCALLBACK pfnCallback
// Argument         : L_VOID* pUserData
// Argument         : pLOADFILEOPTION pLoadOptions
//
L_INT CGifCoder::LoadFile(L_CHAR* pszFile, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT /*nOrder*/, L_UINT /*uFlags*/, FILEREADCALLBACK pfnCallback, L_VOID* pUserData, pLOADFILEOPTION pLoadOptions)
{
	try
	{
		int nErr;

	// Load picture in our format -------------------------------------------------
		m_pfnCallback= pfnCallback;
		m_pUserData= pUserData;
		m_pBitmap= pBitmap;
		nErr= GraphXnamespace::L_FileInfo (pszFile, &m_FileInfo, 0, NULL);
		if (nErr!= SUCCESS)
			throw nErr;

		LOADFILEOPTION lfo;
		if (pLoadOptions== NULL)
		{
			lfo.PageNumber= 1;
			m_pLFO= &lfo;
		}
		else
			m_pLFO= pLoadOptions;


		nErr= OpenPicture (pszFile);
		if (nErr!= SUCCESS)
			throw nErr;

		CreateLTBitmap (pBitmap, nBitsPerPixel);
	}
	catch (int nErr)
	{
		return nErr;
	}

	return SUCCESS;
}








// ************************************************************************************
//
// Description	    : Loads file from memory using a callback function
//
// Function name	: CGifCoder::LoadMemory
// Return type		: L_INT 
// Argument         : L_CHAR L_FAR *pBuffer
// Argument         : pBITMAPHANDLE pBitmap
// Argument         : L_INT nBitsPerPixel
// Argument         : L_INT nOrder
// Argument         : L_UINT uFlags
// Argument         : FILEREADCALLBACK pfnCallback
// Argument         : L_VOID L_FAR *pUserData
// Argument         : L_INT32 nBufferSize
// Argument         : pLOADFILEOPTION pLoadOptions
// Argument         : pFILEINFO pFileInfo
//
L_INT CGifCoder::LoadMemory(L_CHAR L_FAR *pBuffer, pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel, L_INT /*nOrder*/, L_UINT /*uFlags*/, FILEREADCALLBACK pfnCallback, L_VOID L_FAR *pUserData, L_INT32 nBufferSize, pLOADFILEOPTION pLoadOptions, pFILEINFO /*pFileInfo*/)
{
	try
	{
		int nErr;

	// Load picture in our format -------------------------------------------------
		m_pfnCallback= pfnCallback;
		m_pUserData= pUserData;
		m_pBitmap= pBitmap;
		nErr= GraphXnamespace::L_FileInfoMemory (pBuffer, &m_FileInfo, nBufferSize, 0, NULL);
		if (nErr!= SUCCESS)
			throw nErr;

		LOADFILEOPTION lfo;
		if (pLoadOptions== NULL)
		{
			lfo.PageNumber= 1;
			m_pLFO= &lfo;
		}
		else
			m_pLFO= pLoadOptions;


		nErr= OpenPictureMemory ((BYTE*)pBuffer, nBufferSize);
		if (nErr!= SUCCESS)
			throw nErr;


		CreateLTBitmap (pBitmap, nBitsPerPixel);
	}
	catch (int nErr)
	{
		return nErr;
	}

	return SUCCESS;
}






// ************************************************************************************
//
// Description	    : Creates bitmap in GraphX format from our format
//
// Function name	: CGifCoder::CreateLTBitmap
// Return type		: void 
// Argument         : pBITMAPHANDLE pBitmap
// Argument         : L_INT nBitsPerPixel
//
void CGifCoder::CreateLTBitmap(pBITMAPHANDLE pBitmap, L_INT nBitsPerPixel)
{
	int nErr;

// Creating the image in GraphX format --------------------------------------
	int nPage= m_pLFO->PageNumber-1;
	if (nPage< 0 || nPage>= m_nFrames)
		throw ERROR_INV_PARAMETER;

	nErr= GraphXnamespace::L_CreateBitmap (pBitmap, TYPE_CONV, images[nPage].cx, images[nPage].cy, 
		images[nPage].nRes, ORDER_BGR, (LPRGBQUAD)images[nPage].ColTable, TOP_LEFT,
		NULL, 0);		
	if (nErr!= SUCCESS)
		throw nErr;

	pBitmap->Left= images[nPage].x;
	pBitmap->Top= images[nPage].y;
	pBitmap->Delay= images[nPage].delay;
	pBitmap->Background= m_BgColor;
	pBitmap->Transparency= images[nPage].ColTable[images[nPage].TrIndex];
	pBitmap->DisposalMethod= images[nPage].RemoveMeth;

// Coping image data -----------------------------------------------------------
	int nOffsDst, nOffsSrc= 0;
	int DstBytesPerLine= ((pBitmap->Width+3)/4)*4;
	BYTE* pSrcBits= images[nPage].PicBuf;
	BYTE* pDstBits= pBitmap->Addr.Windows.pData;
	int nRes= images[nPage].nRes;

	if (nRes== 8)
	{
		for (int y= 0; y< pBitmap->Height; ++y)
		{
			nOffsSrc= y* (DstBytesPerLine);
			nOffsDst= (pBitmap->Height-y-1)* pBitmap->BytesPerLine;
			for (int x= 0; x< pBitmap->Width; ++x, ++nOffsSrc, ++nOffsDst)
				pDstBits[nOffsDst]= pSrcBits[nOffsSrc];
		}
	}
	else
	{
		for (int y= 0; y< pBitmap->Height; ++y)
		{
			nOffsSrc= y* (DstBytesPerLine);
			nOffsDst= (pBitmap->Height-y-1)* pBitmap->BytesPerLine;
			int nPos= 0;
			for (int x= 0; x< pBitmap->Width; ++x, ++nOffsSrc)
			{
				_asm
				{
					mov		esi, pSrcBits
					add		esi, nOffsSrc
					mov		edi, pDstBits
					add		edi, nOffsDst

					xor		eax, eax
					mov		al, byte ptr [esi]
					
					mov		ecx, 16
					sub		ecx, nRes
					sub		ecx, nPos

					shl		eax, cl
					or		byte ptr [edi], ah
					cmp		ecx, 7
					ja		end
					or		byte ptr [edi+1], al

				end:
				}

// The same code in C++
/*
				WORD w, w2;

				w= pSrcBits[nOffsSrc];
				w<<= (16-nRes-nPos);
				
				w2= w>>8;
				w<<= 8;
				w|= w2;

				*(WORD*)(pDstBits+nOffsDst)|= w;
*/

				nPos+= nRes;
				if (nPos>= 8)
				{
					nPos-= 8;
					++nOffsDst;
				}

			}
		}
	}

	// Converting to passed color resolution ---------------------------------
	if (nBitsPerPixel!= 0 && nBitsPerPixel!= nRes)
	{
		nErr= GraphXnamespace::L_ColorResBitmap (pBitmap, pBitmap, nBitsPerPixel,
			CRF_OPTIMIZEDPALETTE, NULL, NULL, NULL, NULL, NULL);
		if (nErr!= SUCCESS)
			throw nErr;
	}
}
