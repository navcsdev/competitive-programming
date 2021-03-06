#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cctype>
#include<cstring>
#include<iostream>
#include<algorithm>
#include<string>
#include<bitset>
#include<vector>
#include<queue>
#include<map>
#include<set>
#include<list>
#include<stack>
#include<sstream>
#include<utility>
#include<numeric>
#include<functional>
using namespace std;

#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define F(i,a) FOR(i,0,a)
#define ALL(x) x.begin(),x.end()
#define PB push_back
#define MP make_pair
#define NL printf("\n")
#define SP system("pause")
#define IMAX 2147483647
#define TR(container, it) \
      for(typeof(container.begin()) it=container.begin(); it!=container.end(); it++)
typedef long long LL;
#define MAXN 100001
int n,t,m;  //n es el tama�o de la cadena
int p[MAXN],r[MAXN],h[MAXN];
//p es el inverso del suffix array, no usa indices del suffix array ordenado
//h el el tama�o del lcp entre el i-esimo y el i+1-esimo elemento de suffix array ordenado
char s[MAXN];
char s2[MAXN];
bool belong[MAXN];
void fix_index(int *b, int *e) {
   int pkm1, pk, np, i, d, m;
   pkm1 = p[*b + t];
   m = e - b; d = 0;
   np = b - r;
   for(i = 0; i < m; i++) {
      if (((pk = p[*b+t]) != pkm1) && !(np <= pkm1 && pk < np+m)) {
         pkm1 = pk;
         d = i;
      }
      p[*(b++)] = np + d;
   }
}

bool comp(int i, int j) {
   return p[i + t] < p[j + t];
}
void suff_arr() {
   int i, j, bc[256];
   t = 1;
   for(i = 0; i < 256; i++) bc[i] = 0;  //alfabeto
   for(i = 0; i < n; i++) ++bc[int(s[i])]; //counting sort inicial del alfabeto
   for(i = 1; i < 256; i++) bc[i] += bc[i - 1];
   for(i = 0; i < n; i++) r[--bc[int(s[i])]] = i;
   for(i = n - 1; i >= 0; i--) p[i] = bc[int(s[i])];
   for(t = 1; t < n; t *= 2) {
      for(i = 0, j = 1; i < n; i = j++) {
         while(j < n && p[r[j]] == p[r[i]]) ++j;
         if (j - i > 1) {
            sort(r + i, r + j, comp);
            fix_index(r + i, r + j);
         }
      }
   }
}

void lcp() {
   int tam = 0, i, j;
   for(i = 0; i < n; i++)if (p[i] > 0) {
      j = r[p[i] - 1];
      while(s[i + tam] == s[j + tam]) ++tam;
      h[p[i] - 1] = tam;
      if (tam > 0) --tam;
   }
   h[n - 1] = 0;
}

int bin_search(int i,int j)
{
    string check=string(s2,s2+m);
    while(i <= j)
    {
         int mid = (i+j)/2;
         //printf("[%d,%d] %d %s\n",i,j,mid,s+r[mid]);
         if(n - r[mid] >= m)
         { 
            string a = string(s+r[mid],s+r[mid]+m);
            string b = check;
            //printf("[%s]\n",a.c_str());
            //printf("[%s]\n",b.c_str());
            if(string(s+r[mid],s+r[mid]+m) == check)
              return 1;
         }
         if(string(s+r[mid],s+n) < check)
            i = mid+1;
         else
            j = mid-1;
         //SP;
    }
    return 0;
}

int main()
{
    int cas,q;
    int ans;

         gets(s);
         n = strlen(s);
         s[n++] = '$';
         suff_arr();

         scanf("%d",&q);
         F(i,q)
         {
              scanf("%s",s2);           
              m = strlen(s2);
              ans = bin_search(0,n); 
              if(ans)
                 puts("Y")
              else 
                 puts("N");
         }         
         memset(s,'\0',sizeof s);
    return 0;
}
