//Qt
#include "QApplication"
#include "QSplashScreen"
#include "QTime"
#include "QLabel"
#include "QTextCodec"
#include "QSettings"
#include <QtWidgets/QDesktopWidget>
#include "QRegExp"
#include "QDebug"
#include "QDir"

#include "UI_IWindow.h"
#include "Iwindow.h"
#include "ConfigParser.h"

IWindow::IWindow(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
	, m_pUI(nullptr)
{
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));

	m_appDir = QCoreApplication::applicationDirPath() + "/";
}

IWindow::~IWindow()
{
	SaveSettings();
}

int IWindow::GetRootMenuNum() const
{
	QRegExp re("^menu");
	int num = menuBar()->findChildren<QMenu* >(re).size();

	for (int i = 0; i != num; ++i)
	{
		qDebug() << menuBar()->findChildren<QMenu* >(re)[i]->objectName();
	}

	return num;
}

QAction* IWindow::GetAction(const int ID)
{
	if (ID < GetRootMenuNum())
	{
		QRegExp re("^menu");
		QMenu* menu = menuBar()->findChildren<QMenu* >(re)[ID];

		Q_ASSERT(menu);
		QAction* action = menu->menuAction();
		return action;
	}

	return nullptr;
}

void IWindow::LoadSettings()
{
	QSettings settings;

	//! �����ڲ���
	settings.beginGroup("DCIWindow");

	//! Qmainwindow����
	restoreState(settings.value("MainWindowState").toByteArray(), 0);

	//! qwidget����
	resize( settings.value( "size", QSize(800, 600) ).toSize() );
	move( settings.value( "pos", QPoint(200, 200) ).toPoint() );

	bool fullScreen = settings.value("fullScreen",false).toBool();

	if (fullScreen)
		showFullScreen();

	settings.endGroup();
}

void IWindow::SaveSettings()
{
	QSettings settings;

	settings.beginGroup("DCIWindow");

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
}

bool IWindow::ConfigInit(QMainWindow* pInter)
{
	m_pUI = new UI_IWindow(pInter);

	//����sqlite�����ļ����ݿ⣬�����ļ������ݿ����Ʋ���appname
	QString appname = QApplication::applicationName();
	QString databaseName = m_appDir + "Config/" + appname;
	bool cfgState = ConfigParser::ConnectDatabase(databaseName);

	if (cfgState)
	{
		//1.���splash��Ӧ
		ConfigSplash();

		//2.���������ļ�����UI
		ConfigUI();
	}

	return cfgState;

	//QMetaObject::connectSlotsByName(pInter);
}

void IWindow::ConfigFinish(QMainWindow* pInter)
{
	QMetaObject::connectSlotsByName(pInter);
	LoadSettings();
}

QMenuBar* IWindow::GetMenuBar() const
{
	return m_pUI->menuBar;
}

void IWindow::ConfigSplash()
{
	//��ȡinfo
	int splashID = ConfigParser::GetSplashIDStarted();
	SplashInfo info = ConfigParser::GetSplashInfo(splashID);

	if (info.fileName.isEmpty())
	{
		return;
	}
	//������������()
	QSplashScreen* splash = new QSplashScreen();
	QTime time;
	time.start();
	int start = time.elapsed() / 1000;

	//�����������ļ�֧��qrc��ȫ·��
	QString startFile;
	if (info.fileName.at(0) != QLatin1Char(':'))
	{
		startFile = QString("%1%2").arg(m_appDir).arg(info.fileName);
	}
	else
	{
		startFile = info.fileName;
	}

	splash->setPixmap(QPixmap(startFile));

	//�����������ȫ��
	QString title = ConfigParser::GetGlobalTitle(false);
	QLabel* labelFull = new QLabel(title, splash);
	labelFull->setGeometry(25,98,440,55);
	labelFull->setStyleSheet("color: rgb(255, 255, 255);font: 75 18pt '΢���ź�'");
	labelFull->setAttribute(Qt::WA_TranslucentBackground, true);

	//������ + �汾��
	QString softName = ConfigParser::GetSoftName(true);
	QLabel* label = new QLabel(softName, splash);
	label->setGeometry(280,160,160,20);
	label->setStyleSheet("color: rgb(250, 255, 242);font: 12pt '����'");
	label->setAttribute(Qt::WA_TranslucentBackground, true);

	//��ʾ��������()
	splash->show();
	//������ʾ��Ϣ()ruan
	splash->showMessage(info.showInfo, Qt::AlignBottom | Qt::AlignRight, Qt::yellow);

	//����������ʾһ����
	while (time.elapsed()/1000 - start < info.time)
	{
		splash->raise();
	}

	//ɾ����������
	delete splash;
	splash =nullptr;
}

void IWindow::ConfigUI()
{
	//���ñ�������title�� logo
	ConfigTitleBar();

	//���ò˵���
	ConfigMenu();
}

void IWindow::ConfigTitleBar()
{
	QString title = ConfigParser::GetGlobalTitle(true, true);
	setWindowTitle(title);

	//����logo  //:/logo/Resources/logo/DC.ico
	//�ж�logo���Ƿ�����������������qrc·�����������������Ϻ�ľ���·����
	QString logo = ConfigParser::GetLogoFile();
	QString logoFile;

	Q_ASSERT(!logo.isEmpty());
	if (logo.at(0) != QLatin1Char(':'))
	{
		logoFile = QString("%1%2").arg(m_appDir).arg(logo);
	}
	else
	{
		logoFile = logo;
	}

	setWindowIcon(QIcon(logoFile));
}

void IWindow::ConfigMenu()
{
	m_pUI->CreateMenuBySqlite();
}

//void IWindow::on_actionOpen_triggered()
//{
//	auto aa = 9;
//}