﻿#pragma once
#include <QtPlugin>
#include <DC/MPluginInterface.h>

QT_BEGIN_NAMESPACE
class QToolBar;
class QAction;
class QMenu;
QT_END_NAMESPACE

namespace osgViewer {
	class View;
}

class MultiView : public MPluginInterface
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "wly.OpenViz.MPluginInterface" FILE "MultiView.json")
	Q_INTERFACES(MPluginInterface)

public:
	MultiView();
	~MultiView();
	virtual void setupUi(QToolBar *toolBar, QMenu *menu) override;
	virtual void loadContextMenu(QMenu* contextMenu, QTreeWidgetItem* selectedItem) override;

protected:
	virtual void toggle(bool checked) override;
	void initSubView();



public slots:
	void moveToWindow();
	
protected:
	QWidget* _subViewWidget;
	osg::ref_ptr<osgViewer::View> _subView;
	QAction* _action;
	QAction* showInWindow1Action;
	QAction* showInWindow2Action;
};
