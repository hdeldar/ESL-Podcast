#include "BinFileWindow.h"
#include "SqliteDataProvider.h"
#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QDateTime>

bool caseInsensitiveLessThan(const ESLBlog &p1, const ESLBlog &p2)
{
//    QHash<QString, int> monthHash;
//    monthHash["January"] = 1;
//    monthHash["February"] = 2;
//    monthHash["March"] = 3;
//    monthHash["April"] = 4;
//    monthHash["May"] = 5;
//    monthHash["June"] = 6;
//    monthHash["July"] = 7;
//    monthHash["August"] = 8;
//    monthHash["September"] = 9;
//    monthHash["October"] = 10;
//    monthHash["November"] = 10;
//    monthHash["December"] = 12;

//    QString date1 = p1.date();
//    QStringList listdate1 = date1.split("-", QString::SkipEmptyParts);
//    QStringList listdate2 = listdate1[1].split(",", QString::SkipEmptyParts);
//    QString year1 = listdate2[1];
//    QStringList listdate3 = listdate1[1].split(",", QString::SkipEmptyParts);
//    QString month1 = listdate3[0];
//    QString day1 = listdate3[1];



//    QString date2 = p2.date();
//    QStringList listdate12 = date2.split("-", QString::SkipEmptyParts);
//    QStringList listdate22 = listdate12[1].split(",", QString::SkipEmptyParts);
//    QString year2 = listdate22[1];
//    QStringList listdate32 = listdate12[1].split(",", QString::SkipEmptyParts);
//    QString month2 = listdate32[0];
//    QString day2 = listdate32[1];

//    date1 = qString("%1/%2/%3").arg(year1).arg(monthHash[month1]).arg(day1);
//    date2 = qString("%1/%2/%3").arg(year2).arg(monthHash[month2]).arg(day2);

    //"Tuesady - March 19, 2013"
    QString format = "dddd - MMMM d, yyyy";
    QDateTime dt1 = QDateTime::fromString(QString(p1.date()), format);
    QDateTime dt2 = QDateTime::fromString(QString(p2.date()), format);

    return dt1 > dt2;
}

BinFileWindow::BinFileWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pDataProvider(NULL)
{
  m_pDataProvider = new SqliteDataProvider("ESL");
  m_pDataProvider->setDbName(QCoreApplication::applicationDirPath() + "/db/ESL.db");
  m_pDataProvider->open();

  convert();
  serialize();
}

BinFileWindow::~BinFileWindow()
{
    delete m_pDataProvider;
}

void BinFileWindow :: convert()
{
    ESLCategory allCat;
    allCat.setName("All Categoreis");
    allCat.setId(-1);
    allCat.setDescription("all categories");
    m_categories.prepend(allCat);

    QString qry = "SELECT * FROM ESL_Category";
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
        ESLCategory cat;
        cat.setName(m_pDataProvider->getValue("name").toString().toUtf8());
        cat.setDescription(m_pDataProvider->getValue("description").toString().toUtf8());
        cat.setId(m_pDataProvider->getValue("id").toInt());
        m_categories.append(cat);
    }

    //=========================

    qry = "SELECT * FROM ESL_Podcast";
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
        ESLPodcast pod;
        pod.setTitle( m_pDataProvider->getValue("title").toString().toUtf8());
        pod.setId( m_pDataProvider->getValue("id").toInt());
        pod.setYear( m_pDataProvider->getValue("year").toInt());
        pod.setScript( m_pDataProvider->getValue("script").toString().toUtf8());
        pod.setCultureNote(m_pDataProvider->getValue("cultureNote").toString().toUtf8());
        QVariant vComment = m_pDataProvider->getValue("comment");
        if(vComment.isNull())
            pod.setComment("");
        else
            pod.setComment(vComment.toString().toUtf8());
        m_podcasts.append(pod);
    }

    qry = "SELECT * FROM ESL_PodcastCategory pc JOIN  ESL_Category c ON c.id = pc.category";
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
        int id =  m_pDataProvider->getValue("podcast").toInt();
        int catId =  m_pDataProvider->getValue("category").toInt();
        QString name =  m_pDataProvider->getValue("name").toString().toUtf8();
        QString description =  m_pDataProvider->getValue("description").toString().toUtf8();
        ESLCategory cat;
        cat.setId(catId);
        cat.setName(name);
        cat.setDescription( description);
        m_podCategories[id].append(cat);
    }

    qry = "SELECT * FROM ESL_Glossary";
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
        ESLGlossary gloss;
        int podcast =  m_pDataProvider->getValue("podcast").toInt() ;
        gloss.setId(m_pDataProvider->getValue("id").toInt());
        gloss.setPhrase( m_pDataProvider->getValue("phrase").toString().toUtf8());
        gloss.setMeaning( m_pDataProvider->getValue("meaning").toString().toUtf8());
        gloss.setExample( m_pDataProvider->getValue("example").toString().toUtf8());
        gloss.setIdiom( m_pDataProvider->getValue("idiom").toBool());
        m_glossaries[podcast].append(gloss);
    }

    qry = "SELECT * FROM ESL_Blog";
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
        ESLBlog blog;
        blog.setId(m_pDataProvider->getValue("id").toInt());
        blog.setTitle(m_pDataProvider->getValue("title").toString());
        blog.setDate(m_pDataProvider->getValue("date").toString().simplified());
        blog.setImage(m_pDataProvider->getValue("image").toString());
        blog.setPost(m_pDataProvider->getValue("post").toString());

       m_blogPosts.append(blog);
    }
    qSort(m_blogPosts.begin(), m_blogPosts.end(), caseInsensitiveLessThan);
//    qry = "update ESL_Blog set ordering = %1 where id = %2";
//    for (int i = 0, len = m_blogPosts.length(); i < len ; ++i) {
//        int id = m_blogPosts[i].id();
//        m_pDataProvider->executeNoneQuery(qry.arg(len - i).arg(id));
//    }
}

void BinFileWindow::serialize()
{
    QString output = QCoreApplication::applicationDirPath() + "/db/ESL.btk";
    QFile out(output);
     if (!out.open(QIODevice::WriteOnly | QIODevice::Unbuffered))
     {
         qDebug() << "can not open output file";
         return;
     }
     QDataStream catDataStream(&out);
     catDataStream << m_categories << m_podcasts << m_podCategories << m_glossaries << m_blogPosts;
     out.flush();
     out.close();

}

