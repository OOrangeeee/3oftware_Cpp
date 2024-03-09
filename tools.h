#include <vector>
#include <queue>
#include <utility>
#include <cstdlib> 
using namespace std;
vector<int> findShortestPath(vector<vector<char>>& map, pair<int, int> start, pair<int, int> end);
vector<pair<int, int>> findBijectiveMapping(vector<vector<char>>& map, vector<pair<int, int>>& groupA, vector<pair<int, int>>& groupB);
int manhattanDistance(const pair<int, int>& a, const pair<int, int>& b);

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