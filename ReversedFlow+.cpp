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
int c1 = 10;
int c2 = 1;


int main(){
    LL b;
    cout<<"Please input the number of blockers: ";
    cin>>b;
    srand(1007);
    string fileName, file_data;
    cout<<"Please input the dataset: ";
    cin>>file_data;
    cout<<"Please input the model of edge activation probability (0: WC model; 1: TR model; 2: UN model with p=0.01): ";
    int model;
    cin>>model;
    if (model !=0 && model !=1 && model !=2){
        cout<<"Model error!"<<endl;
        return 0;
    }
    cout<<"Please input the algorithm name (RF, RF+): ";
    string algo_name;
    cin>>algo_name;
    if (algo_name != "RF+" && algo_name != "RF"){
        cout<<"Algorithm name error!"<<endl;
        return 0;
    }
    if (algo_name == "RF+"){
        cout<<"Please input c1 (default 10): ";
        cin>>c1;
        cout<<"Please input c2 (default 1): ";
        cin>>c2;
    }

    fileName = "datasets/"+file_data+".txt";
    cout<<"Input file: "<<fileName<<endl;
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
    double beginTime, endTime;

    if (!in.is_open())
	{
		cout << "error" << endl;
		return 0;
	} 

    in >> n >> m;
    cout<<n<<'\t'<<m<<endl;

    /////////////////////////////////
    // Initialization of Seed Set
    cout<<"Seeds: ";
    for (LL i=0;i<10;i++){
        LL seed = i*(n/10);
        sources.push_back(seed);
        cout<<seed<<" ";
    }
    /////////////////////////////////


    inDegree.resize(n);
    trueoutDegree.resize(n);
    G.resize(n);
    vector<LL> candidates; 
    vector<vector<Edge>> e; 
    e.resize(n);

    LL idx;
    for (idx = 0; idx < m; idx++)
    {
        LL x, y;
        in >> x >> y;
        if (x == y)
            continue;
        E.push_back(PII(x, y));
        // E.push_back(PII(y, x));   // for undirected graph 
        inDegree[y]++;  
        // inDegree[x]++;  // for undirected graph

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
    vector<LL> score_heu;
    score_heu.resize(n);
    for (LL i = 0; i < n; i++)
        score_heu[i] = 0;
    vector<LL> trueinDegree;
    trueinDegree.resize(n);

    vector<vector<LL>> e_revSample;
    vector<LL> idx2node;   
    vector<LL> node2idx;   
    vector<bool> terminal;
    terminal.resize(n);
    vector<double> score; 
    score.resize(n);
    vector<LL> vis2;
    vis2.resize(n);

////////////////////////////////////////////////////////////

    for (tag = 1; tag <= T; tag++){
        LL num_sample=0;
        idx2node.resize(0);
        node2idx.resize(n);
        vector<LL> terminal_order_list;
        for (LL i = 0; i < n; i++)
            node2idx[i] = -1;
        e_revSample.resize(0);
        for (LL i = 0; i < n; i++)
            terminal[i] = false;
        for (LL i = 0; i < n; i++){
            score[i] = 0;
            trueinDegree[i] = 0;
        }
        queue<LL> q;
        for (auto x : sources){
            q.push(x);
            score[x] = 1;
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
                    terminal[x]=false;
                    trueinDegree[y]++;
                    score[y] = 1;
                    if (node2idx[y] == -1){
                        idx2node.push_back(y);
                        node2idx[y] = num_sample;
                        e_revSample.push_back(vector<LL>());
                        num_sample++;
                    }
                    e_revSample[node2idx[y]].push_back(x);
                    if (vis[y] != -tag){
                        q.push(y); 
                        terminal[y] = true;
                        terminal_order_list.push_back(y);
                        vis[y] = -tag;
                    }
                }
            }
        }
        if (num_sample == 0)
            continue;
        vis2.resize(num_sample);
        for (LL i = 0; i < num_sample; i++)
            vis2[i] = 1;  
        queue<LL> q2;
        for (auto v_ts: terminal_order_list){
            if (terminal[v_ts]==true){
                q2.push(v_ts);
                vis2[node2idx[v_ts]] = -tag;
            }
        }
        while (!q2.empty())
        {
            LL x = q2.front();
            q2.pop();
            LL deg_tmp = trueinDegree[x];
            for (auto y : e_revSample[node2idx[x]])
            {
                score[y] += score[x] / deg_tmp;
                if (vis2[node2idx[y]] == 1)
                {
                    vis2[node2idx[y]] = -tag;
                    q2.push(y);
                }
            }
        }
        for (LL i=0; i<num_sample; i++){
            score_heu[idx2node[i]] += score[idx2node[i]];
        }

    }
    for (LL i=0; i<n; i++){
        if (is_source(i, sources)){
            score_heu[i] = 0;
        }
    }
    vector<LL> blockers;
    blockers.resize(0);

    for (int i=0; i<b; i++){
        LL max_heu = 0;
        LL max_idx = -1;
        for (auto c: candidates){   
            if (score_heu[c] > max_heu){
                max_heu = score_heu[c];
                max_idx = c;
            }
        }
        blockers.push_back(max_idx);
        score_heu[max_idx] = 0;
    }

    vector<bool> best_blockers;
    best_blockers.resize(n);
    for (LL i = 0; i < n; i++)
        best_blockers[i] = false;
    for (auto blocker: blockers){
        best_blockers[blocker] = true;
    }





if (algo_name == "RF"){
    endTime = clock();
    for (auto blocker: blockers){
        cout<<blocker<<' ';
    }cout<<endl;
    cout<<"Expected Influence Spread of RF: "<<compute_expected(10000, n, e, sources, best_blockers)<<endl;
    cout<<"Time: "<<(double)(endTime - beginTime) / CLOCKS_PER_SEC<<endl;
}



if (algo_name == "RF+"){
    double res_rf = compute_expected(100, n, e, sources, best_blockers);
    cout<<"Expected Influence Spread of RF_raw: "<<res_rf<<endl;

    for (LL i = 0; i < n; i++)
        vis[i] = 1; // vis[i] = -tag when the i-th node is visited in the tag-th round
    score_heu.resize(n);
    for (LL i = 0; i < n; i++)
        score_heu[i] = 0;
    vector<LL> num_paths;  
    num_paths.resize(n);

////////////////////////////////////////////////////////////

    for (tag = 1; tag <= T; tag++){
        
        LL num_sample=0;
        idx2node.resize(0);
        node2idx.resize(n);
        vector<LL> terminal_order_list;
        for (LL i = 0; i < n; i++)
            node2idx[i] = -1;
        
        e_revSample.resize(0);

        for (LL i = 0; i < n; i++)
            terminal[i] = false;

        for (LL i = 0; i < n; i++){
            score[i] = 0;
            trueinDegree[i] = 0;
        }
        for (LL i = 0; i < n; i++)
            num_paths[i] = 0;

        queue<LL> q;
        for (auto x : sources){
            q.push(x);
            score[x] = 1;
            vis[x] = -tag;
            num_paths[x] = 1;
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
                    terminal[x]=false;
                    trueinDegree[y]++;
                    score[y] = 1;
                    num_paths[y] += num_paths[x];
                    if (node2idx[y] == -1){
                        idx2node.push_back(y);
                        node2idx[y] = num_sample;
                        e_revSample.push_back(vector<LL>());
                        num_sample++;
                    }
                    e_revSample[node2idx[y]].push_back(x);

                    if (vis[y] != -tag && num_paths[y] < c1){
                        q.push(y); 
                        terminal[y] = true;
                        terminal_order_list.push_back(y);
                        vis[y] = -tag;
                    }
                }
            }
        }
        if (num_sample == 0)
            continue;
        vis2.resize(num_sample);
        for (LL i = 0; i < num_sample; i++)
            vis2[i] = 1;  
        queue<LL> q2;
        for (auto v_ts: terminal_order_list){
            if (terminal[v_ts]==true){
                q2.push(v_ts);
                vis2[node2idx[v_ts]] = -tag;
            }
        }
        while (!q2.empty())
        {
            LL x = q2.front();
            q2.pop();

            //////////////////////////////
            if (trueinDegree[x] > c2){
                if (!is_source(x, candidates)){
                    candidates.push_back(x);
                }
            }
            /////////////////////////

            LL deg_tmp = trueinDegree[x];
            for (auto y : e_revSample[node2idx[x]])
            {
                score[y] += score[x] / deg_tmp;
                if (vis2[node2idx[y]] == 1)
                {
                    vis2[node2idx[y]] = -tag;
                    q2.push(y);
                }
            }
        }
        for (LL i=0; i<num_sample; i++){
            score_heu[idx2node[i]] += score[idx2node[i]];
        }

    }


    for (LL i=0; i<n; i++){
        if (is_source(i, sources)){
            score_heu[i] = 0;
        }
    }
    vector<LL> blockers_plus;
    blockers_plus.resize(0);

    for (int i=0; i<b; i++){
        LL max_heu = 0;
        LL max_idx = -1;
        for (auto c: candidates){   
            if (score_heu[c] > max_heu){
                max_heu = score_heu[c];
                max_idx = c;
            }
        }
        blockers_plus.push_back(max_idx);
        score_heu[max_idx] = 0;
    }

    // for (auto blocker: blockers_plus){
    //     cout<<blocker<<' ';
    // }cout<<endl;

    vector<bool> best_blockers_plus;
    best_blockers_plus.resize(n);
    for (LL i = 0; i < n; i++)
        best_blockers_plus[i] = false;
    for (auto blocker: blockers_plus){
        best_blockers_plus[blocker] = true;
    }

    double res_rf_plus = compute_expected(100, n, e, sources, best_blockers_plus);
    cout<<"Expected Influence Spread of RF with truncation and candidate selection: "<<res_rf_plus<<endl;
    endTime = clock();

    if (res_rf_plus < res_rf){
        for (auto blocker: blockers_plus){
            cout<<blocker<<' ';
        }cout<<endl;
        cout<<"Select RF_plus; Expected Influence Spread of RF+: "<<compute_expected(10000, n, e, sources, best_blockers_plus)<<endl;
    }
    else {
        for (auto blocker: blockers){
            cout<<blocker<<' ';
        }cout<<endl;
        cout<<"Select RF_raw; Expected Influence Spread of RF+: "<<compute_expected(10000, n, e, sources, best_blockers)<<endl;
    }

    cout<<"Time: "<<(double)(endTime - beginTime) / CLOCKS_PER_SEC<<endl;


}







}