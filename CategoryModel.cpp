#include "CategoryModel.h"
#include <QDebug>

const int ColumnCount = 1;

CategoryModel::CategoryModel(QObject *parent/* = 0*/)
        : QAbstractListModel(parent)
{

}

int CategoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;
    return m_filterList.count();
}

int CategoryModel::columnCount(const QModelIndex &parent) const
{
   return parent.isValid() && parent.column() != 0 ? 0 : ColumnCount;
}

QVariant CategoryModel::data(const QModelIndex &index, int role) const
{
   int row = index.row();
   if (m_filterList.size() <= 0 || m_filterList.size() <= row || row < 0)
      return QVariant();

    ESLCategory item = m_filterList[index.row()];

    switch(role)
    {
    case Qt::DisplayRole:
        return item.name();
    case IdRole:
        return item.id();
    case NameRole:
        return item.name();
    case DescriptionRole:
        return item.description();
    case  Qt::ToolTipRole:
        return item.description();
    }
    return QVariant();
}

Qt::ItemFlags CategoryModel::flags(const QModelIndex &index) const
{
        Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
        if (index.isValid()) {
                theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        }
        return theFlags;
}

void CategoryModel::setItemList( QList<ESLCategory> list )
{
    beginResetModel();
    m_filterList.clear();
    m_filterList = list;
    endResetModel();
}

QHash<int, QByteArray> CategoryModel::roleNames() const
{
    QHash<int, QByteArray> m_roles;
    m_roles[IdRole] = "id";
    m_roles[NameRole] = "name";
    m_roles[DescriptionRole] = "description";
    return m_roles;
}
