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

//osg
#include "osg/Node"

//subself
#include "DlgClassify.h"
//DC
#include "DC/ThreadPool.h"
#include "DC/SettingsManager.h"

//DCDB
#include "DCDb/ObjectLoader.h"
#include <osgDB/ReadFile>

//DCScene
#include "DCScene/scene/SceneView.h"
#include "DCScene/scene/SceneModel.h"

//Manager
#include "Manager/NodeTreeModel.h"
#include "Manager/NodePropertyWidget.h"

const int maxRecentlyOpenedFileNum = 10;

void Test(osg::Node* node)
{
	osg::ref_ptr<osg::Node > node2(node);
}

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
	: IWindow(parent, flags)
	, m_appName(PACKAGE_NAME)
	, m_version(PACKAGE_VERSION)
	, m_pMdiArea( new QMdiArea( this ) )
	, m_bgLoader(nullptr)
{
	bool initState = ConfigInit(this);

	if (initState)
	{
		
		setCentralWidget( m_pMdiArea );

		Init();

		InitManager();

		LoadSettings();
	}

	ConfigFinish(this);

	osg::ref_ptr<osg::Node > node1(new osg::Node);

	Test(node1.get());
	node1->getBound();
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

void MainWindow::InitManager()
{
	//Ϊmanager��װtreeWidget������widget
	QDockWidget* dokwObjects = new QDockWidget(this);
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
	dokwProperties->setWindowTitle("����");
}

void MainWindow::NodeSelected(const QModelIndex &index)
{
	if (CurrentSceneView())
	{
		return;
	}

	if ( index.isValid() )
	{
		CurrentSceneView()->highlight( (osg::Node *)index.internalPointer() );

		// display stats
		m_propertyWidget->displayProperties( (osg::Node *)index.internalPointer() );
	}
}

void MainWindow::on_actionExit_triggered()
{
	close();
}

void MainWindow::LoadSettings()
{
	SettingsManager settings;

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
	SettingsManager settings;

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

void MainWindow::on_menuFile_aboutToShow()
{
	//! �����
	QMenu* recentfiles = menuBar()->findChild<QMenu* >("RecentlyOpenedFiles");
	recentfiles->clear();

	//! �ж��Ƿ����m_recentFiles
	if (m_recentFiles.count() > 0)
	{
		recentfiles->setEnabled(true);
		QStringList::Iterator it = m_recentFiles.begin();

		for (; it != m_recentFiles.end(); ++it)
		{
			recentfiles->addAction(*it);
		}
	}
	else
	{
		recentfiles->setEnabled(false);
	}
}

void MainWindow::on_RecentlyOpenedFiles_triggered(QAction *action)
{
	if ( !action->text().isEmpty() )
	{
		LoadFile( action->text() , "LOAD");
	}
}

void MainWindow::on_actionFullScreen_triggered(bool val)
{
	if (val)
	{
		setWindowState(windowState() | Qt::WindowFullScreen);
		//hideDockWidgets();
	}
	else
	{
		setWindowState(windowState() & ~Qt::WindowFullScreen);
		//showDockWidgets();
	}
}

void MainWindow::on_actionLight_triggered(bool val)
{
	//��ǰ����
	QObject* object = sender();
	if (!object)
	{
		return;
	}
	//ת��Ϊaction���
	QAction* action = static_cast<QAction*>(object);
	
	if (val)
	{
		action->setIcon(QIcon(":/Mainwindow/Resources/tool/64/view_lightOn.png"));
	}
	else
	{
		action->setIcon(QIcon(":/Mainwindow/Resources/tool/64/view_lightOff.png"));
	}

	if (CurrentSceneView())
	{
		CurrentSceneView()->setLightingEnabled(val);
	}
	
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

void MainWindow::on_actionColorGradient_triggered(bool val)
{
	if (CurrentSceneView())
	{
		CurrentSceneView()->setFlatBackgroundColor(!val);
	}
	
}

void MainWindow::on_actionBGColor_triggered()
{
	if (!CurrentSceneView())
	{
		return;
	}
	QColor currColor = CurrentSceneView()->getBgColor();
	QColor c = QColorDialog::getColor(currColor);

	if ( c.isValid() )
		CurrentSceneView()->setBgColor(c);
}

void MainWindow::on_actionOnline_Update_triggered()
{
	//ִ�������װ·���µ�ά������exe����
	QString toolFileName = QString("%1/%2").arg(QApplication::applicationDirPath()).arg("maintenancetool.exe");

	//����tool
	QProcess::startDetached(toolFileName, QStringList(toolFileName));
}

void MainWindow::on_actionAbout_triggered()
{
	QString strAbout = tr("��Ȩ����!\n"
		"�汾:%1\n"
		"��˾:������ά\n").arg(m_version);
	QMessageBox::about(this, tr("About"), strAbout);
}

void MainWindow::on_actionQss3_triggered()
{
	DoStyleSheet(":/Qss/Resources/qss/qss3.qss");
}

void MainWindow::on_actionQssDefault_triggered()
{
	DoStyleSheet(":/Qss/Resources/qss/stylesheet.qss");
}

void MainWindow::on_actionQss2_triggered()
{
	QObject* object = sender();
	if (!object)
	{
		return;
	}
	//ת��Ϊaction���
	QAction* qssAction = static_cast<QAction*>(object);

	if (qssAction->objectName() == "actionQss2")
	{
		DoStyleSheet(":/Qss/Resources/qss/qss2.qss");
	}
}

void MainWindow::on_actionQss1_triggered()
{
	QObject* object = sender();
	if (!object)
	{
		return;
	}
	//ת��Ϊaction���
	QAction* qssAction = static_cast<QAction*>(object);

	if (qssAction->objectName() == "actionQss1")
	{
		DoStyleSheet(":/Qss/Resources/qss/qss1.qss");
	}
	/*else if (qssAction->objectName() == "actionQss2")
	{
		DoStyleSheet(":/Qss/Resources/qss/qss2.qss");
	}
	else if (qssAction->objectName() == "actionQss3")
	{
		DoStyleSheet(":/Qss/Resources/qss/qss3.qss");
	}
	else
	{
		DoStyleSheet(":/Qss/Resources/qss/stylesheet.qss");
	}*/
}

void MainWindow::DoStyleSheet(QString qssFilename)
{
	//������ʽ��
	QFile qssFile(qssFilename);
	qssFile.open(QFile::ReadOnly);

	if(qssFile.isOpen())  

	{  

		QString qss = QLatin1String(qssFile.readAll());  

		qApp->setStyleSheet(qss);  

		qssFile.close();  
	}  
}

void MainWindow::on_actionOpen_triggered()
{
	QString filters = "Files All files (*.*);;(*.ply *.osg *.obj *.txt *.3ds *.stl *.s3c *.osgb)";

	QString file = QFileDialog::getOpenFileName(
		this,
		"ѡ����ļ�",
		m_lastDirectory,
		filters);

	LoadFile(file, "LOAD");
}

//void MainWindow::on_actionAdd_triggered()
//{
//	QString filters = "Files (*.ply *.osg *.obj *.txt *.3ds *.stl);;All files (*.*)";
//
//	QString file = QFileDialog::getOpenFileName(
//		this,
//		"ѡ����ļ�",
//		m_lastDirectory,
//		filters);
//
//	LoadFile(file, "ADD");
//}

void MainWindow::on_actionSplitFiles_triggered()
{
	
}

void MainWindow::on_actionGridFilter_triggered()
{
	DlgClassify dlg;
	dlg.exec();
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

void MainWindow::NewLoadedFile(osg::Node *node, QString type)
{
	// reset the cursor
	QApplication::restoreOverrideCursor();

	if (!node)
	{
		return;
	}

	//! �ڵ�ǰ���������һ��ʵ��
	if(type == "LOAD")
	{
		//! ����һ��view���Ӵ���view���������ڵ�
		DC::SceneView* pNewViewer = CreateNewSceneViewer();
		if (pNewViewer)
		{
			//! ���³�������
			pNewViewer->getModel()->setData( node );

			pNewViewer->resetHome();
		}
	}
	else if(type == "ADD")
	{
		DC::SceneView* pNewViewer = CurrentSceneView();
		if (pNewViewer)
		{
			//! ���³�������
			pNewViewer->getModel()->setData( node );

			pNewViewer->resetHome();
		}
	}
	

	

	//! ����NodeTreeModel����,��������ݽṹ�� osg::node
	//m_nodeTreeModel->setNode(node);

	//! ���±���
	setWindowTitle( windowTitle() + " - " + QFileInfo(m_currentFile).fileName() );
	
	//! ����һ��������ļ�
	AddRecentlyOpenedFile(m_currentFile, m_recentFiles);

	// ���ù��
	QApplication::restoreOverrideCursor();

	//! ����actions״̬
	EnableActions(true);

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