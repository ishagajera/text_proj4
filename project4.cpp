#include<iostream>
#include<vector>
#include<cstdlib>
#include<algorithm>
#include<stdio.h>
#include<string>
#include <chrono>
#include<unordered_map>
#include<fstream>
using namespace std::chrono;
using namespace std;
#define ll long long


int bitapkb(string &T, string &P) {
  /** Your code goes here */
  int m = P.length();
  vector<ll>mp(256,~(ll)0);

  for(int i=0;i<P.length();i++){
    mp[P[i]]= mp[P[i]]& (~((ll)1<<i));
  }
  
  ll mask_txt = ~(ll)0;
  for(int i=0;i<T.length();i++){
    mask_txt <<= 1;
    mask_txt |= mp[T[i]];
    if((mask_txt & (1<<(m-1)))==0)return i-m+1;
  }
  return -1;
}

int brute(string &T, string &P) {
  int match =-1;
  int text_len = T.length();
  int patt_len = P.length();
  for(int i =0;i<text_len;i++){
    int j=0;
    for(;j<patt_len;j++){
      if(T[i+j] != P[j]) break;
    }
    if(j == patt_len) return i;
  }
  return -1;
}

int bitap(string &T, string &P) {
  int tlen = T.length();
  int plen = P.length();
  vector<vector<int>>vec(256,vector<int>(plen,1));
  for(int i=0;i<256;i++){
    for(int j=0;j<plen;j++){
      int val = (int)P[j]; 
      if(val == i) vec[i][plen - 1 - j] = 0;
    }  
  }
   int ctr = plen-1;
   vector<int>ones(plen,1);
  for(int i=0;i<tlen;i++){
    if(ctr == plen -1){
      fill(ones.begin(),ones.end(),1);
    }
    if(ctr == 0) return i-plen+1;
    ones.erase(ones.begin());
    ones.push_back(0);
    for(int j=0;j<plen;j++){
      ones[j] |= vec[(int)T[i]][j];
    }
    if(ones[ctr] == 0){
      ctr--;
    }
    else ctr = plen-1;
  }
  return -1;
}


void fill_bad_table(vector<int>&bad_table, string &P, int n){
  for(int i=0;i<n-1;i++){
    
    bad_table[(int)P[i]] = i;
  }
}
int check_full_string(string &P, string &T, int plen , int tlen){
  int temp = tlen;
  while(P[plen] == T[tlen] && plen > 0 && tlen > 0){
      plen--;
      tlen--;
  }
  if(plen == 0) return temp;
  return tlen;

}

int bmh(string &T, string &P) {
  int txt_len = T.length();
  int patt_len = P.length();
  int ans =-1;
  vector<int>bad_table(128,-1);

  fill_bad_table(bad_table,P,patt_len);

  int j = patt_len -1;
  int i = j;
  int mis =0;
  while(i < txt_len){
    if(P[j] == T[i]){
      if(j ==0) return i;
      else{
        i--;
        j--;
      }
      } 
      else{
        //find mismtach location
        int new_loc = bad_table[(int)T[i]];
        i = i + patt_len - min(j,new_loc+1);
        j = patt_len-1;
      }
  }

  return -1;
}


void construct_lps(string &P, int n, vector<int> &lps){
  
  int len =0;
  int i=1;
  while(i<n){
    if(P[len] == P[i]){
      len+=1;
      lps[i] =len;
      i++; 
    }
    else if(len > 0) len = lps[len-1];
   
      else{
        lps[i] = 0;
        i++;
      }
    
  }
 
}

int kmp(string &T, string &P) {
  /** Your code goes here */
  int text_len = T.length();
  int patt_len = P.length();
  int ans =0;
  vector<int> lps(patt_len,0);
  construct_lps(P,patt_len,lps);
  int i = 0,j=0;
  while(i<text_len){
    if(T[i] == P[j]){
        if(j == patt_len-1) return i-j;
        else{
             i++;
            j++;
        }
    }
    else{
        if(j > 0) j = lps[j-1];
        else i++;
    }
  }
  return -1;
}

int calcHash(string s){
  int n  =s.length();
  int h = -1;
  for(int i=1;i<=n;i++){
    if(h != -1){
      h = h ^ (int(s[i-1] -'a' + 1))<<(n-i);
    }
    else{
      h = (int(s[i-1]) - 'a' + 1)<<(n-i);
    }
  }
  return h;
}

int shiftHash(string &t, string &p, int hold, int ind){
  int h = hold<<1;
  h = h ^(int(t[ind-1] - 'a') + 1)<<(p.length());
  h = h ^(int(t[ind+p.length() -1] - 'a') +1);
  return h;
}

int rk(string T, string &P) {
  int patt_len = P.length();
  int text_len = T.length();
  int hashed_val = calcHash(P);
  int f = -1;
  for(int i=0;i<text_len-patt_len+1;i++){
    if(i==0){
      f = calcHash(T.substr(0,patt_len));
    }
    else{
      f = shiftHash(T,P,f,i);
    }
      if(f == hashed_val){
        int j =0;
        while(j<patt_len && T[i+j] == P[j]) j++;
        if(j == patt_len) return i;
      }

  }

  return -1;
}

void failureFunction(int F[], string &P, int m){
  F[0]=0;
  int i=1,j=0;

  while(i<m){
    if(P[i]==P[j]){
      F[i] = j+1;
      i++;
      j++;
    } else if(j>0){
      j=F[j-1];
    } else {
      F[i]=0;
      i++;
    }
  }
}

int kmpkb(string &T, string &P) {
  /** Your code goes here */
  int n = T.length();
  int m = P.length();
  int F[m];
  failureFunction(F, P, m); 
  int i=0,j=0;
  while(i<n){
    if(T[i]==P[j]){
      if(j==m-1) {
        return i-j;
      } else{
        i++;
        j++;
      }
    } else{
      if(j>0){
        j = F[j-1];
      } else{
        i++;
      }
    }
  }
  return -1;
}
void random_ans(string &T, vector<vector<string>>&p){
      unordered_map<string,vector<double>>mp;
    vector<string> iterator ={"kmp","bmh","rk","bitap","brute"};
    for(int z=0;z<iterator.size();z++){
        for(int itr =2;itr<=10;itr++){
            auto start = high_resolution_clock::now();
            if(iterator[z] == "brute")
                for(int i=0;i<1000;i++) brute(T,p[itr][i]);
            else if(iterator[z] == "kmp")
                for(int i=0;i<1000;i++) kmp(T,p[itr][i]);
            else if(iterator[z] == "bmh")
                for(int i=0;i<1000;i++) bmh(T,p[itr][i]);
            else if(iterator[z] == "rk")
                for(int i=0;i<1000;i++) rk(T,p[itr][i]);
            else 
                for(int i=0;i<1000;i++) bitapkb(T,p[itr][i]);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            mp[iterator[z]].push_back(duration.count()/1000.00);
        }
    }
    ofstream file1("random.txt");
    
    for(auto f :mp){
        file1<<f.first;
        for(auto h : f.second){
            file1<<" "<<h;
        }
        file1<<"\n";
      }
    file1.close();
}

void speech_ans(){
     unordered_map<string,vector<double>>mp;
    vector<string> iterator ={"kmp","bmh","rk","bitap","brute"};
    string myText;
    string T;
    ifstream MyReadFile("speeches.txt");
    while (getline (MyReadFile, myText)) {
    // Output the text from the file
       T += myText;
    //    T +=" ";
    }
    //reading words2.txt:
    string temp;
    vector<vector<string>> p(11);
    ifstream words2("words2.txt");
    // Use a while loop together with the getline() function to read the file line by line
    while (getline (words2, temp)) {
      // Output the text from the file
      // cout << temp;
      
      if(temp.length() >= 2 && temp.length() <= 10){
        if(p[temp.length()].size() > 100) continue;
        p[temp.length()].push_back(temp);
      }
    }
    // cout<<T<<endl;
    cout<<"hi"<<endl;
    for(int z=0;z<iterator.size();z++){
        for(int itr =2;itr<=10;itr++){
            auto start = high_resolution_clock::now();
            // cout<<p[itr][i]<<endl;
            if(iterator[z] == "brute")
                for(int i=0;i<p[itr].size();i++) brute(T,p[itr][i]);
            else if(iterator[z] == "kmp")
                for(int i=0;i<p[itr].size();i++) kmp(T,p[itr][i]);
            else if(iterator[z] == "bmh")
                for(int i=0;i<p[itr].size();i++) bmh(T,p[itr][i]);
            else if(iterator[z] == "rk")
                for(int i=0;i<p[itr].size();i++) rk(T,p[itr][i]);
            else 
                for(int i=0;i<p[itr].size();i++) bitapkb(T,p[itr][i]);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            cout<<"avg duration for: "<<iterator[z]<<" is "<<duration.count()/p[itr].size()<<endl;
            mp[iterator[z]].push_back((float)duration.count()/p[itr].size());
        }
    }
    ofstream file2("speech.txt");
    
    for(auto f :mp){
        file2<<f.first;
        // cout<<f.first<<" ";
        for(auto h : f.second){
            // cout<<h<<" ";   
            file2<<" "<<h;
        }
        file2<<"\n";
        
        // cout<<endl;
    }
    file2.close();
}

int main(){

    srand((unsigned) time(NULL)); 
    int a = 32; //start num
    int b = 91; // offset = last num +1;
    string T;
    vector<vector<string>>p(11);
    for(int i=0;i<10000;i++){
        auto x =  a+ (rand() % b);
        T += (char)x;
    }

    for(int i=2;i<=10;i++){
          vector<string> temp(1000);
        for(int j=0;j<1000;j++){
            for(int k=0;k<i;k++){
                 auto x =  a+ (rand() % b) ;
                 temp[j]  += (char)x;
            }
        }
        p[i] = temp;
    }
    // map is storing average time of running each code for a given algo
    random_ans(T,p);
    
    // ofstream file2("speech.txt");
    speech_ans();

    return 0;
}