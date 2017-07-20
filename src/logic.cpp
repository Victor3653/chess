#include "logic.h"
#include "moveslist.h"
#include <QList>
#include <QByteArray>
#include <QHash>

struct              Figure {
    bool            side;
    unsigned        type;
    int             x;
    int             y;
};


struct              Logic::Impl {
    QList<Figure>   figures;
    int             findByPosition(int x, int y);
    int             moveRange(int fromX, int fromY, int toX, int toY, int type);
    void            restart(void);
};

int                 Logic::Impl::findByPosition(int x, int y) {

    for (auto i = 0; i < figures.size(); ++i) {
        if (figures[i].x != x || figures[i].y != y ) {
            continue;
        }
        return i;
    }
    return -1;
}

int                 Logic::Impl::moveRange(int fromX, int fromY, int toX, int toY, int type) {

    int  stepX  = fromX < toX ? 1 : -1;
    int  stepY  = fromY < toY ? 1 : -1;

    if (fromX == toX)
        stepX = 0;
    if (fromY == toY)
        stepY = 0;
    if (type == Knight) {
        fromX = toX - stepX;
        fromY = toY - stepY;
    }
    while(fromX != toX || fromY != toY)
    {
        fromX += stepX;
        fromY += stepY;
        if (findByPosition(fromX, fromY) >= 0)
            return (1);
    }
    return (0);
}

void                Logic::Impl::restart(void) {

    for (auto i = 0; i < ((figures.size() - 2) / 2); ++i) {
           if (figures[i].side == WHITE) {
               figures[i].x = i;
               figures[i].y = WHITE_SP;
           }
    }
    for (auto i = (figures.size() - 2) / 2; i < figures.size() - 2; ++i) {
           if (figures[i].side == BLACK) {
               figures[i].x = i - (figures.size() / 2);
               figures[i].y = BLACK_SP;
           }
    }
}

Logic::Logic(QObject *parent) : QAbstractListModel(parent), impl(new Impl()), movesList(new MovesList()) {

    impl->figures << Figure { WHITE, Pawn, 0, 6 };
    impl->figures << Figure { WHITE, Pawn, 1, 6 };
    impl->figures << Figure { WHITE, Pawn, 2, 6 };
    impl->figures << Figure { WHITE, Pawn, 3, 6 };
    impl->figures << Figure { WHITE, Pawn, 4, 6 };
    impl->figures << Figure { WHITE, Pawn, 5, 6 };
    impl->figures << Figure { WHITE, Pawn, 6, 6 };
    impl->figures << Figure { WHITE, Pawn, 7, 6 };

    impl->figures << Figure { BLACK, Pawn, 0, 1 };
    impl->figures << Figure { BLACK, Pawn, 1, 1 };
    impl->figures << Figure { BLACK, Pawn, 2, 1 };
    impl->figures << Figure { BLACK, Pawn, 3, 1 };
    impl->figures << Figure { BLACK, Pawn, 4, 1 };
    impl->figures << Figure { BLACK, Pawn, 5, 1 };
    impl->figures << Figure { BLACK, Pawn, 6, 1 };
    impl->figures << Figure { BLACK, Pawn, 7, 1 };

    impl->figures << Figure { WHITE, Rook, 0, 7 };
    impl->figures << Figure { WHITE, Rook, 7, 7 };

    impl->figures << Figure { BLACK, Rook, 0, 0 };
    impl->figures << Figure { BLACK, Rook, 7, 0 };

    impl->figures << Figure { WHITE, Bishop, 2, 7 };
    impl->figures << Figure { WHITE, Bishop, 5, 7 };

    impl->figures << Figure { BLACK, Bishop, 2, 0 };
    impl->figures << Figure { BLACK, Bishop, 5, 0 };

    impl->figures << Figure { WHITE, Knight, 1, 7 };
    impl->figures << Figure { WHITE, Knight, 6, 7 };

    impl->figures << Figure { BLACK, Knight, 1, 0 };
    impl->figures << Figure { BLACK, Knight, 6, 0 };
}

Logic::~Logic() {

}

int                 Logic::boardSize() const {
    return BOARD_SIZE;
}

int                 Logic::rowCount(const QModelIndex & ) const {
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

QVariant            Logic::data(const QModelIndex & modelIndex, int role) const {
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

void                Logic::clear(void) {
    beginResetModel();
    impl->restart();
    endResetModel();
}

bool                Logic::move(int fromX, int fromY, int toX, int toY, bool side, unsigned type) {


    std::cout << fromX << " " << fromY << " " << toX << " " << toY << std::endl;

    int index = impl->findByPosition(fromX, fromY);

    if (index < 0) {
        std::cout << "Index: " << index << std::endl;
        return false;
    }

    int     is_not_in_gameboard = (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE);

    if (is_not_in_gameboard)
        return false;
    else if (!movesList->basicMoves(fromX, fromY, toX, toY, side, type) || impl->moveRange(fromX, fromY, toX, toY, type))
        return false;

    beginResetModel();
    impl->figures[index].x = toX;
    impl->figures[index].y = toY;
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
    endResetModel();
    return true;
}
