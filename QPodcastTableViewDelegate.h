#ifndef QPodcastTableViewDelegate_H
#define QPodcastTableViewDelegate_H

#include <QStyledItemDelegate>

class QPodcastTableViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QPodcastTableViewDelegate(QObject *parent = 0);
        ~QPodcastTableViewDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const;
	void setFilterText(const QString &filterText);
    QSize sizeHint(const QStyleOptionViewItem &option,
            const QModelIndex &index) const;
private:
   QString m_filterText;
   QString m_highlightStyle;   
   QString m_strItemHtml;     
};

#endif // QPodcastTableViewDelegate_H
