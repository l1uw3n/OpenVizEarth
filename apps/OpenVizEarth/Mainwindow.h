#ifndef MYCLASS_H
#define MYCLASS_H

#include "QModelIndex"
#include <QSettings>
#include "DCGui/iwindow.h"

namespace DC
{
	class SceneView;
}

class QMdiArea;

class SceneModel;
class ObjectLoader;
class NodeTreeModel;
class NodePropertyWidget;


class NXDockWidget;
class NXDockWidgetTabBar;

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
	DC::SceneView* CreateNewSceneViewer();

	//! ���ص�ǰ����Ĵ���
	QWidget* ActiveMdiChild();

	//! ��ȡ��ǰ�����
	DC::SceneView* CurrentSceneView();


	//! �����ⲿ����Ĳ���
	void PraseArgs(QVector<QString > args);

	void CreateConnection();

	void ResetViews(bool allClear);

	///���ö����Ŀ���״̬
	void EnableActions(const bool isEnable);

	void Init();

	void initViewWidget();

	void SetCentralWidget();

	//! ���һ��dockwidget����������QtreView,��������ڵ�
	//! �ϰ汾
	void InitManager();

	//! ���һ��dockwidget����������QtreView,��������ڵ�
	//! �°汾
	void InitDockWidget();

	//! ����������ò���
	void SaveSettings();

	//! ���ؼ�¼������
	void LoadSettings();

	//! �ļ���ȡ�ɹ�ʱ����ã�������¼�ļ�������ǰ�ļ���������б���
	void AddRecentlyOpenedFile(const QString &filename, QStringList &filelist);


	//! dockWidget��������==========================================================
	// Add an auto-hide dock widget
	void AddDockWidget(Qt::DockWidgetArea area, NXDockWidget* dockWidget);

	// Add an auto-hide dock widget
	void AddDockWidget(Qt::DockWidgetArea area, NXDockWidget* dockWidget, Qt::Orientation orientation);

	void removeDockWidget(NXDockWidget* dockWidget);

	

	QList<NXDockWidget*> getDockWidgetListAtArea(Qt::DockWidgetArea area);

	

	

	NXDockWidgetTabBar* getDockWidgetBar(Qt::DockWidgetArea area);

	//UI style
	void initUiStyles();

	void setOrAddSetting(const QString& key, const QVariant& value);
	QVariant getOrAddSetting(const QString& key, const QVariant& defaultValue);
protected:
	virtual bool event(QEvent* event) override;

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

	//�� docket
	// Turn on the AutoHide option 
	void dockWidgetPinned(NXDockWidget* dockWidget);

	// Turn off the AutoHide option 
	void dockWidgetUnpinned(NXDockWidget* dockWidget);

	// DockWidget has been docked
	void dockWidgetDocked(NXDockWidget* dockWidget);

	// DockWidget has been undocked
	void dockWidgetUndocked(NXDockWidget* dockWidget);

	void createDockWidgetBar(Qt::DockWidgetArea area);

	void showDockWidget(NXDockWidget* dockWidget);
	void hideDockWidget(NXDockWidget* dockWidget);

	QRect getDockWidgetsAreaRect();

	void adjustDockWidget(NXDockWidget* dockWidget);
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

	//! docket
	// Current active(slide out) dockwidget or null
	NXDockWidget* _dockWidget;

	// List of all created dockwidgets
	QList<NXDockWidget*> _dockWidgets;

	// List of 4 dock tabbars
	QMap<Qt::DockWidgetArea, NXDockWidgetTabBar*> _dockWidgetBar;

	QMenu* _treeWidgetMenu;

	//setting
	QSettings m_globalSettings;
};

#endif // MYCLASS_H
