#ifndef ESLFILEDATAPROVIDER_H
#define ESLFILEDATAPROVIDER_H

#include "IESL.h"
#include <QHash>

class ESLFileDataProvider : public IESL
{
public:
    ESLFileDataProvider();
    ~ESLFileDataProvider();

    virtual QList<ESLCategory> getCategories();

    virtual QList<ESLGlossary> getGlossaries(int podcast);
    virtual QList<ESLGlossary> searchInGlossaries(const QString &phrase);

    virtual QList<ESLPodcast> getPodcasts();
    virtual QList<ESLPodcast> searchInPodcasts(const QString &phrase);
    virtual QList<ESLPodcast> filterPodcastsByTitle(const QString &phrase, int catId, int year);
    virtual QList<int> getYears();
    virtual QList<ESLCategory> getPodcastCategories(int pod);

    virtual ESLPodcast getPodcast(int id);

    virtual QList<ESLBlog> getBlogPosts();
    virtual ESLBlog getBlogPost(int id);
    virtual QList<ESLBlog> searchInBlogPosts(const QString &phrase);

private:
    void init();
private:
    QList<ESLCategory> m_categories;
    QList<ESLPodcast> m_podcasts;
    QList<ESLBlog> m_blogPosts;
    QHash<int, QList<ESLGlossary> > m_glossaries;
    QHash<int, QList<ESLCategory> > m_podCategories;
};

#endif // ESLFILEDATAPROVIDER_H
