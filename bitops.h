#define ffs(x) __builtin_ffs(x)
#define clz(x) __builtin_clz(x)
#define log2(x) (8 * sizeof(x) - 1 - __builtin_clz(x))
