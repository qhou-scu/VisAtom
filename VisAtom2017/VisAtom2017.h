#ifndef VISATOM2017_H
#define VISATOM2017_H

#include <QtWidgets/QMainWindow>
#include "ui_visatom2017.h"
class VisAtomDataSheet;
class VisAtom_LoadTrajecoryThread;
class VisAtomExtendObject;
class Visatom_AtomfilterDlg;
class Visatom_FindatomDlg;

class VisAtom2017 : public QMainWindow
{
	Q_OBJECT

public:
	VisAtom2017(QWidget *parent = Q_NULLPTR);
	~VisAtom2017();
    
private:
	Ui::VisAtom2017Class ui;

    VisAtomDataSheet*m_SampleDataSheet;
    VisAtom_LoadTrajecoryThread*m_LoadDataThread;

	void UpdateSubsampleUI();
	void UpdateSubsampleLIST();
	int  m_OnUpdateSubsampleUI;
	int  m_OnUpdateClipParamUI;

	QLabel m_statulabel;
	QLabel m_subsmaplelabel;
	QProgressBar m_progressbar;
	Visatom_AtomfilterDlg* m_Atomfilter;
	Visatom_FindatomDlg* m_FindatomDlg;

	
public slots:

private slots:
    void OnFileOpen(); 
    void OnFileNext(); 
    void OnFilePrev(); 
    void OpenFile(QString fname, bool selcoldlg = true); 
	void LoadFrom_FMTUNKNOWN(QString fileName);
	void LoadFrom_BOXCFG14(QString fileName);
	void LoadFrom_BOXCFG16(QString fileName);
    void OnSaveImage();
	void OnAboutVisatom2013();

	void OnActionCopyImage();
	void OnActionRotateX(bool checked);
	void OnActionRotateY(bool checked);
	void OnActionRotateZ(bool checked);
	void OnActionSubsample_filter(bool checked);
	void OnActionZoomview(bool checked);
	void OnActionVeiwportCenter(bool checked);
	void OnActionFindatom();
	void OnSelectRotationX();
	void OnSelectRotationY();
	void OnSelectRotationZ();
	void OnSelectZoomScene();
	void OnChangeSceneDistance();
	void OnChangeAtomShapeDot(bool checked);
	void OnChangeAtomShapeCircle(bool checked);
	void OnChangeAtomShapeWireSphere(bool checked);
	void OnChangeAtomShapeSolidSphere(bool checked);
	void OnChangeAtomRadiu();
	void OnVariableAtomSize(bool checked);
	void OnUnifiedAtomSize(bool checked);
	int  OnEditSizeDataButton();
	void OnChangeAtomColor();
	void OnSingleColorRadioButton(bool checked);
	void OnMappingColorRadioButton(bool checked);
	void OnNewColorPolicy();
	void OnScalarwin0SpinBox(double value);
	void OnScalarwin1SpinBox(double value);
	void OnScalarwin2SpinBox(double value);
	void OnScalarwin3SpinBox(double value);
	void OnScalarwin4SpinBox(double value);
	void OnScalarwin5SpinBox(double value);
	void OnChangeGraphQuality(int nfai);

	void OnNewVectorPolicy();
	void OnVectorOptionGroupBox(bool checked);
	void OnSelectVectorStyleLine(bool checked);
	void OnSelectVectorStyleSolid(bool checked);


    void OnSelectSceneStyle(QListWidgetItem*,QListWidgetItem*);
	void OnClipEnabled();
	void OnClipthroughPosRadioButton(bool checked);
	void OnClipthroughAtomRadioButton(bool checked);
	void OnClipthroughAtomSpinBox(int atomid);
	void OnNewClipDirPushButton();
	void OnClipDirComboBoxIndexChanged(int index);
	void OnClipDirCheckBoxChanged(bool enable);

	void GetClipParamFromUI(VisAtomCliper*clip); 
    void OnSelectSubsample(QTreeWidgetItem*item);
	void OnSubsampleListItemChanged(QTreeWidgetItem*item);
	void OnSubsampleListTreeItemDBClick(QTreeWidgetItem*, int index);
	void OnMultipleSelectSubsample();

	void OnShowSceneOperation(bool checked);
	void OnShowSubsampleOption(bool checked);
	void OnShowSubsampleBrowser(bool checked);
	void OnCloseSceneOperation(bool checked);
	void OnCloseSubsampleOption(bool checked);
	void OnCloseSubsampleBrowser(bool checked);
	void OnNormalizeView();

	void OnTrajectoryOptionGroupBox(bool checked);
    void OnTrajectoryDataSRadioButton(bool checkd);
    void OnTrajectoryDataMRadioButton(bool checkd);
    void OnShapeOptionGroupBox(bool checked);

    void OnStartLoadTrajectory(QString& filename, int index, int total); 
    void OnEndLoadTrajectory(QString& filename, int index, int total); 
	void OnTrajectoryThickChanged(double thick);
	void OnResetTrajectoryDataPushButton();

	void OnBoxX0Changed(double X0);
	void OnBoxX1Changed(double X1);
	void OnBoxY0Changed(double Y0);
	void OnBoxY1Changed(double Y1);
	void OnBoxZ0Changed(double Z0);
	void OnBoxZ1Changed(double Z1);
	void OnBoxAutoPostion(bool checked);

    void OnClusterEvelope();
signals:
	void SetMouseAction(int action); 
	void SetSceneDistance(int range,int value);
	void SetAtomRadiu(int range, int value, bool all=false);
	void SetAtomShape(int shape, bool all);
	void SetVariableAtomRadiu(bool variable, bool all);
	void SetGeomStyleVectorVisible(bool show, int style, bool all);
	void SetGeomStyleTrajectyoryVisible(bool show, int style, bool all);
	void SetGeomStyleAtomVisible(bool show, int style, bool all);
	void SetTrajectoryThickChanged(double thick,bool all);
	void ChangeGraphQuality(int);
	void SelectVectorStyleLine(bool checked);
	void SelectVectorStyleSolid(bool checked);
    void OpenExtendedObject(VisAtomExtendObject*ptrObj, QString fname);

};

#endif // VISATOM2017_H
