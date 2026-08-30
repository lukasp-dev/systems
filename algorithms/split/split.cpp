#include <bits/stdc++.h> 
using namespace std;

vector<string> split(const string& s, char delimiter){
    vector<string> ret;
    size_t start = 0;
    
    while(true){
        size_t pos = s.find(delimiter, start);
        
        string token = s.substr(start, pos - start);

        if(!token.empty()){
            ret.push_back(token);
        }

        if(pos == string::npos) break;
        start = pos + 1;
    }

    return ret;
} 

int main(){
    vector<string> ret = split("Hello,World,This,is,a,test", ',');
    for(const string& s : ret){
        cout << s << endl;
    }
    return 0;
}