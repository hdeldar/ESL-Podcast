#include "GlossaryModel.h"
#include <QDebug>

const int ColumnCount = 1;

GlossaryModel::GlossaryModel(QObject *parent/* = 0*/)
        : QAbstractListModel(parent)
{

}

int GlossaryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;
    return m_filterList.count();
}

int GlossaryModel::columnCount(const QModelIndex &parent) const
{
   return parent.isValid() && parent.column() != 0 ? 0 : ColumnCount;
}

QVariant GlossaryModel::data(const QModelIndex &index, int role) const
{
   int row = index.row();
   if (m_filterList.size() <= 0 || m_filterList.size() <= row || row < 0)
      return QVariant();

    ESLGlossary item = m_filterList[index.row()];
    switch(role)
    {
    case IdRole:
        return item.id();
    case PhraseRole:
        return item.phrase();
    case MeaningRole:
        return item.meaning();
    case ExampleRole:
        return item.example();
    case IdiomRole:
        return item.idiom();
    }
    return QVariant();
}

Qt::ItemFlags GlossaryModel::flags(const QModelIndex &index) const
{
        Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
        if (index.isValid()) {
                theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        }
        return theFlags;
}

void GlossaryModel::setItemList( QList<ESLGlossary> list )
{
    beginResetModel();
    m_filterList.clear();
    m_filterList = list;
    endResetModel();
}

QHash<int, QByteArray> GlossaryModel::roleNames() const
{
    QHash<int, QByteArray> m_roles;
    m_roles[IdRole] = "id";
    m_roles[PhraseRole] = "phrase";
    m_roles[MeaningRole] = "meaning";
    m_roles[ExampleRole] = "example";
    m_roles[IdiomRole] = "idiom";
    return m_roles;
}
