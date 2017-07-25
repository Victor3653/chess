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
    // QStringList     tables;
    // tables = chessDatabase.tables();
    // for (int i = 0; i < tables.size(); ++i)
    //      std::cout << tables.at(i).toLocal8Bit().constData() << endl;
}

DataBase::~DataBase(void) {
    if (chessDatabase.isOpen()) {
        chessDatabase.close();
        std::cout << "Database: closed connection" << std::endl;
    }
}

int     DataBase::getFigureIndex(int index) {
    QSqlQuery   query;

    query.prepare("SELECT ListID FROM game_" + currentTable + " where id = :index");
    query.bindValue(":index", index);
    
    if (!query.exec()) {
        std::cout << "err get figure position" << std::endl;
        return (0);
    }

    query.first();
    return (query.value(query.record().indexOf("ListID")).toInt());
}

bool    DataBase::getFigurePosition(int &x, int &y, int index) {
    QSqlQuery   query;
    
    query.prepare("SELECT PosX, PosY FROM game_" + currentTable + " where id = :index");
    query.bindValue(":index", index);

    if (!query.exec()) {
        std::cout << "err get figure position" << std::endl;
        return false;
    }

    query.first();
    int idPosX = query.record().indexOf("PosX");
    int idPosY = query.record().indexOf("PosY");

    x = query.value(idPosX).toInt();
    std::cout << "x: " << x << " ";
    y = query.value(idPosY).toInt();
    std::cout << "y: " << y << std::endl;
    return (true);
}

void    DataBase::addChangesToHistory(Figure *figure, int indexInList) {
    Figure     figureToSave;

    figureToSave.x = figure->x;
    figureToSave.y = figure->y;
    figureToSave.type = figure->type;
    figureToSave.side = figure->side;

    movesHistory << std::make_pair(indexInList, figureToSave);
}

void    DataBase::clearMovesHistory(void) {
    movesHistory.clear();
    std::cout << "Cleared history vector" << std::endl;
}

void    DataBase::serializeMovesHistory(void) {
    
    _createTable();

    for (auto i = 0; i < movesHistory.size(); ++i){
        _insertFigure(std::get<1>(movesHistory.at(i)), std::get<0>(movesHistory.at(i)));
    }
}

void    DataBase::_createTable(void) {
    QString     tableName(QDateTime::currentDateTime().toString("HH_mm_ss_dd_MM_yy"));

    qDebug() << tableName;
    currentTable = tableName;

    QSqlQuery query;
    query.prepare("CREATE TABLE game_" + tableName +  " (ID INTEGER PRIMARY KEY AUTOINCREMENT, ListID INTEGER, PosX INTEGER, PosY INTEGER);");
    if (!query.exec())
        std::cout << "Couldn't create the table" << std::endl;
}

void    DataBase::_insertFigure(const Figure &figure, int indexInList) {
    QSqlQuery   query;

    query.prepare("INSERT INTO game_" + currentTable + " (ListID, PosX, PosY) VALUES (:ListID, :x, :y);");
    query.bindValue(":ListID", indexInList);
    query.bindValue(":x", figure.x);
    query.bindValue(":y", figure.y);

    if (!query.exec())
        std::cout << "could not insert figure" << std::endl;
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
/*
** tmp
*/
void    DataBase::printMovesHistory(void) {
    for (auto i = 0; i < movesHistory.size(); ++i) {
        std::cout << std::get<0>(movesHistory.at(i)) << " ";
        std::cout << "X : " << std::get<1>(movesHistory.at(i)).x << " Y: " <<  std::get<1>(movesHistory.at(i)).y << std::endl;
    }
}
