#ifndef _FILTER_H_
#define _FILTER_H_
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
// https://blog.csdn.net/qiu421/article/details/136458316

float butterWorth2Order(float input);
double butterWorth2Orderdouble(float input);
#ifdef __cplusplus
}
#endif

#endif