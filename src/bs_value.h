#ifndef bs_value_h
#define bs_value_h

#include <stdlib.h>
#include "bs.h"

#define BS_VALUE(type, ...) \
    new_value((bs_value){type, {.type = (struct type){__VA_ARGS__}}})

#define AS_VAL(val, bs_type) (val->as.bs_type)
#define AS_CVAL(val, bs_type) (val->as.bs_type.value)
#define IS_TYPE(val, bs_type) (val->type == bs_type)

typedef enum
{
    BS_BOOL,
    BS_INT64,
    BS_UINT64,
    BS_INT32,
    BS_UINT32,
    BS_INT16,
    BS_UINT16,
    BS_INT8,
    BS_UINT8,
    BS_STRING,
    BS_FUNCTION,
    BS_OBJECT,
} bs_value_type;

typedef struct
{
    bs_value_type type;
    union
    {
        struct BS_BOOL
        {
            bool value;
        } BS_BOOL;
        struct BS_INT8
        {
            int8_t value;
        } BS_INT8;
        struct BS_UINT8
        {
            uint8_t value;
        } BS_UINT8;
        struct BS_INT16
        {
            int16_t value;
        } BS_INT16;
        struct BS_UINT16
        {
            uint16_t value;
        } BS_UINT16;
        struct BS_INT32
        {
            int32_t value;
        } BS_INT32;
        struct BS_UINT32
        {
            uint32_t value;
        } BS_UINT32;
        struct BS_INT64
        {
            int64_t value;
        } BS_INT64;
        struct BS_UINT64
        {
            uint64_t value;
        } BS_UINT64;
        struct BS_STRING
        {
            char *value;
            int length;
        } BS_STRING;
    } as;
} bs_value;

bs_value *new_value(bs_value value);
void free_value(bs_value *value);

#endif