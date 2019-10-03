#ifndef MYCLASS_H
#define MYCLASS_H

#include "QModelIndex"

#include "DCGui/iwindow.h"

namespace DC
{
	class SceneView;
}
class OsgQWidget;
class QMdiArea;

class SceneModel;
class ObjectLoader;
class NodeTreeModel;
class NodePropertyWidget;

namespace osg
{
	class Node;
}

class MainWindow : public IWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~MainWindow();

public:
	bool LoadFile(const QString &file, QString type);

	//! ����һ������
	OsgQWidget* CreateNewSceneViewer();

	//! ���ص�ǰ����Ĵ���
	QWidget* ActiveMdiChild();

	//! ��ȡ��ǰ�����
	OsgQWidget* CurrentSceneView();


	//! �����ⲿ����Ĳ���
	void PraseArgs(QVector<QString > args);
private:
	void CreateConnection();

	void ResetViews(bool allClear);

	///���ö����Ŀ���״̬
	void EnableActions(const bool isEnable);

	void Init();

	//! ���һ��dockwidget����������QtreView,��������ڵ�
	void InitManager();

	//! ����������ò���
	void SaveSettings();

	//! ���ؼ�¼������
	void LoadSettings();

	//! �ļ���ȡ�ɹ�ʱ����ã�������¼�ļ�������ǰ�ļ���������б���
	void AddRecentlyOpenedFile(const QString &filename, QStringList &filelist);

signals:
	void NewFileToLoad(const QString &, QString type);

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

	/*void on_actionAdd_triggered();*/

	void on_actionSplitFiles_triggered();

	///////////////////////////////////////PointCloud///////////////////////////////
	void on_actionGridFilter_triggered();

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

private slots:
	void NodeSelected(const QModelIndex &index);
	
private:

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
};

#endif // MYCLASS_H
