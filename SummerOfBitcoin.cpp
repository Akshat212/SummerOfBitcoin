#include <iostream>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;

struct Mempool {
    string txid;
    int fee;
    int wt;
    vector<string> parents;
};

struct input {
    string txid;
    string fee;
    string wt;
    vector<string> parents;
};

// Comparison function to sort Item according to val/weight
// ratio
bool cmp(struct Mempool a, struct Mempool b)
{
    double r1 = (double)a.fee / (double)a.wt;
    double r2 = (double)b.fee / (double)b.wt;
    return r1 > r2;
}

int main()
{
    vector<input> pool;
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
    vector<Mempool> mpool;

    for(int i = 1 ; i < pool.size() ; i++)
    {
        vector<string> pr;
        pr = pool[i].parents;

        if(pr.size()==0 || pr[0].compare("")==0)
        {
            if(mp.find(pool[i].txid) == mp.end())
            {mp[pool[i].txid] = make_pair(f[i-1],w[i-1]);
            mpool.push_back({pool[i].txid,f[i-1],w[i-1],pr});}
        }
        else
        {
            for(auto it : pr)
            {
                if(mp.find(it) == mp.end())
                {w[i-1] = 1;f[i-1] = -1;
                break;}
                else
                {
                    w[i-1] += mp[it].second;
                    f[i-1] += mp[it].first;
                }
            }
            if(mp.find(pool[i].txid) == mp.end())
            {mp[pool[i].txid] = make_pair(f[i-1],w[i-1]);
            mpool.push_back({pool[i].txid,f[i-1],w[i-1],pr});}
        }
    }

    /*for(int i = 0 ; i < 10 ; i++)
    {
        cout<<mpool[i].txid<<" "<<mpool[i].fee<<" "<<mpool[i].wt<<" ";

        for(auto it : mpool[i].parents)
        cout<<it<<" ";
        cout<<endl;
    }

    cout<<mpool.size();*/

    sort(mpool.begin(),mpool.end(),cmp);

    int W = 4000000; // Maximum weight
    int n = mpool.size();

    ll profit  = 0 , currWt = 0;
    vector<string> ids;

    for(int i = 0 ; i < n ; i++)
    {
        if(mpool[i].fee == -1) continue;
        
        if(currWt + mpool[i].wt <= W)
        {
            currWt += mpool[i].wt;
            profit += mpool[i].fee;

            if(mpool[i].parents.size() > 0)
            {
                for(auto it : mpool[i].parents)
                ids.push_back(it);
            }
            ids.push_back(mpool[i].txid);
        }
        else
        {
            break;
        }
    }
    
    cout<<currWt<<" "<<profit<<endl;// The final weight and maximum profit

    ofstream myfile("block.txt");

    if(myfile.is_open())
    {
        string str;
        for(int i = 0 ; i < ids.size() ; i++)
        myfile<<ids[i]<<endl;
        myfile.close();
    }
    else cerr<<"Unable to open file";


}