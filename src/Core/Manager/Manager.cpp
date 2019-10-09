#pragma execution_character_set("utf-8")

#include "Manager.h"

#include <QVector>
#include <QMenu>
#include <QMetaType>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QMainWindow>
#include <QSignalMapper>
#include <QLineEdit>
#include "QDockWidget"
#include "QTreeView"
#include "QTreeWidgetItem"
#include "QTreeWidget"
#include "QTextBrowser"
#include <osgSim/OverlayNode>
#include <osgEarth/GeoData>
#include <osgEarth/MapNode>

#include "DC/LogHandler.h"

#include "Manager/NodeTreeModel.h"
#include "Manager/NodePropertyWidget.h"

Manager::Manager(QObject* parent /*= NULL*/)
	: QObject()
	, _countLoadingData(0)
	, m_widgetStats(nullptr)
	, m_propertyWidget(nullptr)
{

}

Manager::~Manager()
{
}

void Manager::reset()
{
	
}

void Manager::InitDockWidget()
{
	//�� ��ʼ������̨
	initConsole();
	
	//�� ����tree dock�ڵ�������
	initDataTree();

	//�� �������������
	initToolBox();
}

void Manager::initConsole()
{
	//! ��������̨ui
	 // create the log handler
	connect(LogHandler::getInstance(), SIGNAL(newMessage(const QString &)), this, SLOT(printToLogConsole(const QString &)));
	connect(LogHandler::getInstance(), SIGNAL(newMessages(const QStringList &)), this, SLOT(printToLogLogConsole(const QStringList &)));
	LogHandler::getInstance()->startEmission(true); // start log emission

	QDockWidget* dokConsole = new QDockWidget(m_mainWindow);
	dokConsole->setObjectName(QString::fromUtf8("dokConsole"));
	dokConsole->setMinimumSize(QSize(200, 193));

	dokConsole->setBaseSize(QSize(4, 0));
	dokConsole->setFloating(false);

	QWidget* wgtConsole = new QWidget();
	wgtConsole->setObjectName(QString::fromUtf8("wgtConsole"));

	QGridLayout* layoutProperty = new QGridLayout(wgtConsole);
	layoutProperty->setSpacing(0);
	layoutProperty->setContentsMargins(0, 0, 0, 0);
	layoutProperty->setObjectName(QString::fromUtf8("layoutProperty"));

	m_textBrowserLog = new QTextBrowser(wgtConsole);
	m_textBrowserLog->setObjectName(QString::fromUtf8("textBrowserLog"));

	layoutProperty->addWidget(m_textBrowserLog, 0, 0, 1, 1);
	dokConsole->setWidget(wgtConsole);
	m_mainWindow->addDockWidget(Qt::BottomDockWidgetArea, dokConsole);
	dokConsole->setWindowTitle("����̨");
	// limit log display
	m_textBrowserLog->document()->setMaximumBlockCount(1000);

	//�� ���źŲ�����
}

//�� ������Ա����������еĲ˵�action������ʼ��
void Manager::setupUi(QMainWindow* mainWindow)
{
	//! ��¼������
	m_mainWindow = mainWindow;

	//!3. ����docket���
	InitDockWidget();

	//�� ��ʼ���źŲۼ�action
	// Context menu actions
	//showAttributeTableAction = new QAction(mainWindow);
	//showAttributeTableAction->setObjectName(QStringLiteral("showAttributeTableAction"));
	//showAttributeTableAction->setText(tr("Attributes"));
	//showAttributeTableAction->setToolTip(tr("Show attribute table"));

	//showMetatDataAction = new QAction(mainWindow);
	//showMetatDataAction->setObjectName(QStringLiteral("showMetatDataAction"));
	//showMetatDataAction->setText(tr("Meta Data"));
	//showMetatDataAction->setToolTip(tr("Show meta data"));

	showAllChildrenAction = new QAction(mainWindow);
	showAllChildrenAction->setObjectName(QStringLiteral("showAllChildrenAction"));
	showAllChildrenAction->setText(tr("Show"));
	showAllChildrenAction->setToolTip(tr("Show all children"));

	hideAllChildrenAction = new QAction(mainWindow);
	hideAllChildrenAction->setObjectName(QStringLiteral("hideAllChildrenAction"));
	hideAllChildrenAction->setText(tr("Hide"));
	hideAllChildrenAction->setToolTip(tr("Hide all children"));

	saveNodeAction = new QAction(mainWindow);
	saveNodeAction->setObjectName(QStringLiteral("saveNodeAction"));
	saveNodeAction->setText(tr("Save"));
	saveNodeAction->setToolTip(tr("Save node"));

	deleteNodeAction = new QAction(mainWindow);
	deleteNodeAction->setObjectName(QStringLiteral("deleteNodeAction"));
	deleteNodeAction->setText(tr("Delete"));
	deleteNodeAction->setToolTip(tr("Delete node"));

	
	
}

void Manager::initToolBox()
{

	if (!m_mainWindow)
	{
		return;
	}


	
}

void Manager::initDataTree()
{
	if (!m_mainWindow)
	{
		return;
	}

	_treeWidgetMenu = new QMenu(m_mainWindow);

	// Init dock
	
	//1Ϊmanager��װtreeWidget������widget
	QDockWidget* dokwObjects = new QDockWidget(m_mainWindow);
	dokwObjects->setWindowTitle("����");
	dokwObjects->setObjectName(QString::fromUtf8("dokwObjects"));
	dokwObjects->setEnabled(true);
	dokwObjects->setMinimumSize(QSize(200, 315));
	dokwObjects->setMaximumSize(QSize(400, 524287));
	dokwObjects->setLayoutDirection(Qt::LeftToRight);
	dokwObjects->setStyleSheet(QString::fromUtf8("border-color: rgb(85, 255, 255);\n"
		"gridline-color: rgb(85, 85, 255);"));
	dokwObjects->setFloating(false);
	dokwObjects->setFeatures(QDockWidget::AllDockWidgetFeatures);
	dokwObjects->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);


	QWidget* wgtContext = new QWidget();
	wgtContext->setObjectName(QString::fromUtf8("wgtContext"));
	//�������ֹ�����
	QGridLayout* layoutObject = new QGridLayout(wgtContext);
	layoutObject->setSpacing(0);
	layoutObject->setContentsMargins(0, 0, 0, 0);
	layoutObject->setObjectName(QString::fromUtf8("layoutObject"));

	//���qtreeview
	QTreeView* objTreeView = new QTreeView(wgtContext);
	objTreeView->setObjectName("objTreeView");
	objTreeView->setMinimumSize(QSize(200, 100));

	//! ѡȡģʽ֧��
	objTreeView->setSelectionMode(QAbstractItemView::ExtendedSelection);

	//ΪtreeView���ģ��
	m_nodeTreeModel = new NodeTreeModel();
	objTreeView->setModel(m_nodeTreeModel);

	//! ��Ӳ������ݣ�����ģ����ͼ�ṹ�Խڵ��չʾ
	//m_nodeTreeModel->setNode();

	//�źŲ�
	connect(objTreeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(NodeSelected(const QModelIndex &)));

	layoutObject->addWidget(objTreeView, 0, 0, 1, 1);
	dokwObjects->setWidget(wgtContext);
	m_mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dokwObjects);


	//22����widget
	//��װ����widget
	QDockWidget* dokwProperties = new QDockWidget(m_mainWindow);
	dokwProperties->setObjectName(QString::fromUtf8("dokwProperties"));
	dokwProperties->setMinimumSize(QSize(200, 193));
	dokwProperties->setMaximumSize(QSize(400, 524287));
	dokwProperties->setBaseSize(QSize(4, 0));
	dokwProperties->setFloating(false);
	dokwProperties->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	QWidget* wgtProperty = new QWidget();
	wgtProperty->setObjectName(QString::fromUtf8("wgtProperty"));

	QGridLayout* layoutProperty = new QGridLayout(wgtProperty);
	layoutProperty->setSpacing(0);
	layoutProperty->setContentsMargins(0, 0, 0, 0);
	layoutProperty->setObjectName(QString::fromUtf8("layoutProperty"));

	m_propertyWidget = new NodePropertyWidget(wgtProperty);
	m_propertyWidget->setObjectName(QString::fromUtf8("tblwProperties"));
	m_propertyWidget->setMinimumSize(QSize(200, 100));
	m_propertyWidget->setMaximumSize(QSize(400, 16777215));

	layoutProperty->addWidget(m_propertyWidget, 0, 0, 1, 1);
	dokwProperties->setWidget(wgtProperty);
	m_mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dokwProperties);
	dokwProperties->setWindowTitle("����");

	//�� 333��װͳ��
	QDockWidget* stateProperties = new QDockWidget(m_mainWindow);
	stateProperties->setObjectName(QString::fromUtf8("dokwstateProperties"));
	stateProperties->setMinimumSize(QSize(200, 193));
	stateProperties->setMaximumSize(QSize(400, 524287));
	stateProperties->setBaseSize(QSize(4, 0));
	stateProperties->setFloating(false);
	stateProperties->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	QWidget* wgtSstateProperty = new QWidget();
	wgtSstateProperty->setObjectName(QString::fromUtf8("wgtStateProperty"));

	QGridLayout* layoutStateProperty = new QGridLayout(wgtSstateProperty);
	layoutStateProperty->setSpacing(0);
	layoutStateProperty->setContentsMargins(0, 0, 0, 0);
	layoutStateProperty->setObjectName(QString::fromUtf8("layoutStateProperty"));

	m_widgetStats = new NodePropertyWidget(wgtSstateProperty);
	m_widgetStats->setObjectName(QString::fromUtf8("tblwStateProperties"));
	m_widgetStats->setMinimumSize(QSize(200, 100));
	m_widgetStats->setMaximumSize(QSize(400, 16777215));

	layoutStateProperty->addWidget(m_widgetStats, 0, 0, 1, 1);
	stateProperties->setWidget(wgtSstateProperty);
	m_mainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), stateProperties);
	stateProperties->setWindowTitle("ͳ��");
	

	
}

void Manager::NodeSelected(const QModelIndex &index)
{

	if (index.isValid())
	{
		//CurrentSceneView()->highlight( (osg::Node *)index.internalPointer() );
		// display stats
		//m_propertyWidget->displayProperties( (osg::Node *)index.internalPointer() );
		//(osg::Node *)index.internalPointer()->getNodeMask() == 0
		osg::Node* curSelect = (osg::Node *)index.internalPointer();
		//m_nodeTreeModel->setEnableIndex(index, curSelect->getNodeMask() == 0 ? true : false);
		
		//�� ��������
		m_propertyWidget->displayProperties((osg::Node *)index.internalPointer());
	}
}

void Manager::ChangeSelection(const QItemSelection & selected, const QItemSelection & deselected)
{
	//! ȡ��ѡ��
	QModelIndexList deselectedItems = deselected.indexes();
	{
		for (int i = 0; i < deselectedItems.count(); ++i)
		{
			osg::Node* element = static_cast<osg::Node*>(deselectedItems.at(i).internalPointer());
			if (element)
			{
				//element->SetSelected(false);
			}
		}
	}

	//ִ��ѡ��
	QModelIndexList selectedItems = selected.indexes();
	{
		for (int i = 0; i < selectedItems.count(); ++i)
		{
			osg::Node* element = static_cast<osg::Node*>(selectedItems.at(i).internalPointer());
			if (element)
			{
				//element->SetSelected(true);
				//element->PrepareDisplayForRefresh();
			}
		}
	}


	//! ��������
	//UpdateProperty();

	//QVector<osg::Node* > selEntities;
	//QItemSelectionModel* qism = _nodeTree->selectionModel();
	//QModelIndexList selectedIndexes = qism->selectedIndexes();
	//int i, selCount = selectedIndexes.size();

	//for (i = 0; i < selCount; ++i)
	//{
	//	osg::Node* anEntity = static_cast<osg::Node*>(selectedIndexes[i].internalPointer());
	//	if (anEntity)
	//		selEntities.push_back(anEntity);
	//}

	//emit SelectionChanged(selEntities);
}

void Manager::recordData(osg::Node* node, const QString& name, const QString& parent, bool hidden)
{
	//_nodeTree->addRecord(node, name, parent, hidden);
}

void Manager::recordData(osgEarth::Layer* layer, const QString& name, const QString& parent, osgEarth::GeoExtent* extent, bool hidden)
{
	//_nodeTree->addRecord(layer, name, parent, extent, hidden);
}

void Manager::CreateLayerContainerNode(QString layerNodeName)
{
	//DataRecord *parent = _nodeTree->getRecord(layerNodeName);

	//if (!parent)
	//{
	//	parent = new DataRecord(layerNodeName, _nodeTree->GetRootTree());
	//	_nodeTree->GetRootTree()->addChild(parent);
	//	_nodeTree->GetDataRecords().insert(layerNodeName, parent);
	//	parent->setExpanded(true);
	//}
	
}

void Manager::removeData(const QString& nodeName)
{
	//_nodeTree->removeRecord(nodeName);
}

void Manager::switchData(const QString& nodeName, bool checked)
{
	//_nodeTree->switchRecord(nodeName, checked);
}

void Manager::setWindowMask(const QString& nodeName, int mask)
{
	//_nodeTree->setWindowMask(nodeName, mask);
}

int Manager::getMask(const QString& nodeName)
{
	/*auto record = _nodeTree->getRecord(nodeName);
	if (record)
		return record->mask();
	else*/
		return 0x00000000;
}

//���󶨸�root�ڵ㣬�����������������
void Manager::registerDataRoots(osg::Group* root)
{
	if (m_nodeTreeModel)
	{
		m_nodeTreeModel->setNode(root);

		if (m_propertyWidget)
		{
			m_propertyWidget->displayProperties(root);
		}
		
		if (m_widgetStats)
		{
			m_widgetStats->displayProperties(root);
		}
		
	}

}

const osgEarth::GeoExtent* Manager::getExtent(const QString& name)
{
	return  NULL;
}


QList<QTreeWidgetItem*> Manager::getSelectedItems()
{
	QList<QTreeWidgetItem*> list;
	return list;
}

void Manager::newProject()
{
	reset();
}

void Manager::updateAttributeList(const QString& name, const QVector<attrib>& attribs)
{
	//_attributeLists[name] = attribs;
}

void Manager::updateFeatureTable(const QString& name, const QVector<feature>& features)
{
	//_featureTables[name] = features;
}

void Manager::updateFeatureFieldList(const QString& name, const QStringList& featureList)
{
	//_featureFieldList[name] = featureList;
}

void Manager::showMetaDataSlot()
{
	if (m_mainWindow) {
		return;
	}

	
}

void Manager::doubleClickTreeSlot(QTreeWidgetItem* item, int column)
{
	

}

//�� �ڵ���Ŀ������Ҽ��˵��¼�
void Manager::showDataTreeContextMenu(const QPoint &pos)
{
	
}


