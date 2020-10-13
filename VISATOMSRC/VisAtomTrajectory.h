/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#ifndef _VISATOMTRAJECTORY_H
#define _VISATOMTRAJECTORY_H
#include  "GlcObj.h"

class VisAtomDataSheet; 
class VisAtomSample;
class VisAtomTrajectory:public GlcObj
{
	private:
    protected:
	   QList<VisAtomDataSheet*> m_SampleDataSheets;
       VisAtomSample*m_curSample;
	   float*m_aDx0;
	   float*m_aDy0;
	   float*m_aDz0;
       friend class VisAtomSample;

	   void DrawTrjectoryLine();

    public:
	    VisAtomTrajectory();
	    ~VisAtomTrajectory();

		void AttachSample(VisAtomSample*pSample);

        void Clear();
        void AddData(VisAtomDataSheet*pData);
		int NumberOfDatasheet(){return m_SampleDataSheets.size();} ;
		void SetDataCol(int colx, int coly, int colz);
        VisAtomDataSheet*GetDataSheet(int i);

		int GetDisplacement(int iatom, float&DX, float&DY, float&DZ);
		int GetPathlength(int iatom, float&pathlen);

		void Paint();

};

#endif