#include"tools.h"

template <typename T>
vector<T> merge_vectors(const vector<T>& vec1, const vector<T>& vec2) 
{
    set<T> unique_elements;
    for (const auto& elem : vec1) 
    {
        unique_elements.insert(elem);
    }
    for (const auto& elem : vec2) 
    {
        unique_elements.insert(elem);
    }
    vector<T> merged_vector(unique_elements.begin(), unique_elements.end());
    return merged_vector;
}