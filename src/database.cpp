#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QApplication>



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
