#include "QPodcastTableViewDelegate.h"
#include "PodcastModel.h"
#include <QPainter>
#include <QColor>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QPixmap>

const int Width = 150;
const int Height = 210;
const int GroupHeight = 30;

QPodcastTableViewDelegate::QPodcastTableViewDelegate(QObject *parent) :
    QStyledItemDelegate(parent)   
{	
    m_highlightStyle = "<span style='color:red;background-color:yellow;'>%1</span>";
    m_strItemHtml = "<body style='font-family:Times New Roman;font-size:17px;background-color:%2;'>%1</body>";
}


QPodcastTableViewDelegate::~QPodcastTableViewDelegate()
{

}

void QPodcastTableViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{ 
    QString tmpl = "";
    painter->save();
    QStyleOptionViewItemV4 options = option;

    QTextDocument doc;
    QString val = "";
    switch (index.column()) {
    case 0:
        val = index.data(PodcastModel::IdRole).toString();
        break;
    case 1:
    {
        int cat = index.data(PodcastModel::CategoryRole).toInt();
        if(cat)
            val = QString("<img src=':/resources/images/%1.jpg' width ='130' height = '66' />")
                .arg(cat);
        break;
    }
    case 2:
        val = index.data(PodcastModel::TitleRole).toString();
        break;
    case 3:
        val = index.data(PodcastModel::YearRole).toString();
        break;
    case 4:
        val = QString("%1").arg(index.data(PodcastModel::CommentRole).toString());
        break;
    }

    if(index.column() != 1 &&
            !m_filterText.isEmpty() &&
            val.indexOf(m_filterText,Qt::CaseInsensitive) > -1)
            val = val.replace(m_filterText, m_highlightStyle.arg(m_filterText) ,Qt::CaseInsensitive);

    tmpl = m_strItemHtml.arg(val);
    if(option.state & QStyle::State_Selected)
       tmpl = tmpl.arg("#abcdef");
    else if(index.row() % 2 == 0)
         tmpl = tmpl.arg("white");
    else
        tmpl = tmpl.arg("rgb(240,240,240)");
    doc.setHtml(tmpl);

    painter->translate(options.rect.left(), options.rect.top());
    QRect clip(0, 0, options.rect.width(), options.rect.height());
    doc.setTextWidth(options.rect.width());
    doc.drawContents(painter, clip);
    painter->restore();
  
}

void QPodcastTableViewDelegate::setFilterText( const QString &filterText )
{
    m_filterText = filterText;
}

QSize QPodcastTableViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column()) {
    case 0:
        return QSize(70, 50);
        break;
    case 1:
        return QSize(150, 100);
        break;
    case 2:
        return QSize(350, 50);
        break;
    case 3:
        return QSize(50, 50);
        break;    
    case 4:
        return QSize(500, 50);
        break;
    }
    return QSize(100, 50);
}


