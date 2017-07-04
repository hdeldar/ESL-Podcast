#ifndef BINFILEWINDOW_H
#define BINFILEWINDOW_H

#include "IESL.h"
#include <QMainWindow>

class SqliteDataProvider;

class BinFileWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit BinFileWindow(QWidget *parent = 0);
    ~BinFileWindow();
private:
    void convert();
    void serialize();
private:
    SqliteDataProvider *m_pDataProvider;
    QList<ESLCategory> m_categories;
    QList<ESLPodcast> m_podcasts;
    QList<ESLBlog> m_blogPosts;
    QHash<int, QList<ESLGlossary> > m_glossaries;
    QHash<int, QList<ESLCategory> > m_podCategories;  
    
};

#endif // BINFILEWINDOW_H
