#ifndef QICONMODEMODEL_H
#define QICONMODEMODEL_H

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include "IESL.h"

class PodcastModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PodcastModel(QObject *parent = 0);
    ~PodcastModel(){}

    enum { Id, Category, Title, Comment, Year };
    enum { UserRole = Qt::UserRole, IdRole,TitleRole,CategoryRole,
           ScriptRole,YearRole,MonthRole,CultureNoteRole,CommentRole};
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    Qt::ItemFlags flags(const QModelIndex & index) const ;   
    void setRowCount(int row);
    void setFilterWord(const QString &filterWord);	

protected:
    QHash<int, QByteArray> roleNames() const;
public:
        void setItemList(QList<ESLPodcast> list );
private:
    QList<ESLPodcast> m_filterList;
    QString m_filterWord;

};

#endif // QICONMODEMODEL_H
