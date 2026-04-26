#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "lane.h"
#include <vector>
using namespace std;

struct LanePriority {
    Lane* lane;
    double priority;
    
    LanePriority(Lane* l, double p) : lane(l), priority(p) {}
};

class PriorityQueue {
private:
    vector<LanePriority> heap;
    
    void heapifyUp(int index);
    void heapifyDown(int index);
    int parent(int index);
    int leftChild(int index);
    int rightChild(int index);
    void swap(int i, int j);
    
public:
    PriorityQueue();
    
    void insert(Lane* lane, double priority);
    Lane* extractMax();
    Lane* peekMax() const;
    
    int size() const;
    bool isEmpty() const;
    void clear();
    void displayQueue() const;
    
    void buildFromLanes(vector<Lane*>& lanes);
};

#endif
