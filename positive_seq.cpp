struct OnePosSubSeq {
    int chr_name_;
    off_t start_;
    off_t end_;
    OnePosSubSeq(int chr_name, off_t start, off_t end):
        chr_name_(chr_name), start_(start), end_(end)
        {}
    bool operator < (const OnePosSubSeq& b) const {
        return start_ < b.start_;
    }
};

class PosSubSeq {
public:
    PosSubSeq(std::string name): name_(name) {}
    // insert one subseq
    void insert(int chr_name, off_t start, off_t end) {
        seqs_[chr_name-1].insert(OnePosSubSeq(chr_name, start, end));
    }
    // get one chrome's subseqs.
    const std::set<OnePosSubSeq>& operator [](int chr_name) {
        return seqs[chr_name-1];
    }

private:
    // which kind of cell line this postive set belong to.
    std::string name_;
    // size of positive set.
    int size_;
    // all the positive subsequences.
    std:vector<std::set<OnePosSubSeq>> seqs_(23, std::set<OnePosSubSeq>);
};



/*
chr10	77879368	77879616	.	372	.	2.39407	2.16373	2.0797	89
chr17	772849	773094	.	372	.	2.40203	2.16373	2.07971	69
chr17	13498100	13498413	.	372	.	2.40203	2.16373	2.07971	117
chr17	31236964	31237214	.	372	.	2.40203	2.16373	2.07972	128
chr17	59363382	59363646	.	372	.	2.40203	2.16373	2.07972	118
chr16	35214439	35214811	.	372	.	2.30939	2.16373	2.07972	189
chr22	41571729	41571952	.	372	.	2.42664	2.16373	2.07973	110
chr22	44578459	44578764	.	372	.	2.42664	2.16373	2.07973	129
chr6	11236696	11236873	.	372	.	2.35888	2.16373	2.07974	83
chr6	11724326	11724509	.	372	.	2.35888	2.16373	2.07974	95
chr6	35107771	35107998	.	372	.	2.35888	2.16373	2.07975	141
chr6	57396747	57396982	.	372	.	2.35888	2.16373	2.07975	35
chr6	97634366	97634630	.	372	.	2.35888	2.16373	2.07976	131
chr6	139496528	139496777	.	372	.	2.35888	2.16373	2.07976	183
chr5	654463	654662	.	372	.	2.41333	2.16373	2.07976	109
*/

// fgets
char input_line[150];
while(fgets(input_line, 150, fp) != NULL) {
    int chr_name = 23;
    off_t start = 0, end = 0;
    if(sscanf(input_line, "chr%2d\t%ld\tld%*s\n", &chr_name, &start, &end) == 0) {
        sscanf(input_line, "chrX\t%ld\t%ld%*s\n", &start, &end);
    }
    pos_set.insert(chr_name, start, end);
}
