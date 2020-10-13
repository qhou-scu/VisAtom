#ifndef _TEXTURE_
#define _TEXTURE_
#include "Bitmap.h"

class Texture:public Bitmap
{
     private:
     protected:
		 //int         texture;
		 //int         texDim;
	 public:
		 Texture();
		 ~Texture(); 
		 Texture(char*);
		 int  List(){ return texture;}
		 int  Dim(){ return texDim;}

};
#endif
