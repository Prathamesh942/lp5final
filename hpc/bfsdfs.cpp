#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include<chrono>
#include<omp.h>
using namespace std;
using namespace std::chrono;

class graph{
private:
    vector<vector<int>> g;
    int n;
public:
    graph(int n){
        this->n=n;
        g.resize(n);
    }
    void add(int u,int v){
        g[u].push_back(v);
        g[v].push_back(u);
    }
    float dfs(){
        auto t1 = high_resolution_clock::now();
        stack<int> st;
        vector<int> visited(n,0);
        st.push(0);
        visited[0]=1;
        while(!st.empty()){
            int tp = st.top();
            st.pop();
            for(auto it:g[tp]){
                if(!visited[it]){
                    visited[it]=1;
                    st.push(it);
                }
            }
        }
        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2-t1).count();
    }
    float bfs(){
        auto t1 = high_resolution_clock::now();
        queue<int> q;
        vector<int> visited(n,0);
        q.push(0);
        visited[0]=1;
        while(!q.empty()){
            int tp = q.front();
            q.pop();
            for(auto it:g[tp]){
                if(!visited[it]){
                    visited[it]=1;
                    q.push(it);
                }
            }
        }
        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2-t1).count();
    }
    float dfsp(){
        auto t1 = high_resolution_clock::now();
        stack<int> st;
        vector<int> visited(n,0);
        st.push(0);
        visited[0]=1;
        while(!st.empty()){
            int tp;
            #pragma omp critical
            {
                tp = st.top();
                st.pop();
            }
            
            vector<int> nbr;
            for(auto it:g[tp]){
                if(!visited[it]){
                    visited[it]=1;
                    nbr.push_back(it);
                }
            }
            #pragma omp parallel for
            for(int i=nbr.size()-1; i>=0; i--){
                st.push(nbr[i]);
            }
        }
        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2-t1).count();
    }
    float bfsp(){
        auto t1 = high_resolution_clock::now();
        queue<int> q;
        vector<int> visited(n,0);
        q.push(0);
        visited[0]=1;
        while(!q.empty()){
            int tp;
            #pragma omp critical
            {
                tp = q.front();
                q.pop();
            }
            vector<int> nbr;
            for(auto it:g[tp]){
                if(!visited[it]){
                    visited[it]=1;
                    nbr.push_back(it);
                }
            }
            #pragma omp parallel for
            for(auto it:nbr){
                q.push(it);
            }
        }
        auto t2 = high_resolution_clock::now();
        return duration_cast<microseconds>(t2-t1).count();
    }
};

int main(){
    graph g(10000);
    for(int u=0; u<10000; u++){
        for(int v=u+1; v<10000; v++) g.add(u,v);
    }

    float tds = g.dfs();
    float tbs = g.bfs();
    float tdp = g.dfsp();
    float tbp = g.bfsp();
    cout<<tds/tdp<<" "<<tbs/tbp;
}