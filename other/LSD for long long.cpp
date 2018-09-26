#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <cstring>

unsigned int getByte(long long x, int i)
{
    x = x >> 8 * i;
    x &= 0xff;
    return x;
}

void CountingSort( long long* a, int n, int byte ) {
    std::vector<std::vector<long long int>> c(256);
    for (int i = 0; i < n; ++i) {
        c[getByte(a[i], byte)].push_back(a[i]);
    }
    int count = 0;
    for (int i = 0; i < 256; ++i){
        int k = c[i].size();
        if (k > 0){
            for (int j = 0; j < k; ++j){
                a[count] = c[i][j];
                count++;
            }
        }
    }
}

void LSDSort( long long* a, int n ) {
    for( int r = 0; r < sizeof( long long ); ++r )
        CountingSort( a, n, r );
}

int main()
{
    int n = 0;
    std::cin >> n;
    long long *a = new long long [n];
    for (int i = 0; i < n; ++i){
        std::cin >> a[i];
    }
    LSDSort(a, n);
    for (int i = 0; i < n; ++i){
        std::cout << a[i] << ' ';
    }
    delete [] a;
    return 0;
}
