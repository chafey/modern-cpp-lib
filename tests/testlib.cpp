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
    rleDecode(source, destination);

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