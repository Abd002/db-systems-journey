//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// count_min_sketch.cpp
//
// Identification: src/primer/count_min_sketch.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/count_min_sketch.h"

#include <algorithm>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

namespace bustub {

/**
 * Constructor for the count-min sketch.
 *
 * @param width The width of the sketch matrix.
 * @param depth The depth of the sketch matrix.
 * @throws std::invalid_argument if width or depth are zero.
 */
template <typename KeyType>
CountMinSketch<KeyType>::CountMinSketch(uint32_t width, uint32_t depth) : width_(width), depth_(depth) {
  /** @TODO(student) Implement this function! */

  if (width_ == 0 || depth_ == 0) {
    throw std::invalid_argument("Width and depth must be greater than 0.");
  }

  sketch_matrix_.reserve(depth_);
  for (size_t i = 0; i < depth_; i++) {
    sketch_matrix_.emplace_back(std::vector<uint32_t>(width_, 0));
  }

  mtx_ = std::vector<std::mutex>(depth_);

  /** @spring2026 PLEASE DO NOT MODIFY THE FOLLOWING */
  // Initialize seeded hash functions
  hash_functions_.reserve(depth_);
  for (size_t i = 0; i < depth_; i++) {
    hash_functions_.push_back(this->HashFunction(i));
  }
}

template <typename KeyType>
CountMinSketch<KeyType>::CountMinSketch(CountMinSketch &&other) noexcept : width_(other.width_), depth_(other.depth_) {
  /** @TODO(student) Implement this function! */
  sketch_matrix_ = std::move(other.sketch_matrix_);
  hash_functions_ = std::move(other.hash_functions_);
  mtx_ = std::move(other.mtx_);
}

template <typename KeyType>
auto CountMinSketch<KeyType>::operator=(CountMinSketch &&other) noexcept -> CountMinSketch & {
  /** @TODO(student) Implement this function! */
  width_ = other.width_;
  depth_ = other.depth_;
  sketch_matrix_ = std::move(other.sketch_matrix_);
  hash_functions_ = std::move(other.hash_functions_);
  mtx_ = std::move(other.mtx_);
  return *this;
}

template <typename KeyType>
void CountMinSketch<KeyType>::Insert(const KeyType &item) {
  /** @TODO(student) Implement this function! */
  for (size_t i = 0; i < depth_; i++) {
    std::lock_guard<std::mutex> lock(mtx_[i]);
    sketch_matrix_[i][hash_functions_[i](item) % width_]++;
  }
}

template <typename KeyType>
void CountMinSketch<KeyType>::Merge(const CountMinSketch<KeyType> &other) {
  if (width_ != other.width_ || depth_ != other.depth_) {
    throw std::invalid_argument("Incompatible CountMinSketch dimensions for merge.");
  }
  /** @TODO(student) Implement this function! */
  for (size_t i = 0; i < depth_; i++) {
    for (size_t j = 0; j < width_; j++) {
      sketch_matrix_[i][j] += other.sketch_matrix_[i][j];
    }
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::Count(const KeyType &item) const -> uint32_t {
  /** @TODO(student) Implement this function! */
  uint32_t mn = std::numeric_limits<uint32_t>::max();
  for (size_t i = 0; i < depth_; i++) {
    mn = std::min(mn, sketch_matrix_[i][hash_functions_[i](item) % width_]);
  }
  return mn;
}

template <typename KeyType>
void CountMinSketch<KeyType>::Clear() {
  /** @TODO(student) Implement this function! */
  for (size_t i = 0; i < depth_; i++) {
    for (size_t j = 0; j < width_; j++) {
      sketch_matrix_[i][j] = 0;
    }
  }
}

template <typename KeyType>
auto CountMinSketch<KeyType>::TopK(uint16_t k, const std::vector<KeyType> &candidates)
    -> std::vector<std::pair<KeyType, uint32_t>> {
  /** @TODO(student) Implement this function! */
  std::vector<std::pair<KeyType, uint32_t>> result;

  for (auto const &it : candidates) {
    uint32_t mn = std::numeric_limits<uint32_t>::max();
    for (size_t i = 0; i < depth_; i++) {
      mn = std::min(mn, sketch_matrix_[i][hash_functions_[i](it) % width_]);
    }
    result.push_back({it, mn});
  }
  std::sort(result.begin(), result.end(), [](auto &a, auto &b) { return a.second > b.second; });
  result.resize(k);

  return result;
}

// Explicit instantiations for all types used in tests
template class CountMinSketch<std::string>;
template class CountMinSketch<int64_t>;  // For int64_t tests
template class CountMinSketch<int>;      // This covers both int and int32_t
}  // namespace bustub
