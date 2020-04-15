#pragma once

#include <vector>

/// \brief Adds two numbers together and returns the result.
///
int add(int a, int b);

/**
 * Decode DICOM RLE encoded data in source into destination.
 * http://dicom.nema.org/medical/dicom/2017e/output/chtml/part05/chapter_G.html
 * 
 * @param source DICOM RLE compressed data for a single image frame
 * @param destination presized vector that will receive the decoded image frame
 */

void rleDecode(const std::vector<unsigned char>& source, std::vector<unsigned char>& destination); 
