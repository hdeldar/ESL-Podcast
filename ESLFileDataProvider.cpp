#include "ESLFileDataProvider.h"
#include <QHashIterator>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>

ESLFileDataProvider::ESLFileDataProvider()
{
    init();
}

ESLFileDataProvider::~ESLFileDataProvider()
{

}

QList<ESLCategory> ESLFileDataProvider::getCategories()
{
   return m_categories;
}

QList<ESLGlossary> ESLFileDataProvider::getGlossaries(int podcast)
{
    QList<ESLGlossary> glossList;
    if(m_glossaries.contains(podcast))
        return m_glossaries[podcast];
    return glossList;
}

QList<ESLGlossary> ESLFileDataProvider::searchInGlossaries(const QString &phrase)
{
    Q_UNUSED(phrase);
    QList<ESLGlossary> gloss;
    return gloss;
}

QList<ESLPodcast> ESLFileDataProvider::getPodcasts()
{
   return m_podcasts;
}

QList<ESLPodcast> ESLFileDataProvider::searchInPodcasts(const QString &phrase)
{
    Q_UNUSED(phrase);
    QList<ESLPodcast> podcasts;
    return podcasts;
}

QList<ESLPodcast> ESLFileDataProvider::filterPodcastsByTitle(const QString &phrase, int catId, int year)
{
    QList<ESLPodcast> pods;
    for (int i = 0; i < m_podcasts.length(); ++i) {
        ESLPodcast pod = m_podcasts[i];
        if(!phrase.isEmpty() && !pod.title().contains(phrase,Qt::CaseInsensitive)
                && !pod.comment().contains(phrase,Qt::CaseInsensitive)
                && !pod.script().contains(phrase,Qt::CaseInsensitive)
                && !pod.cultureNote().contains(phrase,Qt::CaseInsensitive)
                && !QString::number(pod.id()).contains(phrase,Qt::CaseInsensitive))
            continue;

        if( catId == -1 && (year == -1 || pod.year() == year))
            pods.append(pod);
        else if(catId > -1 && (year == -1 || pod.year() == year))
        {
            bool findCat = false;
            QList<ESLCategory> cats= m_podCategories[pod.id()];
            for (int c = 0; c < cats.length(); ++c) {
                if(cats[c].id() == catId)
                {
                    findCat = true;
                    break;
                }
            }

            if(findCat)
                pods.append(pod);
        }
    }
    return pods;
}

ESLPodcast ESLFileDataProvider::getPodcast(int id)
{
    ESLPodcast podcast;
    for (int i = 0; i < m_podcasts.length(); ++i) {
        if(m_podcasts[i].id() == id)
            return m_podcasts[i];
    }
    return podcast;
}

QList<int> ESLFileDataProvider::getYears()
{
    QList<int> years;
    return years;
}

QList<ESLCategory> ESLFileDataProvider::getPodcastCategories(int pod)
{
    Q_UNUSED(pod);
    QList<ESLCategory> list;
    return list;
}

QList<ESLBlog> ESLFileDataProvider::getBlogPosts()
{    
    return m_blogPosts;
}

ESLBlog ESLFileDataProvider::getBlogPost(int id)
{
    Q_UNUSED(id);
    ESLBlog post;
    return post;
}

QList<ESLBlog> ESLFileDataProvider::searchInBlogPosts(const QString &phrase)
{   
    if(phrase.isEmpty())
        return m_blogPosts;

    QList<ESLBlog> postList;
    for (int i = 0; i < m_blogPosts.length(); ++i) {
        ESLBlog post = m_blogPosts[i];
        //if(post.title().contains(phrase) || post.post().contains(phrase))
            postList.append(post);
    }
    return postList;
}

void ESLFileDataProvider::init()
{
    QString output = QCoreApplication::applicationDirPath() + "/db/ESL.btk";
    QFile out(output);
     if (!out.open(QIODevice::ReadOnly | QIODevice::Unbuffered))
     {
         qDebug() << "can not open output file";
         return;
     }
     QDataStream dataStream(&out);
     dataStream >> m_categories >> m_podcasts >> m_podCategories >> m_glossaries >> m_blogPosts;
     out.close();          

     for (int i = 0; i < m_podcasts.length(); ++i)
     {
         m_podcasts[i].setCategories(m_podCategories[m_podcasts[i].id()]);
     }
}
