#pragma once
#ifndef STGDATASTRUCTURES_HPP_
#define STGDATASTRUCTURES_HPP_

#include <iostream>
#include <vector>
#include <ranges>
#include <string_view>

std::vector<int> join_vectors(std::vector<int>& vector1, std::vector<int>& vector2);
template<typename R1, typename R2>
requires std::ranges::input_range<R1> && std::ranges::input_range<R2>
auto join_ranges(const R1& range1, const R2& range2);

#endif //STGDATASTRUCTURES_HPP_