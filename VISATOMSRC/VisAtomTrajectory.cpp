/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#include <math.h>
#include <stdio.h>
#include "VisAtomTrajectory.h"
#include "VisAtomSample.h"


VisAtomTrajectory::VisAtomTrajectory()
{
	m_curSample = NULL;
    m_aDx0 = NULL;
    m_aDy0 = NULL;
    m_aDz0 = NULL;
	return;
}


//**************************************************************************
VisAtomTrajectory::~VisAtomTrajectory()
{
	Clear();
    return;
}


//**************************************************************************
void VisAtomTrajectory::Clear()
{

	int i;
	for(i=0; i<this->m_SampleDataSheets.size(); i++)
	{
		if(this->m_SampleDataSheets[i]) delete this->m_SampleDataSheets[i]; 
	}
	this->m_SampleDataSheets.clear();

	if(this->m_aDx0) delete this->m_aDx0; this->m_aDx0=NULL;
	if(this->m_aDy0) delete this->m_aDy0; this->m_aDy0=NULL;
	if(this->m_aDz0) delete this->m_aDz0; this->m_aDz0=NULL;
	return;
}

//**************************************************************************
void VisAtomTrajectory::AddData(VisAtomDataSheet*pData)
{
	 if(this->m_SampleDataSheets.isEmpty())
	 {
		 //save the postion data of the the point
		 this->m_aDx0 = new float[this->m_curSample->m_numAtom];
		 this->m_aDy0 = new float[this->m_curSample->m_numAtom];
		 this->m_aDz0 = new float[this->m_curSample->m_numAtom];
		 memcpy(this->m_aDx0, pData->GetData(pData->GetPosXCol()), sizeof(float)*this->m_curSample->m_numAtom);
		 memcpy(this->m_aDy0, pData->GetData(pData->GetPosYCol()), sizeof(float)*this->m_curSample->m_numAtom);
		 memcpy(this->m_aDz0, pData->GetData(pData->GetPosZCol()), sizeof(float)*this->m_curSample->m_numAtom);
	 }
	 this->m_SampleDataSheets.append(pData);

     float*x0 = this->m_SampleDataSheets[0]->GetData(this->m_curSample->m_pData->GetPosXCol());
     float*y0 = this->m_SampleDataSheets[0]->GetData(this->m_curSample->m_pData->GetPosYCol());
     float*z0 = this->m_SampleDataSheets[0]->GetData(this->m_curSample->m_pData->GetPosZCol());

	 float*dx = pData->GetData(pData->GetPosXCol());
	 float*dy = pData->GetData(pData->GetPosYCol());
	 float*dz = pData->GetData(pData->GetPosZCol());

	 for(int i=0; i<this->m_curSample->m_numAtom; i++)
	 {
		 dx[i] = x0[i] + (dx[i] - this->m_aDx0[i]);
		 dy[i] = y0[i] + (dy[i] - this->m_aDy0[i]);
		 dz[i] = z0[i] + (dz[i] - this->m_aDz0[i]);
	 }

	 return;
}

//**************************************************************************
void VisAtomTrajectory::AttachSample(VisAtomSample*pSample)
{
    this->m_curSample = pSample;
	return;
}

//**************************************************************************
VisAtomDataSheet*VisAtomTrajectory::GetDataSheet(int i)
{
	if(i>= this->m_SampleDataSheets.size()) return NULL;
	else return  this->m_SampleDataSheets[i];
}

//**************************************************************************
void VisAtomTrajectory::SetDataCol(int colx, int coly, int colz)
{
     if(this->m_SampleDataSheets.isEmpty()) return;

	  
     int i,j;
     float*x0 = this->m_SampleDataSheets[0]->GetData(this->m_curSample->m_pData->GetPosXCol());
     float*y0 = this->m_SampleDataSheets[0]->GetData(this->m_curSample->m_pData->GetPosYCol());
     float*z0 = this->m_SampleDataSheets[0]->GetData(this->m_curSample->m_pData->GetPosZCol());

     //restore the old data stte  
	 for(j=0; j<this->m_SampleDataSheets.size(); j++)
	 {
	    float*dx = this->m_SampleDataSheets[j]->GetData(this->m_SampleDataSheets[j]->GetPosXCol());
	    float*dy = this->m_SampleDataSheets[j]->GetData(this->m_SampleDataSheets[j]->GetPosYCol());
	    float*dz = this->m_SampleDataSheets[j]->GetData(this->m_SampleDataSheets[j]->GetPosZCol());
	    for(i=0; i<this->m_curSample->m_numAtom; i++)
	    {
		 dx[i] = dx[i] - x0[i] + this->m_aDx0[i];
		 dy[i] = dy[i] - y0[i] + this->m_aDy0[i];
		 dz[i] = dz[i] - z0[i] + this->m_aDz0[i];
	    }
	 }

	 //reset the new data
	 this->m_SampleDataSheets[0]->SetPosxCol(colx);
	 this->m_SampleDataSheets[0]->SetPosyCol(coly);
	 this->m_SampleDataSheets[0]->SetPoszCol(colz);
	 memcpy(this->m_aDx0, this->m_SampleDataSheets[0]->GetData(colx), sizeof(float)*this->m_curSample->m_numAtom);
	 memcpy(this->m_aDy0, this->m_SampleDataSheets[0]->GetData(coly), sizeof(float)*this->m_curSample->m_numAtom);
	 memcpy(this->m_aDz0, this->m_SampleDataSheets[0]->GetData(colz), sizeof(float)*this->m_curSample->m_numAtom);

	 for(j=0; j<this->m_SampleDataSheets.size(); j++)
	 {
	    //resuore the old data stte  
		this->m_SampleDataSheets[j]->SetPosxCol(colx);
		this->m_SampleDataSheets[j]->SetPosyCol(coly);
		this->m_SampleDataSheets[j]->SetPoszCol(colz);
	    float*dx = this->m_SampleDataSheets[j]->GetData(colx);
	    float*dy = this->m_SampleDataSheets[j]->GetData(coly);
	    float*dz = this->m_SampleDataSheets[j]->GetData(colz);
	    for(i=0; i<this->m_curSample->m_numAtom; i++)
	    {
		 dx[i] = x0[i] + (dx[i] - this->m_aDx0[i]);
		 dy[i] = y0[i] + (dy[i] - this->m_aDy0[i]);
		 dz[i] = z0[i] + (dz[i] - this->m_aDz0[i]);
	    }
	 }

     return;
}

//**************************************************************************
int VisAtomTrajectory::GetDisplacement(int iatom, float&DX, float&DY, float&DZ)
{
	if(this->m_SampleDataSheets.size() == 0) return 0;

     float*x0 = this->m_SampleDataSheets[0]->GetData(this->m_SampleDataSheets[0]->GetPosXCol());
     float*y0 = this->m_SampleDataSheets[0]->GetData(this->m_SampleDataSheets[0]->GetPosYCol());
     float*z0 = this->m_SampleDataSheets[0]->GetData(this->m_SampleDataSheets[0]->GetPosZCol());

	 int n = this->m_SampleDataSheets.size()-1;
     float*x1 = this->m_SampleDataSheets[n]->GetData(this->m_SampleDataSheets[n]->GetPosXCol());
     float*y1 = this->m_SampleDataSheets[n]->GetData(this->m_SampleDataSheets[n]->GetPosYCol());
     float*z1 = this->m_SampleDataSheets[n]->GetData(this->m_SampleDataSheets[n]->GetPosZCol());

	 
	 DX = x1[iatom] - x0[iatom];
	 DY = y1[iatom] - y0[iatom];
	 DZ = z1[iatom] - z0[iatom];
	 return 1;

}
//**************************************************************************
int VisAtomTrajectory::GetPathlength(int iatom, float&pathlen)
{
	if(this->m_SampleDataSheets.size() == 0) return 0;

     float*x0 = this->m_SampleDataSheets[0]->GetData(this->m_SampleDataSheets[0]->GetPosXCol());
     float*y0 = this->m_SampleDataSheets[0]->GetData(this->m_SampleDataSheets[0]->GetPosYCol());
     float*z0 = this->m_SampleDataSheets[0]->GetData(this->m_SampleDataSheets[0]->GetPosZCol());

	 float*x1, *y1, *z1;
	 double dd=0;
	 for(int j=1; j<this->m_SampleDataSheets.size(); j++)
	 {
	    x1 = this->m_SampleDataSheets[j]->GetData(this->m_SampleDataSheets[j]->GetPosXCol());
	    y1 = this->m_SampleDataSheets[j]->GetData(this->m_SampleDataSheets[j]->GetPosYCol());
	    z1 = this->m_SampleDataSheets[j]->GetData(this->m_SampleDataSheets[j]->GetPosZCol());
		dd = pow(x1[iatom] - x0[iatom],2) + pow(y1[iatom] - y0[iatom],2) + pow(z1[iatom] - z0[iatom],2);
		pathlen = pathlen + pow(dd,0.5);

		x0 = x1;
		y0 = y1;
		z0 = z1;
	 }
	 return 1;
}



//**************************************************************************
void VisAtomTrajectory::Paint()
{
	if(this->m_SampleDataSheets.isEmpty()) return;
	if(!this->m_curSample) return;
	 glMatrixMode(GL_MODELVIEW);
     glEnable(GL_DEPTH_TEST);
	 this->DrawTrjectoryLine();
}

//**************************************************************************
void VisAtomTrajectory::DrawTrjectoryLine()
{
	 
	 glEnable(GL_COLOR_MATERIAL);
   
	  int j, i;
	  glDisable(GL_LIGHTING);
      

      VisAtomDataSheet*p0 = this->m_SampleDataSheets[0];
      VisAtomDataSheet*p1 = NULL;
	  float*px0, *py0, *pz0;
	  float*px1, *py1, *pz1;
	  float*s0 = NULL;
	  float*s1 = NULL;
	  int scalcol = VisAtomVisualizingPolicy::GetScalarCol();
	  if(scalcol >= 0) s0 = p0->GetData(scalcol);
		  
	  px0 = p0->GetData(p0->GetPosXCol());
	  py0 = p0->GetData(p0->GetPosYCol());
	  pz0 = p0->GetData(p0->GetPosZCol());

	  int*aGeomStyle = this->m_curSample->m_aGeomStyle;
	  char*aVisStat  =  this->m_curSample->m_aVisStat;
	  float*Red      =  this->m_curSample->m_aRed;  
	  float*Green    =  this->m_curSample->m_aGreen;  
	  float*Blue     =  this->m_curSample->m_aBlue;  
	  char*aType     =  this->m_curSample->m_aType; 

	  int numsub     = this->m_curSample->m_subSamples.size();
      float*thick     = new float[numsub];
	  int *colorstyle = new int[numsub];
	  for(i=0; i<numsub;  i++)
	  {
		  thick[i] = this->m_curSample->m_subSamples[i]->GetCurTrajectoryLinethick();
		  colorstyle[i] = this->m_curSample->m_subSamples[i]->GetCurColorStyle();
	  }

	  int numatoms   =  this->m_curSample->m_numAtom;

	  //draw in single color 
	  for(j=1; j<this->m_SampleDataSheets.size(); j++)
	  {
		  p1 = this->m_SampleDataSheets[j];
	      px1 = p1->GetData(p1->GetPosXCol());
	      py1 = p1->GetData(p1->GetPosYCol());
	      pz1 = p1->GetData(p1->GetPosZCol());
          if(scalcol >= 0) s1 = p1->GetData(scalcol);

		  for(i=0; i<numatoms; i++)
		  {
             if(aVisStat[i]&atom_Statu_Visiable &&
			   (aGeomStyle[i]&VisAtomVisualizingPolicy::TRAJECTORY_STYLE_HASLINE)==VisAtomVisualizingPolicy::TRAJECTORY_STYLE_HASLINE)
		       {
				 glLineWidth(thick[aType[i]-1]); 
 	             glPushName(i);
		         glBegin(GL_LINES);

                 if(colorstyle[aType[i]-1]  == VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE)
				 {
			         glColor3f(Red[i],Green[i], Blue[i]);
 			         glVertex3d(px0[i], py0[i], pz0[i]);
			         glVertex3d(px1[i], py1[i], pz1[i]);
				 }
				 else if(colorstyle[aType[i]-1]  == VisAtomVisualizingPolicy::COLOR_STYLE_MAP)
				 {
					  float R, G, B, A;
                      VisAtomVisualizingPolicy::GetColor(R, G, B, A, i, s0);                    
			          glColor3f(R,G, B);
 			          glVertex3d(px0[i], py0[i], pz0[i]);
                      VisAtomVisualizingPolicy::GetColor(R, G, B, A, i, s0);                    
			          glColor3f(R,G, B);
			          glVertex3d(px1[i], py1[i], pz1[i]);
				 }
				 glEnd();
			     glPopName();
		       }
		  }
		  p0 = p1;
		  px0 = px1;
		  py0 = py1;
          pz0 = pz1;
		  s0  = s1;
	   } 
	  glEnable(GL_LIGHTING);
	  delete thick;
	  delete colorstyle;
	  return;
}













