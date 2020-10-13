/* This is a class used for in VisAtom, a visualiztion tool for
   atomistic simulations.

   AUTHOR:      Qing HOU
   INSTITUTION: Institute of Nuclear Science and Technology, Sichuan University
   HISTROTY:    First version: 1998
   LAST MODIFICATION: 2013
*/
#include <windows.h>	// Standard windows include
#include <gl\gl.h>		// OpenGL library
#include <gl\glu.h>		// OpenGL library

#include <math.h>
#include <stdio.h>

#include "Arrow.h"
#include "Arrowline.h"
#include "Circle.h"
#include "SolidCircle.h"
#include "Style.h"
#include "VisClusterEnvelope.h"

static QString CLASSNAME = "VisClusterEnvelope";
QString VisClusterEnvelope::RUNTIME_CLASSNAME(){ return CLASSNAME;};

static int     CURRENT_EXTNUM  = -1;
static QString CURRENT_FNAME   = "";  


//***********************************************************
ClusterEnvelopeFace::ClusterEnvelopeFace(int nv)
{
	m_ITY0=0; m_IA0=0; m_GIA0=0;
	m_ITY1=0; m_IA1=0; m_GIA1=0;
	m_NV = nv;
	if(m_NV > 0) m_Vert = new float[m_NV*3];
}

ClusterEnvelopeFace::~ClusterEnvelopeFace()
 {
    if(m_Vert) delete m_Vert;
    m_NV= 0;
 }


void ClusterEnvelopeFace::Set(int nv)
 {
     if(m_Vert) delete m_Vert;
	 m_Vert = NULL;
     m_NV   = nv;
	 if(m_NV > 0) m_Vert = new float[m_NV*3];

     return;
 }

 void ClusterEnvelopeFace::Set(int nv, float dvt0[], float dvt1[], float*vert)
 {
	 this->Set(nv);

     int i;
	 for (i=0; i<3; i++) 
	 {
		  m_DVT0[i] = dvt0[i];
		  m_DVT1[i] = dvt1[i];
	 }
	 for (i=0; i<nv; i++)
	 {
		 m_Vert[i*3]   = vert[i*3];
		 m_Vert[i*3+1] = vert[i*3+1];
		 m_Vert[i*3+2] = vert[i*3+2];
	 }
     return;
 }
 void ClusterEnvelopeFace::Paint()
 {
	  int iv;
	  float norm[3];
	  glPushMatrix();

	 //glTranslatef(pos0[0], pos0[1], pos0[2]);
      //determine the normal
	  glTranslatef(this->m_DVT0[0], this->m_DVT0[1], this->m_DVT0[2]);
	  /*glNormal3f(this->m_DVT1[0] - this->m_DVT0[0], 
		         this->m_DVT1[1] - this->m_DVT0[1], 
				 this->m_DVT1[2] - this->m_DVT0[2]);*/

      glBegin(GL_POLYGON);
	    for(iv=0; iv<this->m_NV; iv++)
	    {
           glNormal3f(this->m_Vert[iv*3]   - this->m_DVT0[0], 
		              this->m_Vert[iv*3+1] - this->m_DVT0[1], 
				      this->m_Vert[iv*3+2] - this->m_DVT0[2]);
			glVertex3f(this->m_Vert[iv*3], this->m_Vert[iv*3+1], this->m_Vert[iv*3+2]);
		}
		glEnd();
	  glPopMatrix();

 }

 //***********************************************************
 ClusterEnvelope:: ClusterEnvelope()
 {
	 m_NF=0; 
	 m_Face= NULL;	
	this->m_red   = 0.5f;
	this->m_green = 0.5f;
	this->m_blue  = 0.f;
	this->m_alpha = 1.0f;
 };

ClusterEnvelope::ClusterEnvelope(int nf)
{
	this->m_NF = nf;
	if(this->m_NF > 0) this->m_Face = new ClusterEnvelopeFace*[m_NF];
	for(int i=0; i<this->m_NF; i++) this->m_Face[i] = new ClusterEnvelopeFace();
	this->m_red   = 0.5f;
	this->m_green = 0.5f;
	this->m_blue  = 0.f;
	this->m_alpha = 1.0f;
}

ClusterEnvelope::~ClusterEnvelope()
 {
    if(this->m_Face) 
	{
		for(int i=0; i<this->m_NF; i++)
			if(this->m_Face[i]) delete this->m_Face[i];
	   	delete this->m_Face;
		this->m_Face = NULL;
	}
    this->m_NF= 0;
 }

 
 void ClusterEnvelope::PaintSolidface()
 {
	 glColor4f(this->m_red, this->m_green, this->m_blue, this->m_alpha);
	 for(int i=0; i<this->m_NF; i++)
	 {
		 if(this->m_Face[i]) this->m_Face[i]->Paint();
	 }
 }

 void ClusterEnvelope::PaintWireface()
 {
	 glColor3f(1.f-this->m_red, 1.f-this->m_green, 1.f-this->m_blue);
	 for(int i=0; i<this->m_NF; i++)
	 {
		 if(this->m_Face[i]) this->m_Face[i]->Paint();
	 }
 }



//************************************************************
int	static iscomment( char*str)
{
/*
!  Purpose: to determinte if the string is a commnet line
!  Input:   STRING a string
!  Auther:  Hou Qing, Inst. of Nucl. Sci.and Tech., Sichuan Union University
!            
!
*/

	 for(int i=0; i<strlen(str); i++)
	 {
        if (str[i] == ' ') continue;
		if (str[i] != '!') return 0;
		return 1;
	 }
	 return 0;
}

//************************************************************
int	static haskeyword( char*str, int&at, int&len)
{
/*
!  Purpose: to determinte if the string has keyword
!  Input:   STRING a string
!  Auther:  Hou Qing, Inst. of Nucl. Sci.and Tech., Sichuan Union University
!            
!
*/
     at  = -1;
	 len =  0;
	 for(int i=0; i<strlen(str); i++)
	 {
        if (at<0 && str[i] == ' ') continue;
		if (at<0 && str[i] != '&') return 0;

		if(str[i] == ' '||str[i] == ',' ||str[i] == ':'|| str[i]==char(10)) break; //char(10) new line
		if(at < 0)  at = i;
        len++;  
		
	 }
	 if(at < 0 ) return 0;
	 return 1;
}

//************************************************************
int	static iskeyword( char*str, const char*keyword)
{
/*
!  Purpose: to determinte if the string has the given keyword
!  Input:   STRING a string
!  Auther:  Hou Qing, Inst. of Nucl. Sci.and Tech., Sichuan Union University
!            
!
*/
	 int at, len;
	 if( !haskeyword( str, at, len)) return 0;

     if(len != strlen(keyword)) return 0;

	 for(int i=0; i<len; i++)
        if(str[i+at] != keyword[i]) return 0;   
	 return 1;
}



//************************************************************
int	static extract_numb( char*str, int mxCount, char**strnumb)
{
/*
!  Purpose: to extract number from a string
!  Input:   STRING a string
!  Ouput:   COUNT  the number of numbers found in the string
!           REAL_NUMB the number text founded
!  Note:    the REAL_NUMBi are character varible. These character number
!           could be transformed to real number by DRSTR or RSTRi
! Auther:   Hou Qing, Inst. of Nucl. Sci.and Tech., Sichuan Union University
!            
!
*/
	 int  i, flag, index1, count;

	 count=0;
	 index1=0;
	 i = 0;
	 flag = 0;
	 do 
	 {
	   if(str[i] == ' ' || str[i] == ',') 
	   {
	       if(index1 > 0) 
		   {
            strncpy(strnumb[count-1], &str[i-index1], index1);
            strnumb[count-1][index1] = NULL;
	        index1 = 0;
		   }
	      flag = 0;
	   }
       else 
	   {
	     if((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '+' || str[i] == '.' )
		 {
	        if(flag == 0 ) 
			{ 
			   flag = 1;
	           count ++;
	           if(count > mxCount) break;
			}
		 }
         else
		 {
	        if(flag != 1) flag = 2;
		 }
	   }
          
	  if(flag == 1) index1 ++;

	  i++;
	  if(i >= strlen(str) )
	  {
	     if(index1 > 0) 
		 {
            strncpy(strnumb[count-1], &str[i-index1], index1);
		    strnumb[count-1][index1] = NULL;
		 }
	     break;
	  }
	 } while(1);
	 return count;
}

//***********************************************************
void VisClusterEnvelope::Clear(bool keepsubsample)
{
	if(this->m_Clus) 
	{
		for(int i=0; i<this->m_NC; i++) 
			if(this->m_Clus[i]) delete this->m_Clus[i];
		delete this->m_Clus; 
	}
    this->m_NC = 0;
	this->m_Clus = NULL;
	return;
}

//***********************************************************
VisClusterEnvelope::~VisClusterEnvelope()
{
	Clear();
}
//***********************************************************
VisClusterEnvelope::VisClusterEnvelope()
{
     this->m_NC = 0;
	 this->m_Clus = NULL;
 
}

//***********************************************************
void VisClusterEnvelope::Paint()
{
	
	if(this->m_NC == 0) return;

         Style  Sty;
         glEnable(GL_DEPTH_TEST);
		 glEnable(GL_LIGHTING);
		 Sty.Solid();
         glEnable(GL_COLOR_MATERIAL);
         glEnable(GL_NORMALIZE);

	     for(int i=0; i<this->m_NC; i++)
	     {
		     if(this->m_Clus[i]) this->m_Clus[i]->PaintSolidface();
	     }
		 glDisable(GL_LIGHTING);
	     Sty.Wire();
	     for(int i=0; i<this->m_NC; i++)
	     {
		     if(this->m_Clus[i]) this->m_Clus[i]->PaintWireface();
	     }
     return;
}

int VisClusterEnvelope::CURRENT_FILEINDEX()
{
	return CURRENT_EXTNUM;
}

QString VisClusterEnvelope::CURRENT_FILENAME()
{
	return CURRENT_FNAME;
}
//***********************************************************
#define MAXSTRL 256
#define MAXCOL   10 
#define NUMSTRL  32 
int VisClusterEnvelope::LoadData(QString fname)
{

		 QFile file(fname);
         if(!file.open(QIODevice::ReadOnly|QIODevice::Text)) return 0;


		 //to determine the format of the file
         char buf[MAXSTRL];
		 char*strnumb[MAXCOL];
		 int  i, nc;
		 for(i=0; i<MAXCOL; i++) strnumb[i] = new char[NUMSTRL];

		 int hlines = 0;
         while (!file.atEnd()) 
		 {
				 int len, ind;
				 len = file.readLine(buf, sizeof(buf))-1;
				 //skip empty lines
				 hlines ++;
				 if(len <= 0) continue; 
				 if(iscomment(buf)) continue;
 
				 if(haskeyword(buf, ind, len))
				 {
                    if(iskeyword(buf, "&VORONOI_CLUS") )
						 break;
   				    else
					 {
                         QMessageBox msgBox;
		                 msgBox.setIcon(QMessageBox::Warning);
						 QString str, snum;
						 snum.setNum(hlines);
						 str = "Unknown keyword at line "; 
						 str += snum; 
						 str += ". keyword &VORONOI_CLUS is expected"; 
						 msgBox.setText(str);
						 msgBox.exec();
						 for(i=0; i<MAXCOL; i++) delete strnumb[i];
                         file.close();
						 return 0;
					 }
				 }

         }

		 //to get the number of cluster
         while (!file.atEnd()) 
		 {
				 int len, ind;
				 len = file.readLine(buf, sizeof(buf))-1;
				 //skip empty lines
				 hlines++;
				 if(len <= 0) continue;  
				 if(iscomment(buf)) continue;

 				 if(iskeyword(buf, "&NCLUS") )
				 {
				    if( extract_numb( buf, 1, strnumb) )
					{
					     nc = atoi(strnumb[0]);
						 break;
					}
				 }
				 else
				 {
                     QMessageBox msgBox;
		             msgBox.setIcon(QMessageBox::Warning);
					 QString str, snum;
					 snum.setNum(hlines);
					 str = "Unknown keyword at line "; 
					 str += snum; 
					 str += ". keyword &NCLUS is expected"; 
					 msgBox.setText(str);
					 msgBox.exec();
					 for(i=0; i<MAXCOL; i++) delete strnumb[i];
       		         file.close();
					 return 0;
				 }

         }
         
         // to begine load the data
		 this->Clear();
		 
         this->m_NC = nc;
		 if(this->m_NC > 0)
		 {
		    this->m_Clus = new ClusterEnvelope*[this->m_NC];
			for(i=0; i<this->m_NC; i++) this->m_Clus[i] = NULL;

			nc =0;
		    //to get load the cluster facess
		    int len, ind, ic = -1, nf;
			for(i=0; i<this->m_NC; i++) 
			{
               // get the cluster id
               while (!file.atEnd()) 
		       {
				 len = file.readLine(buf, sizeof(buf))-1;
				 hlines++;
				 if(len <= 0) continue;  
				 if(iscomment(buf)) continue;

				 //the cluster id
 				 if(iskeyword(buf, "&CLUS") )
				 {
					 if(extract_numb( buf, 1, strnumb)) { ic = atoi(strnumb[0])-1; break;}
				 }
				 else
					 continue;
			   }

			   //the number of faces
               while (!file.atEnd()) 
		       {
			     len = file.readLine(buf, sizeof(buf))-1;
				 hlines++;
				 if(len <= 0) continue;  
			     if(iscomment(buf)) continue;

 			   	 if( iskeyword(buf, "&NFACE") )
				 {
					 if(extract_numb( buf, 1, strnumb)) {nf = atoi(strnumb[0]); break;}
					 else {nf = 0; break;};
				 }
				 else
					   continue;
			   }
               //create the envelope of  nf facet
			   this->m_Clus[ic] = new ClusterEnvelope(nf); 

			   //get the vertices of faces
			   int j, iface, nv, iv;
			   for(j=0; j<nf; j++)
			   {
                   while (!file.atEnd()) 
		           {
				     len = file.readLine(buf, sizeof(buf))-1;
				     hlines++;
				     if(len <= 0) continue;  
			         if(iscomment(buf)) continue;
 
 			   	     if( iskeyword(buf, "&IFACE") )
				     {
					    int ncol;
						ncol  = extract_numb( buf, MAXCOL, strnumb);
                        iface = atoi(strnumb[0])-1;
                        this->m_Clus[ic]->m_Face[iface]->m_IA0     = atoi(strnumb[1]);
                        this->m_Clus[ic]->m_Face[iface]->m_GIA0    = atoi(strnumb[2]);
                        this->m_Clus[ic]->m_Face[iface]->m_ITY0    = atoi(strnumb[3]);
                        this->m_Clus[ic]->m_Face[iface]->m_DVT0[0] = atof(strnumb[4]);
                        this->m_Clus[ic]->m_Face[iface]->m_DVT0[1] = atof(strnumb[5]);
                        this->m_Clus[ic]->m_Face[iface]->m_DVT0[2] = atof(strnumb[6]);

                        len   = file.readLine(buf, sizeof(buf))-1;
					    ncol  = extract_numb( buf, MAXCOL, strnumb);
                        this->m_Clus[ic]->m_Face[iface]->m_IA1     = atoi(strnumb[0]);
                        this->m_Clus[ic]->m_Face[iface]->m_GIA1    = atoi(strnumb[1]);
                        this->m_Clus[ic]->m_Face[iface]->m_ITY1    = atoi(strnumb[2]);
                        this->m_Clus[ic]->m_Face[iface]->m_DVT1[0] = atof(strnumb[3]);
                        this->m_Clus[ic]->m_Face[iface]->m_DVT1[1] = atof(strnumb[4]);
                        this->m_Clus[ic]->m_Face[iface]->m_DVT1[2] = atof(strnumb[5]);

						//gett number of vetices
                        len   = file.readLine(buf, sizeof(buf))-1;
						if( iskeyword(buf, "&NVERT") ) 
						{
					       ncol  = extract_numb( buf, 1, strnumb);
						   nv    = atoi(strnumb[0]);
						   this->m_Clus[ic]->m_Face[iface]->Set(nv);
						   for(iv=0; iv<nv; iv++)
						   {
                              len = file.readLine(buf, sizeof(buf))-1;
							  ncol  = extract_numb( buf, 3, strnumb);
                              this->m_Clus[ic]->m_Face[iface]->m_Vert[iv*3  ] = atof(strnumb[0]);
                              this->m_Clus[ic]->m_Face[iface]->m_Vert[iv*3+1] = atof(strnumb[1]);
                              this->m_Clus[ic]->m_Face[iface]->m_Vert[iv*3+2] = atof(strnumb[2]);
						   }
					    } //end for &NVERT 
                        break;
				     } //end for iface if(haskeyword(buf) && iskeyword(buf, "&IFACE") )
				     break;
				   } //end for while (!file.atEnd()) 
			   }//end for (j=0; j<nf; j++)
			   nc++;
			 ////////
			}
		 }
		 file.close();

	     //update the current file name
		 QString ext, dot='.';
         int index = fname.length()-fname.lastIndexOf(dot);

		 ext = fname.section(dot, -1);
		 CURRENT_EXTNUM = ext.toInt(); 
		 CURRENT_FNAME  = fname;
		 CURRENT_FNAME.chop(index);

		 for(i=0; i<MAXCOL; i++) delete strnumb[i];
		 return this->m_NC;
         //this->unsetCursor();

		
}

int VisClusterEnvelope::LoadDataNext()
{

	     if(CURRENT_EXTNUM < 0)    return 0;
		 if(CURRENT_FNAME.isEmpty()) return 0;

		 int ind = CURRENT_EXTNUM + 1;

   	     QString ext, snum, currentfname=CURRENT_FNAME;
         snum.setNum(ind);
		
		 ext='.';
		 if(ind < 10)
 		    ext.append("000");
		  else if(ind < 100)
		    ext.append("00");
		 else if(ind < 1000)
		    ext.append("0");
	     ext.append(snum);
		 currentfname.append(ext);
		 return LoadData(currentfname);
}

int VisClusterEnvelope::LoadDataPrev()
{

	     if(CURRENT_EXTNUM < 0)    return 0;
		 if(CURRENT_FNAME.isEmpty()) return 0;

		 int ind = CURRENT_EXTNUM - 1;

   	     QString ext, snum, currentfname=CURRENT_FNAME;
         snum.setNum(ind);
		
		 ext='.';
		 if(ind < 10)
 		    ext.append("000");
		  else if(ind < 100)
		    ext.append("00");
		 else if(ind < 1000)
		    ext.append('0');
	     ext.append(snum);
		 currentfname.append(ext);
		 return LoadData(currentfname);
}




