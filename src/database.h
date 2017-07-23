#ifndef DATABASE_H
#define DATABASE_H

# include "logic.h"
# include <iostream>
# include <QSqlDatabase>
# include <QSqlQuery>
# include <QSqlRecord>

# define DBPATH     "chess.db"

class DataBase
{
public:
    DataBase(void);
    ~DataBase(void);
    bool            get_next_record(bool &side, int &type, int &x, int &y);

private:
    QSqlDatabase    chessDatabase;
};

#endif
