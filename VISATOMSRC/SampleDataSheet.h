/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#ifndef _SAMPLEDATASHEET_H
#define _SAMPLEDATASHEET_H
#include  "GlcObj.h"

 
class SampleDataSheet
{
	private:
    protected:
       int      m_nCol, m_nRow; 
	   double** m_Data; 
    public:
	    SampleDataSheet();
	    SampleDataSheet(int ncol, int nrow);
	    ~SampleDataSheet();

		//void SetDataForCol_Row(int col, int row, char*str); 
		void SetDataForCol_Row(int col, int row, char*str)
		{     
			m_Data[col][row] = atof(str);
        }
        void SetDataForRow(int row, char**str)
        {
	       for(int i=0; i<m_nCol; i++) SetDataForCol_Row(i, row, str[i]);
		}


};

#endif