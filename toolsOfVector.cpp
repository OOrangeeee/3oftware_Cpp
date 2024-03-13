#include"tools.h"

vector<pair<int, int>> merge_vectors(const vector<pair<int, int>>& vec1, const vector<pair<int, int>>& vec2)
{
	set<pair<int, int>> unique_elements;
	for (const auto& elem : vec1)
	{
		unique_elements.insert(elem);
	}
	for (const auto& elem : vec2)
	{
		unique_elements.insert(elem);
	}
	vector<pair<int, int>> merged_vector(unique_elements.begin(), unique_elements.end());
	return merged_vector;
}