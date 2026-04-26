#include "priorityQueue.h"
#include <iostream>
#include <iomanip>
using namespace std;

PriorityQueue::PriorityQueue() {}

int PriorityQueue::parent(int index) {
    return (index - 1) / 2;
}

int PriorityQueue::leftChild(int index) {
    return 2 * index + 1;
}

int PriorityQueue::rightChild(int index) {
    return 2 * index + 2;
}

void PriorityQueue::swap(int i, int j) {
    LanePriority temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void PriorityQueue::heapifyUp(int index) {
    while (index > 0 && heap[parent(index)].priority < heap[index].priority) {
        swap(index, parent(index));
        index = parent(index);
    }
}

void PriorityQueue::heapifyDown(int index) {
    int maxIndex = index;
    int left = leftChild(index);
    int right = rightChild(index);
    
    if (left < heap.size() && heap[left].priority > heap[maxIndex].priority) {
        maxIndex = left;
    }
    
    if (right < heap.size() && heap[right].priority > heap[maxIndex].priority) {
        maxIndex = right;
    }
    
    if (index != maxIndex) {
        swap(index, maxIndex);
        heapifyDown(maxIndex);
    }
}

void PriorityQueue::insert(Lane* lane, double priority) {
    LanePriority lp(lane, priority);
    heap.push_back(lp);
    heapifyUp(heap.size() - 1);
}

Lane* PriorityQueue::extractMax() {
    if (isEmpty()) {
        cout << " Priority Queue is empty!" << endl;
        return nullptr;
    }
    
    Lane* maxLane = heap[0].lane;
    heap[0] = heap[heap.size() - 1];
    heap.pop_back();
    
    if (!isEmpty()) {
        heapifyDown(0);
    }
    
    return maxLane;
}

Lane* PriorityQueue::peekMax() const {
    if (isEmpty()) return nullptr;
    return heap[0].lane;
}

int PriorityQueue::size() const {
    return heap.size();
}

bool PriorityQueue::isEmpty() const {
    return heap.empty();
}

void PriorityQueue::clear() {
    heap.clear();
}

void PriorityQueue::displayQueue() const {
    cout << "\n╔═══════════════════════════════════════╗" << endl;
    cout << "║   Priority Queue (Max-Heap)           ║" << endl;
    cout << "╚═══════════════════════════════════════╝" << endl;
    
    for (int i = 0; i < heap.size(); i++) {
        cout << i + 1 << ". " << setw(10) << left << heap[i].lane->getLaneName()
             << " | Priority: " << fixed << setprecision(2) 
             << heap[i].priority << endl;
    }
    cout << endl;
}

void PriorityQueue::buildFromLanes(vector<Lane*>& lanes) {
    clear();
    for (Lane* lane : lanes) {
        double priority = lane->calculatePriority();
        insert(lane, priority);
    }
}
