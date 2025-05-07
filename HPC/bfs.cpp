// %%writefile bfs.cpp
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

class Graph {
private:
    int numVertices;
    vector<vector<int>> adj;

    public:
    Graph(int vertices) {
        numVertices = vertices;
        adj.resize(vertices);
    }

    void addEdge(int src, int dest) {
        adj[src].push_back(dest);
        adj[dest].push_back(src);
    }

    void viewGraph() {
        cout << "\nGraph:\n";
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i << " -> ";
            for (int neighbor : adj[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    // Parallel BFS with thread logging
    void bfs(int startVertex) {
        vector<bool> visited(numVertices, false);
        queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        cout << "\nParallel BFS Output:\n";

        while (!q.empty()) {
            int currentVertex = q.front();
            q.pop();
    
            #pragma omp critical
            {
                cout << currentVertex << endl;
            }
    
            // Gather neighbors to process in parallel safely
            vector<int> neighbors = adj[currentVertex];
            #pragma omp parallel for
            for (int i = 0; i < neighbors.size(); i++) {
                int neighbor = neighbors[i];
                int thread_id = omp_get_thread_num();
    
                bool shouldVisit = false;
    
                // Synchronize check-and-mark
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                        shouldVisit = true;
                    }
                }
    
            
                    #pragma omp critical
                    {
                        cout << "Thread " << thread_id << " visiting node " << neighbor << endl;
                    }
            }

        }
    }

    // Helper for parallel DFS
    void dfs(int startVertex) {
        vector<bool> visited(numVertices, false);
        stack<int> s;
    
        visited[startVertex] = true;
        s.push(startVertex);
    
        cout << "\nDFS Output:\n";
    
        while (!s.empty()) {
            int currentVertex = s.top();
            s.pop();
    
            #pragma omp critical
            {
                cout << currentVertex << endl;
            }
    
            // Gather neighbors to process in parallel safely
            vector<int> neighbors = adj[currentVertex];
    
            #pragma omp parallel for
            for (int i = 0; i < neighbors.size(); i++) {
                int neighbor = neighbors[i];
                int thread_id = omp_get_thread_num();
    
                bool shouldVisit = false;
    
                // Synchronize check-and-mark
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        s.push(neighbor);
                        shouldVisit = true;
                    }
                }
    
            
                    #pragma omp critical
                    {
                        cout << "Thread " << thread_id << " visiting node " << neighbor << endl;
                    }
                
            }
        }
    }
     
};

int main() {
    int numVertices, numEdges;
    cout << "Enter number of vertices: ";
    cin >> numVertices;

    Graph graph(numVertices);

    cout << "Enter number of edges: ";
    cin >> numEdges;

    cout << "Enter the edges (source destination):\n";
    for (int i = 0; i < numEdges; i++) {
        int src, dest;
        cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    graph.viewGraph();

    int startVertex;
    cout << "Enter the starting vertex: ";
    cin >> startVertex;
    double startTime = omp_get_wtime();

    graph.bfs(startVertex);
    graph.dfs(startVertex);

    double endTime = omp_get_wtime();

    cout << "\nTotal time for BFS + DFS: " << (endTime - startTime) << " seconds" << endl;

    return 0;
}


// Enter number of vertices: 7
// Enter number of edges: 6
// Enter the edges (source destination):
// 0 1
// 0 2
// 1 3
// 1 4
// 2 5
// 2 6