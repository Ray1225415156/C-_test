#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

// 定义字段结构体
struct Field {
    int id;
    int size;
    unordered_set<int> dependencies;
};

// 内存容器结构体
struct MemoryContainer {
    int size;
    vector<int> allocatedFields;
};

// 从输入文件中读取字段信息和依赖关系
pair<vector<Field>, unordered_map<int, unordered_set<int>>> readInput(const string& filename) {
    ifstream file(filename);
    vector<Field> fields;
    unordered_map<int, unordered_set<int>> adjacencyList;

    if (file.is_open()) {
        int numFields;
        file >> numFields;

        fields.resize(numFields + 1);  // 从1开始编号字段

        for (int i = 1; i <= numFields; ++i) {
            int fieldId, size, numDependencies;
            file >> fieldId >> size >> numDependencies;

            fields[fieldId].id = fieldId;
            fields[fieldId].size = size;

            for (int j = 0; j < numDependencies; ++j) {
                int dependency;
                file >> dependency;
                fields[fieldId].dependencies.insert(dependency);
                adjacencyList[dependency].insert(fieldId);
            }
        }

        file.close();
    } else {
        cerr << "Unable to open input file: " << filename << endl;
    }

    return { fields, adjacencyList };
}

vector<int> topologicalSort(const vector<Field>& fields, const unordered_map<int, unordered_set<int>>& adjacencyList) {
    vector<int> topoOrder;
    queue<int> q;
    unordered_map<int, int> inDegree;

    // 计算每个节点的入度
    for (const auto& entry : adjacencyList) {
        int node = entry.first;
        const auto& dependencies = entry.second;
        inDegree[node] += dependencies.size();
    }

    // 将入度为零的节点推入队列
    for (const auto& field : fields) {
        if (field.dependencies.empty()) {
            q.push(field.id);
        }
    }

    // 拓扑排序过程
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        topoOrder.push_back(current);

        // 检查当前节点是否有依赖关系
        if (adjacencyList.find(current) != adjacencyList.end()) {
            // 减少邻居节点的入度
            for (int neighbor : adjacencyList.at(current)) {
                if (--inDegree[neighbor] == 0) {
                    q.push(neighbor);
                }
            }
        }
    }

    return topoOrder;
}

// 分配内存函数
void allocateMemory(const vector<Field>& fields, const vector<int>& topoOrder, vector<MemoryContainer>& containers) {
    for (int fieldId : topoOrder) {
        int fieldSize = fields[fieldId].size;

        // 尝试将字段分配到已有的容器中
        bool allocated = false;
        for (auto& container : containers) {
            // 检查是否可以将字段放入该容器
            bool fitsInContainer = true;
            for (int allocatedField : container.allocatedFields) {
                if (fields[allocatedField].id != fieldId) {
                    // 检查是否会与已分配的字段重叠
                    // 这里未涉及具体的重叠检查逻辑，留待实际情况扩展

                    // 检查对齐条件
                    // 这里未涉及具体的对齐检查逻辑，留待实际情况扩展
                }
            }

            if (fitsInContainer) {
                container.size += fieldSize;
                container.allocatedFields.push_back(fieldId);
                allocated = true;
                break;
            }
        }

        // 如果无法放入任何现有容器，则创建新容器
        if (!allocated) {
            containers.push_back({ fieldSize, { fieldId } });
        }
    }
}

// 将分配结果写入输出文件
void writeOutput(const vector<MemoryContainer>& containers, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        for (const auto& container : containers) {
            file << "Container size: " << container.size << " bytes" << endl;
            file << "Allocated fields:";
            for (int fieldId : container.allocatedFields) {
                file << " " << fieldId;
            }
            file << endl << endl;
        }
        file.close();
    } else {
        cerr << "Unable to open output file: " << filename << endl;
    }
}

int main() {
    string inputFilename = "input.csv";
    auto input = readInput(inputFilename);
    vector<Field> fields = get<0>(input);
    unordered_map<int, unordered_set<int>> adjacencyList = get<1>(input);
    vector<int> result = topologicalSort(fields, adjacencyList);
    // 输出结果
    for (int node : result) {
        cout << node << " ";
    }
    cout << endl;
    return 0;
}