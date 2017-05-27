//FileName:	MaxHeap.h
//Versoin:	1.0
//Date:		2017-03-03
//Author:	Li Ning
//Brief:	max heap of a specific structure.

//#ifndef MAX_HEAP_H
//#define MAX_HEAP_H

#include <vector>
#include <string>
#include <algorithm>
//-----------------------辅助结构----------------------------
struct Kmer300
{
	unsigned long posInGW;
	//在全基因组中的位置。
	double score;
	//与正例序列计算欧式距离得分。
	std::string seq300;
	//该序列的实际内容

	Kmer300(unsigned long p, double score, std::string seq);
};

Kmer300::Kmer300(unsigned long p, double c, std::string s):
	posInGW(p), score(c), seq300(s) {}

//-----------------辅助结构可能需要移动到其他文件中----------
//---------------最大堆--------------------------------------
class MaxHeap
{
private:
	int m_size;	//堆的大小
	std::vector<Kmer300> m_heap;

public:
	MaxHeap(int size);
	double max() const;
	void push(Kmer300 && entity);

private:
	bool compare(const Kmer300 & left, const Kmer300 & right);
};

MaxHeap::MaxHeap(int size)
	: m_size(size) {}

void MaxHeap::push(Kmer300 && entity)
{
	if(m_heap.size() < m_size)
	{
		m_heap.emplace_back(entity);
		if(m_heap.size() == m_size)
		{
			std::make_heap(m_heap.begin(), m_heap.end(), compare);
		}
	}
	else
	{
		m_heap.emplace_back(entity);
		std::push_heap(m_heap.begin(), m_heap.end(), compare);
		std::pop_heap(m_heap.begin(), m_heap.end(), compare);
		m_heap.pop_back();
	}
}

double MaxHeap::max()
{
	if(m_heap.size() < m_size)
		return 99999;
	return m_heap.front().score;
}

bool MaxHeap::compare(const Kmer300 & left, const Kmer300 & right)
{
	return left.score > right.score;
}
//---------------最大堆--------------------------------------

//euclidean distance with no sqrt;
double euclideanDistance(const std::vector<double>& left, const std::vector<double>& right)
{
	double res;
	for(int i = 0; i < left.size(); i++)
	{
		res += (left[i] - right[i]) * (left[i] - right[i]);
	}
	return res;
}

//从一个fasta数据序列中得到马尔科夫表示
//k表示马尔科夫的阶数
//DNA 顺序为ACTG

std::vector<int> seqToMarkov(const int k, const std::string seq)
{
	int sizeRes = 1;
	for(int i = 0; i < k; i++)
		sizeRes *= 4;
	std::vector<int> res(sizeRes, 0);
	if(seq.size() < k)
		return res;

	int i = 0;
	int num = 0;
	for(; i < k; i++)
		num = num * 4 + ((seq[i] & 6) >> 1);
	res[num] ++;
	for(; i < seq.size(); i++)
	{
		num = num * 4 + ((seq[i] & 6) >> 1);
		num = num % sizeRes;
		res[num] ++;
	}
	return res;
}


//seq 表示序列，这里可能是全基因组序列，或其一部分
//start 和end 分别表示需要截取seq的首尾位置
//norMark表示作为标准的比较向量
//lengthK300 表示序列的长度
//order 表示马尔科夫的阶数
std::vector<Kmer300> maxKinSeq(const std::string& seq, const start, const end,
	const std::vector<double>& norMark, const int lengthK300, const int order,
	int k)
{
	MaxHeap maxheap(k);
	int pos = start;
	int sizeMark = norMark.size();
	std::vector<int> mark;
	std::string subseq;
	double score;
	int leftIndex = 0;
	int rightIndex = 0;

	subseq = seq.substr(pos, lengthK300);
	for(int i = 0; i < order; i++)
		leftIndex = leftIndex * 4 + ((subseq[i] & 6) >> 1);
	for(int i = lengthK300 - order; i < lengthK300; i++)
		rightIndex = rightIndex * 4 + ((subseq[i] & 6) >> 1);
	mark = seqToMarkov(order, subseq);
	score = euclideanDistance(norMark, normalization(mark));
	if(score < maxheap.max())
	{
		maxheap.push(Kmer300(pos, score, subseq));
	}

	for(pos++; pos <= (end - lengthK300 + 1); pos++)
	{
		leftIndex = leftIndex * 4 + ((seq[pos + order - 1] & 6) >> 1);
		leftIndex = leftIndex % sizeMark;
		mark[leftIndex]--;
		rightIndex = rightIndex * 4 + ((seq[pos + lengthK300 - 1] & 6) >> 1)
		rightIndex = rightIndex % sizeMark;
		mark[rightIndex]--;

		score = euclideanDistance(norMark, normalization(mark));
		if(score < maxheap.max())
			maxheap.push(Kmer300(pos, score, seq.substr(pos, lengthK300)));
	}

	return maxheap.m_heap;
}
