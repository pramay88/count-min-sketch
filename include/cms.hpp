#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

class CountMinSketch {
public:
    CountMinSketch(std::size_t width, std::size_t depth);

    void update(std::string_view key, std::uint64_t count = 1);

    std::uint64_t estimate(std::string_view key) const;

    std::size_t width() const noexcept;
    std::size_t depth() const noexcept;

private:
    std::size_t width_;
    std::size_t depth_;

    std::vector<std::vector<std::uint64_t>> table_;

    std::vector<std::uint64_t> seeds_;

    std::uint64_t hash(
        std::string_view key,
        std::uint64_t seed
    ) const;
};