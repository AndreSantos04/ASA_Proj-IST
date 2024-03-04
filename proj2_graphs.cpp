#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <chrono>

auto start_time = std::chrono::high_resolution_clock::now();

using namespace std;

void dfs(int v, vector<vector<int>>& adj, vector<int>& infected, vector<int>& finishOrder) {
    stack<int> s;
    s.push(v);

    while (!s.empty()) {
        int node = s.top();
        if (infected[node] == 0) {
            infected[node] = 1;
        }
        vector<int>::iterator it;

        for(it = adj[node].begin(); it != adj[node].end(); it++){
            if(infected[*it] == 0){
                s.push(*it);
                break;
            }
        }

        if (it == adj[node].end()) {
            finishOrder.push_back(node);
            s.pop();
        }
    }
}

void dfs_reverse(int v, vector<vector<int>>& adjReversed, vector<int>& infected, vector<int>& SCcomponent) {
    stack<int> s;
    s.push(v);

    while (!s.empty()) {

        int node = s.top();

        if (infected[node] == 0) {
            infected[node] = 1;
            SCcomponent.push_back(node);
        }

        vector<int>::iterator it;

        for(it = adjReversed[node].begin(); it != adjReversed[node].end(); it++){
            if(infected[*it] == 0){
                s.push(*it);
                break;
            }
        }

        if (it == adjReversed[node].end()) {
            s.pop();
        }
    }
}

vector<vector<int>> findSCCs(vector<vector<int>>& adj, vector<vector<int>>& adjReversed, vector<int>& infected, vector<int>& nodeSCC_index) {
    int V = adj.size();
    vector<int> finishOrder, SCcomponent;
    vector<vector<int>> SCCs;

    for (int vertex = 0; vertex < V; vertex++) {
        if (!infected[vertex])
            dfs(vertex, adj, infected, finishOrder);
    }

    infected.assign(V, 0);
    for (int i = 0; i < V; i++) {
        int vertex = finishOrder[(V - 1) - i];

        if (!infected[vertex]) {
            dfs_reverse(vertex, adjReversed, infected, SCcomponent);
            SCCs.push_back(SCcomponent);

            for(int node : SCcomponent) {
                nodeSCC_index[node] = SCCs.size() - 1;
            }
            SCcomponent.clear(); 
        }
    }

    return SCCs;
}

//criar o DAG com as SCCs
vector<vector<int>> SCC_DAG(vector<vector<int>>& SCCs, vector<vector<int>>& adjList, vector<int>& nodeSCC_index) {
    size_t n = SCCs.size();
    vector<vector<int>> DAG(n);

    for (size_t i = 0; i < n; i++) {
        for (int node : SCCs[i]) {
            for (int neighbor : adjList[node]) {
                size_t j = nodeSCC_index[neighbor]; // função que retorna a SCC a que o vizinho pertence
                if (i != j) {
                    DAG[i].push_back(j);
                }
            }
        }
    }

    return DAG;
}

int maxDistance(vector<vector<int>>& DAG) {
    int n = DAG.size();
    vector<int> d(n, 0);
    int maxJumps = 0;
    for (int scc = 0; scc < n; scc++) {
        for (int neighbor_scc : DAG[scc]) {
            d[neighbor_scc] = max(d[neighbor_scc], d[scc] + 1);
            
            if(d[neighbor_scc] > maxJumps){
                maxJumps = d[neighbor_scc];
            }
        }
    }
    return maxJumps;
}


int main() {
    
    int V, E;
    if(scanf("%d %d", &V, &E) != 2) {
        cout << "Error reading input" << endl;
        exit(1);
    }
    vector<int> infected(V, 0);
    vector<vector<int>> adjList(V);
    vector<vector<int>> adjT(V);
    vector<int> nodeSCC_index(V);
    // Criar o grafo (adjList):
    for (int i = 0; i < E; i++) {
        int x, y;
        if(scanf("%d %d", &x, &y) != 2) {
            cout << "Error reading input" << endl;
            exit(1);
        }
        adjList[x - 1].push_back(y - 1);
        adjT[y - 1].push_back(x - 1);
    }

    // Vetor com as Componentes fortemente ligadas
    vector<vector<int>> SCCs = findSCCs(adjList, adjT, infected, nodeSCC_index);

    // DAG com as SCCs como vértices
    vector<vector<int>> DAG = SCC_DAG(SCCs, adjList, nodeSCC_index);
 
    cout << maxDistance(DAG) << endl;
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Tempo de execução: " << duration.count() << " microssegundos" << std::endl;
    return 0;

}