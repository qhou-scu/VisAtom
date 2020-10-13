/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#include <math.h>
#include <stdio.h>
#include "VisAtomDataSheet.h"


VisAtomDataSheet::VisAtomDataSheet()
{
	m_nCol = m_nRow = 0;
	m_ColProp = NULL;
    m_pData = NULL;
	m_Boxsize[0] =  1;
	m_Boxsize[1] = -1;
	m_Boxsize[2] =  1;
	m_Boxsize[3] = -1;
	m_Boxsize[4] =  1;
	m_Boxsize[5] = -1;
	return;
}

VisAtomDataSheet::VisAtomDataSheet(int ncol, int nrow)
{
	this->SetColRow(ncol, nrow);
    return;	
}


VisAtomDataSheet::~VisAtomDataSheet()
{
	this->Clear();
    return;
}


void VisAtomDataSheet::Clear()
{
  
    // delete the old data
	if(m_pData) 
	{
		int i;
        for(i=0; i<m_nCol; i++)
		{
			if(m_pData[i]) 
			{
               delete(m_pData[i]);
			   m_pData[i] = NULL;
			}
		}
	}
	if(m_ColProp) delete m_ColProp; m_ColProp = NULL;
	m_nCol=m_nRow=0;
    m_pData=NULL;

	return;
}

//**************************************************************************
void VisAtomDataSheet::SetColRow(int ncol, int nrow)
{
	if(ncol == m_nCol && nrow == m_nRow && m_pData) return
    this->Clear();

	//update the new data 
	m_nCol = ncol;
	m_nRow = nrow;

	int i; 
    if(m_nCol > 0) 
	{
      
	   m_ColProp = new int[m_nCol];
	   m_pData = new float*[m_nCol];
	   for(i=0; i<m_nCol; i++) 
	   {
		   m_ColProp[i] = -1;
		   m_pData[i] = NULL;
	   }
	   if(m_nRow > 0)
	   {
         for(i=0; i<m_nCol; i++) m_pData[i]   = new float[m_nRow];
	   }
	}

	/*if(m_nCol*m_nRow > 0)
	{
       int i;
	   m_ColProp = new int[m_nCol];
	   m_pData = new float*[m_nCol];
	   for(i=0; i<m_nCol; i++)
	   {
		   m_ColProp[i] = -1;
		   m_pData[i]   = new float[m_nRow];
	   }
	}*/
	return;
}








