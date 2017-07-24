#include "logic.h"
#include "moveslist.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    Logic logic;
    std::cout << "before" << std::endl;
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("logic", &logic);
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    return app.exec();
}
