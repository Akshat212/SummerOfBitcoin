#include <iostream>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;

struct Mempool {
    string txid;
    string fee;
    string wt;
    vector<string> parents;
};



int main()
{
    vector<Mempool> pool;
    vector<string> row;
    ifstream ip("mempool.csv");

    string txid;
    string fee;
    string wt;
    string pr;

    while(ip.good()){

        getline(ip,txid,',');
        getline(ip,fee,',');
        getline(ip,wt,',');
        getline(ip,pr,'\n');

        vector<string> p;
        string word = "";
        for (auto x : pr) 
        {
            if (x == ';')
            {
                p.push_back(word);
                word = "";
            }
            else {
                word = word + x;
            }
        }
        if(word!="")
        p.push_back(word);

        pool.push_back({txid,fee,wt,p});
    }

    ip.close();

    vector<int> f,w;

    for(int i = 1 ; i < pool.size() ; i++)
    {int x = stoi(pool[i].fee);
    int y = stoi(pool[i].wt);
    f.push_back(x);
    w.push_back(y);}

    unordered_map<string,pair<int,int>> mp;

    for(int i = 1 ; i < pool.size() ; i++)
    {
        vector<string> pr;
        pr = pool[i].parents;

        if(pr.size()==0 || pr[0].compare("")==0)
        {
            if(mp.find(pool[i].txid) == mp.end())
            mp[pool[i].txid] = make_pair(f[i-1],w[i-1]);
        }
        else
        {
            for(auto it : pr)
            {
                if(mp.find(it) == mp.end())
                {w[i-1] = -1;
                break;}
            }
            if(mp.find(pool[i].txid) == mp.end())
            mp[pool[i].txid] = make_pair(f[i-1],w[i-1]);
        }
    }

}