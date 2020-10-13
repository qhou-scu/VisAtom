#ifndef _STYLE
#define _STYLE
#include "GlcObj.h"

#define Style_default            0
#define Style_SOLID              1
#define Style_SOLID_FRONT_ONLY   2
#define Style_SOLID_BACK_ONLY    3
#define Style_WIRE               4
#define Style_TRANSP_SOLID       5

class Style
{
	private:
		static GLint curStyle;
		GLint oldStyle, thisStyle;

    protected:
    public:
	    Style();
		~Style();
		void Solid( );
		void SolidFrontOnly( );
		void SolidBackOnly( );
		void Wire( );
		void TranspSolid( );
		void SetStyle(GLint);
		void RestoreOldStyle();
};

#endif