#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <set>

// Function to allocate memory for fields based on their widths
std::unordered_map<int, std::vector<int>> allocateMemory(const std::vector<std::pair<int, int>>& fields) {
    std::unordered_map<int, std::vector<int>> allocation_map;
    int current_byte = 0;

    for (const auto& field : fields) {
        int field_num = field.first;
        int field_width = field.second;

        // Calculate number of bytes needed, rounded up to whole bytes
        int num_bytes = (field_width + 7) / 8;

        // Allocate memory addresses ensuring byte alignment
        std::vector<int> allocated_bytes(num_bytes);
        for (int i = 0; i < num_bytes; ++i) {
            allocated_bytes[i] = current_byte++;
        }

        // Store allocated addresses in map
        allocation_map[field_num] = allocated_bytes;
    }

    return allocation_map;
}

// Function to generate output files
void generateOutputFiles(const std::unordered_map<int, std::vector<int>>& allocation_map, const std::vector<std::pair<int, int>>& fields) {
    // Output file 1: output1.csv (memory allocations)
    std::ofstream output1("output1.csv");
    for (const auto& entry : allocation_map) {
        output1 << entry.first;  // Field number
        for (int byte : entry.second) {
            output1 << "," << byte;
        }
        output1 << std::endl;
    }
    output1.close();

    // Output file 2: output2.csv (domain dictionaries)
    std::ofstream output2("output2.csv");
    std::unordered_map<int, std::set<int>> byte_to_fields;

    for (const auto& entry : allocation_map) {
        int field_num = entry.first;
        const std::vector<int>& allocated_bytes = entry.second;

        for (int byte : allocated_bytes) {
            byte_to_fields[byte].insert(field_num);
        }
    }

    for (const auto& entry : byte_to_fields) {
        int byte = entry.first;
        const std::set<int>& fields_set = entry.second;

        output2 << byte;
        for (int field_num : fields_set) {
            output2 << "," << field_num;
        }
        output2 << std::endl;
    }
    output2.close();
}

// Function to read input from input.csv
std::vector<std::pair<int, int>> readInput(const std::string& filename) {
    std::vector<std::pair<int, int>> fields;
    std::ifstream input(filename);

    if (!input.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return fields;
    }

    std::string line;
    while (std::getline(input, line)) {
        std::istringstream iss(line);
        int field_num, width;
        iss >> field_num >> width;

        fields.emplace_back(field_num, width);
    }

    input.close();
    return fields;
}

int main() {
    // Read input from input.csv
    std::vector<std::pair<int, int>> fields = readInput("input.csv");

    // Allocate memory for fields
    std::unordered_map<int, std::vector<int>> allocation_map = allocateMemory(fields);

    // Generate output files output1.csv and output2.csv
    generateOutputFiles(allocation_map, fields);

    std::cout << "Output files generated successfully: output1.csv and output2.csv" << std::endl;

    return 0;
}