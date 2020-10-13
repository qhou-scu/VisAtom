/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#ifndef _VISATOMDATASHEET_H
#define _VISATOMDATASHEET_H
#include  "GlcObj.h"

class VisAtomTrajectory; 
class VisAtomDataSheet
{
	private:
    protected:
       int      m_nCol, m_nRow; 
	   int*     m_ColProp;
	   float** m_pData;
	   float   m_Boxsize[6];

       friend class VisAtomTrajectory;
    public:
	    VisAtomDataSheet();
	    VisAtomDataSheet(int ncol, int nrow);
	    ~VisAtomDataSheet();
		enum COLPROP
		{
			COLPROP_TYPE = 0,
			COLPROP_POSX = 1,
			COLPROP_POSY = 2,
			COLPROP_POSZ = 3,
		};

		int  GetNumCol(){return m_nCol;}
		int  GetNumRow(){return m_nRow;}
		int* GetColProp(){return m_ColProp;}
		int  GetTypeCol(){for(int i=0; i<m_nCol; i++) if(m_ColProp[i] == COLPROP_TYPE) return i; return -1;}	 
		int  GetPosXCol(){for(int i=0; i<m_nCol; i++) if(m_ColProp[i] == COLPROP_POSX) return i; return -1;}	 
		int  GetPosYCol(){for(int i=0; i<m_nCol; i++) if(m_ColProp[i] == COLPROP_POSY) return i; return -1;}	 
		int  GetPosZCol(){for(int i=0; i<m_nCol; i++) if(m_ColProp[i] == COLPROP_POSZ) return i; return -1;}	 
		void SetTypeCol(int col) 
		{
			int oldcol = GetTypeCol();
			if(oldcol >= 0 )  m_ColProp[oldcol] = -1;
			//if col <0, no colum selected for type
			if(col >=0) m_ColProp[col] = COLPROP_TYPE;
		} 
		void SetPosxCol(int col) 
		{
			int oldcol = GetPosXCol();
			if(oldcol >= 0 )  m_ColProp[oldcol] = -1;
            m_ColProp[col] = COLPROP_POSX;
		} 
		void SetPosyCol(int col)
		{
			int oldcol = GetPosYCol();
			if(oldcol >= 0 )  m_ColProp[oldcol] = -1;
			m_ColProp[col] = COLPROP_POSY;
		} 
		void SetPoszCol(int col)
		{
			int oldcol = GetPosZCol();
			if(oldcol >= 0 )  m_ColProp[oldcol] = -1;
			m_ColProp[col] = COLPROP_POSZ;
		} 

        void Clear();
        void SetColRow(int ncol, int nrow);
		
		void SetDataForCol_Row(int col, int row, char*str) {m_pData[col][row] = atof(str);}
		void SetDataForRow(int row, char**str) {for(int i=0; i<m_nCol; i++) SetDataForCol_Row(i, row, str[i]);}
		float*GetData(int col)
		{ 
		  if(!m_pData) return NULL; 
		  if(col>=m_nCol) return NULL; 
		  if(col < 0) return NULL;
		  return m_pData[col];
		}

		void SetBoxSize(float b0, float b1, float b2, float b3, float b4,float b5)
		{
			m_Boxsize[0] = b0;
			m_Boxsize[1] = b1;
			m_Boxsize[2] = b2;
			m_Boxsize[3] = b3;
			m_Boxsize[4] = b4;
			m_Boxsize[5] = b5;
			return;
		}

		void GetBoxSize(float&b0, float&b1, float&b2, float&b3, float&b4, float&b5)
		{
			b0 = m_Boxsize[0];
			b1 = m_Boxsize[1];
			b2 = m_Boxsize[2];
			b3 = m_Boxsize[3];
			b4 = m_Boxsize[4];
			b5 = m_Boxsize[5];
			return;
		}

		int HasBoxSize()
		{
			if(m_Boxsize[0] >= m_Boxsize[1] ||
			   m_Boxsize[2] >= m_Boxsize[3] ||
			   m_Boxsize[4] >= m_Boxsize[5] ) return 0;
			else return 1;
		}

};

#endif