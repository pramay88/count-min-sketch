#include "cms.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

CountMinSketch::CountMinSketch(std::size_t width, std::size_t depth)
    : width_(width),
      depth_(depth),
      table_(depth, std::vector<std::uint64_t>(width, 0)),
      seeds_(depth)
{
    if (width == 0 || depth == 0) {
        throw std::invalid_argument(
            "width and depth must be greater than zero"
        );
    }

    for (std::size_t i = 0; i < depth_; ++i) {
        seeds_[i] = 0x9e3779b97f4a7c15ULL * (i + 1);
    }
}

std::uint64_t CountMinSketch::hash(std::string_view key, std::uint64_t seed) const {
    // 14695981039346656037ULL - FNV-1a 64-bit offset basis
    std::uint64_t h = 14695981039346656037ULL ^ seed;

    for (unsigned char c : key) {
        h ^= c;
        h *= 1099511628211ULL;
    }

    return h;
}

void CountMinSketch::update(std::string_view key, std::uint64_t count){
    for (std::size_t row = 0; row < depth_; ++row) {
        const std::uint64_t h = hash(key, seeds_[row]);

        const std::size_t column = h % width_;

        table_[row][column] += count;
    }
}

std::uint64_t CountMinSketch::estimate(std::string_view key) const {
    std::uint64_t result = std::numeric_limits<std::uint64_t>::max();

    for (std::size_t row = 0; row < depth_; ++row) {
        const std::uint64_t h = hash(key, seeds_[row]);

        const std::size_t column = h % width_;

        result = std::min(result, table_[row][column]);
    }

    return result;
}

std::size_t CountMinSketch::width() const noexcept {
    return width_;
}

std::size_t CountMinSketch::depth() const noexcept {
    return depth_;
}