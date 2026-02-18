#include <iostream>
#include "gtest_lite.h"
#include "Lab1/lab1.h"

int main() {

    // ===== C TESTS =====

    TEST(Lab1_c, swap) {
        int x = 3, y = 7;
        swap_c(&x, &y);
        EXPECT_EQ(7, x);
        EXPECT_EQ(3, y);
    } END

    TEST(Lab1_c, sum) {
        int arr[] = {1, 2, 3, 4, 5};
        const int result = sum_c(arr, 5);
        EXPECT_EQ(15, result);
    } END

    TEST(Lab1_c, move_point) {
        Point p = {2, 3};
        move_point_c(&p, 5, -1);
        EXPECT_EQ(7, p.x);
        EXPECT_EQ(2, p.y);
    } END

    TEST(Lab1_c, increment_valid) {
        int x = 10;
        increment(&x);
        EXPECT_EQ(11, x);
    } END

    TEST(Lab1_c, increment_null) {
        // Should not crash with NULL
        increment(nullptr);
        SUCCEED(); // just passes if no crash occurs
    } END

    TEST(Lab1_c, change) {
        int x = 10;
        change_c(&x);
        EXPECT_EQ(20, x); // expected behavior
    } END

    // ===== C++ TESTS =====

    TEST(Lab1_cpp, swap) {
        int x = 4, y = 9;
        swap_cpp(x, y);
        EXPECT_EQ(9, x);
        EXPECT_EQ(4, y);
    } END

    TEST(Lab1_cpp, move_point) {
        Point p = {1, 1};
        move_point_cpp(p, 3, 4);
        EXPECT_EQ(4, p.x);
        EXPECT_EQ(5, p.y);
    } END

    TEST(Lab1_cpp, change) {
        int x = 10;
        change(x);
        EXPECT_EQ(20, x);
    } END

    TEST(Lab1_cpp, square) {
        const int a = 6;
        const int result = square(a);
        EXPECT_EQ(36, result);
    } END

    return 0;
}