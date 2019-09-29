#include "ToolBoxTreeView.h"

//qt
#include "QTreeView"
#include "QModelIndex"
#include "QItemSelectionModel"
#include "QKeyEvent"

//osg
#include "osg/Node"

#include "ToolBoxTreeModel.h"


ToolBoxTreeView::ToolBoxTreeView(QWidget *parent)
	: QTreeView(parent)
	, m_pToolBoxTreeModel(nullptr)
{

}

ToolBoxTreeView::~ToolBoxTreeView()
{

}

void ToolBoxTreeView::SetModel(ToolBoxTreeModel* model)
{
	m_pToolBoxTreeModel = model;
}

void ToolBoxTreeView::DeleteSelectItems()
{
	//! ѡȡģ��
	QItemSelectionModel* qitemSelect = selectionModel();

	//! �Ƿ�ѡ����item
	QModelIndexList selectIndexes = qitemSelect->selectedIndexes();
	int selectItemsCount = selectIndexes.size();

	if (selectItemsCount == 0)
	{
		return;
	}


}

void ToolBoxTreeView::keyPressEvent(QKeyEvent *event)
{
	//�� ɾ����ǰѡ����Ŀ
	if (event->key() == Qt::Key_E)
	{
		DeleteSelectItems();
	}
}
