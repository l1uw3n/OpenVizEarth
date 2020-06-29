#pragma once

#pragma execution_character_set("utf-8")

//��������ı����������л�����ͳһ�ӿڵĿ⣬����
//�����л�ʹ�û���OsgQWidget����DCScene::SceneView
//��Ϊϵͳ����ҪviewWidget

#include "OsgQWidget/OsgQWidget.h"
//DCScene
#include "DCScene/scene/SceneView.h"
#include "DCScene/scene/SceneModel.h"

#include <ONodeManager/DataManager.h>
#include <Manager/Manager.h>

namespace DC
{
	class SceneView;
}

class OsgQWidget;
class DataManager;

//-------------------ȷ����Ŀʹ�õ�viewWidget��-------------------------------
typedef DC::SceneView OSGViewWidget;
//typedef OsgQWidget OSGViewWidget;

//-------------------ȷ����Ŀʹ�õĽڵ������manager-------------------------------
//typedef Manager UserDataManager;
typedef DataManager UserDataManager;
