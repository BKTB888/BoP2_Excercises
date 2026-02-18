//
// Created by Bánfalvi-Kovács Tivadar on 2026. 02. 18..
//

#include <cstddef>

#include "lab1.h"

///If you want try these out simply copy-paste them
// ===== Refresher for C =====
void change_c(int *r) {
    *r = 20;
}

void increment(int* value) {
    if (value != NULL) {
        ++*value;
    }
}

void swap_c(int* a, int* b) {
    const int temp = *a;
    *a = *b;
    *b = temp;
}

int sum_c(const int* arr, const int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += *(arr + i);
    }
    return 0;
}

void move_point_c(Point* p, int x, int y) {
    p->x = x;
    p->y = y;
}


// ===== C++ =====
void change_cpp(int& r) {
    r = 20;
}

void swap_cpp(int& a, int& b) {
    const int temp = a;
    a = b;
    b = temp;
}

void move_point_cpp(Point& p, int x, int y) {
    p.x = x;
    p.y = y;
}

int square(const int& x) {
    return x*x;
}