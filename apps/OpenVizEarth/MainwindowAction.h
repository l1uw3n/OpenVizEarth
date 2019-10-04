#ifndef MYMainWindowAction_H
#define MYMainWindowAction_H

#include "QModelIndex"
#include <QSettings>
#include "DCGui/iwindow.h"
#include <osg/Object>
#include "DC/DataType.h"
class QMdiArea;
class QProgressBar;
class SceneModel;
class ObjectLoader;
class NodeTreeModel;
class NodePropertyWidget;


class NXDockWidget;
class NXDockWidgetTabBar;
class SettingsManager;

namespace osg
{
	class Node;
}

class MainWindowAction : public IWindow
{
	Q_OBJECT

public:
	MainWindowAction(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~MainWindowAction();

public:
	bool LoadFile(const QString &file, QString type);

	//! ��ȡ��ǰ�����
	OSGViewWidget* CurrentSceneView();

	//! ���ص�ǰ����Ĵ���
	QWidget* ActiveMdiChild();

	//! ����һ������
	OSGViewWidget* CreateNewSceneViewer();



	///���ö����Ŀ���״̬
	void EnableActions(const bool isEnable);

	//! �ļ���ȡ�ɹ�ʱ����ã�������¼�ļ�������ǰ�ļ���������б���
	void AddRecentlyOpenedFile(const QString &filename, QStringList &filelist);
public slots:

	/////////////////////////////////////File/////////////////////////////////////
	/** file
	 *��ȡ�ļ� */
	void on_actionOpen_triggered();

	//! �����ļ�  QString  type ["LOAD" | "ADD"]  ���´��ں�׷��
	void NewLoadedFile(osg::Node *, QString type);

	//! ����һ������򿪵��ļ�
	//void RecentFileActivated(QAction *action);

	//! ��ʼ��������ļ��˵�
	//void SetupRecentFilesMenu();

	void on_actionExit_triggered();

	void on_RecentlyOpenedFiles_triggered(QAction* );

	void on_menuFile_aboutToShow();

	void on_actionAdd_triggered();

	void on_actionSplitFiles_triggered();

	void SwitchMode();


	///////////////////////////////////////view///////////////////////////////////

	///����ɫ
	void on_actionBGColor_triggered();

	///�Ƿ�Ӧ�ý���
	void on_actionColorGradient_triggered(bool);
	
	void on_actionLight_triggered(bool val);

	void on_actionFullScreen_triggered(bool val);

	///tool \n
	///����qss�ļ�
	void DoStyleSheet(QString qssFilename);
	void on_actionQss1_triggered();
	void on_actionQss2_triggered();
	void on_actionQss3_triggered();
	void on_actionQssDefault_triggered();

	//help
	void on_actionOnline_Update_triggered();
	void on_actionAbout_triggered();

	void NodeSelected(const QModelIndex &index);

	//������
	// Progress bar loading done
	void loadingDone();

	// Progress bar loading next
	void loadingProgress(int percent);
	
signals:
	void NewFileToLoad(const QString &, QString type);

private:

public:
	//! ������������ģ����ͼ
	ObjectLoader *m_bgLoader;

	//! �ڵ����ģ����ͼ
	NodeTreeModel* m_nodeTreeModel;
	NodePropertyWidget* m_propertyWidget;

	//! ���ϵͳ������Ϣ
	QString m_lastDirectorySnapshot;
	QString m_lastSnapshotName;
	int m_currentSnapshotNum;

	QString m_appName;
	QString m_version;

	QString m_currentLanguage;

	QMdiArea* m_pMdiArea;


	////////////////////////////////�豸���//////////////////////////////////////////
	bool m_inverseMouseWheel;

	////////////////////////////////�ļ����//////////////////////////////////////////
	///����򿪵��ļ�
	QStringList m_recentFiles;

	//! ��ǰ�ļ�
	QString m_currentFile;

	//! �����ļ�·��
	QString m_lastDirectory;

	//setting
	QSettings m_globalSettings;

	osg::ref_ptr<OSGViewWidget>  m_pCurrentNewViewer;

	SettingsManager* m_SettingsManager;

	//! ����������
	QProgressBar* m_pProgressBar = NULL;
};

#endif // MainWindowAction
