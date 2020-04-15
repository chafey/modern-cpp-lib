#include <modern_cpp_lib/lib.hpp>
#include <cstdlib> 

int add(int a, int b) {
    return a + b;
}

void rleDecode(const std::vector<unsigned char>& source, std::vector<unsigned char>& destination) 
{
    // DICOM RLE begins with a 64 byte header that includes the number of segments and the indexes
    // of the beginning of each segment.  Cast the source data to array of unsigned ints so we can
    // access the header directly
    const unsigned int* header = reinterpret_cast<const unsigned int*>(source.data());

    // Read the number of segments from the header and decode each one
    size_t numSegments = header[0];
    for(size_t segment = 0; segment < numSegments; segment++) {

        // get the index of this segment and the next one so we can determine the
        // range of bytes of this segment
        size_t segmentStart = header[1 + segment]; 
        size_t nextSegmentStart = header[1 + segment + 1];
        size_t endOfSegment = nextSegmentStart ? nextSegmentStart : source.size();

        // initialize our indexes for this segment
        size_t inIndex = segmentStart;
        size_t outIndex = segment;
        
        // decode the segment
        while(inIndex < endOfSegment) {
            char n = source[inIndex++];
            if(n >= 0 && n <= 127) {
                // no run, copy over the next n bytes
                for(size_t i=0; i < n + 1; ++i) {
                    destination[outIndex] = source[inIndex++];
                    outIndex += numSegments;
                }
            } else if(n <= -1 && n >= -127) {
                // run of n bytes
                unsigned char value = source[inIndex++];
                for(size_t j=0; j < -n + 1; ++j) {
                    destination[outIndex] = value;
                    outIndex += numSegments;
                }
            } else if(n == -128) {
                // do nothing
            }
        } 
    }
}

