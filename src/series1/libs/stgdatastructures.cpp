#include "stgdatastructures.hpp"

/**
 * Join two vectors 
 * @details Join vector 1 and vector 2
 * @author Carlos Thadeu
 * @param vector1 a vector
 * @param vector2 a vector 
 * @return a vector (concatenate vector 2 in vector 1)
 */
std::vector<int> join_vectors(const std::vector<int>& vector1, const std::vector<int>& vector2) {
    std::vector<int> result;
    result.reserve(vector1.size() + vector2.size()); // Reservar espaço para evitar realocações
    result.insert(result.end(), vector1.begin(), vector1.end()); // Inserir primeiro vetor
    result.insert(result.end(), vector2.begin(), vector2.end()); // Inserir segundo vetor
    return result;
}

/**
 * Concatenate two data structures 
 * @details Join element 1 and element 2
 * @author Carlos Thadeu
 * @param range1 a data structure
 * @param range2 a data structure
 * @return a data structure (concatenate vector 2 in vector 1)
 */
template<typename R1, typename R2>
requires std::ranges::input_range<R1> && std::ranges::input_range<R2>
auto join_ranges(const R1& range1, const R2& range2) {
    using ValueType = std::ranges::range_value_t<R1>;
    std::vector<ValueType> result;
    
    // Reservar espaço suficiente no vetor de resultados
    result.reserve(std::ranges::distance(range1) + std::ranges::distance(range2));
    
    // Copiar elementos do primeiro range para o vetor de resultados
    std::copy(std::ranges::begin(range1), std::ranges::end(range1), std::back_inserter(result));
    
    // Copiar elementos do segundo range para o vetor de resultados
    std::copy(std::ranges::begin(range2), std::ranges::end(range2), std::back_inserter(result));
    
    return result;
}

// template<typename R1, typename R2>
// requires std::ranges::input_range<R1> && std::ranges::input_range<R2>
// auto join_ranges(const R1& range1, const R2& range2) {
//     using ValueType = std::ranges::range_value_t<R1>;
//     std::vector<ValueType> result;

//     auto joined_range = std::ranges::views::concat(range1, range2);
//     result.reserve(std::ranges::distance(range1) + std::ranges::distance(range2));
//     std::ranges::copy(joined_range, std::back_inserter(result));

//     return result;
// }