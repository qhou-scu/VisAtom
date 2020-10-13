/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/

#ifndef _VISCLUSTEREVELOPE_H
#define _VISCLUSTEREVELOPE_H
#include  "VisAtomExtendObject.h"
//*****************************
class ClusterEnvelopeFace
{
     protected:
		 friend class ClusterEnvelope;
	     friend class VisClusterEnvelope;

		 int m_NV;
		 int m_ITY0, m_IA0, m_GIA0;
		 int m_ITY1, m_IA1, m_GIA1;
		 float m_DVT0[3];
		 float m_DVT1[3];
		 float*m_Vert;
     public:
		 ClusterEnvelopeFace()
		 {	m_ITY0=0; m_IA0=0; m_GIA0=0;
		    m_ITY1=0; m_IA1=0; m_GIA1=0;
            m_NV = 0; m_Vert = NULL;};
         ClusterEnvelopeFace(int nv);
         ~ClusterEnvelopeFace();
         void Set(int nv);
         void Set(int nv, float dvt0[], float dvt1[], float*vert);
		 void Paint(); 
};

//*****************************
class ClusterEnvelope
{
     protected:
		 friend class ClusterEnvelopeFace;
		 friend class VisClusterEnvelope;
		 int m_NF;
		 ClusterEnvelopeFace**m_Face; 
         //display property
	     float m_red, m_green, m_blue, m_alpha; 
         
     public:
		 ClusterEnvelope();
         ClusterEnvelope(int nf);
         ~ClusterEnvelope();
		 void PaintSolidface(); 
		 void PaintWireface(); 
};

//*****************************
class VisClusterEnvelope:public VisAtomExtendObject
{
	private:

    protected:
       friend class ClusterEnvelope;
	   int m_NC;
	   ClusterEnvelope**m_Clus; 
    public:
		QString RUNTIME_CLASSNAME();

	    VisClusterEnvelope();
	    ~VisClusterEnvelope();
  	    void Clear(bool keepsubsample=false);
 		virtual void Paint();
		int LoadData(QString fname);
		int LoadDataNext();
		int LoadDataPrev();
		static int CURRENT_FILEINDEX();
		static QString CURRENT_FILENAME();
		int HasData(){ return m_NC;}

 };
 

#endif