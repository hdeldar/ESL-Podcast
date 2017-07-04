#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include "IESL.h"

class CategoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit CategoryModel(QObject *parent = 0);
    ~CategoryModel(){}

    enum { UserRole = Qt::UserRole, IdRole,NameRole, DescriptionRole};
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const ;

protected:
    QHash<int, QByteArray> roleNames() const;
public:
        void setItemList(QList<ESLCategory> list );
private:
    QList<ESLCategory> m_filterList;
    
};

#endif // CATEGORYMODEL_H
