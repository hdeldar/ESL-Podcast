#ifndef IESL_H
#define IESL_H

#include <QString>
#include <QList>
#include <QObject>
#include <QDataStream>

//=======================================================

class ESLCategory: QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString description READ description WRITE setDescription)
    Q_PROPERTY(int id READ id WRITE setId)

public:
    explicit ESLCategory(QObject *parent = 0) : QObject(parent){}
    ESLCategory(const ESLCategory &copy)
        : QObject(){ *this = copy; }

    ESLCategory &operator =(const ESLCategory &other)
    {
        m_id = other.id();
        m_name = other.name();
        m_description = other.description();
        return *this;
    }

    QString name() const { return m_name;}
    void setName(const QString &name) { m_name = name;}

    QString description() const { return m_description;}
    void setDescription(const QString &description) { m_description = description;}

    int id() const { return m_id;}
    void setId(int id) { m_id = id;}

    //for serialization
    friend QDataStream& operator <<( QDataStream& stream, const ESLCategory& cat )
    {
       return stream << cat.m_id << cat.m_name << cat.m_description;
    }
    //for deserialization
    friend QDataStream& operator >>( QDataStream& stream, ESLCategory& cat )
    {
        return stream >> cat.m_id >> cat.m_name >> cat.m_description;
    }

private:
    int m_id;
    QString m_name;
    QString m_description;

};

//=======================================================

class ESLGlossary: QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int podcast READ podcast WRITE setPodcst)

    Q_PROPERTY(QString phrase READ phrase WRITE setPhrase)
    Q_PROPERTY(QString meaning READ meaning WRITE setMeaning)
    Q_PROPERTY(QString example READ example WRITE setExample)

    Q_PROPERTY(bool idiom READ idiom WRITE setIdiom)

public:
    explicit ESLGlossary(QObject *parent = 0) : QObject(parent){}
    ESLGlossary(const ESLGlossary &copy)
        : QObject(){ *this = copy; }
    ESLGlossary &operator =(const ESLGlossary &other)
    {
        m_id = other.id();
        m_podcast = other.podcast();
        m_phrase = other.phrase();
        m_meaning = other.meaning();
        m_example = other.example();
        m_idiom = other.idiom();
        return *this;
    }

    int id() const { return m_id;}
    void setId(int id) { m_id = id;}

    int podcast() const { return m_podcast;}
    void setPodcst(int podcst) { m_podcast = podcst;}

    QString phrase() const { return m_phrase;}
    void setPhrase(const QString &phrase) { m_phrase = phrase;}

    QString meaning() const { return m_meaning;}
    void setMeaning(const QString &meaning) { m_meaning = meaning;}

    QString example() const { return m_example;}
    void setExample(const QString &example) { m_example = example;}

    bool idiom() const { return m_idiom;}
    void setIdiom(bool idiom) { m_idiom = idiom;}

    //for serialization
    friend QDataStream& operator <<( QDataStream& stream, const ESLGlossary& gloss )
    {
       return stream << gloss.m_id << gloss.m_podcast << gloss.m_phrase
                        << gloss.m_meaning << gloss.m_example << gloss.m_idiom;
    }
    //for deserialization
    friend QDataStream& operator >>( QDataStream& stream, ESLGlossary& gloss )
    {
        return stream >> gloss.m_id >> gloss.m_podcast >> gloss.m_phrase
                         >> gloss.m_meaning >> gloss.m_example >> gloss.m_idiom;
    }


private:
    int m_id;
    int m_podcast;
    QString m_phrase;
    QString m_meaning;
    QString m_example;
    bool m_idiom;
};

//=======================================================

class ESLPodcast: QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(int year READ year WRITE setYear)
    Q_PROPERTY(int month READ month WRITE setMonth)

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString script READ script WRITE setScript)
    Q_PROPERTY(QString comment READ comment WRITE setComment)
    Q_PROPERTY(QString cultureNote READ cultureNote WRITE setCultureNote)

    Q_PROPERTY(QList<ESLCategory> categories READ categories WRITE setCategories)

public:
    explicit ESLPodcast(QObject *parent = 0) : QObject(parent){}
    ESLPodcast(const ESLPodcast &copy)
        : QObject(){ *this = copy; }

    ESLPodcast &operator =(const ESLPodcast &other)
    {
        m_id = other.id();
        m_title = other.title();
        m_year = other.year();
        m_month = other.month();
        m_script = other.script();
        m_comment = other.comment();
        m_cultureNote = other.cultureNote();
        m_categories = other.categories();
        return *this;
    }

    int id() const { return m_id;}
    void setId(int id) { m_id = id;}

    int year() const { return m_year;}
    void setYear(int year) { m_year = year;}

    int month() const { return m_month;}
    void setMonth(int month) { m_month = month;}

    QString title() const { return m_title;}
    void setTitle(const QString &title) { m_title = title;}

    QString script() const { return m_script;}
    void setScript(const QString &script) { m_script = script;}

    QString comment() const { return m_comment;}
    void setComment(const QString &comment) { m_comment = comment;}

    QString cultureNote() const { return m_cultureNote;}
    void setCultureNote(const QString &cultureNote) { m_cultureNote = cultureNote;}

    QList<ESLCategory> categories() const { return m_categories;}
    void setCategories(QList<ESLCategory> cat){ m_categories = cat;}

    //for serialization
    friend QDataStream& operator <<( QDataStream& stream, const ESLPodcast& pod )
    {
       return stream << pod.m_id << pod.m_title << pod.m_script
                        << pod.m_year << pod.m_month << pod.m_comment
                           << pod.m_cultureNote << pod.m_categories;
    }
    //for deserialization
    friend QDataStream& operator >>( QDataStream& stream, ESLPodcast& pod )
    {
        return stream >> pod.m_id >> pod.m_title >> pod.m_script
                         >> pod.m_year >> pod.m_month >> pod.m_comment
                            >> pod.m_cultureNote >> pod.m_categories;
    }

private:
    int m_id;
    QString m_title;
    QString m_script;
    int m_year;
    int m_month;
    QString m_comment;
    QString m_cultureNote;
    QList<ESLCategory> m_categories;
};

//=======================================================

class ESLBlog: QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId)

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString post READ post WRITE setPost)
    Q_PROPERTY(QString date READ date WRITE setDate)
    Q_PROPERTY(QString image READ image WRITE setImage)

public:
    explicit ESLBlog(QObject *parent = 0) : QObject(parent){}
    ESLBlog(const ESLBlog &copy)
        : QObject(){ *this = copy; }
    ESLBlog &operator =(const ESLBlog &other)
    {
        m_id = other.id();
        m_title = other.title();
        m_post = other.post();
        m_date = other.date();
        m_image = other.image();
        return *this;
    }

    int id() const { return m_id;}
    void setId(int id) { m_id = id;}

    QString title() const { return m_title;}
    void setTitle(const QString &title) { m_title = title;}

    QString post() const { return m_post;}
    void setPost(const QString &post) { m_post = post;}

    QString date()const { return m_date;}
    void setDate(const QString &date) { m_date = date;}

    QString image() const { return m_image;}
    void setImage(const QString &image) { m_image = image;}

    //for serialization
    friend QDataStream& operator <<( QDataStream& stream, const ESLBlog& blog )
    {
       return stream << blog.m_id << blog.m_title << blog.m_post
                        << blog.m_date << blog.m_image;
    }
    //for deserialization
    friend QDataStream& operator >>( QDataStream& stream, ESLBlog& blog )
    {
        return stream >> blog.m_id >> blog.m_title >> blog.m_post
                         >> blog.m_date >> blog.m_image;
    }

private:
    int m_id;
    QString m_title;
    QString m_post;
    QString m_date;
    QString m_image;
};

//=======================================================

class IESL
{
public: 
    virtual QList<ESLCategory> getCategories() = 0;

    virtual QList<ESLGlossary> getGlossaries(int podcast) = 0;
    virtual QList<ESLGlossary> searchInGlossaries(const QString &phrase) = 0;

    virtual QList<ESLPodcast> getPodcasts() = 0;
    virtual QList<ESLPodcast> searchInPodcasts(const QString &phrase) = 0;
    virtual QList<ESLPodcast> filterPodcastsByTitle(const QString &phrase, int catId, int year) = 0;
    virtual QList<int> getYears() = 0;
    virtual QList<ESLCategory> getPodcastCategories(int pod) = 0;

    virtual ESLPodcast getPodcast(int id) = 0;

    virtual QList<ESLBlog> getBlogPosts() = 0;
    virtual ESLBlog getBlogPost(int id) = 0;
    virtual QList<ESLBlog> searchInBlogPosts(const QString &phrase) = 0;
};

#endif // IESL_H
