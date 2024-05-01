#include "intersect.h"

std::set<Point> MakePointsQueue(std::vector<Line> &lines) {
    std::set<Point> points_queue;
    for (auto &line: lines){
        points_queue.insert(line.first_point);
        points_queue.insert(line.second_point);
        points_queue.insert(line._first_point);
        points_queue.insert(line._second_point);
    }
    return points_queue;
}

std::vector<std::pair<int, int>> ComputeIntersections(std::vector<Line> lines) {
    std::vector<std::pair<Line, Line>> result;
    std::vector<Point> int_points;

    for (size_t i = 0; i < lines.size(); i++){
        lines[i].first_point.line_index = i;
        lines[i].second_point.line_index = i;
        lines[i]._second_point.line_index = i;
        lines[i]._first_point.line_index = i;
    }
    std::set<Point> points_queue = MakePointsQueue(lines);
    std::set<Line> lines_set;

    while(!points_queue.empty()) {
        auto const& cur_point = *points_queue.begin();
        points_queue.erase(points_queue.begin());
        auto const& cur_line = lines[cur_point.line_index];
        if (cur_line.type == HORIZONTAL) {
            // HORIZONTAL
            if (cur_point.type == LEFT) {
                lines_set.insert(cur_line);
            } else if (cur_point.type == RIGHT) {
                lines_set.erase(cur_line);
            }
        } else {
            // VERTICAL
            auto int_line_it = lines_set.lower_bound(cur_line);
            while (int_line_it != lines_set.end() &&
                    int_line_it->_first_point.y >= cur_line._first_point.y){
                result.emplace_back(*int_line_it, cur_line);
                int_line_it++;
            }
        }
    }
    std::vector<std::pair<int, int>> inds;
    for (auto line_pairs: result){
        inds.push_back({ line_pairs.first.first_point.line_index, line_pairs.second.first_point.line_index });
    }
    return inds;
}
