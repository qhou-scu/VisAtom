#include <QFile.h>
#include <VisAtom_LoadTrajecoryThread.h>
#include "VisAtomTrajectory.h"
#include "VisAtomWidget.h"


VisAtom_LoadTrajecoryThread::VisAtom_LoadTrajecoryThread( QObject  *parent)
    : QThread(parent)
{
   m_pRender = NULL;
   m_loaded = 0;
   m_colx = m_coly = m_colz = -1;

}

VisAtom_LoadTrajecoryThread::~VisAtom_LoadTrajecoryThread()
{  
	this->Clear();

}

void VisAtom_LoadTrajecoryThread::Clear()
{
	m_pRender = NULL;
	m_SampleFileList.clear();
	return;
}

//***************************************************************
void VisAtom_LoadTrajecoryThread::SetFileSerial(QString fname, int from, int to, int step, int colx, int coly, int colz)
{

	m_SampleFileList.clear();

	int i;
	QString fileName, ext, snum;
	for(i=from; i<=to;i=i+step)
	{
	    if(i<10)        ext = ".000";
	    else if(i<100)  ext = ".00";
	    else if(i<1000) ext = ".0";
   
	    snum.setNum(i);
	    ext.append(snum);
	    fileName = fname;
	    fileName.append(ext);
		m_SampleFileList.append(fileName);
   }
   m_loaded = 0;
   m_colx  = colx;
   m_coly  = coly;
   m_colz  = colz;
   return;
}

void VisAtom_LoadTrajecoryThread::AttachRender(VisAtomWidget*pRenader)
{
	this->m_pRender = pRenader;
}


QString VisAtom_LoadTrajecoryThread::GetFilename(int i)
{
	if(m_SampleFileList.isEmpty()) return tr("");
	if(i >= m_SampleFileList.size()) return tr("");
	return m_SampleFileList[i]; 
}

QString VisAtom_LoadTrajecoryThread::FirstFilename()
{
	if(m_SampleFileList.isEmpty()) return tr("");
	return m_SampleFileList[0]; 
}
QString VisAtom_LoadTrajecoryThread::LasttFilename()
{
	if(m_SampleFileList.isEmpty()) return tr("");
	return m_SampleFileList[m_SampleFileList.size()-1]; 
}

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

#define MAXCOL  64
#define MAXSTRL 32
void VisAtom_LoadTrajecoryThread::StartLoadData()
{
	 if(this->m_SampleFileList.isEmpty()) return;
	 if(this->m_loaded) return;

	 VisAtomTrajectory*pTrajectory = this->m_pRender->GetTrajectory(); 
	 pTrajectory->Clear();

     
	 int nrow=0, ncol=0, i,j, hlines;
     char buf[MAXCOL*MAXSTRL];
     //determin the format of file
	 QFile file;
	 //open the first file to determine the format
	 file.setFileName(this->m_SampleFileList[0]);
     file.open(QIODevice::ReadOnly|QIODevice::Text);
	 hlines = 0;
     while (!file.atEnd()) 
	 {
		 int len;
		 len = file.readLine(buf, sizeof(buf))-1;
		 if(len <= 0) {hlines++; continue;}
		 if(buf[0] == '&' && buf[1] == 'B' && buf[2] == 'O' && buf[3] == 'X'&& buf[4] == 'C'&& buf[5] == 'F'&& buf[6] == 'G'&& 
		    buf[7] == '1' && buf[8] == '4')
		 {
             LoadFrom_BOXCFG14();
			break;
		 }

		 else 
	     if(buf[0] == '&' && buf[1] == 'B' && buf[2] == 'O' && buf[3] == 'X'&& buf[4] == 'C'&& buf[5] == 'F'&& buf[6] == 'G'&& 
		    buf[7] == '1' && buf[8] == '5')
		 {
             LoadFrom_BOXCFG14();
			break;
		 }
		 else
	     if(buf[0] == '&' && buf[1] == 'B' && buf[2] == 'O' && buf[3] == 'X'&& buf[4] == 'C'&& buf[5] == 'F'&& buf[6] == 'G'&& 
		    buf[7] == '1' && buf[8] == '6')
		 {
             LoadFrom_BOXCFG16();
			break;
		 }

		 else
		 {
            LoadFrom_FMTUNKNOWN();
            break;
		 }
      }
	 file.close();
}


void VisAtom_LoadTrajecoryThread::LoadFrom_FMTUNKNOWN()
{
	 if(this->m_SampleFileList.isEmpty()) return;
	 if(this->m_loaded) return;

	 VisAtomTrajectory*pTrajectory = this->m_pRender->GetTrajectory(); 
	 pTrajectory->Clear();


	 int nrow=0, ncol=0, i,j, flag, sline;
     char buf[MAXCOL*MAXSTRL];
	 char*strnumb[MAXCOL];
	 for(i=0; i<MAXCOL; i++) strnumb[i] = new char[MAXSTRL];

     emit OnStartLoadFile(this->m_SampleFileList[0], 0, this->m_SampleFileList.size()); 
	 QFile file;
	 //open the first file to determine the number of cols and rows
	 file.setFileName(this->m_SampleFileList[0]);
     file.open(QIODevice::ReadOnly|QIODevice::Text);
	 sline = 0;
     while (!file.atEnd()) 
	 {
			 int len, lentrim;
			 len = file.readLine(buf, sizeof(buf))-1;
			 if(len <= 0) {sline++; continue;}
			 flag = 0;
 			 lentrim = 0;
			 for(i=0; i<len; i++)
			 {
				 if(buf[i] == ' ') continue;
				 lentrim++;
				 if(buf[i] == '!' || buf[i] == '&') 
				  {
					flag = 1; sline++; 
					break; 
				  }
				 else break;
			 }
			 if(flag) continue; 
			 if(lentrim <= 0 ) { if(nrow <=0) sline++; continue;}
			 nrow ++;
			 if(nrow == 1) ncol = extract_numb( buf, MAXCOL, strnumb);
         }
	 file.close();

	 //load the position and diaplacement data of the first point
	 for(j=0; j<this->m_SampleFileList.size(); j++)
	 {
		 VisAtomDataSheet*pData = new VisAtomDataSheet(ncol, nrow);
		 int ncol1; 

         file.setFileName(this->m_SampleFileList[j]);
		 file.open(QIODevice::ReadOnly|QIODevice::Text);
         //the lenth of the hearder for files could be different
		 //thus we should redetermine sline for each file
		 sline = 0;
         while (!file.atEnd()) 
	     {
			 int len, lentrim;
			 len = file.readLine(buf, sizeof(buf))-1;
			 if(len <= 0) {sline++; continue;}
			 flag = 0;
 			 lentrim = 0;
			 for(i=0; i<len; i++)
			 {
				 if(buf[i] == ' ') continue;
				 lentrim++;
				 if(buf[i] == '!' || buf[i] == '&') 
				  {
					flag = 1; sline++; 
					break; 
				  }
				 else break;
			 }
			 if(flag) continue; 
			 if(lentrim <= 0 ) { if(nrow <=0) sline++; continue;}
			 break;
         }
	     file.close();
		 //   
		 file.open(QIODevice::ReadOnly|QIODevice::Text);
		 for(i=0; i<sline; i++)
		    file.readLine(buf, sizeof(buf));
		 for(i=0; i<nrow; i++)
		 {
		    file.readLine(buf, sizeof(buf));
			ncol1 = extract_numb( buf, MAXCOL, strnumb);
			if(ncol1 != ncol) 
			{
               ncol1 = ncol;
			}
            pData->SetDataForRow(i, strnumb);
		 }
		 file.close();
		 //add the data to the trajectory object
		 pData->SetPosxCol(this->m_colx);
		 pData->SetPosyCol(this->m_coly);
		 pData->SetPoszCol(this->m_colz);
		 pTrajectory->AddData(pData);
		 //The system down if we use GL here,why?
		 //this->m_pRender->updateGL();
	    emit OnEndLoadFile(this->m_SampleFileList[j], j, this->m_SampleFileList.size()); 
	 }
	 this->m_loaded = 1;

     //clear the temp data
	 for(i=0; i<MAXCOL; i++) delete strnumb[i];
}

void VisAtom_LoadTrajecoryThread::LoadFrom_BOXCFG14()
{
	 if(this->m_SampleFileList.isEmpty()) return;
	 if(this->m_loaded) return;

	 VisAtomTrajectory*pTrajectory = this->m_pRender->GetTrajectory(); 
	 pTrajectory->Clear();


	 int nrow=0, ncol=0, i,j, flag, sline;
     char buf[MAXCOL*MAXSTRL];
	 char*strnumb[MAXCOL];
	 for(i=0; i<MAXCOL; i++) strnumb[i] = new char[MAXSTRL];

     emit OnStartLoadFile(this->m_SampleFileList[0], 0, this->m_SampleFileList.size()); 
	 QFile file;
	 //open the first file to determine the number of cols and rows
	 file.setFileName(this->m_SampleFileList[0]);
     file.open(QIODevice::ReadOnly|QIODevice::Text);

	 flag = 0;
     while (!file.atEnd()) 
	 {
		 int len;
		 len = file.readLine(buf, sizeof(buf))-1;
		 if(flag==0 && buf[0] == '&' && buf[1] == 'N' && buf[2] == 'A' && buf[3] == 'T'&& buf[4] == 'O'&& buf[5] == 'M')
		 {
			 extract_numb( buf, MAXCOL, strnumb);
             nrow = nrow+atoi(strnumb[1]); 
			 flag = 1;
		 }
		 if(buf[0] == '&' && buf[1] == 'T' && buf[2] == 'Y' && buf[3] == 'P'&& buf[4] == 'E')
		 {
			if(ncol <= 0)
		   {
  			   len = file.readLine(buf, sizeof(buf))-1;
			   ncol = extract_numb( buf, MAXCOL, strnumb);
			}
			flag = 0;
			break;
		 }
      }
	  file.close();

	 //load the position and displacement data of the first point
	 for(j=0; j<this->m_SampleFileList.size(); j++)
	 {
		 VisAtomDataSheet*pData = new VisAtomDataSheet(ncol, nrow);

         file.setFileName(this->m_SampleFileList[j]);
		 file.open(QIODevice::ReadOnly|QIODevice::Text);

		 int ii;
		 ii = 0; flag = 0;
         while (!file.atEnd()) 
		 {
			 int len,na;
			 len = file.readLine(buf, sizeof(buf))-1;
			 if(flag==0 && buf[0] == '&' && buf[1] == 'N' && buf[2] == 'A' && buf[3] == 'T'&& buf[4] == 'O'&& buf[5] == 'M')
			 {
				 extract_numb( buf, MAXCOL, strnumb);
                 na = atoi(strnumb[1]); 
				 flag = 1;
			 }
		     if(buf[0] == '&' && buf[1] == 'T' && buf[2] == 'Y' && buf[3] == 'P'&& buf[4] == 'E')
			 {
		       for(i=0; i<na; i++)
		      {
		         file.readLine(buf, sizeof(buf));
			     ncol = extract_numb( buf, MAXCOL, strnumb);
                 pData->SetDataForRow(ii, strnumb);
 				 ii++;
		       }
			   flag = 0;
			   break;
			 }
		 }
		 file.close();
		 //add the data to the trajectory object
		 pData->SetPosxCol(this->m_colx);
		 pData->SetPosyCol(this->m_coly);
		 pData->SetPoszCol(this->m_colz);
		 pTrajectory->AddData(pData);
		 //The system down if we use GL here,why?
		 //this->m_pRender->updateGL();
	    emit OnEndLoadFile(this->m_SampleFileList[j], j, this->m_SampleFileList.size()); 
	 }
	 this->m_loaded = 1;

     //clear the temp data
	 for(i=0; i<MAXCOL; i++) delete strnumb[i];
}

void VisAtom_LoadTrajecoryThread::LoadFrom_BOXCFG16()
{
	 if(this->m_SampleFileList.isEmpty()) return;
	 if(this->m_loaded) return;

	 VisAtomTrajectory*pTrajectory = this->m_pRender->GetTrajectory(); 
	 pTrajectory->Clear();


	 int nrow=0, ncol=0, i,j, flag, sline;
     char buf[MAXCOL*MAXSTRL];
	 char*strnumb[MAXCOL];
	 for(i=0; i<MAXCOL; i++) strnumb[i] = new char[MAXSTRL];

     emit OnStartLoadFile(this->m_SampleFileList[0], 0, this->m_SampleFileList.size()); 
	 QFile file;
	 //open the first file to determine the number of cols and rows
	 file.setFileName(this->m_SampleFileList[0]);
     file.open(QIODevice::ReadOnly|QIODevice::Text);

	 flag = 0;
     while (!file.atEnd()) 
	 {
		 int len;
		 len = file.readLine(buf, sizeof(buf))-1;
		 if(flag==0 && buf[0] == '&' && buf[1] == 'N' && buf[2] == 'A' && buf[3] == 'T'&& buf[4] == 'O'&& buf[5] == 'M')
		 {
			 extract_numb( buf, MAXCOL, strnumb);
             nrow = nrow+atoi(strnumb[0]); 
			 flag = 1;
		 }
		 if(buf[0] == '&' && buf[1] == 'T' && buf[2] == 'Y' && buf[3] == 'P'&& buf[4] == 'E')
		 {
			if(ncol <= 0)
		   {
  			   len = file.readLine(buf, sizeof(buf))-1;
			   ncol = extract_numb( buf, MAXCOL, strnumb);
			}
			flag = 0;
			break;
		 }
      }
	  file.close();

	 //load the position and displacement data of the first point
	 for(j=0; j<this->m_SampleFileList.size(); j++)
	 {
		 VisAtomDataSheet*pData = new VisAtomDataSheet(ncol, nrow);

         file.setFileName(this->m_SampleFileList[j]);
		 file.open(QIODevice::ReadOnly|QIODevice::Text);

		 int ii;
		 ii = 0; flag = 0;
         while (!file.atEnd()) 
		 {
			 int len,na;
			 len = file.readLine(buf, sizeof(buf))-1;
			 if(flag==0 && buf[0] == '&' && buf[1] == 'N' && buf[2] == 'A' && buf[3] == 'T'&& buf[4] == 'O'&& buf[5] == 'M')
			 {
				 extract_numb( buf, MAXCOL, strnumb);
                 na = atoi(strnumb[0]); 
				 flag = 1;
			 }
		     if(buf[0] == '&' && buf[1] == 'T' && buf[2] == 'Y' && buf[3] == 'P'&& buf[4] == 'E')
			 {
		       for(i=0; i<na; i++)
		      {
		         file.readLine(buf, sizeof(buf));
			     ncol = extract_numb( buf, MAXCOL, strnumb);
                 pData->SetDataForRow(ii, strnumb);
 				 ii++;
		       }
			   flag = 0;
			   break;
			 }
		 }
		 file.close();
		 //add the data to the trajectory object
		 pData->SetPosxCol(this->m_colx);
		 pData->SetPosyCol(this->m_coly);
		 pData->SetPoszCol(this->m_colz);
		 pTrajectory->AddData(pData);
		 //The system down if we use GL here,why?
		 //this->m_pRender->updateGL();
	    emit OnEndLoadFile(this->m_SampleFileList[j], j, this->m_SampleFileList.size()); 
	 }
	 this->m_loaded = 1;

     //clear the temp data
	 for(i=0; i<MAXCOL; i++) delete strnumb[i];
}



void VisAtom_LoadTrajecoryThread::SetDataCol(int colx, int coly, int colz)
{
	   m_colx  = colx;
       m_coly  = coly;
       m_colz  = colz;
	   if(this->m_loaded)
	   {
	     VisAtomTrajectory*pTrajectory = this->m_pRender->GetTrajectory(); 
		 pTrajectory->SetDataCol(this->m_colx, this->m_coly, this->m_colz);
	 }
	  return;
 }




void VisAtom_LoadTrajecoryThread::run()
{
}

