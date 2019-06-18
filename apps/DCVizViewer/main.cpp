
//Qt
#include <QtWidgets/QApplication>
#include "QFile"
#include "QTextCodec"
#include "QTranslator"

//osg
#include <osgDB/Registry>

#include "Mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	osgDB::Registry::instance()->getDataFilePathList().push_back( QString(qApp->applicationDirPath() + "/Resources/data/").toStdString () );

	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));

	//�������Է�����
	QTranslator pTranslator;
	//���������ļ�
	pTranslator.load(":/translate/Resources/languages/zh_cn/qt_zh_CN.lng");
	//Ӧ�ó���װ���Է�����
	a.installTranslator(&pTranslator);

	//������ʽ��
	QFile qssFile(":/Qss/Resources/qss/stylesheet.qss");  

	qssFile.open(QFile::ReadOnly);  

	if(qssFile.isOpen())  

	{  

		QString qss = QLatin1String(qssFile.readAll());  

		qApp->setStyleSheet(qss);  

		qssFile.close();  
	}  

	//! �����в�������
	int argc_ = argc;
    QVector<QByteArray> data;
    QVector<QString > argv_;

    // get the command line arguments as unicode string
    {
        QStringList args = a.arguments();
        for (QStringList::iterator it = args.begin(); it != args.end(); ++it) {
            argv_.push_back(*it);
        }
        //argv_.push_back(0); // 0-terminated string
    }

	MainWindow w;
	
	w.showMaximized();
	if(argv_.size() > 1)
	{
		w.PraseArgs(argv_);
	}
	
	return a.exec();
}
