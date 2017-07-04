#ifndef SqliteDataProvider_H
#define SqliteDataProvider_H
#include "IDbDataProvider.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QDebug>

class SqliteDataProvider: public IDbDataProvider
{
public:   
    ~SqliteDataProvider();
    SqliteDataProvider(const QString &connectionName);
    virtual bool open() const;
    virtual void close() const;
	virtual int executeNoneQuery(const QString &qryStr) const;
	virtual void executeReader(const QString &qryStr) const;
	virtual QVariant executeScalar(const QString &qryStr) const;
	virtual bool hasNext() const;
	virtual QVariant getValue(int columnIndex) const;
	virtual QVariant getValue(QString columnName) const;

    void setDbName(const QString &name);      
	bool hasError();

private:  
    QString m_dbName;
    mutable QSqlDatabase m_db;
    mutable QSqlQuery *m_pQuery;
    mutable bool m_hasError;
};

#endif // SqliteDataProvider_H
