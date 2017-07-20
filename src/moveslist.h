#ifndef MOVESLIST_H
# define MOVESLIST_H

# include "logic.h"
# include <iostream>

# define WHITE_SP     6
# define BLACK_SP     1
# define WHITE          true
# define BLACK          false

class MovesList
{
public:
    MovesList(void);
    ~MovesList(void);

    int         basicMoves(int x_start, int y_start, int x_end, int y_end, bool side, int type);
private:
    int         _pawn_move(int x_start, int y_start, int x_end, int y_end, bool side, int type);
    int         _rook_move(int x_start, int y_start, int x_end, int y_end, bool side, int type);
};

#endif // MOVESLIST_H
