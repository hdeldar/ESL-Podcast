/*
  this class provides a simple way to connecting to database
  and exceuting queries
*/

#include "SqliteDataProvider.h"

//constructor with db name parameter
SqliteDataProvider::SqliteDataProvider(const QString &connectionName)
{  
    m_hasError = false;
    m_pQuery = NULL;
    m_db = QSqlDatabase::addDatabase("QSQLITE",connectionName);
}

//set db name
void SqliteDataProvider::setDbName(const QString &name)
{
    m_dbName = name;
    m_db.setDatabaseName(m_dbName);
}

//open database
/*virtual*/ bool SqliteDataProvider::open() const
{
     if (!m_db.open()) {
         qDebug() << m_db.lastError().text();
         m_hasError = true;
         return false;
     }
     m_pQuery = new QSqlQuery (m_db);
     m_hasError = false;
     return true;
}

//close database
/*virtual*/ void SqliteDataProvider::close() const
{
    m_db.close();
}

////////////////////////////////////////////
///////////////////////////////////////////
////////////////////////////////////////////

//execute sql query like insert, delete and update
/*virtual*/ int SqliteDataProvider::executeNoneQuery(const QString &qryStr) const
{
    //return executeScalar(qryStr).toInt();
	if(m_pQuery->exec(qryStr))
	{
		m_hasError = false;	   
		return m_pQuery->numRowsAffected();
	}
	else
	{
		qDebug() << "Error = " + m_pQuery->lastError().text();
		m_hasError = true;
		return -1;
	}
}

//execute sql query like select
/*virtual*/ void SqliteDataProvider::executeReader(const QString &qryStr) const
{
    if(m_pQuery->exec(qryStr))
    {
       m_hasError = false;
    }
    else
    {
       m_hasError = true;
       qDebug() << "Error = " + m_pQuery->lastError().text();
    }
}

//execute sql query like count sum avg and etc
/*virtual*/ QVariant SqliteDataProvider::executeScalar(const QString &qryStr) const
{    
    if(m_pQuery->exec(qryStr))
    {
       m_hasError = false;	   
       m_pQuery->first() ;
       return m_pQuery->value(0);
    }
    else
    {
       qDebug() << "Error = " + m_pQuery->lastError().text();
       m_hasError = true;
       return -1;
    }
}

//has next result
/*virtual*/ bool SqliteDataProvider::hasNext() const
{
    return m_pQuery->next();
}

//get current result column value by index
/*virtual*/ QVariant SqliteDataProvider::getValue(int columnIndex) const
{
   //Q_ASSERT(cellIndex > 0);
    return m_pQuery->value(columnIndex);
}

//get current result column value by name
/*virtual*/ QVariant SqliteDataProvider::getValue(QString columnName) const
{
   QSqlRecord rec = m_pQuery->record();
   int nameCol = rec.indexOf(columnName);
   if(nameCol == -1)
       return "Not Found " + columnName;
   return m_pQuery->value(nameCol);
}

//destructor
SqliteDataProvider::~SqliteDataProvider()
{
    if(m_db.isOpen())
        close();

    if(m_pQuery)
		delete m_pQuery;    
}

bool SqliteDataProvider::hasError()
{
	return m_hasError;
}


