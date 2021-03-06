
#ifndef NODE_TREEMODEL_H
#define NODE_TREEMODEL_H

#include "manager_global.h"

// Qt
#include <QtCore/QAbstractItemModel>

// OSG
#include <osg/ref_ptr>

//  Forward declarations

namespace osg
{
class Node;
class Object;
}

namespace osgSim {
	class Group;
	class OverlayNode;
}

namespace osgEarth {
	class Layer;
	class Map;
	class Layer;
}

namespace osgEarth {
	class GeoExtent;
	class Layer;
}

class QTreeView;
//  Class Declaration

class MANAGER_EXPORT NodeTreeModel :
    public QAbstractItemModel
{
    Q_OBJECT

public:

    enum
    {
        COL_NAME = 0,
        COL_TYPE,
        COL_DESCRIPTION,
        NB_COL
    };

    NodeTreeModel(QTreeView *parent = 0);
    virtual ~NodeTreeModel();

    void                setNode(osg::Node *node);
    osg::Node*          getNode();
    const osg::Node*    getNode() const;

    osg::Node*          getNode(const QModelIndex &index);

	void                clear(); //{ reset(); }

    Qt::ItemFlags       flags(const QModelIndex &index) const;

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex index(osg::Node* entity);
    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool hasChildren(const QModelIndex &parent) const
    { return rowCount(parent) > 0; }

    QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void setEnableIndex(const QModelIndex &index, bool);

    QModelIndex searchForNode( osg::Node *node,const QModelIndex &parent = QModelIndex() );
    QModelIndex searchForName( const QString &name, const QModelIndex &parent = QModelIndex() );

	osg::Node* NodeFromIndex(const QModelIndex &index) const;

	//! 更新checkState
	void UpdateCheckState(osg::Node* entity, const bool isCheck);

	//! 节点管理相关的接口
	void addRecord(osg::Node* node, const QString& name, const QString& parentName, bool hidden = false);
	void addRecord(osgEarth::Layer* layer, const QString& name, const QString& parentName, osgEarth::GeoExtent* extent = NULL, bool hidden = false);
	void removeRecord(const QString& name);

	//! 根据子实体对象(意味着这个传入的实体，必须有父对象)，插入到model中
	void InsertToModel(osg::Node* childEntity);
protected:

    inline osg::Node* getPrivateData(const QModelIndex &index) const { return reinterpret_cast<osg::Node*>( index.internalPointer() ); }

private:

    osg::ref_ptr<osg::Node> m_rootNode;
    QHash<QString, QIcon> m_hashIcon;

	//视图
	QTreeView* m_treeView;
};

#endif // _OSGTREEMODEL_H_
