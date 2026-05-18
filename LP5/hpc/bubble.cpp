#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

void parallel_bfs(vector<vector<int>> &graph, int start, int V) {
    vector<bool> visited(V, false);
    vector<int> current_level;
    
    visited[start] = true;
    current_level.push_back(start);
    
    cout << "Parallel BFS Traversal: ";
    
    while (!current_level.empty()) {
        vector<int> next_level;
        
        // Parallelize the exploration of the current level
        #pragma omp parallel for
        for (int i = 0; i < current_level.size(); i++) {
            int node = current_level[i];
            
            // Critical section to prevent garbled console output
            #pragma omp critical
            {
                cout << node << " ";
            }
            
            for (int neighbor : graph[node]) {
                // First check (without lock for speed)
                if (!visited[neighbor]) {
                    
                    // Critical section to prevent multiple threads from pushing the same neighbor
                    #pragma omp critical
                    {
                        // Double-check inside the lock
                        if (!visited[neighbor]) {
                            visited[neighbor] = true;
                            next_level.push_back(neighbor);
                        }
                    }
                }
            }
        }
        current_level = next_level;
    }
    cout << endl;
}

int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    cin >> V >> E;
    
    vector<vector<int>> graph(V);
    
    cout << "Enter edges (u v):\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // undirected graph
    }
    
    int start;
    cout << "Enter starting node: ";
    cin >> start;
    
    parallel_bfs(graph, start, V);
    
    return 0;
}
