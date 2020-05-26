#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <modern_cpp_lib/lib.hpp>
#include <fstream>
#include <iostream>

void readFile(std::string fileName, std::vector<unsigned char>& vec) {
    // open the file:
    std::ifstream file(fileName, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
                std::istream_iterator<unsigned char>(file),
                std::istream_iterator<unsigned char>());
}


enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}


TEST_CASE("RLE Decode Happy Path", "[main]") {
    
    // Read RLE and uncompressed fixtures
    std::vector<unsigned char> source;
    readFile("../../tests/fixtures/ct.rle", source);
    std::vector<unsigned char> uncompressed;
    readFile("../../tests/fixtures/ct.raw", uncompressed);

    // allocate space in the destination
    std::vector<unsigned char> destination;
    destination.resize(512 * 512 * 2);

    // decode it
    timespec start, finish, delta;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for(auto i=0; i < 1; i++) {
        rleDecode(source, destination);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &finish);
    sub_timespec(start, finish, &delta);
    printf("RleDecode took %d.%.9ld seconds\n", (int)delta.tv_sec, delta.tv_nsec);

    // verify bytes match
    REQUIRE(destination.size() == uncompressed.size());
    for(size_t i=0; i < destination.size(); i++) {
        if(destination[i] != uncompressed[i]) {
            REQUIRE(i != -1);
        }
    }
}

#ifdef CATCH_CONFIG_ENABLE_BENCHMARKING
TEST_CASE("RLE Decode Benchmark", "[main]") {
    // Read RLE and uncompressed fixtures
    std::vector<unsigned char> source;
    readFile("../../tests/fixtures/ct.rle", source);
    std::vector<unsigned char> uncompressed;
    readFile("../../tests/fixtures/ct.raw", uncompressed);

    // allocate space in the destination
    std::vector<unsigned char> destination;
    destination.resize(512 * 512 * 2);

    // decode it
    BENCHMARK("rleDecode") {
        return rleDecode(source, destination);
    };
}
#endif