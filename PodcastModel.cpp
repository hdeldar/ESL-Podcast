#include "PodcastModel.h"
#include <QDebug>
#include <QPixmap>


const int ColumnCount = 5;
const int Offset = 100;

PodcastModel::PodcastModel(QObject *parent/* = 0*/)
        : QAbstractListModel(parent)
{

}

int PodcastModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return 0;
    return m_filterList.count();
}

int PodcastModel::columnCount(const QModelIndex &parent) const
{
   return parent.isValid() && parent.column() != 0 ? 0 : ColumnCount;
}

QVariant PodcastModel::data(const QModelIndex &index, int role) const
{	
   int row = index.row();
   if (m_filterList.size() <= 0 || m_filterList.size() <= row || row < 0)
      return QVariant();

    ESLPodcast item = m_filterList[index.row()];
    QString highlight = "<span style='color:red;background-color:yellow;'><b>%1</b></span>";
    bool isFilter = true;
    if(m_filterWord == "strong" || m_filterWord == "div"
                        || m_filterWord == "p"
                        || m_filterWord == "br" || m_filterWord == "span"
                        || m_filterWord == "class"
                        || m_filterWord == "b" || m_filterWord == "a")
        isFilter = false;
	switch(role)
	{
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        switch (index.column()) {
        case Id: return item.id();
        case Title: return item.title();
        case Year: return item.year();
        case Comment: return item.comment();
//        default: Q_ASSERT(false);
        }
        break;
    }
    case IdRole:
        return item.id();
    case TitleRole:
    {
        QString title = item.title();
        if(!m_filterWord.isEmpty() && isFilter)
            title = title.replace(m_filterWord, highlight.arg(m_filterWord),Qt::CaseInsensitive);
        return title;
    }
    case ScriptRole:
    {
        QString script = item.script();

        //i found solution at url : http://www.fileformat.info/info/unicode/char/92/index.htm
        script = script.replace(QChar(0x0092), "'");
        script = script.replace(QChar(0x00C4), "");
        script = script.replace(QChar(0x00EE), "");
        script = script.replace(QChar(0x0082), " ");
        script = script.replace(QChar(0x0091), "");
        script = script.replace(QChar(0x0093), "");
        script = script.replace(QChar(0x0094), "");
        script = script.replace(QChar(0x0096), "");
        script = script.replace(QChar(0x0097), " ");

        if(!m_filterWord.isEmpty() && isFilter)
            script = script.replace(m_filterWord, highlight.arg(m_filterWord),Qt::CaseInsensitive);
        return script;
    }
    case YearRole:
        return item.year();
    case MonthRole:
        return item.month();
    case CommentRole:
    {
        QString comment = item.comment();
        if(!m_filterWord.isEmpty() && isFilter)
            comment = comment.replace(m_filterWord, highlight.arg(m_filterWord),Qt::CaseInsensitive);
        return comment;
    }
    case CultureNoteRole:
    {
        QString cultureNote = item.cultureNote();
        if(!m_filterWord.isEmpty() && isFilter)
            cultureNote = cultureNote.replace(m_filterWord, highlight.arg(m_filterWord),Qt::CaseInsensitive);
        return cultureNote;
    }
    case CategoryRole:
    {
        int cat = -1 ;
        if(item.categories().length() > 0)
            cat = item.categories()[0].id();
        return cat;
    }
	case Qt::TextAlignmentRole:
		return Qt::AlignLeft;
    case  Qt::ToolTipRole:
    {
        if(index.column() == Comment)
            return item.comment();
    }
    case Qt::SizeHintRole:
        return QSize(200,150);
    }

    return QVariant();
}

QVariant PodcastModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */ ) const
{   
    if (role != Qt::DisplayRole)
            return QVariant();
    if (orientation == Qt::Horizontal) {
        switch (section) {
            case Id: return tr("No");
            case Category: return tr("Category");
            case Title: return tr("Title");
            case Comment: return tr("Year");
            case Year: return tr("Comment");

        }
    }
    return QVariant();
}

Qt::ItemFlags PodcastModel::flags(const QModelIndex &index) const
{
        Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
        if (index.isValid()) {
                theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEnabled;
        }
        return theFlags;
}

void PodcastModel::setItemList( QList<ESLPodcast> list )
{
    beginResetModel();
    m_filterList.clear();
    m_filterList = list;
    endResetModel();
}

QHash<int, QByteArray> PodcastModel::roleNames() const
{
    QHash<int, QByteArray> m_roles;
    m_roles[IdRole] = "id";
    m_roles[CategoryRole] = "category";
    m_roles[TitleRole] = "title";
    m_roles[ScriptRole] = "script";
    m_roles[YearRole] = "year";
    m_roles[MonthRole] = "month";
    m_roles[CultureNoteRole] = "cultureNote";
    m_roles[CommentRole] = "comment";
    return m_roles;
}


void PodcastModel::setFilterWord(const QString &filterWord)
{
    m_filterWord = filterWord;
}
