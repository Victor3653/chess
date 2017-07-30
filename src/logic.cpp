#include "logic.h"
#include "moveslist.h"
#include "database.h"

struct              Logic::Impl {
    QList<Figure>   figures;
    int32_t         findByPosition(int32_t x, int32_t y);
    bool            moveRange(int32_t fromX, int32_t fromY, int32_t toX, int32_t toY);
};

int32_t             Logic::Impl::findByPosition(int32_t x, int32_t y) {
    for (int32_t i = 0; i < figures.size(); ++i) {
        if (figures[i].x != x || figures[i].y != y ) {
            continue;
        }
        return (i);
    }
    return (-1);
}

bool                Logic::Impl::moveRange(int32_t fromX, int32_t fromY, int32_t toX, int32_t toY) {

    int32_t  stepX = fromX < toX ? 1 : -1;
    int32_t  stepY = fromY < toY ? 1 : -1;

    if (fromX == toX)
        stepX = 0;
    else
        fromX += stepX;
    if (fromY == toY)
        stepY = 0;
    else
        fromY += stepY;
    while((fromX != toX || fromY != toY) && fromX < BOARD_SIZE && fromY < BOARD_SIZE)
    {
        if (findByPosition(fromX, fromY) >= 0)
            return (true);
        fromX += stepX;
        fromY += stepY;
        qDebug() << "here";
    }
    return (false);
}

Logic::Logic(QObject *parent) : QAbstractListModel(parent), 
                                impl(new Impl()), 
                                movesList(new MovesList()), 
                                db(new DataBase()) {
}

Logic::~Logic() {

}

int                 Logic::boardSize(void) const {
    return (BOARD_SIZE);
}

int                 Logic::gamesSaved(void) const{
    return (db->tablesSize());
}

void                Logic::clear(void) {
    beginResetModel();
    impl->figures.clear();
    endResetModel();
    db->clearMovesHistory();
    _turn = 0;
    qDebug() << "Logic: chess board is empty";
}

bool                Logic::move(int fromX, int fromY, int toX, int toY) {
    int32_t         fromIndex = impl->findByPosition(fromX, fromY);
    int32_t         toIndex = impl->findByPosition(toX, toY);
    bool             in_gameboard = (toX >= 0 && toX < BOARD_SIZE && toY >= 0 && toY < BOARD_SIZE);
    int32_t         isHit = 0;
    // What it is 42, maybe it's an end of game or begining or maybe it's a new one
    if (_turn == 42 || ((bool)_turn) == impl->figures[fromIndex].side)
        return (false);
    
    if (fromIndex < 0 || !in_gameboard) {
        qDebug() << "Logic: bad margins";
        return (false);
    }
    // if chess piece is blocked, Knight cant be blocked
    if (impl->figures[fromIndex].type != Knight) {
        if (impl->moveRange(fromX, fromY, toX, toY)) {
            qDebug() << "Logic: move is blocked ";
            return (false);
        }
    }
    // if chess piece hits
    if (toIndex > 0) {
        // if pawn hits
        if (impl->figures[fromIndex].type == Pawn) {
            if (!movesList->pawnHit(fromX, fromY, toX, toY, impl->figures[fromIndex].side)) {
                qDebug() << "Logic: wrong pawn hit";
                return (false);
            }
        }
        // any other piece hits
        else {
            if (!movesList->basicMoves(fromX, fromY, toX, toY, impl->figures[fromIndex].side, impl->figures[fromIndex].type)) {
                qDebug() << "Logic: wrong hit move";
                return (false);
            }
        }
        // ally piece 
        if (impl->figures[fromIndex].side == impl->figures[toIndex].side) {
            qDebug() << "Logic: can't hit ally";
            return (false);
        }
        // update model that was hit
        else {
            qDebug() << "Logic: hitting opponent";
            if (impl->figures[toIndex].type == King) {
                std::cout << ((impl->figures[fromIndex].side == WHITE) ? "White" : "Black")  << " wins" << std::endl;
                _turn = 42;
            }
            isHit = 1;
        }
    }
    // just walking
    else {
        if (!movesList->basicMoves(fromX, fromY, toX, toY, impl->figures[fromIndex].side, impl->figures[fromIndex].type)) {
            qDebug() << "Logic: wrong move";
            return (false);
        }
    }
    // update model of piece that walked
    _saveMove(fromIndex, toIndex, isHit, toX, toY);
    if (isHit)
        _applyChanges(-1, -1, toIndex);
    _applyChanges(toX, toY, fromIndex);
    
    qDebug() << ((impl->figures[fromIndex].side == WHITE) ? "Black" : "White")  << " turn now";
    if (_turn != 42)
        _turn = _turn == 1 ? 0 : 1;
    return (true);
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
    _turn = 0;
    qDebug() << "Logic: new game created";
}

void                Logic::saveGame(void) {
    db->serializeMovesHistory();
}

void                Logic::selectGame(int index) {
    db->setCurrentTable(index);
    _turn = 42;
}

QString             Logic::gameName(int index) {
    return db->getTableName(index);
}

void                Logic::next(void) {
    int32_t     x = 0;
    int32_t     y = 0;
    int32_t     figureIndex = 0;
    int32_t     toIndex = 0;

    db->setRecordIndex(db->getRecordIndex() + 1);
    if (db->getFigureIndexRecord(figureIndex, false) && db->getRecord(x, y, true)) {
        qDebug() << "Logic: next";
        toIndex = impl->findByPosition(x, y);
        if (toIndex > 0) {
            qDebug() << "Logic: get hit record";
            _applyChanges(-1, -1, toIndex);
        }
        _applyChanges(x, y, figureIndex);
    }
}

void                Logic::prev(void) {
    int32_t     x = 0;
    int32_t     y = 0;
    int32_t     figureIndex = 0;

    if (db->getFigureIndexRecord(figureIndex, false) && db->getRecord(x, y, false)) {
        _applyChanges(x, y, figureIndex);
        qDebug() << "Logic: prev";
        if (db->isRecordOfHIt()) {
            qDebug() << "Logic: get hit record";
            if (db->getFigureIndexRecord(figureIndex, true) && db->getRecord(x, y, true))
                _applyChanges(x, y, figureIndex);
        }
        db->setRecordIndex(db->getRecordIndex() - 1);
    }
}

int                 Logic::rowCount(const QModelIndex & ) const {
    return (impl->figures.size());
}

QHash<int, QByteArray>       Logic::roleNames(void) const {
    QHash<int, QByteArray> names;
    names.insert(Roles::Side      , "side");
    names.insert(Roles::Type      , "type");
    names.insert(Roles::PositionX , "positionX");
    names.insert(Roles::PositionY , "positionY");
    return (names);
}

QVariant            Logic::data(const QModelIndex & modelIndex, int role) const {
    if (!modelIndex.isValid()) {
        return (QVariant());
    }

    int index = static_cast<int>(modelIndex.row());

    if (index >= impl->figures.size() || index < 0) {
        return (QVariant());
    }

    Figure      &figure = impl->figures[index];

    switch (role) {
    case Roles::Side     : return figure.side;
    case Roles::Type     : return figure.type;
    case Roles::PositionX: return figure.x;
    case Roles::PositionY: return figure.y;
    }

    return (QVariant());
}

void                Logic::_applyChanges(int32_t x, int32_t y, int32_t index) {
    beginResetModel();
    impl->figures[index].x = x;
    impl->figures[index].y = y;
    QModelIndex topLeft = createIndex(index, 0);
    QModelIndex bottomRight = createIndex(index, 0);
    emit dataChanged(topLeft, bottomRight);
    endResetModel();
}

void                Logic::_saveMove(int32_t listID, int32_t listIDH, int32_t isHit, int32_t toX, int32_t toY) {
    if (isHit) {
        db->addToMovesHistory(&impl->figures[listID], listID, &impl->figures[listIDH], listIDH);
        qDebug() << "Logic: saving move";
    }
    else {
        db->addToMovesHistory(&impl->figures[listID], listID, toX, toY);
        qDebug() << "Logic: saving hit move";
    }
}
