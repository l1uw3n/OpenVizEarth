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

class DataManager;

namespace osg
{
	class Node;
}

namespace DC
{
	class SceneView;
}

class MainWindow : public IWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~MainWindow();

	void setOrAddSetting(const QString& key, const QVariant& value);
	QVariant getOrAddSetting(const QString& key, const QVariant& defaultValue);

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
	

	//! ����������ò���
	void SaveSettings();

	//! ���ؼ�¼������
	void LoadSettings();

	//! �ļ���ȡ�ɹ�ʱ����ã�������¼�ļ�������ǰ�ļ���������б���
	void AddRecentlyOpenedFile(const QString &filename, QStringList &filelist);


	//! dockWidget��������==========================================================
	

	

	

	

	QList<NXDockWidget*> getDockWidgetListAtArea(Qt::DockWidgetArea area);

	

	

	NXDockWidgetTabBar* getDockWidgetBar(Qt::DockWidgetArea area);

	//UI style
	void initUiStyles();
protected:
	//virtual bool event(QEvent* event) override;

signals:
	void NewFileToLoad(const QString &, QString type);

	
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

	DC::SceneView* m_pCurrentNewViewer;
};

#endif // MYCLASS_H
