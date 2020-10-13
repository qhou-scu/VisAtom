
#ifndef _BITMAP_
#define _BITMAP_
#include "GlcObj.h"

class Bitmap:public GlcObj
{
     private:
     protected:
	     BITMAPINFO *info;	
         void       *bits;
		 int         texture;
		 int         texDim;
         GLubyte *ConvertRGB(BITMAPINFO *, void *);
	 public:
		 Bitmap();
		 Bitmap(char*);
		 ~Bitmap();
		 int  LoadFromFile(char *);
         int  ReadFromViewport();
         int  ReadFromViewport(int, int, int, int);
         int  SaveToFile(char *);
         int  SaveToFile(QString fname); 

         int  SaveToClipBoard();
		 void Paintat(float, float, float, int, int);
		 void Paintat(float, float, float);
         int  Print();
         int  Print(PRINTDLG);	
		 void MakeTexture();
		 int  Texture(){ return texture;}
		 void PutonAsTexture();
		 void PutonAsTexture(GlcObj*);
		 void* GetBits(){ return bits;};
		 long StoreSize();
		 BITMAPINFO * GetInfo(){ return info;};

};
#endif
