//
// Created by Bánfalvi-Kovács Tivadar on 2026. 02. 18..
//

#ifndef EXCERCISES_LAB1_H
#define EXCERCISES_LAB1_H

struct Point {
    int x;
    int y;
};

//C
void change_c(int* r);
void swap_c(int* a, int* b);
int sum_c(const int* arr, int size);
void move_point_c(Point* p, int x, int y);
void increment(int* value);

//C++
void change_cpp(int& r);
void swap_cpp(int& a, int& b);
void move_point_cpp(Point& p, int x, int y);
int square(const int& x);
#endif //EXCERCISES_LAB1_H