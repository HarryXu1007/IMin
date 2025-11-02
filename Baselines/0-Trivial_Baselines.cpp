#pragma GCC optimize(3,"Ofast","inline")
#include <iostream>  
#include <vector>    
#include <string>    
#include <algorithm> 
#include <bits/stdc++.h> 
#include <typeinfo>
#include <iomanip>
#include "utils.h"
using namespace std;

#define LL long long
#define LD long double
#define PII pair<LL, LL>

int T = 10000;
int LJS = 10;
int c2 = 1;


int main(){
    int b;
    cout<<"Please input the number of blockers: ";
    cin>>b;
    srand(1007);
    string fileName, file_data, algo_name;
    cout<<"Please input the dataset: ";
    cin>>file_data;
    cout<<"Please input the model of edge activation probability (0: WC model; 1: TR model; 2: UN model with p=0.01): ";
    int model;
    cin>>model;
    if (model !=0 && model !=1 && model !=2){
        cout<<"Model error!"<<endl;
        return 0;
    }
    cout<<"Please input the algorithm name (rd, deg, pr): ";
    cin>>algo_name;
    if (algo_name != "rd" && algo_name != "deg" && algo_name != "pr"){
        cout<<"Algorithm name error!"<<endl;
        return 0;
    }

    fileName = "datasets/"+file_data+".txt";
    ifstream in(fileName);
    vector<vector<Edge>> G; 
    vector<LL> sources;   
    vector<PII> E;          
    vector<LL> inDegree;   
    // vector<LL> trueinDegree;   
    vector<LL> trueoutDegree;   
    vector<bool> visited_vertices;  
    LL n,m;
    // double total_time = 0;
    double beginTime, endTime2;

    if (!in.is_open())
	{
		cout << "error" << endl;
		return 0;
	} 

    in >> n >> m;
    cout<<n<<'\t'<<m<<endl;
    
    cout<<"Seeds: ";
    for (LL i=0;i<10;i++){
        LL seed = i*(n/10);
        sources.push_back(seed);
        cout<<seed<<" ";
    }  cout<<endl;


    inDegree.resize(n);
    trueoutDegree.resize(n);
    G.resize(n);
    vector<LL> candidates; 
    vector<vector<Edge>> e; 
    e.resize(n);
    set<LL> node_set;

    LL idx;
    for (idx = 0; idx < m; idx++)
    {
        LL x, y;
        in >> x >> y;
        if (x == y)
            continue;


        E.push_back(PII(x, y));
        // E.push_back(PII(y, x));   // for undirected graph fs0
        node_set.insert(x);       
        node_set.insert(y);    
        inDegree[y]++;  
        // inDegree[x]++;  // for undirected graph fs0
        if (is_source(x, sources)){
            candidates.push_back(y); 
        }   
    }

    for (auto edge : E)
    {
        LL x = edge.first, y = edge.second;
        double p;
        if (model == 0) // WC model
            p = 1.0 / ((double)inDegree[y]);
        else if (model == 1) // TR model
            p = randEdge();
        else if (model == 2) // UN model
            p = 0.01;
        e[x].push_back(Edge(y, p));
    }

////////////////////////////////////////////////////////////
    beginTime = clock();      
    LL tag;
    vector<LL> vis;
    vis.resize(n);
    for (LL i = 0; i < n; i++)
        vis[i] = 1; // vis[i] = -tag when the i-th node is visited in the tag-th round
    vector<LL> blockers;
    vector<bool> best_blockers(n, false);

if (algo_name == "rd"){
    vector<LL> node_vec(node_set.begin(), node_set.end());
    random_shuffle(node_vec.begin(), node_vec.end());
    set<LL> source_set(sources.begin(), sources.end());
    for (size_t i = 0; blockers.size() < b && i < node_vec.size(); ++i) {
        LL candidate = node_vec[i];
        if (source_set.find(candidate) == source_set.end()) {
            blockers.push_back(candidate);
            best_blockers[candidate] = true;
        }
    }
}

if (algo_name == "deg"){
    vector<double> score_deg;
    score_deg.resize(n);
    for (LL i = 0; i < n; i++)
        score_deg[i] = 0;

    
    for (tag = 1; tag <= T; tag++){
        // cout<<tag<<' ';
        queue<LL> q;
        for (auto x : sources){
            q.push(x);
            vis[x] = -tag;
        }
        while (!q.empty())
        {
            LL x = q.front();
            q.pop();
            for (auto edge : e[x])
            {
                LL y = edge.to;
                double p = edge.p;
                if (p == 0)
                    continue;
                if (check((int)round(1.0 / p))){
                    score_deg[x] += 1.0 / ((double) T);
                    if (vis[y] != -tag){
                        vis[y] = -tag;
                        q.push(y);
                    }
                }
            }
        }
    }

    for (LL i=0; i<n; i++){
        if (is_source(i, sources)){
            score_deg[i] = 0;
        }
    }


    for (int i=0; i<b; i++){
        double max_deg = -1;
        LL max_idx = -1;
        for (LL c=0; c<n; c++){  
            if (score_deg[c] > max_deg){
                max_deg = score_deg[c];
                max_idx = c;
            }
        }
        blockers.push_back(max_idx);
        best_blockers[max_idx] = true;
        score_deg[max_idx] = -1;
    }
}

if (algo_name == "pr"){
    vector<double> score_pr;
    score_pr.resize(n);
    for (LL i = 0; i < n; i++)
        score_pr[i] = 0;

    double d = 0.85; // damping factor
    int max_iter = 100;
    vector<double> pr(n, 1.0 / n);
    vector<double> pr_new(n, 0.0);

    for (int iter = 0; iter < max_iter; iter++) {
        fill(pr_new.begin(), pr_new.end(), (1.0 - d) / n);
        for (LL u = 0; u < n; ++u) {
            if (e[u].empty()) continue;
            double share = pr[u] * d / e[u].size();
            for (auto &edge : e[u]) {
                pr_new[edge.to] += share;
            }
        }
        swap(pr, pr_new);
    }

    for (LL i = 0; i < n; ++i) {
        score_pr[i] = pr[i];
    }

    for (LL i=0; i<n; i++){
        if (is_source(i, sources)){
            score_pr[i] = 0;
        }
    }


    for (int i=0; i<b; i++){
        double max_deg = -1;
        LL max_idx = -1;
        for (LL c=0; c<n; c++){   
            if (score_pr[c] > max_deg){
                max_deg = score_pr[c];
                max_idx = c;
            }
        }
        blockers.push_back(max_idx);
        best_blockers[max_idx] = true;
        score_pr[max_idx] = -1;
    }
}




    cout << "Blockers: ";
    for (auto blocker : blockers) {
        cout << blocker << " ";
    }
    cout << endl;


    endTime2 = clock();
    cout<<"dataset: "<<file_data<< "\tNeighbor true min_spread: "<<compute_expected(10000, n, e, sources, best_blockers);
    cout<<"\tTime: "<<(double)(endTime2 - beginTime) / CLOCKS_PER_SEC<<endl;


    
}