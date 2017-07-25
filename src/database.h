#ifndef DATABASE_H
#define DATABASE_H

# include "logic.h"
# include <QSqlDatabase>
# include <QSqlQuery>
# include <QSqlRecord>
# include <QVector>
# include <iostream>
# include <utility>

# define DBPATH     "chess.db"


class DataBase
{
public:
    DataBase(void);
    ~DataBase(void);
    int             getFigureIndex(int index);
    bool            getFigurePosition(int &x, int &y, int index);
    void            addChangesToHistory(Figure *figure, int indexInList);
    void            clearMovesHistory(void);
    void            printMovesHistory(void);
    int             tablesSize(void);
    QString         getTableName(int index);
    void            serializeMovesHistory(void);

private:
    void            _createTable(void);
    void            _insertFigure(const Figure &figure, int indexInList);
    QSqlDatabase    chessDatabase;
    QString         currentTable;
    QVector<std::pair<int, Figure>> movesHistory;
};

#endif
