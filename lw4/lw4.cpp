//18. Проект научно-технической программы задан с помощью
//ориентированного графа. Вершина графа соответствует отдельному
//исследованию, а дуги показывают очередность выполнения
//исследований (каждое исследование может начаться не ранее
//окончания предшествующих исследований).  Продолжительность
//каждого исследования известна. Требуется:
//1) проверить граф на отсутствие циклов;
//2) поиском в ширину найти путь наибольшей трудоемкости.
//Путем считается последовательность работ, которые должны
//выполняться друг за другом.  Трудоемкость пути - суммарная
//продолжительность работ на этом пути (10).
//
//Тимофеев Захар ПС-21 Clion 2023

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <limits>
#include <unordered_map>

using namespace std;

class Graph {
    int V; // Количество вершин
    vector<vector<pair<int, int>>> adj; // Список смежности (вершина, вес)

    bool isCyclicUtil(int v, vector<bool> &visited, vector<bool> &recStack) {
        if (!visited[v]) {
            visited[v] = true;
            recStack[v] = true;

            for (auto i : adj[v]) {
                if (!visited[i.first] && isCyclicUtil(i.first, visited, recStack)) {
                    return true;
                } else if (recStack[i.first]) {
                    cout << "Cycle detected between " << v << " and " << i.first << endl;
                    return true;
                }
            }
        }
        recStack[v] = false;
        return false;
    }


public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v, int weight) {
        adj[u].push_back(make_pair(v, weight));
    }

    bool isCyclic() {
        vector<bool> visited(V, false);
        vector<bool> recStack(V, false);

        for (int i = 0; i < V; i++)
            if (isCyclicUtil(i, visited, recStack))
                return true;

        return false;
    }

    void addZeroVertexEdges() {
        vector<bool> hasIncomingEdge(V, false);

        for (int u = 1; u < V; ++u) {
            for (auto i : adj[u]) {
                hasIncomingEdge[i.first] = true;
            }
        }

        for (int u = 1; u < V; ++u) {
            if (!hasIncomingEdge[u]) {
                addEdge(0, u, 0); // Ребро с весом 0 от вершины 0 к вершине u
            }
        }
    }

    void findLongestPath(int s) {
        vector<pair<int, int>> pathDetails(V, {-1, -1});
        vector<int> dist(V, 0);
        queue<int> q;

        dist[s] = 0;
        q.push(s);

        int maxWeight = 0;
        int lastVertex = s;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto i : adj[u]) {
                if (dist[i.first] < dist[u] + i.second || i.second == 0) {
                    dist[i.first] = dist[u] + i.second;
                    pathDetails[i.first] = {u, i.second};
                    q.push(i.first);

                    if (dist[i.first] > maxWeight) {
                        maxWeight = dist[i.first];
                        lastVertex = i.first;
                    }
                }
            }
        }

        cout << "Longest path: ";
        int current = lastVertex;
        vector<pair<int, int>> reversedPath;

        while (pathDetails[current].first > 0) {
            reversedPath.push_back({pathDetails[current].first, current});
            current = pathDetails[current].first;
        }

        for (int i = reversedPath.size() - 1; i >= 0; --i) {
            cout << reversedPath[i].first  << " -> " << reversedPath[i].second << " (weight " << pathDetails[reversedPath[i].second].second << ") ";
        }
        cout << "\nTotal weight: " << maxWeight << endl;
    }

};

int main() {
    ifstream dataFile("input.txt");
    ifstream pathsFile("paths.txt");

    if (!dataFile.is_open() || !pathsFile.is_open()) {
        cerr << "Unable to open file";
        return 1;
    }

    unordered_map<int, int> vertexMap;
    int numVertices = 0;
    string line;

    while (getline(dataFile, line)) {
        istringstream iss(line);
        string vertexName;
        int weight;
        iss >> numVertices >> vertexName >> weight;

        vertexMap[numVertices] = weight;
        numVertices++;
    }

    Graph g(numVertices);

    while (getline(pathsFile, line)) {
        istringstream iss(line);
        int startVertex, endVertex;
        iss >> startVertex >> endVertex;
        int weight = vertexMap[startVertex];
        // Assuming vertices are represented by numbers in paths.txt
        g.addEdge(startVertex, endVertex, weight);
    }
    g.addZeroVertexEdges();

    if (g.isCyclic())
        cout << "Graph contains a cycle\n";
    else {
        cout << "No cycles in graph\n";
        g.findLongestPath(0);
    }

    return 0;
}