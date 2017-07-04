#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QModelIndex>
#include "IESL.h"
#include <QSystemTrayIcon>

class PodcastModel;
class CategoryModel;
class GlossaryModel;
class BlogModel;

class QMediaPlayer;

class QPodcastTableViewDelegate;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void filterPodcastByTitle();
    QString getAppPath() const;
    void setPodcast(int podcast);
    void setCat(int cat);
    void setFilterWord(const QString &word);
    bool existsFile(const QString &file) const;
    void filterBlog(const QString &phrase = QString());
    QString getPost(int index) const;
    int postCount() const{ return m_blogList.length();}

    void showPost();
    QString convertTime(qint64 num);

    void showPodcastInfo();

public slots:
    void onPodcastDblClicked(QModelIndex index);
    void onSearch(QString);
    void onCategoryClicked(QModelIndex);

    void onNextPost();
    void onPrevPost();

    void onChangePost(int);

    void onPostFontSizeChanged();

    void onPlayPause();

    void onPlayerPositionChanged(qint64);
    void onPlayerDurationChanged(qint64);

    void onSliderPlayerChanged(int);

    void iconActivated(QSystemTrayIcon::ActivationReason);

    void onBtnDemoClicked();
    
private:
    Ui::MainWindow *ui;
    IESL *m_pEslDataProvider;

    PodcastModel *m_pPodcastModel;
    CategoryModel *m_pCategoryModel;
    GlossaryModel *m_pGlossaryModel;
    BlogModel *m_pBlogModel;

    QPodcastTableViewDelegate * m_pPodcastDeleagte;

    QList<ESLBlog> m_blogList;

    int m_selectedCat;
    int m_selectedYear;
    QString m_filterWord;

    int m_currentPost;
    int m_postFontSize;

    bool m_firstTime;

    QMediaPlayer *m_pPlayer;
    qint64 m_totalTime;

    int m_podId;
    QString m_title;
    QString m_script;
    QString m_cultureNote;

};

#endif // MAINWINDOW_H
