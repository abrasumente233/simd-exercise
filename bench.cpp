#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <benchmark/benchmark.h>

#include "naive_base64.h"
#include "neon_base64.h"

static const char *paragraph =
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec semper ullamcorper purus, cursus feugiat odio. Nullam risus tortor, malesuada id aliquam non, posuere in tellus. Sed consectetur turpis in sem vulputate luctus. Etiam molestie, urna quis venenatis molestie, libero lectus pellentesque ipsum, congue porttitor leo libero sed nisi. Morbi justo turpis, tristique vel blandit sed, sollicitudin et erat. Maecenas sollicitudin enim turpis. Aenean metus erat, varius quis quam quis, finibus interdum arcu. Fusce convallis sagittis turpis efficitur ullamcorper. Nunc sem justo, fermentum viverra magna nec, blandit sagittis ex. Donec tincidunt arcu id diam aliquam, ac molestie dui tincidunt.\
\
Suspendisse tempus, justo eget pretium sollicitudin, metus sem eleifend sem, eget ullamcorper justo metus et lorem. Nulla auctor arcu dolor, et posuere sapien varius at. Proin ut convallis elit. Phasellus quis malesuada elit. Ut non euismod metus. Morbi mollis ante nec arcu dignissim, ut feugiat ligula dapibus. Mauris pharetra convallis orci, nec scelerisque mauris pretium id. Integer convallis fringilla purus, in mattis neque porta id. Aliquam ornare a tortor ac dictum. Etiam vitae feugiat arcu. Sed sed turpis eget nisl placerat porttitor. Duis suscipit metus et risus condimentum ullamcorper. Nulla ullamcorper lacus arcu, quis blandit purus cursus eu. Proin mollis lacinia dolor nec faucibus. Sed placerat odio est, in dictum odio egestas sed.\
\
In tristique dolor metus, volutpat porttitor libero lacinia at. Praesent fermentum vel arcu ut gravida. Sed aliquam feugiat faucibus. Donec sed auctor magna. Fusce tortor mauris, porttitor sit amet massa ac, commodo venenatis dolor. Sed pellentesque dolor commodo, elementum tellus nec, imperdiet turpis. Quisque condimentum vel quam vitae ultricies. Pellentesque ac tellus sit amet metus commodo consequat. Vestibulum rhoncus nisi sed mauris hendrerit scelerisque ut nec magna. Vivamus hendrerit porttitor efficitur. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Donec rutrum, turpis vel luctus hendrerit, nisl arcu bibendum neque, et placerat libero felis a justo. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Fusce pharetra ligula ac ipsum molestie volutpat. Integer ultrices bibendum ex. Donec dignissim hendrerit magna, at auctor leo eleifend sed.\
\
Nam id vestibulum libero. Mauris a dictum urna, quis vestibulum tellus. Pellentesque eget dui sapien. Maecenas sodales dictum elit, et tincidunt justo congue sit amet. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Donec ac ante lorem. Praesent maximus elit at metus lacinia semper. Sed diam dui, sollicitudin ac vestibulum eget, viverra iaculis est. Maecenas imperdiet arcu eget maximus vestibulum. Duis quis dolor lacinia, feugiat nunc ac, accumsan leo. Donec bibendum, nunc ut tristique dignissim, nunc nunc dignissim arcu, vel ultricies diam ipsum a metus. Ut bibendum orci quis quam ultricies faucibus. Pellentesque pulvinar id mi vitae tristique. Nulla facilisi. Donec felis risus, lacinia nec varius nec, pharetra eget lorem. Ut eu nibh sit amet nibh dignissim pulvinar eget vehicula turpis.\
\
Vivamus pretium hendrerit blandit. Sed sit amet nisi mauris. Curabitur nec turpis sit amet ex elementum suscipit tincidunt eget arcu. Duis dolor eros, lacinia sit amet sollicitudin tincidunt, finibus id risus. Quisque porttitor, sapien in placerat sodales, erat ligula dapibus ligula, quis vehicula urna dui non tortor. Vivamus eu fermentum neque, ac sagittis massa. Sed varius magna sed auctor tincidunt. Quisque est ligula, suscipit et velit ac, sagittis mattis diam.";

static inline size_t encoded_length(size_t bytes) {
    size_t round_up = (bytes + 2) / 3 * 3;
    return round_up * 4;
}

static void bench_naive(benchmark::State &state) {
    uint8_t *buf = (uint8_t *)malloc(encoded_length(strlen(paragraph)) + 1);
    size_t bytes = strlen(paragraph);

    for (auto _ : state) {
        naive_base64::encode((const uint8_t *)paragraph, bytes, buf);
        benchmark::DoNotOptimize(buf);
    }

    state.SetItemsProcessed(bytes);
}

static void bench_neon(benchmark::State &state) {
    uint8_t *buf = (uint8_t *)malloc(encoded_length(strlen(paragraph)) + 1);
    size_t bytes = strlen(paragraph);

    for (auto _ : state) {
        neon_base64::encode((const uint8_t *)paragraph, bytes, buf);
        benchmark::DoNotOptimize(buf);
    }

    state.SetItemsProcessed(bytes);
}


BENCHMARK(bench_naive);
BENCHMARK(bench_neon);

#if 1
BENCHMARK_MAIN();
#else
int main() {
    uint8_t *buf = (uint8_t *)malloc(encoded_length(strlen(paragraph)) + 1);
    size_t bytes = strlen(paragraph);
    printf("len: %zu\n", bytes);

    //naive_base64::encode((const uint8_t *)paragraph, bytes, buf);
    neon_base64::encode((const uint8_t *)paragraph, bytes, buf);
    printf("%s\n", buf);
    return 0;
}
#endif
