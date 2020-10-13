/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

//#include <Glc.h>
#include <math.h>
#include <stdio.h>
#include "SampleDataSheet.h"


SampleDataSheet::SampleDataSheet()
{
	m_nCol = m_nRow = 0;
    m_Data = NULL;
	return;
}

SampleDataSheet::SampleDataSheet(int ncol, int nrow)
{
	m_nCol = ncol;
	m_nRow = nrow;
	if(m_nCol*m_nRow > 0)
	{
       int i;
	   m_Data = new double*[m_nCol];
	   for(i=0; i<m_nCol; i++) m_Data[i] = new double[m_nRow];
	}
	return;
}


SampleDataSheet::~SampleDataSheet()
{
	if(m_Data) 
	{
		int i;
        for(i=0; i<m_nCol; i++)
		{
			if(m_Data[i]) 
			{
               delete(m_Data[i]);
			   m_Data[i] = NULL;
			}
		}
	}
	m_nCol=m_nRow=0;
    m_Data=NULL;
    return;
}






