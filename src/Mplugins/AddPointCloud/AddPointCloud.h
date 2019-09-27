﻿#pragma once
#include <QtPlugin>
#include <DC/MPluginInterface.h>

QT_BEGIN_NAMESPACE
class QToolBar;
class QAction;
class QMenu;
QT_END_NAMESPACE

class AddPointCloud : public MPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "wly.OpenViz.MPluginInterface" FILE "AddPointCloud.json")
	Q_INTERFACES(MPluginInterface)

public:
	AddPointCloud();
	~AddPointCloud();
	virtual void setupUi(QToolBar *toolBar, QMenu *menu) override;

protected:
	void setupStyleTab();

protected slots:
	void addPointCloud();
	void setPointSize(int size);

protected:
	void loadPointCloudModel(const QString& fileName);

private:
	float _pointSize;
};
