#include <stdio.h>
#define FOR(a,b) for(a=0;a<b;a++)
#define BITSPERWORD 32
#define SHIFT 5
#define MASK 0x1F
#define M 10000000
int a[1 + M/BITSPERWORD];

void set(int i){ a[i>>SHIFT] |= (1<<(i & MASK)); }
void clr(int i){ a[i>>SHIFT] &= ~(1<<(i & MASK)); }
int test(int i){ return a[i>>SHIFT] & (1<<(i & MASK)); }


