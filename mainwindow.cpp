#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ESLDbDataProvider.h"
#include "ESLFileDataProvider.h"
#include "PodcastModel.h"
#include "CategoryModel.h"
#include "GlossaryModel.h"
#include "BlogModel.h"
#include <QFile>
#include <QStringList>
#include "QPodcastTableViewDelegate.h"
#include <QSettings>
#include <QMediaPlayer>
#include <QUrl>
#include <QDesktopServices>

const int DEFAULT_FONT_SIZE = 24;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_selectedCat(-1),
    m_selectedYear(-1),
    m_currentPost(0),
    m_postFontSize(DEFAULT_FONT_SIZE),
    m_firstTime(true),
    m_podId(1),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);

//    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,
//                SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));


//    trayIcon->setIcon(QIcon(":/resources/images/esl.ico"));
//    trayIcon->show();

    //m_pEslDataProvider = new ESLDbDataProvider();
    m_pEslDataProvider = new ESLFileDataProvider();

    m_pPodcastModel = new PodcastModel(this);
    m_pCategoryModel = new CategoryModel(this);
    m_pGlossaryModel = new GlossaryModel(this);
    m_pBlogModel = new BlogModel(this);

    QList<ESLCategory> cats = m_pEslDataProvider->getCategories();
    m_pCategoryModel->setItemList(cats);

    ui->listView_cats->setModel(m_pCategoryModel);
    QModelIndex rootIndex = ui->listView_cats->rootIndex();
    ui->listView_cats->setCurrentIndex(ui->listView_cats->model()->index(0,0,rootIndex));

    m_pPodcastDeleagte = new QPodcastTableViewDelegate(this);

    m_pPlayer = new  QMediaPlayer(this);
    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(onPlayerPositionChanged(qint64)));
    connect(m_pPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(onPlayerDurationChanged(qint64)));

    filterPodcastByTitle();
    filterBlog();
    showPost();
    //---
    ui->splitter_2->setStretchFactor(0, 2);
    ui->splitter_2->setStretchFactor(1, 1);

}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_pEslDataProvider!=NULL)
        delete m_pEslDataProvider;
}

QString MainWindow::getAppPath() const
{
    return QCoreApplication::applicationDirPath();
}

void MainWindow::filterPodcastByTitle()
{
    QList<ESLPodcast> pods = m_pEslDataProvider->filterPodcastsByTitle(
                m_filterWord, m_selectedCat, m_selectedYear);
    m_userNotes = m_pEslDataProvider->getUserNotes();
    m_pPodcastModel->setItemList(pods);
    m_pPodcastDeleagte->setFilterText(m_filterWord);
    ui->tableView_podcast_list->setModel(m_pPodcastModel);

    ui->tableView_podcast_list->setItemDelegate(m_pPodcastDeleagte);
    ui->tableView_podcast_list->resizeColumnsToContents();
    ui->tableView_podcast_list->resizeRowsToContents();
    ui->tableView_podcast_list->verticalHeader()->hide();
    ui->tableView_podcast_list->scrollToTop();

    if(m_firstTime)
    {
        m_firstTime = false;
        QModelIndex rootIndex = ui->tableView_podcast_list->rootIndex();
        QSettings settings("ESL Podcast", "ESL Podcast");
        int lastPodId = settings.value("lastPodcast").toInt();
        int lastIndex = 0;
        for (int i = 0; i < pods.length(); ++i) {
            if(pods.at(i).id() == lastPodId)
            {
                lastIndex = i;
                break;
            }
        }
        ui->tableView_podcast_list->setCurrentIndex(ui->tableView_podcast_list->model()->index(lastIndex,0,rootIndex));
        ui->tableView_podcast_list->doubleClicked(ui->tableView_podcast_list->model()->index(lastIndex,0,rootIndex));
    }
}

void MainWindow::filterBlog(const QString &phrase)
{
    m_blogList = m_pEslDataProvider->searchInBlogPosts(phrase);
    m_pBlogModel->setItemList(m_blogList);

    QSettings settings("ESL Podcast", "ESL Podcast");
    int lastPostIndex = settings.value("lastPost").toInt();

    int fs = settings.value("postFontSize").toInt();
    if(fs >= 18)
    {
        m_postFontSize = fs;
        ui->slider_post_zoom->setValue(m_postFontSize);
    }

    for (int i = 0; i < m_blogList.length(); ++i) {
        ui->comboBox_post->addItem(QString::number(i + 1) + " - " + m_blogList.at(i).title(),i);
    }

    ui->comboBox_post->setCurrentIndex(lastPostIndex);
}

QString MainWindow::getPost(int index) const
{
    if(index < 0 || index >= m_blogList.length())
        return "";
    ESLBlog blog= m_blogList[index];
    QString images = blog.image();
    QStringList imgList = images.split(",", QString::SkipEmptyParts);
    QString htmlImg = QString("<html><head><style type='text/css'>"
            "p,ul,li{padding:3px;font-family:Times New Roman;font-size:%1px;line-height:%2px;}"
            "</style></head><body>").arg(m_postFontSize).arg(m_postFontSize + 8);
    for (int i = 0; i < imgList.length() ; ++i) {
        htmlImg += QString("<br /><img src='file:///%1' style='float:right;display:inline-block;'/>")
                .arg(getAppPath() + "/blogImages/" + imgList.at(i));
    }
    QString post = QString("%4<br /><h6 style='color:#009933;font-family:Times New Roman; font-size:%5px;'>%1</h6>"
                           "<h6 style='font-family:Times New Roman;font-size:%6px;'><u>%2</u></h6>"
                           "<p>%3</p>");
    post += "</body></hrml>";
    return post.arg(blog.date()).arg(blog.title())
            .arg(blog.post()).arg(htmlImg)
            .arg(m_postFontSize - 7).arg(m_postFontSize - 2);
}

void MainWindow::showPost()
{
    if(m_currentPost >= 0 && m_currentPost < m_blogList.length())
    {
        ui->textBrowser_currentPost->setHtml(getPost(m_currentPost));        
        QSettings settings("ESL Podcast", "ESL Podcast");
        settings.setValue("lastPost", m_currentPost);
    }
}

void MainWindow::setPodcast(int podcast)
{
    QList<ESLGlossary> gloss = m_pEslDataProvider->getGlossaries(podcast);
    m_pGlossaryModel->setItemList(gloss);
}

void MainWindow::setCat(int cat)
{
    m_selectedCat = cat;
    filterPodcastByTitle();
}

void MainWindow::setFilterWord(const QString &word)
{
    m_filterWord = word;
    m_pPodcastModel->setFilterWord(m_filterWord);
    filterPodcastByTitle();
}

bool MainWindow::existsFile(const QString &file) const
{
    return QFile::exists(file);
}

void MainWindow::showPodcastInfo()
{
    QString script = QString("<h2 style='color:#003399;'>%1 - %2</h2>"
                             "<p style='font-family:Times New Roman;font-size:%4px;line-height:%5px;'>%3</p>")
            .arg(m_podId)
            .arg(m_title)
            .arg(m_script)
            .arg(m_postFontSize - 7)
            .arg(m_postFontSize + 2);
    ui->textBrowser_script->setHtml(script);
    ui->textBrowser_culture_note->setHtml(
                QString("<p style='font-family:Times New Roman;font-size:%2px;line-height:%3px;'>%1</p>")
                .arg(m_cultureNote)
                .arg(m_postFontSize - 7)
                .arg(m_postFontSize + 2));

    QList<ESLGlossary> glossList = m_pEslDataProvider->getGlossaries(m_podId);

    QString glossHtml = "";
    for (int i = 0; i < glossList.length(); ++i) {
        ESLGlossary gloss = glossList.at(i);
        glossHtml += QString("<p style='font-family:Times New Roman;font-size:%4px;line-height:%5px;'>"
                             "<b>%1</b> - %2 <br /><font style='color:#162E0B;'>*%3</span></p>")
                .arg(gloss.phrase())
                .arg(gloss.meaning())
                .arg(gloss.example())
                .arg(m_postFontSize - 7)
                .arg(m_postFontSize + 2);
    }
    ui->textBrowser_glossary->setHtml(glossHtml);
    ui->textEdit->setText(getUserNote(m_podId));
}


QString MainWindow::getUserNote(int podcastId)
{
   for (auto u : m_userNotes)
   {
      if (u.id() == podcastId)
         return u.note();
   }
   return "";
}

void MainWindow::setUserNote(int podcastId, QString note)
{
   if(note.isEmpty())
      return;
   bool bfind = false;
   for (ESLPodcastUserNote& u : m_userNotes)
   {
      if (u.id() == podcastId)
      {
         bfind = true;
         u.setNote(note);
         break;
      }
   }
   if (!bfind)
   {
      m_userNotes.append(ESLPodcastUserNote(podcastId, note));
      bfind = true;
   }
   //if(bfind)
   //   serialize();
}

void MainWindow::serialize()
{
   QString output = QCoreApplication::applicationDirPath() + "/db/UserNotes.btk";
   QFile outUser(output);
   if (!outUser.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
   {
      qDebug() << "can not open output file";
      return;
   }
   QDataStream dataStreamUser(&outUser);
   dataStreamUser << m_userNotes;
   outUser.close();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
   serialize();
}

void MainWindow::onPodcastDblClicked(QModelIndex index)
{
    m_podId = index.data(PodcastModel::IdRole).toInt();
    QSettings settings("ESL Podcast", "ESL Podcast");
    settings.setValue("lastPodcast", m_podId);

    m_title = index.data(PodcastModel::TitleRole).toString();
    m_script = index.data(PodcastModel::ScriptRole).toString();
    m_cultureNote = index.data(PodcastModel::CultureNoteRole).toString();
     ui->tabWidget->setCurrentIndex(1);
    showPodcastInfo();
    //==========================================
    QString src = QString("%1/audio/%2.mp3").arg(getAppPath()).arg(m_podId);
    if(existsFile(src))
    {
        m_pPlayer->setMedia(QUrl::fromLocalFile(src));
        m_pPlayer->setVolume(100);        
        //m_pPlayer->play();
        m_pPlayer->setPosition(0);
//        ui->btn_playPause->setStyleSheet("#btn_playPause{border:0;background-color:transparent;"
//                                         "background-image: url(:/resources/images/pause.png);"
//                                         "background-repeat:no-repeat;background-position: center center;"
//                                         "height:22px;width: 30px;}#btn_playPause:hover{"
//                                         "background-image: url(:/resources/images/pause_mo.png);"
//                                         "}");
    }
    else {
        m_pPlayer->setMedia(NULL);
        m_pPlayer->stop();
        m_pPlayer->setPosition(0);
        ui->slider_player->setRange(0, 0);       
       
    }   
 ui->btn_playPause->setStyleSheet("#btn_playPause{border:0;background-color:transparent;"
                                         "background-image: url(:/resources/images/play.png);"
                                         "background-repeat:no-repeat;background-position: center center;"
                                         "height:22px;width: 30px;}#btn_playPause:hover{"
                                         "background-image: url(:/resources/images/play_mo.png);"
                                         "}");
}

void MainWindow::onSearch(QString word)
{
    setFilterWord(word);
}

void MainWindow::onCategoryClicked(QModelIndex index)
{
    int id = index.data(CategoryModel::IdRole).toInt();
    m_selectedCat = id;
    setFilterWord(m_filterWord);
}

void MainWindow::onNextPost()
{
    if(m_currentPost > 0)
    {
        m_currentPost--;
        //showPost();
        ui->comboBox_post->setCurrentIndex(m_currentPost);
    }
}

void MainWindow::onPrevPost()
{
    if(m_currentPost < m_blogList.length() - 1)
    {
        m_currentPost++;
        //showPost();
        ui->comboBox_post->setCurrentIndex(m_currentPost);
    }
}

void MainWindow::onChangePost(int i)
{
    m_currentPost = i;
    showPost();
}

void MainWindow::onPostFontSizeChanged()
{
   auto val = ui->slider_post_zoom->value();
   if (val != m_postFontSize)
   {
      m_postFontSize = val;
      QSettings settings("ESL Podcast", "ESL Podcast");
      settings.setValue("postFontSize", m_postFontSize);
      showPost();
      showPodcastInfo();
   }
}

void MainWindow::onPlayPause()
{
    if(m_pPlayer->state() == QMediaPlayer::PlayingState)
    {
        m_pPlayer->pause();
        ui->btn_playPause->setStyleSheet("#btn_playPause{border:0;background-color:transparent;"
                                         "background-image: url(:/resources/images/play.png);"
                                         "background-repeat:no-repeat;background-position: center center;"
                                         "height:22px;width: 30px;}#btn_playPause:hover{"
                                         "background-image: url(:/resources/images/play_mo.png);"
                                         "}");
    }
    else if(m_pPlayer->state() == QMediaPlayer::PausedState || m_pPlayer->state() == QMediaPlayer::StoppedState)
    {
        m_pPlayer->play();
        ui->btn_playPause->setStyleSheet("#btn_playPause{border:0;background-color:transparent;"
                                         "background-image: url(:/resources/images/pause.png);"
                                         "background-repeat:no-repeat;background-position: center center;"
                                         "height:22px;width: 30px;}#btn_playPause:hover{"
                                         "background-image: url(:/resources/images/pause_mo.png);"
                                         "}");
    }

}

void MainWindow::onPlayerPositionChanged(qint64 pos)
{
    ui->slider_player->setValue(pos);
    ui->lbl_duration->setText(QString("%1 / %2").arg(convertTime(pos/1000))
                               .arg(convertTime(m_totalTime/1000)));
}

void MainWindow::onPlayerDurationChanged(qint64 dur)
{
    m_totalTime = dur;
    ui->slider_player->setRange(0,m_totalTime);
    ui->lbl_duration->setText(QString("%1 / %2").arg(convertTime(0))
                               .arg(convertTime(m_totalTime/1000)));
}

void MainWindow::onSliderPlayerChanged(int pos)
{
    if(m_pPlayer->isAvailable())
        m_pPlayer->setPosition(pos);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason)
{
    show();
}

void MainWindow::onBtnDemoClicked()
{
    //QString link = "http://www.eslpod.ir";
    //QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::onUserNoteTextChanged()
{
   setUserNote(m_podId, ui->textEdit->toPlainText());
   
}

QString MainWindow::convertTime(qint64 num)
{
        int minute = (int)((num %3600) / 60);
        int second = num % 60;

        QString sMin = minute >= 10 ? QString::number(minute) : QString("0%1").arg(minute);
        QString sSec = second >= 10 ? QString::number(second) : QString("0%1").arg(second);
        QString time = QString("%1:%2").arg(sMin).arg(sSec);
        return time;
}

