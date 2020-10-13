/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2017
*/

#ifndef _VISATOMEXTENDOBJECT_H
#define _VISATOMEXTENDOBJECT_H
#include  "GlcObj.h"
#include  "QString.h"
//*****************************
class VisAtomExtendObject:public GlcObj
{
	private:

    public:
		VisAtomExtendObject(){};
		~VisAtomExtendObject(){};

		virtual QString RUNTIME_CLASSNAME(){ return QObject::tr("");};
		virtual void Paint(){};
		virtual int LoadData(QString fname) {return 0;};
		virtual int HasData(){ return 0;}
 };
 

#endif