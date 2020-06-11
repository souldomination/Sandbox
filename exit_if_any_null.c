#include <stdlib.h>

#define AT_1(x) (x)
#define AT_2(x, ...) ((x) && AT_1(__VA_ARGS__))
#define AT_3(x, ...) ((x) && AT_2(__VA_ARGS__))
#define AT_4(x, ...) ((x) && AT_3(__VA_ARGS__))
#define AT_5(x, ...) ((x) && AT_4(__VA_ARGS__))
#define AT_6(x, ...) ((x) && AT_5(__VA_ARGS__))
#define AT_7(x, ...) ((x) && AT_6(__VA_ARGS__))
#define AT_8(x, ...) ((x) && AT_7(__VA_ARGS__))
#define AT_9(x, ...) ((x) && AT_8(__VA_ARGS__))
#define AT_10(x, ...) ((x) && AT_9(__VA_ARGS__))
#define AT_11(x, ...) ((x) && AT_10(__VA_ARGS__))
#define AT_12(x, ...) ((x) && AT_11(__VA_ARGS__))
#define AT_13(x, ...) ((x) && AT_12(__VA_ARGS__))
#define AT_14(x, ...) ((x) && AT_13(__VA_ARGS__))
#define AT_15(x, ...) ((x) && AT_14(__VA_ARGS__))
#define AT_16(x, ...) ((x) && AT_15(__VA_ARGS__))

#define VA_GENERIC(                                                            \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16,     \
    x, ...                                                                     \
) x
#define ALL_TRUE(...)                                                          \
VA_GENERIC(                                                                    \
    __VA_ARGS__,                                                               \
    AT_16, AT_15, AT_14, AT_13, AT_12, AT_11, AT_10, AT_9,                     \
    AT_8, AT_7, AT_6, AT_5, AT_4, AT_3, AT_2, AT_1                             \
)(__VA_ARGS__)

#define EXIT_IF_ANY_NULL(...)                                                  \
do {                                                                           \
    if (!ALL_TRUE(__VA_ARGS__)) {                                              \
        if (myid == 0) {                                                       \
            fprintf(stderr, "ERROR: cannot allocate memory");                  \
        }                                                                      \
        exit(0);                                                               \
    }                                                                          \
} while (0)

int main(void) {
    int *a = malloc(8 * sizeof(int));
    double *b = malloc(4 * sizeof(double));
   
    // check if any of a and b is NULL
    EXIT_IF_ANY_NULL(a, b);
    
    // some codes using a and b...
}
