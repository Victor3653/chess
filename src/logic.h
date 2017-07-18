#pragma once
#include <memory>
#include <QAbstractListModel>

#define white true
#define black !true

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
        Pawn  = 0b00000000
    };

public:
    explicit   Logic(QObject *parent = 0);
    ~Logic();

    Q_PROPERTY(int boardSize READ boardSize CONSTANT);
    int                                         boardSize() const;

    Q_INVOKABLE void             clear(void);
    Q_INVOKABLE bool             move(int fromX, int fromY, int toX, int toY, unsigned type);

protected:
    int                                         rowCount(const QModelIndex & parent) const override;
    QVariant                               data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray>      roleNames() const override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};
