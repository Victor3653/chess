#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QDateTime>

DataBase::DataBase() {
    
    // qDebug() << "App path : " << qApp->applicationDirPath();
    chessDatabase = QSqlDatabase::addDatabase("QSQLITE");
    chessDatabase.setDatabaseName("chess.db");

    if (chessDatabase.open())
        std::cout << "Database: connection ok" << std::endl;
    else
        std::cout << "Error: connection with database fail" << std::endl;
}

DataBase::~DataBase(void) {
    if (chessDatabase.isOpen()) {
        chessDatabase.close();
        std::cout << "Database: closed connection" << std::endl;
    }
}

int     DataBase::getFigureIndex(void) {
    QSqlQuery   query;

    query.prepare("SELECT ListID FROM " + currentTable + " where id = :index");
    query.bindValue(":index", _recordIndex);
    
    if (!query.exec()) {
        std::cout << "err get figure position" << std::endl;
        return (0);
    }

    query.first();
    return (query.value(query.record().indexOf("ListID")).toInt());
}

bool    DataBase::getFigurePosition(int &x, int &y) {
    QSqlQuery   query;
    
    query.prepare("SELECT PosX, PosY FROM " + currentTable + " where id = :index");
    query.bindValue(":index", _recordIndex);

    if (!query.exec()) {
        std::cout << "err get figure x & y" << std::endl;
        return false;
    }

    query.first();
    int idPosX = query.record().indexOf("PosX");
    int idPosY = query.record().indexOf("PosY");

    x = query.value(idPosX).toInt();
    y = query.value(idPosY).toInt();
    return (true);
}

void    DataBase::addToMovesHistory(Figure *figure, int listID, int isHit, int toX, int toY) {

    movesHistory << std::make_pair(PrevPosition {figure->x, figure->y, listID}, 
                                   NextPosition {toX, toY, isHit});
    qDebug() << "List id :" << listID  << "Is hit:" << isHit << "Old pos:" << figure->x << figure->y 
    << "New pos:" <<  toX << toY;
}

void    DataBase::clearMovesHistory(void) {
    movesHistory.clear();
    std::cout << "Cleared history vector" << std::endl;
}

void    DataBase::serializeMovesHistory(void) {
    
    _createTable();

    for (auto i = 0; i < movesHistory.size(); ++i){
        _insertMove(std::get<0>(movesHistory[i]), std::get<1>(movesHistory[i]));
        qDebug() << "Inserted [" << i << "] move";
    }
}

void    DataBase::_createTable(void) {
    QString     tableName(QDateTime::currentDateTime().toString("HH_mm_ss_dd_MM_yy"));

    qDebug() << "Table name: " << tableName;
    currentTable = tableName;

    QSqlQuery query;
    query.prepare("CREATE TABLE game_" + tableName +  " (   ID INTEGER PRIMARY KEY AUTOINCREMENT, \
                                                            ListID   INTEGER, \
                                                            isHit    INTEGER, \
                                                            PrevPosX INTEGER, \
                                                            PrevPosY INTEGER, \
                                                            NextPosX INTEGER, \
                                                            NextPosY INTEGER);");
    if (!query.exec())
        qDebug() << "Couldn't create the table" ;
}

int     DataBase::_countRows(void) {
    QSqlQuery   query("select count(*) from " + currentTable);

    query.first();
    return query.value(0).toInt();
}

void    DataBase::_insertMove(const PrevPosition &prev, const NextPosition &next) {
    QSqlQuery   query;

    query.prepare("INSERT INTO game_" + currentTable + " (ListID, isHit, PrevPosX, PrevPosY, NextPosX, NextPosY) VALUES \
                                                         (:ListID, :isHit, :PrevPosX, :PrevPosY, :NextPosX, :NextPosY);");
    query.bindValue(":ListID", prev.index);
    query.bindValue(":isHit", next.isHit);
    query.bindValue(":PrevPosX", prev.x);
    query.bindValue(":PrevPosY", prev.y);
    query.bindValue(":NextPosX", next.x);
    query.bindValue(":NextPosY", next.y);

    if (!query.exec())
        qDebug() << "Couldn't insert figure" << query.lastError().text();
}

int     DataBase::tablesSize(void) {
    return chessDatabase.tables().size();
}

QString DataBase::getTableName(int index) {
    if (index == 0)
        return QString("Click to select saved game");
    QString name(chessDatabase.tables().at(index).toLocal8Bit().constData());
    return name;
}

void    DataBase::setCurrentTable(int index) {
    currentTable = getTableName(index);
    _recordIndex = 1;
    _maxRecordIndex = _countRows();
}

void    DataBase::setRecordIndex(int set) {
    _recordIndex = set;
}

int     DataBase::getRecordIndex(void) {
    return _recordIndex;
}

// bool    DataBase::checkRecordIndex(int step) {
//     int     index = _recordIndex + step;

//     if (index >= _maxRecordIndex && index >= 0)
//         return true;
//     return false;
// }

void   DataBase::getRecord(int &x, int &y, int &figureIndex) {
    QSqlQuery   query;

    query.prepare("select NextPosX, NextPosY, ListID from " + currentTable + " where id = :index");
    query.bindValue(":index", _recordIndex);

    if (!query.exec())
        qDebug() << "Failed to get next record";
    query.first();
    x = query.value(query.record().indexOf("NextPosX")).toInt();
    y = query.value(query.record().indexOf("NextPosY")).toInt();
    figureIndex = query.value(query.record().indexOf("ListID")).toInt();
}
