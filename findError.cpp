#include"tools.h"

// 定义用于std::pair<int, int>的哈希函数
struct pair_hash 
{
    template <class T1, class T2>
    size_t operator () (const pair<T1, T2>& pair) const 
    {
        auto hash1 = hash<T1>{}(pair.first);
        auto hash2 = hash<T2>{}(pair.second);
        return hash1 ^ hash2;
    }
};

vector<pair<int, int>> findAllPairDuplicates(const vector<pair<int, int>>& vec) 
{
    unordered_map<pair<int, int>, list<int>, pair_hash> indexMap;
    vector<pair<int, int>> duplicates;

    for (int i = 0; i < vec.size(); ++i) 
    {
        auto& indices = indexMap[vec[i]];
        for (int prevIndex : indices)
        {
            duplicates.emplace_back(prevIndex, i);
        }
        indices.push_back(i);
    }
    return duplicates;
}

vector<pair<int, int>> find_equal_pairs(const vector<pair<int, int>>& arrayA, const vector<pair<int, int>>& arrayB)
{
	vector<pair<int, int>> result;
	set<pair<int, int>> added;
	if (arrayA.size() != arrayB.size())
	{
		return result;
	}
	for (size_t i = 0; i < arrayA.size(); ++i)
	{
		for (size_t j = 0; j < arrayB.size(); ++j)
		{
			if (arrayA[i] == arrayB[j] && arrayA[j] == arrayB[i])
			{
				pair<int, int> to_add = make_pair(min(i, j), max(i, j));
				if (added.find(to_add) == added.end())
				{
					result.push_back(to_add);
					added.insert(to_add);
				}
			}
		}
	}
	return result;
}