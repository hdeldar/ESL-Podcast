#ifndef BLOGMODEL_H
#define BLOGMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include "IESL.h"

class BlogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BlogModel(QObject *parent = 0);
    ~BlogModel(){}

    enum { UserRole = Qt::UserRole, IdRole, TitleRole, DateRole, PostRole, ImageRole};
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const ;

protected:
    QHash<int, QByteArray> roleNames() const;
public:
        void setItemList(QList<ESLBlog> list );
private:
    QList<ESLBlog> m_filterList;

};

#endif // BLOGMODEL_H
