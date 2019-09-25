//qt
#include "Mainwindow.h"
#include "QGridLayout"
#include "QFileDialog"
#include "QMessageBox"
#include "QProcess"
#include "QColorDialog"
#include "QApplication"
#include <QtWidgets/QDesktopWidget>
#include "QLocale"
#include "QAction"
#include "QMenuBar"
#include "QDockWidget"
#include "QTreeView"
#include "QTreeWidgetItem"
#include "QTreeWidget"
#include "QHeaderView"
#include "QMdiSubWindow"
#include "QMdiArea"
#include "QDebug"
#include "QtGui/QColor"
#include "QTableWidget"

#include <assert.h>

//osg
#include "osg/Node"

//subself
//DC
#include "DC/AppSettings.h"
#include "DC/ThreadPool.h"

//DCDB
#include "DCDb/ObjectLoader.h"
#include <osgDB/ReadFile>

//DCScene
#include "DCScene/scene/SceneView.h"
#include "DCScene/scene/SceneModel.h"

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osg/MatrixTransform>
#include <osgManipulator/TabBoxDragger>
#include <osgGA/StateSetManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/ReadFile>
#include <osgEarth/Map>
#include <osgEarth/MapNode>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarth/ImageLayer>
#include<osgEarthDrivers/gdal/GDALOptions>

#include<osgEarthDrivers/bing/BingOptions>
#include <osgEarth/Registry>

//Manager
#include "Manager/NodeTreeModel.h"
#include "Manager/NodePropertyWidget.h"

#include "NXDockWidget.h"
#include "NXDockWidgetTabBar.h"
#include "NXDockWidgetTabButton.h"

const int maxRecentlyOpenedFileNum = 10;

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: IWindow(parent, flags)
	, m_appName(PACKAGE_NAME)
	, m_version(PACKAGE_VERSION)
	, m_pMdiArea( new QMdiArea( this ) )
	, m_bgLoader(nullptr)
	, _dockWidget(nullptr)
{
	
}

MainWindow::~MainWindow()
{
	ThreadPool::getInstance()->stop();
	SaveSettings();
	if (m_bgLoader)
	{
		delete m_bgLoader;
		m_bgLoader = nullptr;
	}
	
}

void MainWindow::SetCentralWidget()
{
	setCentralWidget(m_pMdiArea);
}

void MainWindow::PraseArgs(QVector<QString > args)
{
	DC::SceneView* pNewViewer = CreateNewSceneViewer();
	
	if (pNewViewer)
	{
		if(args.size() > 1)
		{
			for(int i = 1; i != args.size(); ++i)
			{
				qDebug(args[i].toStdString().c_str());
				osg::Node* node1 = osgDB::readNodeFile( args[i].toStdString() );
				//! ���³�������
				pNewViewer->getModel()->setData( node1 );
			}
		}

		pNewViewer->resetHome();
	}
	
}

DC::SceneView* MainWindow::CreateNewSceneViewer()
{
	DC::SceneView* sceneView = new DC::SceneView(this);
	sceneView->setModel(new SceneModel(this));
	QMdiSubWindow* subWindow = m_pMdiArea->addSubWindow( sceneView );

	subWindow->showMaximized();

	return sceneView;
}

//! ���ص�ǰ����Ĵ���
QWidget* MainWindow::ActiveMdiChild()
{
	if (QMdiSubWindow *activeSubWindow = m_pMdiArea->activeSubWindow())
		return qobject_cast<QWidget *>(activeSubWindow->widget());

	return 0;
}

DC::SceneView* MainWindow::CurrentSceneView()
{
	DC::SceneView* pViewer = static_cast<DC::SceneView* >(ActiveMdiChild());

	return pViewer;
}

void MainWindow::CreateConnection()
{
	//file
	//connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(DoActionOpen()));

	//view


	//tool
	//connect(ui.actionQss1, SIGNAL(triggered()), this, SLOT(DoStyleSheet()));
	//connect(ui.actionQss2, SIGNAL(triggered()), this, SLOT(DoStyleSheet()));
	//connect(ui.actionQss3, SIGNAL(triggered()), this, SLOT(DoStyleSheet()));
	//connect(ui.actionQssDefault, SIGNAL(triggered()), this, SLOT(DoStyleSheet()));

	//help
	//connect(ui.actionOnline_Update, SIGNAL(triggered()), this, SLOT(DoOnlineUpdate()));
	
	//connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(DoAbout()));
}

void MainWindow::setOrAddSetting(const QString& key, const QVariant & value)
{
	if (value.isValid())
	{
		m_globalSettings.setValue(key, value);
		m_globalSettings.sync();
	}
}

QVariant MainWindow::getOrAddSetting(const QString& key, const QVariant & defaultValue)
{
	auto  found = m_globalSettings.value(key);

	if (!found.isValid())
	{
		if (defaultValue.isValid())
		{
			m_globalSettings.setValue(key, defaultValue);
			m_globalSettings.sync();
		}

		return defaultValue;
	}

	return found;
}


void MainWindow::initViewWidget()
{
	DC::SceneView* pNewViewer = CreateNewSceneViewer();
	if (pNewViewer)
	{
		/*osg::ref_ptr<osgEarth::Map> map = new osgEarth::Map;
		osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(map);
		osgEarth::Drivers::BingOptions bing;
		osgEarth::Drivers::GDALOptions gdal;
		osgEarth::Drivers::TMSOptions tms;
		tms.url() = "D:/data/world/tms.xml";
		gdal.url() = "H:\\osgearthSDK\\Data\\world.tif";
		map->addLayer(new osgEarth::ImageLayer("My", gdal));*/
		//osg::ref_ptr < osg::Node > mapNode = osgDB::readNodeFile("Resources\earth_files\geocentric.earth");
		//osg::ref_ptr<osgEarth::MapNode> mapNode = new osgEarth::MapNode(node.get() );


		QString  baseMapPath;
		QString  mode = getOrAddSetting("Base mode", "geocentric").toString();
		if (mode == "projected")
		{
			baseMapPath = QStringLiteral("Resources/earth_files/projected.earth");
		}
		else if (mode == "geocentric")
		{
			baseMapPath = QStringLiteral("Resources/earth_files/geocentric.earth");
		}
		else
		{
			QMessageBox::warning(nullptr, "Warning", "Base map settings corrupted, reset to projected");
			//_settingsManager->setOrAddSetting("Base mode", "projected");
			baseMapPath = QStringLiteral("resources/earth_files/projected.earth");
		}
		osg::ref_ptr<osgDB::Options>  myReadOptions = osgEarth::Registry::cloneOrCreateOptions(0);
		osgEarth::Config              c;
		c.add("elevation_smoothing", false);
		osgEarth::TerrainOptions  to(c);
		osgEarth::MapNodeOptions  defMNO;
		defMNO.setTerrainOptions(to);

		myReadOptions->setPluginStringData("osgEarth.defaultOptions", defMNO.getConfig().toJSON());

		osg::Node *baseMap = osgDB::readNodeFile(baseMapPath.toStdString(), myReadOptions);


	   //! ���³�������
		pNewViewer->getModel()->setData(baseMap);

		pNewViewer->resetHome();
	}
}

void MainWindow::InitManager()
{
	//Ϊmanager��װtreeWidget������widget
	QDockWidget* dokwObjects = new QDockWidget(this);
	dokwObjects->setWindowTitle(QString::fromLocal8Bit("����") );
	dokwObjects->setObjectName(QString::fromUtf8("dokwObjects"));
	dokwObjects->setEnabled(true);
	dokwObjects->setMinimumSize(QSize(200, 315));
	dokwObjects->setMaximumSize(QSize(400, 524287));
	dokwObjects->setLayoutDirection(Qt::LeftToRight);
	dokwObjects->setStyleSheet(QString::fromUtf8("border-color: rgb(85, 255, 255);\n"
		"gridline-color: rgb(85, 85, 255);"));
	dokwObjects->setFloating(false);
	dokwObjects->setFeatures(QDockWidget::AllDockWidgetFeatures);
	dokwObjects->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);


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

	//ΪtreeView���ģ��
	m_nodeTreeModel = new NodeTreeModel();
	objTreeView->setModel(m_nodeTreeModel);

	//�źŲ�
	connect( objTreeView, SIGNAL( clicked ( const QModelIndex &) ), this, SLOT( NodeSelected(const QModelIndex & ) ) );

	layoutObject->addWidget(objTreeView, 0, 0, 1, 1);
	dokwObjects->setWidget(wgtContext);
	addDockWidget(static_cast<Qt::DockWidgetArea>(1), dokwObjects);


	//����widget
	//��װ����widget
	QDockWidget* dokwProperties = new QDockWidget(this);
	dokwProperties->setObjectName(QString::fromUtf8("dokwProperties"));
	dokwProperties->setMinimumSize(QSize(200, 193));
	dokwProperties->setMaximumSize(QSize(400, 524287));
	dokwProperties->setBaseSize(QSize(4, 0));
	dokwProperties->setFloating(false);
	dokwProperties->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

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
	addDockWidget(static_cast<Qt::DockWidgetArea>(1), dokwProperties);
	dokwProperties->setWindowTitle(QString::fromLocal8Bit("����"));
}


QList<NXDockWidget *>  MainWindow::getDockWidgetListAtArea(Qt::DockWidgetArea area)
{
	QList<NXDockWidget *>  dockWidgetList;

	for (NXDockWidget *dockWidget : _dockWidgets)
	{
		if ((dockWidget->getArea() == area) && (dockWidget->isDocked()))
		{
			dockWidgetList.push_back(dockWidget);
		}
	}

	return dockWidgetList;
}

void MainWindow::initUiStyles()
{
	auto a  = children();
	for (auto child : children())
	{
		NXDockWidget *dock = dynamic_cast<NXDockWidget *>(child);

		if (dock)
		{
			//dockWidgetUnpinned(dock);
			dock->setFixedWidth(250);
		}
	}
}


static Qt::ToolBarArea  dockAreaToToolBarArea(Qt::DockWidgetArea area)
{
	switch (area)
	{
	case Qt::LeftDockWidgetArea:

		return Qt::LeftToolBarArea;
	case Qt::RightDockWidgetArea:

		return Qt::RightToolBarArea;
	case Qt::TopDockWidgetArea:

		return Qt::TopToolBarArea;
	case Qt::BottomDockWidgetArea:

		return Qt::BottomToolBarArea;
	default:

		return Qt::ToolBarArea(0);
	}
}


NXDockWidgetTabBar * MainWindow::getDockWidgetBar(Qt::DockWidgetArea area)
{
	assert(_dockWidgetBar.find(area) != _dockWidgetBar.end());

	auto  it = _dockWidgetBar.find(area);

	if (it != _dockWidgetBar.end())
	{
		return *it;
	}

	return nullptr;
}

void MainWindow::LoadSettings()
{
	AppSettings settings;
	settings.clear();
	//! �����ڲ���
	settings.beginGroup("MainWindow");

	//! Qmainwindow����
	bool aa = restoreState(settings.value("MainWindowState").toByteArray(), 0);

	//! qwidget����
	resize( settings.value( "size", QSize(800, 600) ).toSize() );
	move( settings.value( "pos", QPoint(200, 200) ).toPoint() );

	bool fullScreen = settings.value("fullScreen",false).toBool();

	if (fullScreen)
		showFullScreen();

	settings.endGroup();


	////////////////////////////Application���//////////////////////////////////////////////
	settings.beginGroup("Application");

	//! ����¼·��
	m_lastDirectory = settings.value("lastDirectory","/home").toString();

	//! ������ļ��б�
	m_recentFiles = settings.value( "recentlyOpenedFiles").toStringList();
	m_inverseMouseWheel = settings.value("inverseMouseWheel", true).toBool();
	
	if (CurrentSceneView())
	{
		CurrentSceneView()->setEnabledInverseMouseWheel(m_inverseMouseWheel);
	}
	

	m_currentLanguage = settings.value("currentLanguage","").toString();

	if ( m_currentLanguage.isEmpty() )
		m_currentLanguage = QLocale::system().name().left(2);

	//! ����ɫ
	QColor color = settings.value( "bgcolor",QColor(50,50,50) ).value<QColor>();
	if (CurrentSceneView())
	{
		CurrentSceneView()->setBgColor(color);
	}
	

	settings.endGroup();
}

void MainWindow::SaveSettings()
{
	AppSettings settings;

	settings.beginGroup("MainWindow");

	settings.setValue( "MainWindowState",saveState(0) );

	settings.setValue( "size", size() );
	settings.setValue( "pos", pos() );

	settings.setValue( "fullScreen", isFullScreen() );

	// save the screen number for the splashsreeen ...
	if ( QApplication::desktop()->isVirtualDesktop() )
		settings.setValue( "screenNumber", QApplication::desktop()->screenNumber( pos() ) );
	else
		settings.setValue( "screenNumber", QApplication::desktop()->screenNumber(this) );

	settings.setValue( "screenNumber", QApplication::desktop()->screenNumber(this) );

	settings.endGroup();

	settings.beginGroup("Application");

	settings.setValue("lastDirectory", m_lastDirectory);

	settings.setValue("currentLanguage", m_currentLanguage);
	settings.setValue("inverseMouseWheel", m_inverseMouseWheel);

	// recent files
	settings.setValue("recentlyOpenedFiles", m_recentFiles);

	// scene background
	if (CurrentSceneView())
	{
		settings.setValue( "bgcolor", CurrentSceneView()->getBgColor() );
	}
	

	settings.endGroup();
}

void MainWindow::Init()
{
	//����һ����̨��д�߳�
	m_bgLoader = new ObjectLoader();
	m_bgLoader->moveToThread( ThreadPool::getInstance()->getThread() );
	connect( this,SIGNAL( NewFileToLoad(const QString &, QString ) ),m_bgLoader,SLOT( newObjectToLoad(const QString &, QString) ) );
	connect( m_bgLoader,SIGNAL( newObjectToView(osg::Node *, QString) ),this,SLOT( NewLoadedFile(osg::Node *, QString) ) );

	//�źŲ�����
	CreateConnection();

	//��ʼ����ť״̬
	EnableActions(false);
}

void MainWindow::AddRecentlyOpenedFile(const QString &filename, QStringList &filelist)
{
	QFileInfo fi(filename);

	if ( filelist.contains( fi.absoluteFilePath() ) )
		return;

	if ( filelist.count() >= maxRecentlyOpenedFileNum )
		filelist.removeLast();

	filelist.prepend( fi.absoluteFilePath() );
}


void MainWindow::EnableActions(const bool isEnable)
{
	QAction* actionLight = GetMenuBar()->findChild<QAction* >("actionLight");
	if (!actionLight)
	{
		return;
	}
	actionLight->setEnabled(isEnable);
	if (isEnable)
	{
		//actionLight->setCheckable(isEnable);
		actionLight->setIcon(QIcon(":/Mainwindow/Resources/tool/64/view_lightOn.png"));
	}
	else
	{
		actionLight->setIcon(QIcon(":/Mainwindow/Resources/tool/64/view_lightOff.png"));
	}
}





bool MainWindow::LoadFile(const QString &file, QString type)
{
	if ( file.isEmpty() || !QFileInfo(file).exists() )
		return false;

	//saveIfNeeded();
	QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

	m_currentFile = file;
	m_lastDirectory = QFileInfo(file).absolutePath();

	// enable actions
	//enableActions(false);

	emit NewFileToLoad(file, type);

	return true;
}

void MainWindow::ResetViews(bool allClear)
{
	//ui->widgetSceneView->resetSelection();

	// reset 3d view
	if (allClear)
	{
		//m_sceneModel->setData(NULL);
	}
}
