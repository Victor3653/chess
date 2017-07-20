#include "moveslist.h"

MovesList::MovesList(void) {
    std::cout << "Const moves list" << std::endl;
}

MovesList::~MovesList(void) {
    std::cout << "Destr moves list" << std::endl;
}

int         MovesList::_pawn_move(int x_start, int y_start, int x_end, int y_end, bool side, int type) {

    int     delta = (side == true) ? -1 : 1;

    if ((y_start == WHITE_SP && side == WHITE) || (y_start == BLACK_SP && side == BLACK))
    {
        if ((x_start == x_end) && ((delta * 2) == (y_end - y_start)))
            return (1);
    }
    if (x_start == x_end && delta == (y_end - y_start))
        return (1);
    return(0);
 }


int         MovesList::_rook_move(int x_start, int y_start, int x_end, int y_end, bool side, int type) {

    int     y_direction = y_start - y_end;
    int     x_direction = x_start - x_end;

    if (y_direction != 0  && x_direction != 0)
        return (0);
    return (1);
}

int         MovesList::_bishop_move(int x_start, int y_start, int x_end, int y_end, bool side, int type) {

}

int         MovesList::basicMoves(int x_start, int y_start, int x_end, int y_end, bool side, int type) {

    int     ret_value;

    switch (type) {
    case Logic::Pawn:
        ret_value = _pawn_move(x_start, y_start, x_end, y_end, side, type);
        break;
    case Logic::Rook:
        ret_value = _rook_move(x_start, y_start, x_end, y_end, side, type);
        break;
    case Logic::Bishop:
        ret_value =
        break;
    default:
        ret_value = 0;
        break;
    }
    return (ret_value);
}
