#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

struct ListElem {
    size_t front, back;
    bool free;

    int query_index, heap_index;

    ListElem(size_t front, size_t back, bool free, size_t query_index, size_t heap_index)
        : front(front), back(back), free(free), query_index(query_index), heap_index(heap_index) {
    }
};

/*
struct HistElem {
    std::list<ListElem>::iterator list_iter;
    size_t status;  // 1 - alloc, 2 - rem, 0 - not alloc

    HistElem(std::list<ListElem>::iterator list_iter, size_t status)
        : list_iter(list_iter), status(status) {
    }
};
*/

struct HeapElem {
    std::list<ListElem>::iterator list_iter;

    explicit HeapElem(std::list<ListElem>::iterator list_iter) : list_iter(list_iter) {
    }
    size_t Front() const {
        return list_iter->front;
    }
    size_t Size() const {
        return list_iter->back - list_iter->front;
    }

    bool operator>(const HeapElem& other) const {
        if (Size() > other.Size()) {
            return false;
        } else if (Size() < other.Size()) {
            return true;
        } else if (Front() < other.Front()) {
            return false;
        } else {
            return true;
        }
    }
};

class BinaryHeap {
private:
    std::vector<HeapElem> data_;

    void Heapify(size_t index) {
        size_t left_child, right_child, smallest_child;

        smallest_child = index;
        left_child = 2 * index + 1;
        right_child = 2 * index + 2;

        if (left_child < data_.size() && data_[smallest_child] > data_[left_child]) {
            smallest_child = left_child;
        }
        if (right_child < data_.size() && data_[smallest_child] > data_[right_child]) {
            smallest_child = right_child;
        }

        while (smallest_child != index) {
            // std::swap(data_[index], data_[smallest_child]);
            Swap(index, smallest_child);

            index = smallest_child;

            smallest_child = index;
            left_child = 2 * index + 1;
            right_child = 2 * index + 2;

            if (left_child < data_.size() && data_[smallest_child] > data_[left_child]) {
                smallest_child = left_child;
            }
            if (right_child < data_.size() && data_[smallest_child] > data_[right_child]) {
                smallest_child = right_child;
            }
        }
    }

    void Swap(size_t index_first, size_t index_second) {
        std::swap(data_[index_first], data_[index_second]);

        // size_t heap_index = data_[index_first].list_iter->heap_index;
        // data_[index_first].list_iter->heap_index = data_[index_second].list_iter->heap_index;
        // data_[index_second].list_iter->heap_index = heap_index;

        data_[index_first].list_iter->heap_index = index_first;
        data_[index_second].list_iter->heap_index = index_second;

        // std::swap(data_[index_first].list_iter->heap_index,
        // data_[index_second].list_iter->heap_index);
    }

public:
    explicit BinaryHeap(size_t reserve_size = 0) {
        data_.reserve(reserve_size);
    }

    size_t Push(HeapElem elem) {
        data_.push_back(elem);

        size_t index, parent_index;
        for (index = data_.size() - 1, parent_index = (index - 1) / 2;
             index > 0 && data_[parent_index] > data_[index];
             index = parent_index, parent_index = (index - 1) / 2) {
            // std::swap(data_[index], data_[parent_index]);
            Swap(index, parent_index);
        }

        data_[index].list_iter->heap_index = index;
        return index;  // ?
    }
    HeapElem Pop() {
        HeapElem result = data_.front();
        // std::swap(data_.front(), data_.back());
        Swap(0, data_.size() - 1);

        data_.pop_back();
        Heapify(0);

        result.list_iter->heap_index = -1;
        return result;
    }
    HeapElem Pop(size_t index) {
        HeapElem result = data_[index];
        // std::swap(data_[index], data_.back());
        Swap(index, data_.size() - 1);

        data_.pop_back();
        Heapify(index);  // ? or 0

        result.list_iter->heap_index = -1;
        return result;
    }
    HeapElem Top() const {
        return data_.front();
    }
    bool Empty() {
        return data_.empty();
    }
};


const size_t kMaxm = 100000;

struct Segment {
    Segment *prev, *next;
    bool free;
    int front, back, heap_index;

    Segment(Segment *prev, Segment *next, bool free, int front, int back, int heap_index)
        : prev(prev), next(next), free(free), front(front), back(back), heap_index(heap_index) {
        if (prev) {
            prev->next = this;
        }
        if (next) {
            next->prev = this;
        }
    }

    void Remove() {
        if (prev) {
            prev->next = next;
        }
        if (next) {
            next->prev = prev;
        }
    }
};

int var_n, var_m, request_status_end, heap_end, requests_status[kMaxm];
Segment *heap[kMaxm], *requests[kMaxm];
// std::vector<Segment*> heap(kMaxm), requests(kMaxm); // delete doesnt work with vectors :(

// 0: declined, 1: allocated, 2: removed

void Swap(int first, int second) {
    Segment *t = heap[first];
    heap[first] = heap[second];
    heap[second] = t;
    heap[first]->heap_index = first;
    heap[second]->heap_index = second;
}

bool Better(int first, int second) {
    int first_size = heap[first]->back - heap[first]->front,
        second_size = heap[second]->back - heap[second]->front;
    if (first_size > second_size) {
        return true;
    }
    if (first_size < second_size) {
        return false;
    }
    if (heap[first]->front < heap[second]->front) {
        return true;
    }
    if (heap[first]->front > heap[second]->front) {
        return false;
    }
    return true;
}

void Heapify(int index) {
    bool loop = true;
    while (loop) {
        int current = index;
        if (((index << 1) + 1 < heap_end) && Better((index << 1) + 1, current)) {
            current = (index << 1) + 1;
        }
        if (((index << 1) + 2 < heap_end) && Better((index << 1) + 2, current)) {
            current = (index << 1) + 2;
        }
        if (index == current) {
            return;
        }
        Swap(index, current);
        index = current;
    }
}

void HeapRemove() {
    heap_end -= 1;
    if (heap_end) {
        Swap(0, heap_end);
        Heapify(0);
    }
}

void HeapLift(int index) {
    while (index && Better(index, (index - 1) / 2)) {
        Swap(index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

void HeapRemove(int index) {
    Swap(index, heap_end - 1);
    heap_end -= 1;
    if (index >= heap_end) {
        return;
    } else {
        HeapLift(index);
        Heapify(index);
    }
}

void HeapAdd(Segment *segment) {
    segment->heap_index = heap_end;
    heap[heap_end] = segment;
    HeapLift(heap_end++);
}

int Allocate(int memory_size) {
    Segment *segment_current = heap[0];
    if (!heap_end) {
        // allocation impossible
        requests_status[request_status_end++] = 0;

        int result = -1;
        return result;
    }
    if (segment_current->back - segment_current->front < memory_size) {
        // allocation impossible
        requests_status[request_status_end++] = 0;

        int result = -1;
        return result;
    }
    requests_status[request_status_end++] = 1;
    requests[request_status_end - 1] =
        new Segment(segment_current->prev, segment_current, false, segment_current->front,
                    segment_current->front + memory_size, -1);

    int result = segment_current->front + 1;
    segment_current->front += memory_size;
    if (segment_current->front < segment_current->back) {
        // decrease the key
        Heapify(segment_current->heap_index);

        return result;
    } else {
        // remove the segment
        segment_current->Remove();
        HeapRemove();
        delete segment_current;

        return result;
    }
}

void Deallocate(int hist_index) {
    hist_index -= 1;

    requests_status[request_status_end++] = 2;
    if (!requests_status[hist_index]) {
        return;
    }
    requests_status[hist_index] = 2;
    Segment *segment_current = requests[hist_index], *segment_prev = segment_current->prev,
            *segment_next = segment_current->next;
    bool bool_prev = segment_prev && segment_prev->free,
         bool_next = segment_next && segment_next->free;
    if (!bool_prev && !bool_next) {
        // create hist_index new segment
        segment_current->free = true;
        HeapAdd(segment_current);
        return;
    } else if (!bool_next) {
        // enlarge the previous segment
        segment_prev->back = segment_current->back;
        HeapLift(segment_prev->heap_index);
        segment_current->Remove();
        delete segment_current;
        return;
    } else if (!bool_prev) {
        // enlarge the next segment
        segment_next->front = segment_current->front;
        HeapLift(segment_next->heap_index);
        segment_current->Remove();
        delete segment_current;
        return;
    } else {
        // enlarge the previous with cur + next
        segment_prev->back = segment_next->back;
        HeapLift(segment_prev->heap_index);
        segment_current->Remove();
        delete segment_current;

        HeapRemove(segment_next->heap_index);
        segment_next->Remove();
        delete segment_next;
    }
}

int main() {
    std::cin >> var_n >> var_m;

    heap_end = 1;
    heap[0] = new Segment(nullptr, nullptr, true, 0, var_n, 0);

    for (int i = 0; i < var_m; ++i) {
        int var_t;
        std::cin >> var_t;

        if (var_t > 0) {
            int result;
            result = Allocate(var_t);
            std::cout << result << '\n';
        } else {
            Deallocate(-var_t);
        }
    }

    return 0;
}
