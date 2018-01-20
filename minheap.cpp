class NegSubSeq {
    NegSubSeq(int size): size_(size) {}
    void insert(OneNegSubSeq&& seq) {
        minheap_.push(seq);
        if (minheap_.size() > size_) {
            minheap_.pop();
        }
    }
private:
    std::priority_queue<OneNegSubSeq, std::vector<OneNegSubSeq>, OneNegSubSeqCmp> minheap_;
    int size_;
};


struct OneNegSubSeq {
    double score;
    int chr_name;
    off_t start;
    off_t end;
};

