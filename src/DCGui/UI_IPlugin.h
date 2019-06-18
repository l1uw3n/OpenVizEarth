//##########################################################################
//#                                                                        #
//#                            DCLW                                        #
//#                                                                        #
//#  This library is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU Lesser General Public License(LGPL) as  #
//#  published by the Free Software Foundation; version 2 of the License.  #
//#                                                                        #
//#  This library is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU Lesser General Public License for more details.                   #
//#                                                                        #
//#          COPYRIGHT: DCLW             								   #
//#                                                                        #
//##########################################################################

/*==========================================================================
*�����ܸ�����
*
*���ߣ�dclw         ʱ�䣺$time$
*�ļ�����$safeitemname$
*�汾��V2.1.0
*
*�޸��ߣ�          ʱ�䣺
*�޸�˵����
*===========================================================================
*/

#include "QObject"
#include "QMenuBar"
#include "QMenu"
#include "QAction"
#include "QToolBar"
#include "QMap"
#include "QApplication"

#include "QFile"
#include "QSqlDatabase"
#include "QDebug"
#include "QSqlError"
#include "QSqlQuery"
#include "QSqlRecord"

#include "ConfigParser.h"

#include "dcguiDLL.h"

class DCGUI_EXPORT UI_IPlugin : public QObject
{
	Q_OBJECT
public:
	//! ���������洢����������menu����
	typedef QMap<QString, QMenu* > MenuMap;
	MenuMap menuMap;

	//! ���������洢�����Ķ���menu������������
	MenuMap rootmenusMap;

	//! ���������洢����������toolBar����
	typedef QMap<QString, QToolBar* > ToolBarMap;
	ToolBarMap toolbarMap;

	//Qmainwindow
	QObject* pObject;

	UI_IPlugin(QObject* object)
	{
		pObject = object;
	}

	void CreateMenuBySqlite(QString cfgFile)
	{
		QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE"); 


		if (!QFile(cfgFile).exists())
		{
			return;
		}

		database.setDatabaseName(cfgFile);

		//�����ݿ�
		if(!database.open())
		{  
			qDebug()<<database.lastError();
			//qFatal("failed to connect.") ;
			return;
		}

		//�������ļ��л�ȡ�˵�������
		MenuVec mvec = ConfigParser::GetMenuVec();
		int menuNum = mvec.size();

		//i����һ���˵�����
		for (int i = 0; i != menuNum; ++i)
		{
			//������ò���Ϊ���أ�����ʾ
			if (!mvec[i].isShow)
			{
				continue;
			}

			QMenu* cm = new QMenu;
			menuMap[mvec[i].objectName] = cm;
			rootmenusMap[mvec[i].objectName] = cm;
			cm->setObjectName(mvec[i].objectName);
			cm->setTitle(mvec[i].title);

			//����źŲ�
			if (!mvec[i].eventName.isEmpty())
			{
				QObject::connect(cm, SIGNAL(aboutToShow()), pObject, 
					qFlagLocation( QString("%1%2()").arg(1).arg(mvec[i].eventName).toStdString().c_str() ));
			}

			//�Ƿ񴴽�������
			QToolBar* currentToolbar = nullptr;
			if (!mvec[i].toolbar.isEmpty())
			{
				currentToolbar = new QToolBar;
				currentToolbar->setObjectName(mvec[i].toolbar);
				currentToolbar->setWindowTitle(mvec[i].toolBarTitle);
				toolbarMap[mvec[i].objectName] = currentToolbar;
			}

			//Ϊ��ǰ�˵���Ӷ��������ݻ�ȡ��classID��action���л�ȡ���еĶ��������
			MenuActionVec actionVec = ConfigParser::GetMenuActionVec(mvec[i].classID);
			
			//j����һ���˵���action����
			for (int j = 0; j != actionVec.size(); ++j)
			{
				//���ж��Ƿ��ǲ˵�,�ǲ˵��ʹ����˵�������¼
				if (actionVec[j].isMenu)
				{
					//�����˵�
					//�ж��Ƿ񴴽��༶�˵�
					if (!actionVec[j].isShow)
					{
						continue;
					}

					//��ӵ����˵���,�Ȼ�ȡ���˵�,���˵���������continue
					if (menuMap.find(actionVec[j].parentMenu) == menuMap.end())
					{
						continue;
					}

					QMenu* sMenu = new QMenu;
					menuMap[actionVec[j].objectName] = sMenu;
					sMenu->setObjectName(actionVec[j].objectName);
					sMenu->setTitle(actionVec[j].text);

					//����źŲ�
					if (!actionVec[j].eventName.isEmpty())
					{
						QObject::connect(sMenu, SIGNAL(aboutToShow()), pObject, 
							qFlagLocation( QString("%1%2()").arg(1).arg(actionVec[j].eventName).toStdString().c_str() ));
					}

					
					QMenu* pMenu = menuMap[actionVec[j].parentMenu];
					//�ж�cactionǰ���Ƿ���ӷָ���
					if (actionVec[j].separator)
					{
						pMenu->addSeparator();
					}
					pMenu->addAction(sMenu->menuAction());
				}
				else //���ǲ˵��϶���action
				{
					//������ò���Ϊ���أ�����ʾ
					if (!actionVec[j].isShow)
					{
						continue;
					}

					//��ӵ����˵���,�Ȼ�ȡ���˵�,���˵���������continue
					if (menuMap.find(actionVec[j].parentMenu) == menuMap.end())
					{
						continue;
					}

					QAction* caction = new QAction(0);
					caction->setText(actionVec[j].text);
					caction->setObjectName(actionVec[j].objectName);

					//����źŲ�
					if (!actionVec[j].eventName.isEmpty())
					{
						QObject::connect(caction, SIGNAL(triggered()), pObject, 
							qFlagLocation( QString("%1%2()").arg(1).arg(actionVec[j].eventName).toStdString().c_str() ));
					}

					//����������
					if (actionVec[j].hasToolbar && currentToolbar)
					{
						caction->setToolTip(actionVec[j].tooltip);

						QString img = actionVec[j].toolImage;
						QString imgFile;
						if (img.at(0) != QLatin1Char(':'))
						{
							imgFile = QString("%1/%2").arg(QApplication::applicationDirPath()).arg(img);
						}
						else
						{
							imgFile = img;
						}
						QIcon icon1;
						icon1.addFile(imgFile, QSize(), QIcon::Normal, QIcon::Off);
						caction->setIcon(icon1);

						if (actionVec[j].separator)
						{
							currentToolbar->addSeparator();
						}

						currentToolbar->addAction(caction);
					}

					//��������
					caction->setEnabled(actionVec[j].enable);
					caction->setCheckable(actionVec[j].checkable);
					caction->setChecked(actionVec[j].checked);

					QMenu* pMenu = menuMap[actionVec[j].parentMenu];
					//�ж�cactionǰ���Ƿ���ӷָ���
					if (actionVec[j].separator)
					{
						pMenu->addSeparator();
					}
					
					pMenu->addAction(caction);
					caction->setParent(pMenu);
				}
				
			}
		}

		database.close();
	}

};