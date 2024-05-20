#include <limits.h>

#ifdef PATH_MAX
    // Use PATH_MAX
#else
    // Defina seu próprio valor máximo
    constexpr size_t PATH_MAX = 4096;  // Valor arbitrário para o comprimento máximo do caminho
#endif
