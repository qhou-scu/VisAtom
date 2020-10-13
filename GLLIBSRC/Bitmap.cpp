#include "bitmap.h"

Bitmap::Bitmap()
{
	info = NULL;	
    bits = NULL;
	texture = 0;
}

Bitmap::Bitmap(char*filename)
{
	info = NULL;	
    bits = NULL;
	texture = 0;
    LoadFromFile(filename);
}


Bitmap::~Bitmap()
{
	if(info != NULL) 
	{   free(info);
		info = NULL;	
	}
	if(bits != NULL)
	{
		free(bits);
        bits = NULL;
	}
	texture = 0;
}


int Bitmap::LoadFromFile(char*filename)
{
  FILE			*fp;		/* Open file pointer */
  long			bitsize,	/* Size of bitmap */
                infosize;	/* Size of header information */
  BITMAPFILEHEADER	header;		/* File header */
  void *oldbits;


  if(info != NULL )
  {
	  free(info);
	  info = NULL;
  }
  if(bits != NULL)
  {
	  free(bits);
	  bits = NULL;
  }
 /*
  * Try opening the file; use "rb" mode to read this *binary* file.
  */
  if ((fp = fopen(filename, "rb")) == NULL)
    return (-1);

 /*
  * Read the file header and any following bitmap information...
  */

  if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1)
  {
   /*
    * Couldn't read the file header - return NULL...
    */

    fclose(fp);
    return (-1);
  };

  if (header.bfType != 'MB')	/* Check for BM reversed... */
  {
   /*
    * Not a bitmap file - return NULL...
    */

    fclose(fp);
    return (-1);
  };

  infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
  if ((info = (BITMAPINFO *)malloc(infosize)) == NULL)
  {
   /*
    * Couldn't allocate memory for bitmap info - return NULL...
    */

    fclose(fp);
    return (-1);
  };

  if (fread(info, 1, infosize, fp) < infosize)
  {
   /*
    * Couldn't read the bitmap header - return NULL...
    */

    free(info);
    fclose(fp);
    return (-1);
  };

 /*
  * Now that we have all the header info read in, allocate memory for the
  * bitmap and read *it* in...
  */
  if ((bitsize = info->bmiHeader.biSizeImage) == 0)
    bitsize = (info->bmiHeader.biWidth *
               info->bmiHeader.biBitCount + 7) / 8 *
  	           abs(info->bmiHeader.biHeight);

  if ((oldbits = malloc(bitsize)) == NULL)
  {
   /*
    * Couldn't allocate memory - return NULL!
    */

    free(info);
    fclose(fp);
    return (-1);
  };

  if (fread(oldbits, 1, bitsize, fp) < bitsize)
  {
   /*
    * Couldn't read bitmap - free memory and return NULL!
    */

    free(info);
    free(oldbits);
    fclose(fp);
    return (-1);
  };

 /*
  * OK, everything went fine - return the allocated bitmap...
  */

  fclose(fp);
  bits = ConvertRGB(info, oldbits);
  free(oldbits);
  oldbits = NULL;
  return 0;
}

int  Bitmap::SaveToFile(QString fname)
{

  FILE			*fp;		/* Open file pointer */
  long			size,		/* Size of file */
			infosize,	    /* Size of bitmap info */
			bitsize;	    /* Size of bitmap pixels */
  BITMAPFILEHEADER	header;	/* File header */



 if(info == NULL || bits == NULL) return -1;
 /*
  * Try opening the file; use "wb" mode to write this *binary* file.
  */
  QFile file(fname);
  file.open(QIODevice::WriteOnly);

  if (info->bmiHeader.biSizeImage == 0)	/* Figure out the bitmap size */
      bitsize = (info->bmiHeader.biWidth *
                info->bmiHeader.biBitCount + 7) / 8 *
	            abs(info->bmiHeader.biHeight);
  else
    bitsize = info->bmiHeader.biSizeImage;

  infosize = sizeof(BITMAPINFOHEADER);
  switch (info->bmiHeader.biCompression)
  {
    case BI_BITFIELDS :
        infosize += 12;			/* Add 3 RGB doubleword masks */
        if (info->bmiHeader.biClrUsed == 0)
	  break;
    case BI_RGB :
        if (info->bmiHeader.biBitCount > 8 &&
            info->bmiHeader.biClrUsed == 0)
	  break;
    case BI_RLE8 :
    case BI_RLE4 :
        if (info->bmiHeader.biClrUsed == 0)
          infosize += (1 << info->bmiHeader.biBitCount) * 4;
	else
          infosize += info->bmiHeader.biClrUsed * 4;
	break;
  };

  size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;

 /*
  * Write the file header, bitmap information, and bitmap pixel data...
  */

  header.bfType      = 'MB';		/* Non-portable... sigh */
  header.bfSize      = size;
  header.bfReserved1 = 0;
  header.bfReserved2 = 0;
  header.bfOffBits   = sizeof(BITMAPFILEHEADER) + infosize;

  if (fwrite(&header, 1, sizeof(BITMAPFILEHEADER), fp) < sizeof(BITMAPFILEHEADER))
  {
   /*
    * Couldn't write the file header - return...
    */

    fclose(fp);
    return (-1);
  };

  if (fwrite(info, 1, infosize, fp) < infosize)
  {
   /*
    * Couldn't write the bitmap header - return...
    */

    fclose(fp);
	return (-1);

  }
  return 0;
}


int Bitmap::SaveToFile(char*filename)
{
  FILE			*fp;		/* Open file pointer */
  long			size,		/* Size of file */
			infosize,	    /* Size of bitmap info */
			bitsize;	    /* Size of bitmap pixels */
  BITMAPFILEHEADER	header;	/* File header */



 if(info == NULL || bits == NULL) return -1;
 /*
  * Try opening the file; use "wb" mode to write this *binary* file.
  */
  if ((fp = fopen(filename, "wb")) == NULL)   return (-1);

  if (info->bmiHeader.biSizeImage == 0)	/* Figure out the bitmap size */
      bitsize = (info->bmiHeader.biWidth *
                info->bmiHeader.biBitCount + 7) / 8 *
	            abs(info->bmiHeader.biHeight);
  else
    bitsize = info->bmiHeader.biSizeImage;

  infosize = sizeof(BITMAPINFOHEADER);
  switch (info->bmiHeader.biCompression)
  {
    case BI_BITFIELDS :
        infosize += 12;			/* Add 3 RGB doubleword masks */
        if (info->bmiHeader.biClrUsed == 0)
	  break;
    case BI_RGB :
        if (info->bmiHeader.biBitCount > 8 &&
            info->bmiHeader.biClrUsed == 0)
	  break;
    case BI_RLE8 :
    case BI_RLE4 :
        if (info->bmiHeader.biClrUsed == 0)
          infosize += (1 << info->bmiHeader.biBitCount) * 4;
	else
          infosize += info->bmiHeader.biClrUsed * 4;
	break;
  };

  size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;

 /*
  * Write the file header, bitmap information, and bitmap pixel data...
  */

  header.bfType      = 'MB';		/* Non-portable... sigh */
  header.bfSize      = size;
  header.bfReserved1 = 0;
  header.bfReserved2 = 0;
  header.bfOffBits   = sizeof(BITMAPFILEHEADER) + infosize;

  if (fwrite(&header, 1, sizeof(BITMAPFILEHEADER), fp) < sizeof(BITMAPFILEHEADER))
  {
   /*
    * Couldn't write the file header - return...
    */

    fclose(fp);
    return (-1);
  };

  if (fwrite(info, 1, infosize, fp) < infosize)
  {
   /*
    * Couldn't write the bitmap header - return...
    */

    fclose(fp);
    return (-1);
  };

  if (fwrite(bits, 1, bitsize, fp) < bitsize)
  {
   /*
    * Couldn't write the bitmap - return...
    */

    fclose(fp);
    return (-1);
  };

 /*
  * OK, everything went fine - return...
  */

  fclose(fp);
  return (0);
}


long Bitmap::StoreSize()
{
  long			size,		/* Size of file */
			infosize,	    /* Size of bitmap info */
			bitsize;	    /* Size of bitmap pixels */

 if(info == NULL || bits == NULL) return -1;

  if (info->bmiHeader.biSizeImage == 0)	/* Figure out the bitmap size */
      bitsize = (info->bmiHeader.biWidth *
                info->bmiHeader.biBitCount + 7) / 8 *
	            abs(info->bmiHeader.biHeight);
  else
    bitsize = info->bmiHeader.biSizeImage;

  infosize = sizeof(BITMAPINFOHEADER);
  switch (info->bmiHeader.biCompression)
  {
    case BI_BITFIELDS :
        infosize += 12;			/* Add 3 RGB doubleword masks */
        if (info->bmiHeader.biClrUsed == 0)
	  break;
    case BI_RGB :
        if (info->bmiHeader.biBitCount > 8 &&
            info->bmiHeader.biClrUsed == 0)
	  break;
    case BI_RLE8 :
    case BI_RLE4 :
        if (info->bmiHeader.biClrUsed == 0)
          infosize += (1 << info->bmiHeader.biBitCount) * 4;
	else
          infosize += info->bmiHeader.biClrUsed * 4;
	break;
  };

  size = sizeof(BITMAPFILEHEADER) + infosize + bitsize;
  return size;
}


int Bitmap::SaveToClipBoard()
{
  long			size,		/* Size of file */
			infosize,	    /* Size of bitmap info */
			bitsize;	    /* Size of bitmap pixels */
  long i;
  BYTE *p;


 if(info == NULL || bits == NULL) return -1;
  //*** to determine the size of clipboard
  if (info->bmiHeader.biSizeImage == 0)	/* Figure out the bitmap size */
      bitsize = (info->bmiHeader.biWidth *
                info->bmiHeader.biBitCount + 7) / 8 *
	            abs(info->bmiHeader.biHeight);
  else
      bitsize = info->bmiHeader.biSizeImage;

  infosize = sizeof(BITMAPINFOHEADER);
  switch (info->bmiHeader.biCompression)
  {
    case BI_BITFIELDS :
        infosize += 12;			/* Add 3 RGB doubleword masks */
        if (info->bmiHeader.biClrUsed == 0)
	  break;
    case BI_RGB :
        if (info->bmiHeader.biBitCount > 8 &&
            info->bmiHeader.biClrUsed == 0)
	  break;
    case BI_RLE8 :
    case BI_RLE4 :
        if (info->bmiHeader.biClrUsed == 0)
          infosize += (1 << info->bmiHeader.biBitCount) * 4;
	else
          infosize += info->bmiHeader.biClrUsed * 4;
	break;
  };

  size = infosize + bitsize;
  // allocate memory
  HGLOBAL hClipBoard = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,size);
  LPVOID  bClipBoard = GlobalLock(hClipBoard);

   OpenClipboard(NULL);
   EmptyClipboard();
 
   p = (BYTE*)bClipBoard;
   i = infosize;
   memcpy(p, info, infosize);
   p = p + i;
   memcpy(p, (BYTE*)bits, bitsize);

   HANDLE pp = SetClipboardData(CF_DIB,hClipBoard); 
   CloseClipboard();

  return (0);
}


/*
 * 'ReadDIBitmap()' - Read the current OpenGL viewport into a
 *                    24-bit RGB bitmap.
 *
 * Returns the bitmap pixels if successful and NULL otherwise.
 */

int Bitmap::ReadFromViewport( )
{
  long		i, j,			/* Looping var */
  		    bitsize,		/* Total size of bitmap */
		    width;		    /* Aligned width of a scanline */
  GLint		viewport[4];	/* Current viewport */
  GLubyte	*rgb,			/* RGB looping var */
		temp;			/* Temporary var for swapping */

  if(info != NULL )
  {
	  free(info);
	  info = NULL;
  }
  if(bits != NULL)
  {
	  free(bits);
	  bits = NULL;
  }
 /*
  * Grab the current viewport...
  */
  glGetIntegerv(GL_VIEWPORT, viewport);

 /*
  * Allocate memory for the header and bitmap...
  */

  if ((info = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER))) == NULL)
  {
   /*
    * Couldn't allocate memory for bitmap info - return NULL...
    */

    return (-1);
  };

  width   = viewport[2] * 3;		/* Real width of scanline */
  width   = (width + 3) & ~3;		/* Aligned to 4 bytes */
  bitsize = width * viewport[3];	/* Size of bitmap, aligned */

  if ((bits = calloc(bitsize, 1)) == NULL)
  {
   /*
    * Couldn't allocate memory for bitmap pixels - return NULL...
    */

    free(info);
    return (NULL);
  };

 /*
  * Read pixels from the framebuffer...
  */

  glFinish();				/* Finish all OpenGL commands */
  glPixelStorei(GL_PACK_ALIGNMENT, 4);	/* Force 4-byte alignment */
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

  glReadPixels(0, 0, viewport[2], viewport[3], GL_RGB, GL_UNSIGNED_BYTE,
               bits);

 /*
  * Swap red and blue for the bitmap...
  */

  for (i = 0; i < viewport[3]; i ++)
    for (j = 0, rgb = ((GLubyte *)bits) + i * width;
         j < viewport[2];
	 j ++, rgb += 3)
    {
      temp   = rgb[0];
      rgb[0] = rgb[2];
      rgb[2] = temp;
    };

 /*
  * Finally, initialize the bitmap header information...
  */

  info->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
  info->bmiHeader.biWidth         = viewport[2];
  info->bmiHeader.biHeight        = viewport[3];
  info->bmiHeader.biPlanes        = 1;
  info->bmiHeader.biBitCount      = 24;
  info->bmiHeader.biCompression   = BI_RGB;
  info->bmiHeader.biSizeImage     = bitsize;
  info->bmiHeader.biXPelsPerMeter = 2952; /* 75 DPI */
  info->bmiHeader.biYPelsPerMeter = 2952; /* 75 DPI */
  info->bmiHeader.biClrUsed       = 0;
  info->bmiHeader.biClrImportant  = 0;

  return 0;
}

int Bitmap::ReadFromViewport(int xPos, int yPos, int w, int h)
{
  long		i, j,			/* Looping var */
  		    bitsize,		/* Total size of bitmap */
		    width;		    /* Aligned width of a scanline */
  GLubyte	*rgb,			/* RGB looping var */
		temp;			/* Temporary var for swapping */

  if(info != NULL )
  {
	  free(info);
	  info = NULL;
  }
  if(bits != NULL)
  {
	  free(bits);
	  bits = NULL;
  }
 /*
  * Allocate memory for the header and bitmap...
  */

  if ((info = (BITMAPINFO *)malloc(sizeof(BITMAPINFOHEADER))) == NULL)
  {
   /*
    * Couldn't allocate memory for bitmap info - return NULL...
    */

    return (-1);
  };

  width   = w * 3;		            /* Real width of scanline */
  width   = (width + 3) & ~3;		/* Aligned to 4 bytes */
  bitsize = width * h;	             /* Size of bitmap, aligned */

  if ((bits = calloc(bitsize, 1)) == NULL)
  {
   /*
    * Couldn't allocate memory for bitmap pixels - return NULL...
    */

    free(info);
    return (NULL);
  };

 /*
  * Read pixels from the framebuffer...
  */

  glFinish();				/* Finish all OpenGL commands */
  glPixelStorei(GL_PACK_ALIGNMENT, 4);	/* Force 4-byte alignment */
  glPixelStorei(GL_PACK_ROW_LENGTH, 0);
  glPixelStorei(GL_PACK_SKIP_ROWS, 0);
  glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

  glReadPixels(xPos, yPos, w, h, GL_RGB, GL_UNSIGNED_BYTE, bits);

 /*
  * Swap red and blue for the bitmap...
  */

  for (i = 0; i < h; i ++)
    for (j = 0, rgb = ((GLubyte *)bits) + i * width;
         j < w;
	 j ++, rgb += 3)
    {
      temp   = rgb[0];
      rgb[0] = rgb[2];
      rgb[2] = temp;
    };

 /*
  * Finally, initialize the bitmap header information...
  */

  info->bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
  info->bmiHeader.biWidth         = w;
  info->bmiHeader.biHeight        = h;
  info->bmiHeader.biPlanes        = 1;
  info->bmiHeader.biBitCount      = 24;
  info->bmiHeader.biCompression   = BI_RGB;
  info->bmiHeader.biSizeImage     = bitsize;
  info->bmiHeader.biXPelsPerMeter = 2952; /* 75 DPI */
  info->bmiHeader.biYPelsPerMeter = 2952; /* 75 DPI */
  info->bmiHeader.biClrUsed       = 0;
  info->bmiHeader.biClrImportant  = 0;

  return 0;
}



/*
 * 'PrintDIBitmap()' - Print a bitmap to a GDI printer.
 */

 int Bitmap::Print()	
{
  PRINTDLG	pd;			/* Print dialog information */
  long	xsize,			/* Size of printed image */
		ysize,
		xoffset,		/* Offset from edges for image */
		yoffset;
  RECT		rect;			/* Page rectangle */
  DOCINFO	di;			    /* Document info */
  HDC		hdc;			/* Device context for bitmap */
  HBITMAP	bitmap;			/* Bitmap image */
  HBRUSH	brush;			/* Background brush for page */
  HCURSOR	busy,			/* Busy cursor */
		oldcursor;		    /* Old cursor */


 /*
  * Range check...
  */
  if (info == NULL || bits == NULL)
    return (0);

 /*
  * Initialize a PRINTDLG structure before displaying a standard Windows
  * print dialog...
  */

  memset(&pd, 0, sizeof(pd));
  pd.lStructSize = sizeof(pd);
  pd.hwndOwner   = NULL;
  pd.Flags       = PD_RETURNDC|PD_PRINTTOFILE|PD_PAGENUMS;
  pd.hInstance   = NULL;
  //pd.nFromPage   = 1;
  //pd.nToPage     = 1;

  if (!PrintDlg(&pd))
    return (0);		/* User chose 'cancel'... */
 /*
  * OK, user wants to print, so set the cursor to 'busy' and start the
  * print job...
  */
  busy      = LoadCursor(NULL, IDC_WAIT);
  oldcursor = SetCursor(busy);

  SetMapMode(pd.hDC, MM_TEXT);
  di.cbSize      = sizeof(DOCINFO);
  //di.lpszDocName = "OpenGL Image";
  di.lpszOutput  = NULL;

  StartDoc(pd.hDC, &di);
  StartPage(pd.hDC);
 /*
  * Clear the background to white...
  */
  rect.top    = 0;
  rect.left   = 0;
  rect.right  = GetDeviceCaps(pd.hDC, HORZRES);
  rect.bottom = GetDeviceCaps(pd.hDC, VERTRES);
  brush       = CreateSolidBrush(0x00ffffff);
  FillRect(pd.hDC, &rect, brush);
 /*
  * Stretch the bitmap to fit the page...
  */

  hdc    = CreateCompatibleDC(pd.hDC);
  bitmap = CreateDIBitmap(hdc, &(info->bmiHeader), CBM_INIT, bits, info,
                          DIB_RGB_COLORS);
  SelectObject(hdc, bitmap);

  xsize = rect.right;
  ysize = xsize * info->bmiHeader.biHeight / info->bmiHeader.biWidth;
  if (ysize > rect.bottom)
  {
    ysize = rect.bottom;
    xsize = ysize * info->bmiHeader.biWidth / info->bmiHeader.biHeight;
  };

  xoffset = (rect.right - xsize) / 2;
  yoffset = (rect.bottom - ysize) / 2;
  /*StretchBlt(pd.hDC, xoffset, yoffset, xsize, ysize,
             hdc, 0, 0, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
             SRCCOPY); 
			 */
  StretchBlt(pd.hDC, xoffset, yoffset, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
             hdc, 0, 0, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
             SRCCOPY); 

 /*
  * That's it.  End the print job and free anything we allocated...
  */

  EndPage(pd.hDC);
  EndDoc(pd.hDC);
  DeleteDC(pd.hDC);

  DeleteObject(bitmap);
  DeleteObject(brush);
  DeleteObject(busy);
  DeleteDC(hdc);

 /*
  * Restore the cursor and return...
  */

  SetCursor(oldcursor);

  return (1);
}

 int Bitmap::Print(PRINTDLG pd)	
{
  long	xsize,			/* Size of printed image */
		ysize,
		xoffset,		/* Offset from edges for image */
		yoffset;
  RECT		rect;			/* Page rectangle */
  DOCINFO	di;			    /* Document info */
  HDC		hdc;			/* Device context for bitmap */
  HBITMAP	bitmap;			/* Bitmap image */
  HBRUSH	brush;			/* Background brush for page */

 /*
  * Range check...
  */
  if (info == NULL || bits == NULL)
    return (0);


  SetMapMode(pd.hDC, MM_TEXT);
  di.cbSize      = sizeof(DOCINFO);
  di.lpszDocName = (LPCWSTR)"OpenGL Image";
  di.lpszOutput  = NULL;

  StartDoc(pd.hDC, &di);
  StartPage(pd.hDC);
 /*
  * Clear the background to white...
  */
  rect.top    = 0;
  rect.left   = 0;
  rect.right  = GetDeviceCaps(pd.hDC, HORZRES);
  rect.bottom = GetDeviceCaps(pd.hDC, VERTRES);
  brush       = CreateSolidBrush(0x00ffffff);
  FillRect(pd.hDC, &rect, brush);
 /*
  * Stretch the bitmap to fit the page...
  */

  hdc    = CreateCompatibleDC(pd.hDC);
  bitmap = CreateDIBitmap(hdc, &(info->bmiHeader), CBM_INIT, bits, info,
                          DIB_RGB_COLORS);
  SelectObject(hdc, bitmap);

  xsize = rect.right;
  ysize = xsize * info->bmiHeader.biHeight / info->bmiHeader.biWidth;
  if (ysize > rect.bottom)
  {
    ysize = rect.bottom;
    xsize = ysize * info->bmiHeader.biWidth / info->bmiHeader.biHeight;
  };

  xoffset = (rect.right - xsize) / 2;
  yoffset = (rect.bottom - ysize) / 2;
  StretchBlt(pd.hDC, xoffset, yoffset, xsize, ysize,
             hdc, 0, 0, info->bmiHeader.biWidth, info->bmiHeader.biHeight,
             SRCCOPY);

 /*
  * That's it.  End the print job and free anything we allocated...
  */

  EndPage(pd.hDC);
  EndDoc(pd.hDC);
  DeleteDC(pd.hDC);

  DeleteObject(bitmap);
  DeleteObject(brush);
  DeleteDC(hdc);
  return (1);
}



void Bitmap::Paintat(float xoffset, float yoffset, float zoffset, int w, int h)	/* I - Client area rectangle */
{
  GLfloat	xscale,		/* Scaling in X direction */
		    yscale;		/* Scaling in Y direction */

    xscale  = (float)w / (float)info->bmiHeader.biWidth;
    yscale  = (float)h / (float)info->bmiHeader.biHeight;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelZoom(xscale, yscale);
    glRasterPos3f(xoffset, yoffset, zoffset);
    glDrawPixels(info->bmiHeader.biWidth, info->bmiHeader.biHeight,
		 GL_RGB, GL_UNSIGNED_BYTE, bits);

//  glFinish();
}


void Bitmap::Paintat(float xoffset, float yoffset, float zoffset)	
{
  int w, h;

    w = info->bmiHeader.biWidth;
    h = info->bmiHeader.biHeight;

    Paintat(xoffset, yoffset, zoffset, w, h); 
}

GLubyte* Bitmap::ConvertRGB(BITMAPINFO *info, void *bits)
{
  int		i, j,			/* Looping vars */
  		bitsize,		/* Total size of bitmap */
		width;			/* Aligned width of bitmap */
  GLubyte	*newbits;		/* New RGB bits */
  GLubyte	*from, *to;		/* RGB looping vars */


 /*
  * Allocate memory for the RGB bitmap...
  */

  width   = 3 * info->bmiHeader.biWidth;
  width   = (width + 3) & ~3;	
  bitsize = width * info->bmiHeader.biHeight;
  if ((newbits = (GLubyte *)calloc(bitsize, 1)) == NULL)
    return (NULL);

 /*
  * Copy the original bitmap to the new array, converting as necessary.
  */

  switch (info->bmiHeader.biCompression)
  {
    case BI_RGB :
        if (info->bmiHeader.biBitCount == 24)
	{
         /*
          * Swap red & blue in a 24-bit image...
          */

          for (i = 0; i < info->bmiHeader.biHeight; i ++)
	    for (j = 0, from = ((GLubyte *)bits) + i * width,
	             to = newbits + i * width;
		 j < info->bmiHeader.biWidth;
		 j ++, from += 3, to += 3)
            {
              to[0] = from[2];
              to[1] = from[1];
              to[2] = from[0];
            };
	};
	break;
    case BI_RLE4 :
    case BI_RLE8 :
    case BI_BITFIELDS :
        break;
  };

  return (newbits);
}

void Bitmap::MakeTexture()
{
  if(texture >=1 ) return;
  glNewList(texture = glGenLists(1), GL_COMPILE);
    if (info->bmiHeader.biHeight == 1||info->bmiHeader.biWidth == 1)
	{
       glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
       glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	   texDim = GL_TEXTURE_1D;
	}
	else
	{
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	   texDim = GL_TEXTURE_2D;
	}

    if (info->bmiHeader.biHeight == 1)
    {
       glTexImage1D(GL_TEXTURE_1D, 0, 3, info->bmiHeader.biWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);
       glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else if (info->bmiHeader.biWidth == 1)
       {
         glTexImage1D(GL_TEXTURE_1D, 0, 3, info->bmiHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bits);
         glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
       }
       else
      {
        glTexImage2D(GL_TEXTURE_2D, 0, 3, info->bmiHeader.biWidth, info->bmiHeader.biHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, bits);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      };
  glEndList();

  return;
}









