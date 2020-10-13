
#ifndef VISATOMWLOADDATATHREAD_H
#define VISATOMWLOADDATATHREAD_H

#include <QThread>
#include <QString.h>

class VisAtomWidget;
class VisAtom_LoadTrajecoryThread : public QThread
{
    Q_OBJECT

public:
    VisAtom_LoadTrajecoryThread(QObject * parent = 0);
    ~VisAtom_LoadTrajecoryThread();
	void SetFileSerial(QString fname, int from, int to, int step, int vx, int vy, int vz);
	void SetDataCol(int vx, int vy, int vz);
	void AttachRender(VisAtomWidget*pRenader);

	QString GetFilename(int i);
	QString FirstFilename();
	QString LasttFilename();
	void Clear();
    void StartLoadData();
	void LoadFrom_FMTUNKNOWN();
	void LoadFrom_BOXCFG14();
	void LoadFrom_BOXCFG16();

	int IsDataLoaded(){return m_loaded;}
public slots:
    

signals:
   void OnStartLoadFile(QString& filename, int index, int total); 
   void OnEndLoadFile(QString& filename, int index, int total); 
protected:
   VisAtomWidget*m_pRender;
   QList<QString> m_SampleFileList;
   int m_loaded, m_colx, m_coly, m_colz;
   void run();
};

#endif
