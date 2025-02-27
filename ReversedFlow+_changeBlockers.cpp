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
int C2 = 1;

int main(){
    int b;
    cout<<"Please input the number of blockers: ";
    cin>>b;
    srand(42);
    string fileName, file_data;
    cout<<"Please input the dataset: ";
    cin>>file_data;
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
    double beginTime, endTime, endTime2;

    if (!in.is_open())
	{
		cout << "error" << endl;
		return 0;
	} 

    in >> n >> m;
    cout<<n<<'\t'<<m<<endl;
    cout<<"Seeds: ";
    // TODO: initialize the seeds here.
    sources.push_back(0);

    inDegree.resize(n);
    trueoutDegree.resize(n);
    G.resize(n);
    vector<LL> candidates; 

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
    // cout<<"Have pushed "<<idx<<" edges"<<endl;

    vector<vector<Edge>> e; 
    e.resize(n);
    for (auto edge : E)
    {
        LL x = edge.first, y = edge.second;
        double p;
        p = 1.0 / ((double)inDegree[y]);
        e[x].push_back(Edge(y, p));
    }
    // cout<<"G input ok."<<endl;

////////////////////////////////////////////////////////////

    LL tag;
    vector<LL> vis;
    vis.resize(n);
    for (LL i = 0; i < n; i++)
        vis[i] = 1; 
    vector<LL> score_heu;
    score_heu.resize(n);
    for (LL i = 0; i < n; i++)
        score_heu[i] = 0;
    vector<LL> trueinDegree;
    trueinDegree.resize(n);

    LL num_truncated = 0;

    vector<vector<LL>> e_revSample;
    vector<LL> idx2node;   
    vector<LL> node2idx;   
    vector<bool> terminal;
    terminal.resize(n);
    vector<double> score; 
    score.resize(n);
    vector<LL> num_inpath;  
    num_inpath.resize(n);
    
    vector<LL> vis2;

////////////////////////////////////////////////////////////

    beginTime = clock();       
    for (tag = 1; tag <= T; tag++){
        LL num_sample=0;
        idx2node.resize(0);
        node2idx.resize(n);
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
            num_inpath[i] = 0;

        queue<LL> q;
        for (auto x : sources){
            q.push(x);
            score[x] = 1;
            vis[x] = -tag;
            num_inpath[x] = 1;
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
                    num_inpath[y] += num_inpath[x];
                    if (node2idx[y] == -1){
                        idx2node.push_back(y);
                        node2idx[y] = num_sample;
                        e_revSample.push_back(vector<LL>());
                        num_sample++;
                    }
                    e_revSample[node2idx[y]].push_back(x);

                    if (num_inpath[y] >= LJS){
                        num_truncated++;
                    }

                    if (vis[y] != -tag && num_inpath[y] < LJS){
                        q.push(y); 
                        terminal[y] = true;
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
        for (LL i=0; i<num_sample; i++){
            if (terminal[idx2node[i]]) 
            {
                q2.push(idx2node[i]);                   
                vis2[i] = -tag;                     
            }
        }
        while (!q2.empty())
        {
            LL x = q2.front();
            q2.pop();
            if (trueinDegree[x]>C2){
                if (!is_source(x, candidates)){
                    candidates.push_back(x);
                }
            }
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

    endTime = clock();
    cout<<"Time: "<<(double)(endTime - beginTime) / CLOCKS_PER_SEC<<endl;


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

    for (auto blocker: blockers){
        cout<<blocker<<' ';
    }cout<<endl;

    vector<bool> best_blockers;
    best_blockers.resize(n);
    for (LL i = 0; i < n; i++)
        best_blockers[i] = false;
    for (auto blocker: blockers){
        best_blockers[blocker] = true;
    }

    endTime2 = clock();

    // cout<<"num_truncated: "<<num_truncated<<endl;

    cout<<"RF+ true min_spread: "<<compute_expected(10000, n, e, sources, best_blockers)<<endl;
    cout<<"Time2: "<<(double)(endTime2 - beginTime) / CLOCKS_PER_SEC<<endl;














}