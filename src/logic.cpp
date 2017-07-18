#include "logic.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <iostream>

struct                                  Figure {
    bool                                side;
    unsigned                         type;
    int                                   x;
    int                                   y;
};


struct                                  Logic::Impl {
    QList<Figure>                 figures;
    int                                   findByPosition(int x, int y);
    int                                   test(unsigned type);
};

int                                       Logic::Impl::findByPosition(int x, int y) {
    for (auto i = 0; i < figures.size(); ++i) {
        if (figures[i].x != x || figures[i].y != y ) {
            continue;
        }
        return i;
    }
    return -1;
}

int                                       Logic::Impl::test(unsigned type) {
    std::cout << "type is :" << type << std::endl;
    return (0);
}

Logic::Logic(QObject *parent) : QAbstractListModel(parent), impl(new Impl()) {
    impl->figures << Figure { white, Pawn, 1, 0};
    impl->figures << Figure { black, Pawn, 6, 7};
}

Logic::~Logic() {

}

int                                         Logic::boardSize() const {
    return BOARD_SIZE;
}

int                                         Logic::rowCount(const QModelIndex & ) const {
    return impl->figures.size();
}

QHash<int, QByteArray>       Logic::roleNames(void) const {
    QHash<int, QByteArray> names;
    names.insert(Roles::Side         , "side");
    names.insert(Roles::Type         , "type");
    names.insert(Roles::PositionX , "positionX");
    names.insert(Roles::PositionY , "positionY");
    return names;
}

QVariant                                Logic::data(const QModelIndex & modelIndex, int role) const {
    if (!modelIndex.isValid()) {
        return QVariant();
    }

    int index = static_cast<int>(modelIndex.row());

    if (index >= impl->figures.size() || index < 0) {
        return QVariant();
    }

    Figure      &figure = impl->figures[index];

    switch (role) {
    case Roles::Side     : return figure.side;
    case Roles::Type     : return figure.type;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
    }

    return QVariant();
}

void                                       Logic::clear(void) {
    beginResetModel();
    impl->figures.clear();
    endResetModel();
}

bool                                       Logic::move(int fromX, int fromY, int toX, int toY, unsigned type) {

    std::cout << fromX << " " << fromY << " " << toX << " " << toY << std::endl;
    int index = impl->findByPosition(fromX, fromY);

    impl->test(type);

    if (index < 0) {
        std::cout << "Index: " << index << std::endl;
        return false;
    }
  // bug in last function, it needs to know from pos
    if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE || impl->findByPosition(toX, toY) >= 0) {

        std::cout << "False " << std::endl;
        return false;
    }

    impl->figures[index].x = toX;
    impl->figures[index].y = toY;
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
    return true;
}
