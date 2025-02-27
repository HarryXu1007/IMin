#include <iostream>  // 用于输入输出操作
#include <vector>    // 用于使用向量容器
#include <string>    // 用于使用字符串
#include <algorithm> // 用于使用算法函数
#include <bits/stdc++.h> // 用于使用一些常用的数据结构
#include <typeinfo>
#include <iomanip>
using namespace std;

#define LL long long

// mt19937 rand_num(42);
random_device rd;
mt19937 rand_num(rd());

struct Edge
{
    int to;
    double p;
    Edge(int a = 0, double b = 0) : to(a), p(b) {}
};

double randEdge()
{
    uniform_int_distribution<int> dist(1, 3);
    int x = dist(rand_num);
    if (x == 1)
        return 0.1;
    else if (x == 2)
        return 0.01;
    return 0.001;
}

bool check(int x)
{
    uniform_int_distribution<int> dist(1, x);
    if (dist(rand_num) == 1)
        return true;
    return false;
}

bool is_source(int x, vector<LL> sources)
{
    for (auto source : sources)
        if (source == x)
            return true;
    return false;
}

bool is_a_blocker(int x, vector<int> blockers)
{
    for (auto b : blockers)
        if (b == x)
            return true;
    return false;
}

vector<double> score2prob(vector<double> score, vector<LL> sources, int n)
{
    
    vector<double> prob;
    prob.resize(n);
    double sum = 0;
    for (int i = 0; i < n; i++){
        if (is_source(i, sources))
            score[i] = 0;
        sum += score[i];
    }
        
    for (int i = 0; i < n; i++)
        prob[i] = score[i] / sum;
    return prob;
}

vector<double> score2prob_exp(vector<double> score, vector<LL> sources, int n)
{
    vector<double> prob;
    prob.resize(n);
    double sum = 0;
    double max_element = 0;
    for (int i = 0; i < n; i++)
    {
        if (is_source(i, sources))
            score[i] = 0;
        if (score[i] > max_element)
            max_element = score[i];
    }
    for (int i = 0; i < n; i++)
    {
        score[i] = pow(2, (score[i] - max_element));
        sum += score[i];
    }
    for (int i = 0; i < n; i++)
        prob[i] = score[i] / sum;
    return prob;
}

double compute_expected(int T, int n, vector<vector<Edge>> e, vector<LL> sources, vector<bool> is_blocker)
{
    vector<int> vis;
    vis.resize(n);
    double sum = 0;
    for (int tag = 1; tag <= T; tag++)
    {
        vector<vector<Edge>> eSample;
        eSample.resize(n);
        queue<int> q;
        for (auto x : sources)
            q.push(x), vis[x] = -tag;
        while (!q.empty())
        {
            int x = q.front();
            q.pop();
            for (auto y : e[x])
            {
                if (y.p == 0)
                    continue;
                if (is_blocker[y.to])
                    continue;
                if (check((int)round(1.0 / y.p)))
                {
                    eSample[x].push_back(y.to);
                    if (vis[y.to] != -tag)
                        q.push(y.to), vis[y.to] = -tag;
                }
            }
        }
        for (auto x : sources)
            vis[x] = tag, sum += 1.0, q.push(x);
        while (!q.empty())
        {
            int x = q.front();
            q.pop();
            for (auto y : eSample[x])
                if (vis[y.to] != tag)
                    vis[y.to] = tag, sum += 1.0, q.push(y.to);
        }
    }
    return sum / ((double)T);
}