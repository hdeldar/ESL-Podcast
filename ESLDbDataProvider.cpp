#include "ESLDbDataProvider.h"
#include <QHashIterator>
#include <QCoreApplication>

ESLDbDataProvider::ESLDbDataProvider()
    : m_pDataProvider(NULL)
{
    m_pDataProvider = new SqliteDataProvider("ESL");
    m_pDataProvider->setDbName(QCoreApplication::applicationDirPath() + "/db/ESL.db");
    m_pDataProvider->open();
    init();
}

ESLDbDataProvider::~ESLDbDataProvider()
{
    if(m_pDataProvider)
        delete m_pDataProvider;
}

QList<ESLCategory> ESLDbDataProvider::getCategories()
{
   return m_categories;
}

QList<ESLGlossary> ESLDbDataProvider::getGlossaries(int podcast)
{
    QList<ESLGlossary> glossList;
    QString qry = QString("SELECT * FROM ESL_Glossary WHERE podcast=%1").arg(podcast);
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
        ESLGlossary gloss;
        gloss.setId(m_pDataProvider->getValue("id").toInt());
        gloss.setPhrase( m_pDataProvider->getValue("phrase").toString().toUtf8());
        gloss.setMeaning( m_pDataProvider->getValue("meaning").toString().toUtf8());
        gloss.setExample( m_pDataProvider->getValue("example").toString().toUtf8());
        gloss.setIdiom( m_pDataProvider->getValue("idiom").toBool());
        glossList.append(gloss);
    }
    return glossList;
}

QList<ESLGlossary> ESLDbDataProvider::searchInGlossaries(const QString &phrase)
{
    Q_UNUSED(phrase);
    QList<ESLGlossary> gloss;
    return gloss;
}

QList<ESLPodcast> ESLDbDataProvider::getPodcasts()
{
   return m_podcasts;
}

QList<ESLPodcast> ESLDbDataProvider::searchInPodcasts(const QString &phrase)
{
    Q_UNUSED(phrase);
    QList<ESLPodcast> podcasts;
    return podcasts;
}

QList<ESLPodcast> ESLDbDataProvider::filterPodcastsByTitle(const QString &phrase, int catId, int year)
{
    QList<ESLPodcast> pods;
    for (int i = 0; i < m_podcasts.length(); ++i) {
        ESLPodcast pod = m_podcasts[i];
        if(!phrase.isEmpty() && !pod.title().contains(phrase,Qt::CaseInsensitive))
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

ESLPodcast ESLDbDataProvider::getPodcast(int id)
{
    ESLPodcast podcast;
    for (int i = 0; i < m_podcasts.length(); ++i) {
        if(m_podcasts[i].id() == id)
            return m_podcasts[i];
    }
    return podcast;
}

QList<int> ESLDbDataProvider::getYears()
{
    QList<int> years;
    QString qry = "SELECT DISTINCT(year) FROM ESL_Podcast";
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
         years.append(m_pDataProvider->getValue("year").toInt());
    }
    return years;
}

QList<ESLCategory> ESLDbDataProvider::getPodcastCategories(int pod)
{
    Q_UNUSED(pod);
    QList<ESLCategory> list;
    return list;
}

QList<ESLBlog> ESLDbDataProvider::getBlogPosts()
{
    QList<ESLBlog> posts;
    return posts;
}

ESLBlog ESLDbDataProvider::getBlogPost(int id)
{
    Q_UNUSED(id);
    ESLBlog post;
    return post;
}

QList<ESLBlog> ESLDbDataProvider::searchInBlogPosts(const QString &phrase)
{
    Q_UNUSED(phrase);
    QList<ESLBlog> posts;
    return posts;
}

void ESLDbDataProvider::init()
{
    //===========================
    //load categories
    ESLCategory allCat;
    allCat.setName("All Categoreis");
    allCat.setId(-1);
    allCat.setDescription("all categories");
    m_categories.append(allCat);
    QString qry = "SELECT * FROM ESL_Category";
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
        ESLCategory cat;
        cat.setName(m_pDataProvider->getValue("name").toString().toUtf8());
        cat.setDescription(m_pDataProvider->getValue("description").toString().toUtf8());
        cat.setId(m_pDataProvider->getValue("id").toInt());
        m_categories.append(cat);
    }
    //===========================
    //load podcasts
    qry = "SELECT * FROM ESL_Podcast ORDER BY id DESC";
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
    //=========================== 
    //load pod categories
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
    //=============================
    for (int i = 0; i < m_podcasts.length(); ++i)
    {
        m_podcasts[i].setCategories(m_podCategories[m_podcasts[i].id()]);
    }

    qry = "SELECT * FROM ESL_Blog";
    m_pDataProvider->executeReader(qry);
    while (m_pDataProvider->hasNext()) {
        ESLBlog blog;
        blog.setId(m_pDataProvider->getValue("id").toInt());
        blog.setTitle(m_pDataProvider->getValue("title").toString());
        blog.setDate(m_pDataProvider->getValue("date").toString());
        blog.setImage(m_pDataProvider->getValue("image").toString());
        blog.setPost(m_pDataProvider->getValue("post").toString());

       m_blogPosts.append(blog);
    }
}
