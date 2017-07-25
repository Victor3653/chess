#include "logic.h"
#include "moveslist.h"
#include "database.h"
#include <QList>
#include <QByteArray>
#include <QHash>
#include <QListWidget>

struct              Logic::Impl {
    QList<Figure>   figures;
    int             findByPosition(int x, int y);
    bool            moveRange(int fromX, int fromY, int toX, int toY);
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

bool                Logic::Impl::moveRange(int fromX, int fromY, int toX, int toY) {

    int  stepX  = fromX < toX ? 1 : -1;
    int  stepY  = fromY < toY ? 1 : -1;

    if (fromX == toX)
        stepX = 0;
    else
        fromX += stepX;
    if (fromY == toY)
        stepY = 0;
    else
        fromY += stepY;
    while(fromX != toX || fromY != toY)
    {
        if (findByPosition(fromX, fromY) >= 0)
            return (true);
        fromX += stepX;
        fromY += stepY;
    }
    return (false);
}

Logic::Logic(QObject *parent) : QAbstractListModel(parent), impl(new Impl()), movesList(new MovesList()), db(new DataBase()) {

}

Logic::~Logic() {

}

int                 Logic::boardSize(void) const {
    return BOARD_SIZE;
}

int                 Logic::gamesSaved(void) const{
    return db->tablesSize();
}

int                 Logic::rowCount(const QModelIndex & ) const {
    return impl->figures.size();
}

QHash<int, QByteArray>       Logic::roleNames(void) const {
    QHash<int, QByteArray> names;
    names.insert(Roles::Side      , "side");
    names.insert(Roles::Type      , "type");
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
    impl->figures.clear();
    endResetModel();
    std::cout << "History" << std::endl;
    db->printMovesHistory();
    std::cout << "Clearing" << std::endl;
    db->clearMovesHistory();
    std::cout << "Cleared" << std::endl;
    db->printMovesHistory();
}

bool                Logic::move(int fromX, int fromY, int toX, int toY) {
    std::cout << fromX << " " << fromY << " " << toX << " " << toY << std::endl;

    static int      turn = 0;
    int             fromIndex = impl->findByPosition(fromX, fromY);
    int             toIndex = impl->findByPosition(toX, toY);
    bool            in_gameboard = (toX >= 0 && toX < BOARD_SIZE && toY >= 0 && toY < BOARD_SIZE);
    // What it is 42, maybe it's an end of game or begining of maybe it's a new one
    if (turn == 42 || ((bool)turn) == impl->figures[fromIndex].side)
        return false;
    
    if (fromIndex < 0 || !in_gameboard){
        std::cout << "bad margins"<< std::endl;
        return false;
    }
    // if chess piece is blocked, Knight cant be blocked
    if (impl->figures[fromIndex].type != Knight) {
        if (impl->moveRange(fromX, fromY, toX, toY)) {
            std::cout << "is blocked " << std::endl;
            return false;
        }
    }
    // if chess piece hits
    if (toIndex > 0) {
        // if pawn hits
        if (impl->figures[fromIndex].type == Pawn) {
            if (!movesList->pawnHit(fromX, fromY, toX, toY, impl->figures[fromIndex].side)) {
                std::cout << "wrong pawn hit" << std::endl;
                return false;
            }
        }
        // any other piece hits
        else {
            if (!movesList->basicMoves(fromX, fromY, toX, toY, impl->figures[fromIndex].side, impl->figures[fromIndex].type)) {
                std::cout << "wrong move" << std::endl;
                return false;
            }
        }
        // ally piece 
        if (impl->figures[fromIndex].side == impl->figures[toIndex].side) {
            std::cout << "hitting teammate" << std::endl;
            return false;
        }
        // update model that was hit
        else {
            std::cout << "hitting opponent" << std::endl;
            beginResetModel();
            if (impl->figures[toIndex].type == King) {
                std::cout << ((impl->figures[fromIndex].side == WHITE) ? "White" : "Black")  << " wins" << std::endl;
                turn = 42;
            }
            impl->figures[toIndex].x = -1;
            impl->figures[toIndex].y = -1;
            QModelIndex topLeft = createIndex(toIndex, 0);
            QModelIndex bottomRight = createIndex(toIndex, 0);
            emit dataChanged(topLeft, bottomRight);
            endResetModel();
            saveMove(toIndex);
        }
    }
    // just walking
    else {
        if (!movesList->basicMoves(fromX, fromY, toX, toY, impl->figures[fromIndex].side, impl->figures[fromIndex].type)) {
            std::cout << "Third 2" << std::endl;
            return false;
        }
    }
    // update model of piece that walked
    beginResetModel();
    impl->figures[fromIndex].x = toX;
    impl->figures[fromIndex].y = toY;
    QModelIndex topLeft = createIndex(fromIndex, 0);
    QModelIndex bottomRight = createIndex(fromIndex, 0);
    emit dataChanged(topLeft, bottomRight);
    endResetModel();

    saveMove(fromIndex);
    
    std::cout << ((impl->figures[fromIndex].side == WHITE) ? "Black" : "White")  << " turn now" << std::endl;
    if (turn != 42)
        turn = turn == 1 ? 0 : 1;

    return true;
}

void                Logic::newGame(void) {
    for (auto x = 0; x < 8; ++x) {
        impl->figures << Figure { BLACK, Pawn, x, 1};
        impl->figures << Figure { WHITE, Pawn, x, 6};
    }
    impl->figures << Figure { WHITE, Rook, 0, 7 };
    impl->figures << Figure { WHITE, Rook, 7, 7 };
    impl->figures << Figure { WHITE, Bishop, 2, 7 };
    impl->figures << Figure { WHITE, Bishop, 5, 7 };
    impl->figures << Figure { WHITE, Knight, 1, 7 };
    impl->figures << Figure { WHITE, Knight, 6, 7 };
    impl->figures << Figure { WHITE, Queen, 3, 7};
    impl->figures << Figure { WHITE, King, 4, 7};

    impl->figures << Figure { BLACK, Rook, 0, 0 };
    impl->figures << Figure { BLACK, Rook, 7, 0 };
    impl->figures << Figure { BLACK, Bishop, 2, 0 };
    impl->figures << Figure { BLACK, Bishop, 5, 0 };
    impl->figures << Figure { BLACK, Knight, 1, 0 };
    impl->figures << Figure { BLACK, Knight, 6, 0 };
    impl->figures << Figure { BLACK, Queen, 4, 0};
    impl->figures << Figure { BLACK, King, 3, 0};
}

void                Logic::saveMove(int index) {
    db->addChangesToHistory(&(impl->figures[index]), index);
}

void                Logic::saveGame(void) {
    db->serializeMovesHistory();
}

QString              Logic::gameName(int index) {
    return db->getTableName(index);
}
