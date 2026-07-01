#include "comparators.h"
#include <stdio.h>

int char_comparator(void *val1, void *val2){
    char c1 = *(char *)val1;
    char c2 = *(char *)val2;
    return c1 < c2;
}

int short_comparator(void *val1, void *val2){
    short s1 = *(short *)val1;
    short s2 = *(short *)val2;
    return s1 < s2;
}

int int_comparator(void *val1, void *val2){
    int i1 = *(int *)val1;
    int i2 = *(int *)val2;
    return i1 < i2;
}

int long_comparator(void *val1, void *val2){
    long l1 = *(long *)val1;
    long l2 = *(long *)val2;
    return l1 < l2;
}

int long_long_comparator(void *val1, void *val2){
    long long ll1 = *(long long *)val1;
    long long ll2 = *(long long *)val2;
    return ll1 < ll2;
}

int float_comparator(void *val1, void *val2){
    float f1 = *(float *)val1;
    float f2 = *(float *)val2;
    return f1 < f2;
}

int double_comparator(void *val1, void *val2){
    double d1 = *(double *)val1;
    double d2 = *(double *)val2;
    return d1 < d2;
}
