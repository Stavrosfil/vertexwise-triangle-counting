
// #if defined(DEBUG) && DEBUG > 0
// #define DEBUG_PRINT(fmt, args...) fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##args)
// #else
// #define DEBUG_PRINT(fmt, args...) /* Don't do anything in release builds */
// #endif

// #ifdef DEBUG
// #define DEBUG_PRINT(...) \
//     do { \
//         fprintf(stderr, __VA_ARGS__); \
//     } while (false)
// #else
// #define DEBUG_PRINT(...) \
//     do { \ } while (false)
// #endif

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)                                                                                                 \
    do {                                                                                                               \
    } while (0)
#endif
