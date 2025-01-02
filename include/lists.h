#ifndef LISTS_H
#define LISTS_H

#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

/*------------------.
| Generic Macros    |
`-----------------*/

#define ListDef(TypeName, DataType) \
    typedef struct { \
        DataType* items; \
        size_t count; \
    } TypeName

#define ListInit(TypeName, DataType, END_TOKEN)                         \
    TypeName result = {0};                                              \
                                                                        \
    if (first == END_TOKEN) {                                           \
        return result;                                                  \
    }                                                                   \
                                                                        \
    va_list args;                                                       \
                                                                        \
    /* First pass: Count arguments */                                   \
    va_start(args, first);                                              \
    size_t count = 1;                                                   \
    DataType current;                                                   \
    while ((current = va_arg(args, DataType)) != END_TOKEN) {           \
        count++;                                                        \
    }                                                                   \
    va_end(args);                                                       \
                                                                        \
    /* Allocate memory for items */                                     \
    result.items = (DataType*)malloc(sizeof(DataType) * count);         \
    if (!result.items) {                                                \
        return result;                                                  \
    }                                                                   \
    result.count = count;                                               \
                                                                        \
    /* Second pass: Populate items array */                             \
    va_start(args, first);                                              \
    result.items[0] = first;                                            \
    for (size_t i = 1; i < count; i++) {                                \
        result.items[i] = va_arg(args, DataType);                       \
    }                                                                   \
    va_end(args);                                                       \
                                                                        \
    return result;                                                      \

#define ListFree(list) \
    if(list->items != NULL) \
        free(list->items); \
    list->items = NULL; \
    list->count = 0;

/*------------------.
| size_t list       |
`-----------------*/

ListDef(Sizes, size_t);
#define SIZESEND ((size_t) -1)
static inline Sizes SizesInit(size_t first, ...)
{
    ListInit(Sizes, size_t, SIZESEND)
}
static inline void SizesFree(Sizes *sizes)
{
    ListFree(sizes);
}

/*------------------.
| double list       |
`-----------------*/

ListDef(Doubles, double);
#define DOUBLESEND ((double) -1)
static inline Doubles DoublesInit(double first, ...)
{
    ListInit(Doubles, double, DOUBLESEND)
}
static inline void DoublesFree(Doubles *list)
{
    ListFree(list);
}
void DoublesZero(Doubles* doubles, size_t count);
void DoublesRand(Doubles* doubles, size_t count);

/*------------------.
| double* list      |
`-----------------*/

ListDef(Doubles2D, double*);
#define DOUBLES2DEND NULL
static inline Doubles2D Doubles2DInit(double* first, ...)
{
    ListInit(Doubles2D, double*, NULL);
}
static inline void Doubles2DFree(Doubles2D* list)
{
    ListFree(list);
}

#endif // LISTS_H
