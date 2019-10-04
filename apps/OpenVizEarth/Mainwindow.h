#ifndef MYCLASS_H
#define MYCLASS_H

#include "QModelIndex"
#include <QSettings>

#include "DC/DataType.h"
#include "MainWindowAction.h"


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


class MainWindow : public MainWindowAction
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~MainWindow();

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
