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
 
#define MAXN 150010            //max size of the word + '$' characters
 
typedef long long LL;

struct word{
       string str;
       int size;
       int old;
       bool operator<(const word &other) const        //comparator to do topological sort
       {
            if(size != other.size)
               return size < other.size;
            if(str != other.str)
               return str < other.str;
            return old < other.old;
       }
} p;
 
int belong[MAXN];           //stores where the suffix array r[i] belongs to
                            //in other words belong[r[i]] means that the suffix r[i]
                            //belong to the word[j]
                            
int size;					//size of the string
int t;  					//pow 2 helper

/*
 * suffix array SA holds the starting indexes of all suffixes in a sorted way
 * such that the suffix starting at SA[i] is lexicographically lower than
 * the suffix starting at SA[i + 1]
 */
int SA[MAXN];

/*
 * SA_inverse is the inverse of the suffix array
 * SA_inverse describes the position 'j' in the suffix array of a suffix starting at SA[i]
 */
int SA_inverse[MAXN];

/*
 * lcp is the longest common prefix between suffixes starting at SA[i] and SA[i + 1]
 */
int lcp[MAXN];

/*
 * str is the initial string concatenated with '$'
 */
char str[MAXN];

void fix_index(int *begin, int *end)
{
   int position;			//partial position
   int new_position;		//new position of the suffix starting at SA[i]
   	   	   	   	   	   	   	//this position goes from old_position up to
   	   	   	   	   	   	    //old_position + bucket_size
   int old_position;		//old position of this suffix starting at SA[i] i = begin..end
   int i;					//loop
   int difference;			//difference between this suffix and the next in the bucket
   int bucket_size;			//number of suffixes to be correctly positioned

   position = SA_inverse[*begin + t];
   bucket_size = end - begin;
   difference = 0;
   old_position = begin - SA;

   for(i = 0; i < bucket_size; i++)
   {
	  //compare the positions of the suffixes SA[i + t] i = begin..end
	  //if they are the same that means that they still have the t-th character in common
	  //but if they are different we know that the difference is the number of
	  //positions advanced from *begin
      if (((new_position = SA_inverse[*begin + t]) != position) &&
    		  !(old_position <= position && new_position < old_position + bucket_size))
      {
         position = new_position;
         difference = i;
      }
      //store the new position of the suffix starting at SA[i] i = begin..end
      //in the SA_inverse array
      SA_inverse[*(begin++)] = old_position + difference;
   }
}

//compare the t-th element of the suffixes
bool comp(int i, int j)
{
   return SA_inverse[i + t] < SA_inverse[j + t];
}

void suffix_array() {
   int i, j;				//loop
   int counting[256];		//help to do counting sort
   t = 1;					//t is the number of already checked characters
   	   	   	   	   	   	   	//t is a power of 2
   memset(counting, 0, sizeof counting);

   //counting the occurrences of every first character from
   //all suffixes of the string str
   for(i = 0; i < size; ++i)
	   ++counting[int(str[i])];

   //get prefix sums
   //from this we can deduce that the number of suffixes
   //that start with the character i is equal to counting[i] - counting[i - 1]
   //and that the number of sorted suffixes that must go before this
   //bucket is equal to counting[i - 1]
   for(i = 1; i < 256; ++i)
	   counting[i] += counting[i - 1];

   //after doing the counting sort for each starting character of every suffix
   //we have the suffixes partially sorted in buckets according to the same
   //starting character, therefore we assign to every suffix that belongs
   //to a common bucket starting with some initial character a sequential number
   //that goes from counting[i] - 1 to counting[i - 1] inclusive
   for(i = 0; i < size; ++i)
	   SA[--counting[int(str[i])]] = i;

   //initially for all suffixes starting with the same letter we know that
   //they all belong to the bucket j and the bucket j is of size k where
   //k is the number of suffixes that start with the same character
   //therefore the inverse of the suffix array will hold the same number
   //for all k common suffixes that start with the same starting character
   //and that number is equal to the number of sorted suffixes that must go
   //before this bucket of suffixes
   for(i = size - 1; i >= 0; i--)
	   SA_inverse[i] = counting[int(str[i])];

   //now for every bucket sort it by the first t starting characters
   //where t is a power of 2
   for(t = 1; t < size; t *= 2)
   {
      for(i = 0, j = 1; i < size; i = j++)
      {
    	 //get the bucket that start with the same first t characters
         while(j < size && SA_inverse[SA[j]] == SA_inverse[SA[i]])
        	 ++j;
         if (j - i > 1)
         {
        	//sort this bucket according to the first t characters
            sort(SA + i, SA + j, comp);
            //update SA_inverse so that it holds the the correct position
            //of the suffixes starting at SA[k] k = i..j
            fix_index(SA + i, SA + j);
         }
      }
   }
}

void build_lcp()
{
	int tam = 0;		//initially no common characters
	int i, j;			//loop
	for(i = 0; i < size; i++)
		if (SA_inverse[i] > 0)	//check only for the suffixes that have a predecessor
		{
			//get the previous suffix from the SA
			j = SA[SA_inverse[i] - 1];

			//check starting from the character at position tam in the string
			while(str[i + tam] == str[j + tam])
				++tam;

			//LCP is the common characters from suffix[j] and suffix[i]
			lcp[SA_inverse[i] - 1] = tam;

			//we know that the next suffix must share at least tam - 1 characters
			//only if tam is bigger than zero
			if (tam > 0)
				--tam;
		}
	//size of the last suffix is zero
	lcp[size - 1] = 0;
}

int bin_search(char *q, int m)
{
	int lower, upper;	//bounds
	lower = 0;			//lower bound
	upper = size;		//upper bound
	while(lower <= upper)
	{
		 //get the mid index
		 int mid = (lower + upper) >> 1;
		 int cmp = strncmp(str + SA[mid], q, size);		//check if the word exists
		 if(cmp == 0)				//the string q is a substring of str
			return mid;				//return the position in the string str

		 //divide the searching space into two spaces
		 if(cmp < 0)
			lower = mid + 1;
		 else
			upper = mid - 1;
	}
	return -1;
}

int main()
{
    char pattern[MAXN];
    int n_words;              //number of words of this test case
    int n_pattern;
    int i,j,k;                //loop
    int check;
    //dictionary
    scanf("%d", &n_words);
    vector<word> dict(n_words);
    for(i = 0; i < n_words; ++i)
    {
        cin >> p.str;
        p.old = i;
        p.size = p.str.size();
        dict[i] = p;
    }
    sort(dict.begin(), dict.end());
    size = 0;
    for(i = 0; i < n_words; ++i)
    {
        for(j = 0; dict[i].str[j]; ++j)
            str[size] = dict[i].str[j],
            belong[size++] = i;
        str[size++] = (char) (i % 20 + '$');
    }
    
    suffix_array();
    build_lcp();
    printf("here");
    //patterns
    scanf("%d", &n_pattern);
    for(i = 0; i < n_pattern; ++i)
    {
        scanf("%s", pattern);
        int pattern_size = strlen(pattern);
        int find_mid = bin_search(pattern, pattern_size);

        if(find_mid == -1)
        {
             puts("-1");
             continue;
        }
        
        //mark this location in the graph
        set<int> answer;
        answer.insert(dict[belong[SA[find_mid]]].old);

        //check backwards in the suffix array
        check = find_mid - 1;
        while(check >= 0 && lcp[check] >= pattern_size)
        {
           answer.insert(dict[belong[SA[check]]].old);
           --check;
        }

        //check forward in the suffix array
        check = find_mid;
        while(check < size && lcp[check] >= pattern_size)
        {
           answer.insert(dict[belong[SA[check + 1]]].old);
           ++check;
        }
        
        j = 0;
        TR(answer, it)
        {
            if(j)putchar(' ');
            printf("%d", *it);
            ++j;
            if(j == 10)
               break;       
        }
        NL;
    }
    return 0;
}
