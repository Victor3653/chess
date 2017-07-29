#pragma once
#include <memory>
#include <QAbstractListModel>

class           MovesList;
class           DataBase;

struct          Figure {
    bool        side;
    unsigned    type;
    int         x;
    int         y;
};

class Logic: public QAbstractListModel
{
    Q_OBJECT
public:

    enum GlobalConstants {
        BOARD_SIZE = 8
    };

    enum Roles {
        Side = Qt::UserRole,
        Type,
        PositionX,
        PositionY
    };

    enum Types {
        Pawn,
        Rook,
        Bishop,
        Knight,
        Queen,
        King
    };

public:
    explicit   Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT);
    int                       boardSize(void) const;
    Q_PROPERTY(int gamesSaved READ gamesSaved CONSTANT);
    int                       gamesSaved(void) const;

    Q_INVOKABLE void          clear(void);
    Q_INVOKABLE bool          move(int fromX, int fromY, int toX, int toY);
    Q_INVOKABLE void          newGame(void);
    // Saves game
    Q_INVOKABLE void          saveGame(void);
    Q_INVOKABLE void          selectGame(int index);
    Q_INVOKABLE QString       gameName(int index);
    Q_INVOKABLE void          next(void);
    Q_INVOKABLE void          prev(void);

protected:
    int                       rowCount(const QModelIndex & parent) const override;
    QVariant                  data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray>    roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
    std::unique_ptr<MovesList> movesList;
    std::unique_ptr<DataBase>  db;
    int                        _turn;
    void                       _applyChanges(int x, int y, int index);
    // Saves move
    void                       _saveMove(int listID, int listIDH, int isHit, int toX, int toY);
};
