# include <iostream>
# include <vector>
# include <stack>
# include <queue>
# include <omp.h>

using namespace std;

class Graph
{
    private:
    int NumVertices;
    vector<vector<int>> adj;

    public:
    Graph(int vertices)
    {
        NumVertices=vertices;
        adj.resize(vertices);
    }

    void addEdge(int src, int dest)
    {
        adj[src].push_back(dest);
        adj[dest].push_back(src);
    }

    void viewGraph()
    {
        cout<<"Graph : "<<endl;
        for (int i=0;i<NumVertices;i++)
        {
            cout<<"Vertex : "<<i<<" ";
            for (int neighbor :adj[i])
            {
                cout<<neighbor<<" ";
            }
            cout<<endl;
        }
    }

    void bfs(int StartVertex)
    {
        vector<bool> visited (NumVertices,false);
        queue<int> q;

        visited[StartVertex]=true;
        q.push(StartVertex);

        cout<<"Parallel BFS Output : "<<endl;

        while (!q.empty())
        {
            int current_vertex=q.front();
            q.pop();

            #pragma omp critical
            {
                cout<<current_vertex<<endl;
            }

            vector<int> neighbors=adj[current_vertex];

            #pragma omp parallel for
            for(int i=0;i<neighbors.size();i++)
            {
                int neighbor=neighbors[i];
                int thread_id=omp_get_thread_num();

                bool shouldVisit=false;

                #pragma omp critical
                {
                    if (!visited[neighbor])
                    {
                        visited[neighbor]=true;
                        q.push(neighbor);
                        shouldVisit=true;
                    }
                }

                #pragma omp critical
                {
                    cout<<"Thread "<<thread_id<<" visiting Vertex : "<<neighbor<<endl;
                }

            }
        }
    }

    void dfs(int StartVertex)
    {
        vector<bool> visited(NumVertices,false);
        stack<int> s;

        visited[StartVertex]=true;
        s.push(StartVertex);

        cout<<"Parallel DFS Output : "<<endl;

        while(!s.empty())
        {
            int current_vertex=s.top();
            s.pop();

            #pragma omp critical
            {
                cout<<"Vertex : "<<current_vertex<<endl;
            }

            vector<int> neighbors=adj[current_vertex];

            #pragma omp parallel for
            for (int i=0;i<neighbors.size();i++)
            {
                int neighbor=neighbors[i];
                int thread_id=omp_get_thread_num();

                bool shouldVisit=false;

                #pragma omp critical
                {
                    if (!visited[neighbor])
                    {
                        visited[neighbor]=true;
                        s.push(neighbor);

                        shouldVisit=true;
                    }
                }

                #pragma omp critical
                {
                    cout<<"Thread "<<thread_id<<" is visiting vertex "<<neighbor<<endl;
                }
            }
        }

    }
};

int main()
{
    int NumVertices,NumEdges;

    cout<<"Enter the no. of vertices of the graph : ";
    cin>>NumVertices;

    Graph graph(NumVertices);

    cout<<"Enter the no. of edges : ";
    cin>>NumEdges;

    int src,dest;
    cout<<"Enter the edge as source dest"<<endl;
    for (int i=0;i<NumEdges;i++)
    {
        cin>>src>>dest;
        graph.addEdge(src,dest);
    }
    cout<<endl;

    graph.viewGraph();

    cout<<"Enter the starting vertex : ";
    int startvertex;
    cin>>startvertex;

    double start,end;

    start=omp_get_wtime();
    graph.bfs(startvertex);
    end=omp_get_wtime();

    cout<<"Time taken for parallel BFS is : "<<end-start<<" seconds"<<endl;

    start=omp_get_wtime();
    graph.dfs(startvertex);
    end=omp_get_wtime();

    cout<<"Time taken for parallel DFS is : "<<end-start<<" seconds"<<endl;
}