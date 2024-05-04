#include <iostream>

#include <vector>
#include <array>
#include <queue>
#include <algorithm>

enum RobotStatus { TL, TR, RL, RR, BL, BR, LL, LR };
const int kRobotStatusSize = 8;

struct Coordinates {
    int row, col;
};

struct Cell {
    char type;  // '.' or '#'
    std::array<bool, kRobotStatusSize> used;
    std::array<int, kRobotStatusSize> path_size;
    std::array<Coordinates, kRobotStatusSize> prev_crd;
    std::array<RobotStatus, kRobotStatusSize> prev_status;

    Cell() {
        used.fill(false);
    }
};

bool Update(std::vector<std::vector<Cell>>& field, const Coordinates& cur_crd,
            const Coordinates& next_crd, RobotStatus cur_status, RobotStatus next_status) {
    int path_size = field[cur_crd.row][cur_crd.col].path_size[cur_status];
    if (!field[next_crd.row][next_crd.col].used[next_status]) {
        field[next_crd.row][next_crd.col].path_size[next_status] = path_size + 1;

        field[next_crd.row][next_crd.col].prev_crd[next_status] = {cur_crd.row, cur_crd.col};
        field[next_crd.row][next_crd.col].prev_status[next_status] = cur_status;
        field[next_crd.row][next_crd.col].used[next_status] = true;
        return true;
    } else {
        if (path_size + 1 < field[next_crd.row][next_crd.col].path_size[next_status]) {
            field[next_crd.row][next_crd.col].path_size[next_status] = path_size + 1;

            field[next_crd.row][next_crd.col].prev_crd[next_status] = {cur_crd.row, cur_crd.col};
            field[next_crd.row][next_crd.col].prev_status[next_status] = cur_status;
            return true;
        }
    }
    return false;
}

int main() {
    int row_count, col_count;

    std::cin >> row_count >> col_count;

    std::vector<std::vector<Cell>> field(row_count, std::vector<Cell>(col_count));

    for (int row = 0; row < row_count; ++row) {
        for (int col = 0; col < col_count; ++col) {
            std::cin >> field[row][col].type;
        }
    }

    int s_row, s_col, f_row, f_col;
    std::cin >> s_row >> s_col >> f_row >> f_col;
    --s_row;
    --s_col;
    --f_row;
    --f_col;

    // calc

    std::queue<std::pair<Coordinates, RobotStatus>> queue_for_bfs;
    for (int status = 0; status < kRobotStatusSize; ++status) {
        field[s_row][s_col].path_size[status] = 0;
        field[s_row][s_col].used[status] = true;
        field[s_row][s_col].prev_crd[status] = {-1, -1};
        // field[s_row][s_col].prev_status[status] = 0;
        queue_for_bfs.push({{s_row, s_col}, static_cast<RobotStatus>(status)});
    }

    while (!queue_for_bfs.empty()) {
        std::pair<Coordinates, RobotStatus> queue_elem = queue_for_bfs.front();
        Coordinates& cur_crd = queue_elem.first;
        RobotStatus& cur_status = queue_elem.second;

        Coordinates next_crd;

        // go up
        next_crd = {cur_crd.row - 1, cur_crd.col};
        if (next_crd.row >= 0 && field[next_crd.row][next_crd.col].type == '.') {
            switch (cur_status) {
                case TL:
                    if (Update(field, cur_crd, next_crd, cur_status, TL)) {
                        queue_for_bfs.push({next_crd, TL});
                    }
                    break;
                case TR:
                    if (Update(field, cur_crd, next_crd, cur_status, TR)) {
                        queue_for_bfs.push({next_crd, TR});
                    }
                    break;
                case RL:
                    if (Update(field, cur_crd, next_crd, cur_status, TR)) {
                        queue_for_bfs.push({next_crd, TR});
                    }
                    break;
                case LR:
                    if (Update(field, cur_crd, next_crd, cur_status, TL)) {
                        queue_for_bfs.push({next_crd, TL});
                    }
                    break;
                case BL:
                    if (Update(field, cur_crd, next_crd, cur_status, TL)) {
                        queue_for_bfs.push({next_crd, TL});
                    }
                    break;
                case BR:
                    if (Update(field, cur_crd, next_crd, cur_status, TR)) {
                        queue_for_bfs.push({next_crd, TR});
                    }
                    break;
                default:
                    break;
            }
        }

        // go right
        next_crd = {cur_crd.row, cur_crd.col + 1};
        if (next_crd.col < col_count && field[next_crd.row][next_crd.col].type == '.') {
            switch (cur_status) {
                case RL:
                    if (Update(field, cur_crd, next_crd, cur_status, RL)) {
                        queue_for_bfs.push({next_crd, RL});
                    }
                    break;
                case RR:
                    if (Update(field, cur_crd, next_crd, cur_status, RR)) {
                        queue_for_bfs.push({next_crd, RR});
                    }
                    break;
                case LL:
                    if (Update(field, cur_crd, next_crd, cur_status, RL)) {
                        queue_for_bfs.push({next_crd, RL});
                    }
                    break;
                case LR:
                    if (Update(field, cur_crd, next_crd, cur_status, RR)) {
                        queue_for_bfs.push({next_crd, RR});
                    }
                    break;
                case TR:
                    if (Update(field, cur_crd, next_crd, cur_status, RL)) {
                        queue_for_bfs.push({next_crd, RL});
                    }
                    break;
                case BL:
                    if (Update(field, cur_crd, next_crd, cur_status, RR)) {
                        queue_for_bfs.push({next_crd, RR});
                    }
                    break;
                default:
                    break;
            }
        }

        // go bottom
        next_crd = {cur_crd.row + 1, cur_crd.col};
        if (next_crd.row < row_count && field[next_crd.row][next_crd.col].type == '.') {
            switch (cur_status) {
                case BL:
                    if (Update(field, cur_crd, next_crd, cur_status, BL)) {
                        queue_for_bfs.push({next_crd, BL});
                    }
                    break;
                case BR:
                    if (Update(field, cur_crd, next_crd, cur_status, BR)) {
                        queue_for_bfs.push({next_crd, BR});
                    }
                    break;
                case TL:
                    if (Update(field, cur_crd, next_crd, cur_status, BL)) {
                        queue_for_bfs.push({next_crd, BL});
                    }
                    break;
                case TR:
                    if (Update(field, cur_crd, next_crd, cur_status, BR)) {
                        queue_for_bfs.push({next_crd, BR});
                    }
                    break;
                case LL:
                    if (Update(field, cur_crd, next_crd, cur_status, BR)) {
                        queue_for_bfs.push({next_crd, BR});
                    }
                    break;
                case RR:
                    if (Update(field, cur_crd, next_crd, cur_status, BL)) {
                        queue_for_bfs.push({next_crd, BL});
                    }
                    break;
                default:
                    break;
            }
        }

        // go left
        next_crd = {cur_crd.row, cur_crd.col - 1};
        if (next_crd.col >= 0 && field[next_crd.row][next_crd.col].type == '.') {
            switch (cur_status) {
                case LL:
                    if (Update(field, cur_crd, next_crd, cur_status, LL)) {
                        queue_for_bfs.push({next_crd, LL});
                    }
                    break;
                case LR:
                    if (Update(field, cur_crd, next_crd, cur_status, LR)) {
                        queue_for_bfs.push({next_crd, LR});
                    }
                    break;
                case RL:
                    if (Update(field, cur_crd, next_crd, cur_status, LL)) {
                        queue_for_bfs.push({next_crd, LL});
                    }
                    break;
                case RR:
                    if (Update(field, cur_crd, next_crd, cur_status, LR)) {
                        queue_for_bfs.push({next_crd, LR});
                    }
                    break;
                case TL:
                    if (Update(field, cur_crd, next_crd, cur_status, LR)) {
                        queue_for_bfs.push({next_crd, LR});
                    }
                    break;
                case BR:
                    if (Update(field, cur_crd, next_crd, cur_status, LL)) {
                        queue_for_bfs.push({next_crd, LL});
                    }
                    break;
                default:
                    break;
            }
        }
        queue_for_bfs.pop();
    }

    // path_size

    bool path_found = false;
    int path_size = -1;
    RobotStatus final_status = TL;
    for (int status = 0; status < kRobotStatusSize; ++status) {
        if (field[f_row][f_col].used[status]) {
            if (!path_found) {
                path_size = field[f_row][f_col].path_size[status];

                final_status = static_cast<RobotStatus>(status);
                path_found = true;
            } else {
                if (field[f_row][f_col].path_size[status] < path_size) {
                    path_size = field[f_row][f_col].path_size[status];
                    final_status = static_cast<RobotStatus>(status);
                }
            }
        }
    }

    std::cout << path_size << '\n';

    // debug print
    /*
    for (int row = 0; row < row_count; ++row) {
        for (int col = 0; col < col_count; ++col) {
            for (int status = 0; status < kRobotStatusSize; ++status) {
                std::cout << "r: " << row << "   c: " << col << "   s: " << status << "   "
                          << static_cast<bool>(field[row][col].used[status]) << " "
                          << "   path: " << field[row][col].path_size[status] << " ("
                          << field[row][col].prev_crd[status].row << ' '
                          << field[row][col].prev_crd[status].col << ") "
                          << field[row][col].prev_status[status] << '\n_val';
            }
            std::cout << '\n_val';
        }
    }
    */
    // return 0;

    // path

    if (path_found) {
        Coordinates cur_crd = {f_row, f_col};
        RobotStatus cur_status = final_status;

        std::vector<Coordinates> reversed_path;
        reversed_path.reserve(path_size + 1);
        reversed_path.push_back(cur_crd);

        while (field[cur_crd.row][cur_crd.col].path_size[cur_status] > 0) {
            Coordinates prev_crd = field[cur_crd.row][cur_crd.col].prev_crd[cur_status];
            RobotStatus prev_status = field[cur_crd.row][cur_crd.col].prev_status[cur_status];

            cur_crd = prev_crd;
            cur_status = prev_status;
            reversed_path.push_back(cur_crd);
        }

        for (int index = reversed_path.size() - 1; index >= 0; --index) {
            std::cout << reversed_path[index].row + 1 << ' ' << reversed_path[index].col + 1
                      << '\n';
        }
    }

    return 0;
}
