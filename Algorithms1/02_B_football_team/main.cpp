#include <iostream>
#include <vector>
#include <algorithm>

struct Footballer {
    size_t efficiency;
    size_t index;

    Footballer() : efficiency(0), index(0) {
    }

    Footballer(size_t efficiency, size_t index) : efficiency(efficiency), index(index) {
    }

    bool operator<(const Footballer& other) const {
        return this->efficiency < other.efficiency;
    }
};

std::vector<Footballer> MakeFootballers() {
    size_t size;
    std::cin >> size;

    std::vector<Footballer> result;
    result.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        size_t value;
        std::cin >> value;
        result.push_back(Footballer(value, i));
    }

    return (result);
}

void BottomUpMerge(std::vector<Footballer>& elems, size_t i_left, size_t i_right, size_t i_end,
                   std::vector<Footballer>& buffer) {
    size_t index_left = i_left, index_right = i_right;
    for (size_t index = i_left; index < i_end; ++index) {
        if (index_left < i_right &&
            (index_right >= i_end || elems[index_left] < elems[index_right])) {
            buffer[index] = elems[index_left];
            index_left = index_left + 1;
        } else {
            buffer[index] = elems[index_right];
            index_right = index_right + 1;
        }
    }
}

void BottomUpMergeSort(std::vector<Footballer>& elems) {
    std::vector<Footballer> buffer(elems.size());
    for (size_t width = 1; width < elems.size(); width = 2 * width) {
        for (size_t index = 0; index < elems.size(); index = index + 2 * width) {
            BottomUpMerge(elems, index, std::min(index + width, elems.size()),
                          std::min(index + 2 * width, elems.size()), buffer);
        }
        std::move(buffer.begin(), buffer.end(), elems.begin());
    }
}

struct Team {
    size_t index_begin, index_end;
    int64_t efficiency;

    Team(size_t index_begin, size_t index_end, int64_t efficiency)
        : index_begin(index_begin), index_end(index_end), efficiency(efficiency) {
    }

    size_t Size() const {
        return index_end - index_begin;
    }
};

Team BestTeam(const std::vector<Footballer>& footballers) {
    if (footballers.size() == 1) {
        return Team(0, 1, footballers[0].efficiency);
    }
    Team current_team(0, 2, footballers[0].efficiency + footballers[1].efficiency);
    Team best_team = current_team;
    while (current_team.index_end < footballers.size()) {
        while (current_team.Size() >= 2 &&
               footballers[current_team.index_begin].efficiency +
                       footballers[current_team.index_begin + 1].efficiency <
                   footballers[current_team.index_end].efficiency) {
            current_team.efficiency -= footballers[current_team.index_begin++].efficiency;
        }

        current_team.efficiency += footballers[current_team.index_end++].efficiency;

        if (current_team.efficiency >= best_team.efficiency) {
            best_team = current_team;
        }
    }

    return best_team;
}

int main() {
    std::vector<Footballer> footballers = MakeFootballers();
    BottomUpMergeSort(footballers);

    Team team = BestTeam(footballers);

    std::cout << team.efficiency << std::endl;

    std::vector<size_t> team_indices(footballers.size());
    for (size_t i = team.index_begin; i < team.index_end; ++i) {
        team_indices[footballers[i].index] = 1;
    }

    for (size_t i = 0; i < team_indices.size(); ++i) {
        if (team_indices[i] > 0) {
            std::cout << i + 1 << ' ';
        }
    }

    return 0;
}
