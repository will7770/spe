#pragma once
#include <stdint.h>

typedef uint8_t ui8;
typedef int8_t i8;

typedef uint16_t ui16;
typedef int16_t i16;

typedef uint32_t ui32;
typedef int32_t i32;

typedef uint64_t ui64;
typedef int64_t i64;

typedef float f32;
typedef double f64;
typedef long double f128;


typedef struct {
    ui32 x;
    ui32 y;
} Vec2;

typedef struct {
    f32 x;
    f32 y;
} FVec2;

static inline FVec2 fvec2_mult(FVec2 v1, FVec2 v2) {
    return (FVec2){ v1.x*v2.x, v1.y*v2.y };
}

static inline FVec2 fvec2_add(FVec2 v1, FVec2 v2) {
    return (FVec2){ v1.x+v2.x, v1.y+v2.y };
}

static inline FVec2 fvec2_scalar(FVec2 v, f32 mul) {
    return (FVec2){ v.x*mul, v.y*mul };
}