#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "cms.hpp"

namespace {

constexpr std::size_t DEFAULT_WIDTH = 500;
constexpr std::size_t DEPTH = 5;
constexpr std::uint64_t SEED = 123456789ULL;
constexpr std::uint64_t KEY_RANGE = 10'000;
constexpr std::uint64_t DEFAULT_N = 100'000;

using Counts = std::unordered_map<std::string, std::uint64_t>;

struct Result {
    std::uint64_t total_error = 0;
    std::uint64_t max_error = 0;

    std::size_t exact = 0;
    std::size_t overestimated = 0;
    std::size_t underestimated = 0;

    std::string max_error_key;
};

std::uint64_t parse_n(int argc, char* argv[]) {
    if (argc > 3) {
        throw std::runtime_error("Usage: cms_test [N] [WIDTH]");
    }

    if (argc == 1) {
        return DEFAULT_N;
    }

    const auto n = std::stoull(argv[1]);

    if (n == 0) {
        throw std::runtime_error("N must be greater than 0");
    }

    return n;
}

std::size_t parse_width(int argc, char* argv[]) {
    if (argc < 3) {
        return DEFAULT_WIDTH;
    }

    const auto width = std::stoull(argv[2]);

    if (width == 0) {
        throw std::runtime_error("WIDTH must be greater than 0");
    }

    return static_cast<std::size_t>(width);
}

Counts generate_uniform_stream(CountMinSketch& cms, std::uint64_t n) {
    Counts exact;
    exact.reserve(KEY_RANGE);

    // 64-bit Mersenne Twister pseudo-random number generator (PRNG)
    std::mt19937_64 rng(SEED);

    for (std::uint64_t i = 0; i < n; ++i) {
        const auto value = rng() % KEY_RANGE;
        const std::string key = std::to_string(value);

        ++exact[key];
        cms.update(key);
    }

    return exact;
}

Result compare(
    const CountMinSketch& cms,
    const Counts& exact
) {
    Result result;

    for (const auto& [key, actual] : exact) {
        const auto estimated = cms.estimate(key);

        if (estimated < actual) {
            ++result.underestimated;

            std::cerr << "CMS underestimated key " << key
                      << ": actual=" << actual
                      << ", estimated=" << estimated << '\n';

            continue;
        }

        const auto error = estimated - actual;

        result.total_error += error;

        if (error == 0) {
            ++result.exact;
        } else {
            ++result.overestimated;
        }

        if (error > result.max_error) {
            result.max_error = error;
            result.max_error_key = key;
        }
    }

    return result;
}

void print_results(
    std::uint64_t n,
    std::size_t width,
    const Counts& exact,
    const Result& result
) {
    const double average_error =
        static_cast<double>(result.total_error) /
        exact.size();

    const double exact_percentage =
        100.0 * static_cast<double>(result.exact) /
        exact.size();

    const double overestimated_percentage =
        100.0 * static_cast<double>(result.overestimated) /
        exact.size();

    std::cout << "\n========================================\n";
    std::cout << "        Count-Min Sketch Test\n";
    std::cout << "========================================\n";

    std::cout << "Stream size       : " << n << '\n';
    std::cout << "Distinct keys     : " << exact.size() << '\n';
    std::cout << "Width             : " << width << '\n';
    std::cout << "Depth             : " << DEPTH << '\n';
    std::cout << "Key range         : " << KEY_RANGE << '\n';
    std::cout << "Seed              : " << SEED << '\n';

    std::cout << "\nError statistics\n";
    std::cout << "----------------------------------------\n";

    std::cout << "Max error         : " << result.max_error;

    if (!result.max_error_key.empty()) {
        std::cout << " (key=" << result.max_error_key << ')';
    }

    std::cout << '\n';

    std::cout << "Total error       : "
              << result.total_error << '\n';

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Average error     : "
              << average_error << '\n';

    std::cout << "\nAccuracy\n";
    std::cout << "----------------------------------------\n";

    std::cout << "Exact             : "
              << result.exact
              << " (" << exact_percentage << "%)\n";

    std::cout << "Overestimated     : "
              << result.overestimated
              << " (" << overestimated_percentage << "%)\n";

    std::cout << "Underestimated    : "
              << result.underestimated << '\n';

    std::cout << "\n";

    if (result.underestimated == 0) {
        std::cout << "NO-UNDERESTIMATION TEST PASSED\n";
    } else {
        std::cout << "NO-UNDERESTIMATION TEST FAILED\n";
    }
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        const auto n = parse_n(argc, argv);
        const auto width = parse_width(argc, argv);

        CountMinSketch cms(width, DEPTH);

        const auto exact = generate_uniform_stream(cms, n);

        const auto result = compare(cms, exact);

        print_results(n, width, exact, result);

        return result.underestimated == 0 ? 0 : 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}