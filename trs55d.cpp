#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct Field {
    int id;
    int width;
    vector<int> neighbors;
};

// 解析输入文件
vector<Field> readInput(const string &filename) {
    vector<Field> fields;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        Field field;
        ss >> field.id;
        ss >> field.width;
        int neighbor;
        while (ss >> neighbor) {
            field.neighbors.push_back(neighbor);
        }
        fields.push_back(field);
    }
    return fields;
}

// 分配内存
vector<vector<int>> allocateMemory(const vector<Field> &fields) {
    // 初始化内存容器
    vector<bool> memory1B(64, false);
    vector<bool> memory2B(88, false);
    vector<bool> memory4B(68, false);
    unordered_map<int, vector<int>> allocations;

    // 记录每个字段的分配结果
    for (const auto &field : fields) {
        int bytes = field.width / 8;
        bool allocated = false;
        // 尝试分配到合适的容器
        if (bytes <= 1) {
            for (int i = 0; i < 64 && !allocated; ++i) {
                if (!memory1B[i]) {
                    memory1B[i] = true;
                    allocations[field.id] = {i};
                    allocated = true;
                }
            }
        }
        if (!allocated && bytes <= 2) {
            for (int i = 0; i < 88 && !allocated; ++i) {
                if (!memory2B[i]) {
                    memory2B[i] = true;
                    allocations[field.id] = {64 + i, 64 + i + 1};
                    allocated = true;
                }
            }
        }
        if (!allocated) {
            for (int i = 0; i < 68 && !allocated; ++i) {
                if (!memory4B[i]) {
                    memory4B[i] = true;
                    allocations[field.id] = {152 + i, 152 + i + 1, 152 + i + 2, 152 + i + 3};
                    allocated = true;
                }
            }
        }
    }
    
    // 转换结果为输出格式
    vector<vector<int>> result;
    for (const auto &field : fields) {
        vector<int> entry = {field.id};
        for (int addr : allocations[field.id]) {
            entry.push_back(addr);
        }
        result.push_back(entry);
    }

    return result;
}

// 写入输出文件
void writeOutput(const string &filename, const vector<vector<int>> &allocations) {
    ofstream file(filename);
    for (const auto &entry : allocations) {
        for (size_t i = 0; i < entry.size(); ++i) {
            if (i > 0) file << ",";
            file << entry[i];
        }
        file << "\n";
    }
}

int main() {
    string inputFile = "input.csv";
    string outputFile = "output1.csv";

    vector<Field> fields = readInput(inputFile);
    vector<vector<int>> allocations = allocateMemory(fields);
    writeOutput(outputFile, allocations);

    return 0;
}
