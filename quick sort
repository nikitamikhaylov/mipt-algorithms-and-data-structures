#include <cstdio>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <algorithm>
#include <ctime>

const int MAXSTACK = 256;
const int maxn = 25000005;

static const int buf_size = 1 << 16;

inline int getChar() {
	static char buf[buf_size];
	static int len = 0, pos = 0;
	if (pos == len) {
        pos = 0;
        len = fread(buf, 1, buf_size, stdin);
        if (!len) {
            return -1;
        }
	}
	return buf[pos++];
}

inline bool readInt(int &x) {
    x = 0;
    int c = getChar() - '0';
    if (c < 0) {
        return false;
    }
    while (0 <= c && c <= 9) {
        x = x * 10 + c;
        c = getChar() - '0';
    }
    return true;
}

static int write_pos = 0;
static char write_buf[buf_size];

inline void writeChar( int x ) {
	if (write_pos == buf_size) {
        fwrite(write_buf, 1, buf_size, stdout);
        write_pos = 0;
	}
	write_buf[write_pos++] = x;
}

inline void writeInt(int x) {
	static int s[12];
	int n = 0;
	while (x || !n) {
        s[n++] = '0' + x % 10;
        x /= 10;
	}
	while (n--) {
        writeChar(s[n]);
	}
}

struct Flusher {
	~Flusher() {
		if (write_pos) {
            fwrite(write_buf, 1, write_pos, stdout);
            write_pos = 0;
		}
	}
} flusher;



void InsertionSort(int *left, int *right) {
    for (int *i = left; i != right; ++i) {
        for (int *j = i, *k = i - 1; j != left && *j < *k; --j, --k) {
            std::swap(*j, *k);
        }
    }
}

int* BeginStack[MAXSTACK];
int* EndStack[MAXSTACK];

void CircleQuickSort (int *start, int *finish) {
    BeginStack[1] = start;
    EndStack[1] = finish - 1;
    int StackPos = 1;
    int *left, *right;
    int n;
    int *middle;
    int *i, *j;
    srand(42);
    do {
        left = BeginStack[ StackPos ];
        right = EndStack[ StackPos ];
        --StackPos;
        do {
            n = right - left + 1;
            if (n <= 45) {
                InsertionSort(left, right + 1);
                left = right + 1;
            } else {
                middle = left + (n >> 1);
                i = left;
                j = right;

                int pivot = *(left + rand() % n);

                for (;;) {
                    while (*i < pivot) ++i;
                    while (pivot < *j) --j;

                    if (i > j) break;
                    std::swap(*i, *j);
                    ++i; --j;
                }
                if ( i < middle ) {
                    ++StackPos;
                    BeginStack[ StackPos ] = i;
                    EndStack[ StackPos ] = right;
                    right = j;
                } else {
                    ++StackPos;
                    BeginStack[ StackPos ] = left;
                    EndStack[ StackPos ] = j;
                    left = i;
                }
            }
        } while (left < right);
    } while (StackPos != 0);
}

int a[maxn];

int main()
{
    int n = 0;
    while (readInt(a[n])) {
        ++n;
    }

    CircleQuickSort(a, a + n);

    for (int i = 9; i < n; i += 10){
        writeInt(a[i]);
        writeChar(' ');
    }

    return 0;
}
