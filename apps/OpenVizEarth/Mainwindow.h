#ifndef MYCLASS_H
#define MYCLASS_H

#include "QModelIndex"
#include <QSettings>

#include "MainWindowAction.h"

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

class MainWindow : public MainWindowAction
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~MainWindow();

	void setOrAddSetting(const QString& key, const QVariant& value);
	QVariant getOrAddSetting(const QString& key, const QVariant& defaultValue);

public:

	//! �����ⲿ����Ĳ���
	void PraseArgs(QVector<QString > args);

	void CreateConnection();

	void ResetViews(bool allClear);

	

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
protected:
	//virtual bool event(QEvent* event) override;



	
public:

	
};

#endif // MYCLASS_H
