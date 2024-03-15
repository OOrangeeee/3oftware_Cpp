#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <queue>
#include <utility>
#include <map>
#include <cstdlib> 
#include <unordered_map>
#include <list>
#include <set>
#include <algorithm>
#include <stack>
using namespace std;
vector<int> findShortestPath(vector<vector<char>>& map, pair<int, int> start, pair<int, int> end);
vector<pair<int, int>> findBijectiveMapping(vector<vector<char>>& map, vector<pair<int, int>>& groupA, vector<pair<int, int>>& groupB);
vector<pair<int, int>> findAllPairDuplicates(const vector<pair<int, int>>& vec);
vector<pair<int, int>> find_equal_pairs(const vector<pair<int, int>>& arrayA, const vector<pair<int, int>>& arrayB);
vector<pair<int, int>> merge_vectors(const vector<pair<int, int>>& vec1, const vector<pair<int, int>>& vec2);
vector<vector<pair<int, int>>> partitionGround(vector<vector<char>>& ground);
void removeIndices(vector<vector<pair<int, int>>>& r, vector<int>& indices);
void removeIndices_for_2(vector<vector<int>>& r, vector<int>& indices);
void removeIndices_for_3(vector<int>& r, vector<int>& indices);
void removeIndices_for_4(vector<pair<int, int>>& r, vector<int>& indices);


#ifndef SORTED_LIST_H
#define SORTED_LIST_H
template <typename T>
class SortedList
{
private:
    vector<T> data;

public:
    void insert(const T& value)
    {
        auto it = upper_bound(data.begin(), data.end(), value, greater<T>());
        data.insert(it, value);
    }

    void remove(const T& value)
    {
        auto it = find(data.begin(), data.end(), value);
        if (it != data.end())
        {
            data.erase(it);
        }
    }

    void removeAt(size_t index)
    {
        if (index < data.size())
        {
            data.erase(data.begin() + index);
        }
    }

    bool contains(const T& value) const
    {
        return binary_search(data.begin(), data.end(), value, greater<T>());
    }

    size_t size() const
    {
        return data.size();
    }

    bool empty() const
    {
        return data.empty();
    }

    const T& operator[](size_t index) const
    {
        return data[index];
    }

    T& operator[](size_t index)
    {
        return data[index];
    }

    typename vector<T>::const_iterator begin() const
    {
        return data.begin();
    }

    typename vector<T>::const_iterator end() const
    {
        return data.end();
    }
};
#endif // SORTED_LIST_H
