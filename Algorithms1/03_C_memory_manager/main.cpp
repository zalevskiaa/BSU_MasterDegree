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

struct HistElem {
    std::list<ListElem>::iterator list_iter;
    size_t status;  // 1 - alloc, 2 - rem, 0 - not alloc

    HistElem(std::list<ListElem>::iterator list_iter, size_t status)
        : list_iter(list_iter), status(status) {
    }
};

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

int Allocate(std::list<ListElem>& elems_list, BinaryHeap& heap, std::vector<HistElem>& hist,
             size_t memory_size, size_t query_index) {
    // allocation declined: heap is empty
    if (heap.Empty()) {
        return -2; // before cout it will be increased by 1, so the answer is -1
    }

    HeapElem heap_elem = heap.Top();

    // allocation declined: not enough space in segment
    if (heap_elem.Size() < memory_size) {
        hist[query_index] = HistElem(elems_list.end(), 0);
        return -2;  // before cout it will be increased by 1, so the answer is -1
    }

    // case allocate whole segment
    if (heap_elem.Size() == memory_size) {
        heap_elem.list_iter->free = false;
        heap_elem.list_iter->query_index = query_index;
        heap_elem.list_iter->heap_index = -1;

        heap.Pop();

        hist[query_index] = HistElem(heap_elem.list_iter, 1);
        return heap_elem.list_iter->front;
    }

    // case allocate part of segment
    size_t alloc_front = heap_elem.list_iter->front;
    size_t alloc_back = heap_elem.list_iter->front + memory_size;
    size_t free_front = alloc_back;
    size_t free_back = heap_elem.list_iter->back;

    ListElem alloc_list_elem(alloc_front, alloc_back, false, query_index, -1);
    ListElem free_list_elem(free_front, free_back, true, query_index, -1);
    auto alloc_list_iter = elems_list.insert(heap_elem.list_iter, alloc_list_elem);
    auto free_list_iter = elems_list.insert(heap_elem.list_iter, free_list_elem);

    heap.Pop();
    // size_t heap_index =
    heap.Push(HeapElem(free_list_iter));
    // free_list_iter->heap_index = heap_index;

    elems_list.erase(heap_elem.list_iter);
    hist[query_index] = HistElem(alloc_list_iter, 1);

    return alloc_list_elem.front;
}

void Deallocate(std::list<ListElem>& elems_list, BinaryHeap& heap, std::vector<HistElem>& hist,
                int hist_index) {
    HistElem hist_elem = hist[hist_index];

    // case cant deallocate
    if (hist_elem.status != 1) {
        return;
    }

    auto list_current = hist_elem.list_iter;
    auto list_prev = (list_current != elems_list.begin() ? prev(list_current) : elems_list.end());
    auto list_next = next(list_current);

    // case simple deallocate
    if ((list_prev == elems_list.end() || !list_prev->free) &&
        (list_next == elems_list.end() || !list_next->free)) {
        list_current->free = true;

        // size_t heap_index =
        heap.Push(HeapElem(list_current));
        // list_current->heap_index = heap_index;

        list_current->query_index = hist_index;

        hist[hist_index] = HistElem(list_current, 2);
        return;
    }

    // case merge with next free
    if ((list_prev == elems_list.end() || !list_prev->free) &&
        !(list_next == elems_list.end() || !list_next->free)) {
        HeapElem heap_elem = heap.Pop(list_next->heap_index);
        list_next->front = list_current->front;
        heap.Push(heap_elem);

        elems_list.erase(list_current);
        hist[hist_index] = HistElem(list_current, 2);
        return;
    }

    // case merge with prev free
    if (!(list_prev == elems_list.end() || !list_prev->free) &&
        (list_next == elems_list.end() || !list_next->free)) {
        HeapElem heap_elem = heap.Pop(list_prev->heap_index);
        list_prev->back = list_current->back;
        heap.Push(heap_elem);

        elems_list.erase(list_current);
        hist[hist_index] = HistElem(list_current, 2);
        return;
    }

    // case merge with prev and next free
    HeapElem heap_elem_prev = heap.Pop(list_prev->heap_index);
    heap.Pop(list_next->heap_index);

    list_prev->back = list_next->back;
    heap.Push(heap_elem_prev);

    elems_list.erase(list_current);
    elems_list.erase(list_next);

    hist[hist_index] = HistElem(list_current, 2);
}

int main() {
    int var_n, var_m;
    std::cin >> var_n >> var_m;

    std::list<ListElem> elems_list;
    BinaryHeap heap(var_m);
    std::vector<HistElem> hist(var_m, HistElem(elems_list.end(), -1));
    // hist.reserve(var_m);

    elems_list.push_back(ListElem(0, var_n, true, -1, 0));
    heap.Push(HeapElem(elems_list.begin()));

    for (int i = 0; i < var_m; ++i) {
        int var_t;
        std::cin >> var_t;

        if (var_t > 0) {
            int ans;
            ans = Allocate(elems_list, heap, hist, var_t, i);
            std::cout << ans + 1 << '\n';
        } else {
            Deallocate(elems_list, heap, hist, -(var_t + 1));
        }
    }

    return 0;
}
