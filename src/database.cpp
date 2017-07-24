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

bool    DataBase::get_next_record(bool &side, int &type, int &x, int &y) {

    QSqlQuery   query("SELECT * FROM newGame where id = 1");

    query.first();
    int idSide = query.record().indexOf("Side");
    int idType = query.record().indexOf("Type");
    int idPosX = query.record().indexOf("PosX");
    int idPosY = query.record().indexOf("PosY");

    side = query.value(idSide).toBool();
    std::cout << "Side is: " << (side == true ? "white" : "black") << " ";
    type = query.value(idType).toInt();
    std::cout << "type is: " << type << " ";
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


/*
** tmp
*/
void    DataBase::printMovesHistory(void) {
    for (auto i = 0; i < movesHistory.size(); ++i) {
        std::cout << std::get<0>(movesHistory.at(i)) << " ";
        std::cout << "X : " << std::get<1>(movesHistory.at(i)).x << " Y: " <<  std::get<1>(movesHistory.at(i)).y << std::endl;
    }
}
