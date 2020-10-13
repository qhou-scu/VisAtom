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
#include "VisAtomVisualizingPolicy.h"
#include "VisAtomSubSample.h"



void VisAtomSubSample::Clear()
{
	this->m_group.clear();

	while(!this->m_region.isEmpty())
	{
          VisAtomRegionData*p = this->m_region[0];
		  delete p;
		  this->m_region.removeAt(0);
	}
	
	this->m_dataregion.clear();
	this->m_datacol.clear();
	this->m_datacol.clear();

	while(!this->m_vispolicy.isEmpty())
	{
		VisAtomVisualizingPolicy*p = this->m_vispolicy[0];
		delete p;
		this->m_vispolicy.removeAt(0);
	}

	return;
}

VisAtomSubSample::~VisAtomSubSample()
{
	Clear();
}

VisAtomSubSample::VisAtomSubSample()
{
	  this->m_statu = VisAtomSubSample::SUBSAMPLE_VISIABLE;
	  VisAtomVisualizingPolicy*p;
	  //create the first default visualization policy
	  p = new VisAtomVisualizingPolicy();
	  p->SetColorStyle(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
      p->SetColorByDefaultTable(0);
	  this->m_vispolicy.append(p);

	  //create the secod visualization policy
	  p = new VisAtomVisualizingPolicy();
	  p->SetColorStyle(VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
	  this->m_vispolicy.append(p);

	  this->m_curvispolicy = 0;
	  this->m_pData = NULL;
      return;
}

VisAtomSubSample::VisAtomSubSample(int atype)
{
      this->m_statu = VisAtomSubSample::SUBSAMPLE_VISIABLE; 
	  //create the first default visualization policy
	  VisAtomVisualizingPolicy*p;
	  p = new VisAtomVisualizingPolicy();
	  p->SetColorStyle(VisAtomVisualizingPolicy::COLOR_STYLE_SINGLE);
	  p->SetColorByDefaultTable(atype-1);
	  this->m_vispolicy.append(p);

	  //create the secod visualization policy
	  p = new VisAtomVisualizingPolicy();
	  p->SetColorStyle(VisAtomVisualizingPolicy::COLOR_STYLE_MAP);
	  this->m_vispolicy.append(p);

	  this->m_group.append(atype);
	  this->m_curvispolicy = 0;
  	  this->m_pData = NULL;
	  return;
}

bool VisAtomSubSample::CheckMembership(int atype, float x, float y, float z, float scalar)
{
	int i, count;
	int ingroup, inregion, inscalar;

	//check if belong to one of the groups
	ingroup = 1;
	if(!this->m_group.isEmpty())
	{
		ingroup = 0;
		for(i=0; i<this->m_group.count(); i++)
		{
			if(atype == this->m_group[i])
			{
              ingroup = 1;
			  break;
			}
		}
	}

	//check if belong to one of the regions
	inregion = 1;
	if(!this->m_region.isEmpty())
	{
		inregion = 0;
		for(i=0; i<this->m_region.count(); i++)
		{
			//checking
		}
	}

	//check if belong to one of the scarlar regions
	inscalar = 1;
	if(!this->m_dataregion.isEmpty())
	{
		inscalar = 0;
		for(i=0; i<this->m_dataregion.count(); i++)
		{
			//checking
		}
	}
	//***
	return ingroup&&inregion&&inscalar;
}


int VisAtomSubSample::GetNumberofAtoms()
{
	  int i,j, num, col, row;
	  float*type, *x, *y, *z;
	  float*scalar;

	  if(!m_pData) return 0;

	  row = m_pData->GetNumRow();
	  //get type
	  col = m_pData->GetTypeCol();
	  if(col >=0) type = m_pData->GetData(col);
	  else type = NULL;

	  //get position
	  col = m_pData->GetPosXCol();
	  x = m_pData->GetData(col);
	  
	  col = m_pData->GetPosYCol();
	  y = m_pData->GetData(col);
	  
	  col = m_pData->GetPosZCol();
	  z = m_pData->GetData(col);


      num = 0;
	  if(VisAtomVisualizingPolicy::GetScalarCol() >=0)
	  {
		  scalar = m_pData->GetData(VisAtomVisualizingPolicy::GetScalarCol());
		  if(type) 
		  {  for(i=0; i<row; i++)
		           if(this->CheckMembership((int)(type[i]+0.000001), x[i], y[i], z[i],  scalar[i])) num++;
		  }
		  else 
		  {
             for(i=0; i<row; i++)
		           if(this->CheckMembership(1, x[i], y[i], z[i],  scalar[i])) num++;
		  }

	  }
	  else
		  if(type)
		  {
		     for(i=0; i<row; i++)
		         if(this->CheckMembership((int)(type[i]+0.000001), x[i], y[i], z[i])) num++;
		  }
		  else
		  {
		     for(i=0; i<row; i++)
		         if(this->CheckMembership(1, x[i], y[i], z[i])) num++;
		  }
      return num;
}

void VisAtomSubSample::AddVisualPolicy(VisAtomVisualizingPolicy*policy)
{
    VisAtomVisualizingPolicy*p = new VisAtomVisualizingPolicy(policy);
	this->m_vispolicy.append(p);
}

 