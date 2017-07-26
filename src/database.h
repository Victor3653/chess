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

struct PrevPosition {
    int x;
    int y;
    int index;
};

struct NextPosition {
    int x;
    int y;  
    int isHit;
};

class DataBase
{
public:
    DataBase(void);
    ~DataBase(void);
    int             getFigureIndex(void);
    bool            getFigurePosition(int &x, int &y);
    void            printMovesHistory(void);
    int             tablesSize(void);
    QString         getTableName(int index);
    void            setCurrentTable(int index);
    // saves move to vector
    void            addToMovesHistory(Figure *figure, int listID, int isHit, int toX, int toY);
    void            clearMovesHistory(void);
    // saves to db moves history
    void            serializeMovesHistory(void);
    // set & get for recordIndex
    void            setRecordIndex(int set);
    int             getRecordIndex(void);
    void            getRecord(int &x, int &y, int &figureIndex);

private:
    void            _createTable(void);
    int             _countRows(void);
    void            _insertMove(const PrevPosition &prev, const NextPosition &next);
    QSqlDatabase    chessDatabase;
    QString         currentTable;
    int             _recordIndex;
    int             _maxRecordIndex;
    QVector<std::pair<PrevPosition, NextPosition>> movesHistory;
};

#endif
