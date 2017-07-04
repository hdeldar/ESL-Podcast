#ifndef GLOSSARYMODEL_H
#define GLOSSARYMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include "IESL.h"

class GlossaryModel: public QAbstractListModel
{
    Q_OBJECT
public:
    GlossaryModel(QObject *parent = 0);
    ~GlossaryModel(){}

    enum { UserRole = Qt::UserRole, IdRole, PhraseRole, MeaningRole, ExampleRole, IdiomRole};
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const ;

protected:
    QHash<int, QByteArray> roleNames() const;
public:
        void setItemList(QList<ESLGlossary> list );
private:
    QList<ESLGlossary> m_filterList;
};

#endif // GLOSSARYMODEL_H
