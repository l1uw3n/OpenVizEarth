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
//#          COPYRIGHT: DCLW             																	 #
//#                                                                        #
//##########################################################################

/*==========================================================================
*�����ܸ�����IWindow��Ҫ���������Ϊ���࣬���ز��������ɲ˵�����������
������壻����̨�Լ������У����Դ��ڵ�
*
*���ߣ�dclw         ʱ�䣺$time$
*�ļ�����$safeitemname$
*�汾��V4.1.0
*
*�޸��ߣ�          ʱ�䣺
*�޸�˵����
*===========================================================================
*/

#ifndef IWINDOW_H
#define IWINDOW_H

//Qt
#include "QMainWindow"

#include "dcguiDLL.h"

class UI_IWindow;
class DCGUI_EXPORT IWindow : public QMainWindow
{
	Q_OBJECT
public:
	IWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~IWindow();

	/******************************�ӿ�****************************************/
	QMenuBar* GetMenuBar() const;

	bool ConfigInit(QMainWindow* pInter);
	void ConfigFinish(QMainWindow* pInter);

	//! ��ȡmenu��ͷ�Ĳ˵���Ŀ����
	int GetRootMenuNum() const;

	//! ��ȡmenu��ͷ�Ĳ˵���ָ��ID��Ӧ��action����������˵���ʱ����Ϊ����˵�λ��ʹ��
	QAction* GetAction(const int ID);
private:
	//��������
	void ConfigSplash();

	void ConfigUI();
	void ConfigTitleBar();
	void ConfigMenu();

	//����״̬
	void LoadSettings();
	void SaveSettings();

private slots:
	//void on_actionOpen_triggered();

private:
	/***********************************·������(dir�����Ķ��������ָ���)*****************************************************/
	QString m_appDir;

	/***********************************UI*************************************************/
	UI_IWindow* m_pUI;
};

#endif // IWINDOW_H
