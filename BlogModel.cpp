#include "BlogModel.h"
#include <QDebug>

const int ColumnCount = 1;

BlogModel::BlogModel(QObject *parent/* = 0*/)
        : QAbstractListModel(parent)
{

}

int BlogModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;
    return m_filterList.count();
}

int BlogModel::columnCount(const QModelIndex &parent) const
{
   return parent.isValid() && parent.column() != 0 ? 0 : ColumnCount;
}

QVariant BlogModel::data(const QModelIndex &index, int role) const
{
   int row = index.row();
   if (m_filterList.size() <= 0 || m_filterList.size() <= row || row < 0)
      return QVariant();

    ESLBlog item = m_filterList[index.row()];

    switch(role)
    {
    case IdRole:
        return item.id();
    case TitleRole:
        return item.title();
    case DateRole:
        return item.date();
    case  PostRole:
        return item.post();
    case  ImageRole:
        return item.image();
    }
    return QVariant();
}

Qt::ItemFlags BlogModel::flags(const QModelIndex &index) const
{
        Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
        if (index.isValid()) {
                theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        }
        return theFlags;
}

void BlogModel::setItemList( QList<ESLBlog> list )
{
    beginResetModel();
    m_filterList.clear();
    m_filterList = list;
    endResetModel();
}

QHash<int, QByteArray> BlogModel::roleNames() const
{
    QHash<int, QByteArray> m_roles;
    m_roles[IdRole] = "id";
    m_roles[TitleRole] = "title";
    m_roles[DateRole] = "date";
    m_roles[PostRole] = "post";
    m_roles[ImageRole] = "image";
    return m_roles;
}
