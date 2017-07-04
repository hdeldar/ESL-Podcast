#ifndef IDbDataProvider_H
#define IDbDataProvider_H
#include <QString>
#include <QVariant>

class IDbDataProvider
{
public:
    virtual bool open() const = 0;
    virtual void close() const = 0;
    virtual int executeNoneQuery(const QString &qryStr) const = 0;
    virtual void executeReader(const QString &qryStr) const = 0;
    virtual QVariant executeScalar(const QString &qryStr) const = 0;
    virtual bool hasNext() const = 0;
    virtual QVariant getValue(int columnIndex) const = 0;
    virtual QVariant getValue(QString columnName) const = 0;
};


#endif // SqliteDataProvider_H
