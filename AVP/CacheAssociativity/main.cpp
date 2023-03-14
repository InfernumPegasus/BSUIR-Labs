#include <matplot/matplot.h>

using namespace std;
using namespace chrono;
using namespace matplot;

constexpr auto MAX_CACHE_SIZE = 16 * 1024 * 1024;
constexpr auto TOTAL_ELEMENTS_IN_CACHE = MAX_CACHE_SIZE / 64;

constexpr auto PAD_SIZE = 64 - sizeof(size_t);

struct Element {
    size_t index;
    [[maybe_unused]] char pad[PAD_SIZE];
};

void init(Element *data, int associativity) {
    const auto BLOCK_SIZE_IN_BYTES = MAX_CACHE_SIZE / associativity;
    const auto BLOCK_SIZE_IN_ELEMENTS = BLOCK_SIZE_IN_BYTES / 64;
    for (size_t block = 0; block < BLOCK_SIZE_IN_ELEMENTS; ++block) {
        for (size_t element = 0; element < associativity - 1; ++element) {
            data[block + element * TOTAL_ELEMENTS_IN_CACHE].index =
                    block + (element + 1) * TOTAL_ELEMENTS_IN_CACHE;
        }
        data[block + (associativity - 1) * TOTAL_ELEMENTS_IN_CACHE].index
                = (block == BLOCK_SIZE_IN_ELEMENTS - 1) ? 0 : block + 1;
    }
}

void drawPlot(const vector<long> &accessTimeVector, int n) {
    plot(accessTimeVector);
    title("Access time");
    xlabel("N_MAX");
    xrange({2, static_cast<double>(n)});
    ylabel("Access time");
    show();
}

int main() {
    constexpr auto N_MIN = 2;
    constexpr auto N_MAX = 30;

    vector<long> accessTimeVector;
    auto *array = new(align_val_t{64}) Element[N_MAX * TOTAL_ELEMENTS_IN_CACHE];
    for (int associativity = N_MIN; associativity <= N_MAX; ++associativity) {
        init(array, associativity);
        volatile size_t t{};

        auto start = high_resolution_clock::now();
        for (int i = 0; i < 1'000'000'000; ++i) {
            t = array[t].index;
        }
        auto period = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
        cout << associativity << " : " << period << " ms\n";
        accessTimeVector.push_back(period);
    }
    operator delete(array, align_val_t{64});
    drawPlot(accessTimeVector, N_MAX);
}

/*
 *
 *
 * tlb l1 l2
 */