#include <iostream>
#include "gtest_lite.h"
#include "Lab1/lab1.h"

/// I am using a library to help me test your solutions,
/// it uses macros, but I hope it works intuitively

int end() {
    if (finished == 9 && !gtest_lite::test.fail()) {
        std::cout << "Great! All done!" << std::endl;
        return 0;
    }
    std::cout << "Finished = " << finished << std::endl;
    return 1;
}

int main() {

    // ===== C TESTS =====

    if (finished < 1) return end();
    TEST(Lab1_c, change) {
        int x = 10;
        change_c(&x);
        EXPECT_EQ(20, x); // expected behavior
    } END

    if (finished < 2) return end();
    TEST(Lab1_c, increment_valid) {
        int x = 10;
        increment(&x);
        EXPECT_EQ(11, x);
    } END
    TEST(Lab1_c, increment_null) {
        // Should not crash with NULL
        increment(NULL);
        SUCCEED(); // just passes if no crash occurs
    } END

    if (finished < 3) return end();
    TEST(Lab1_c, swap) {
        int x = 3, y = 7;
        swap_c(&x, &y);
        EXPECT_EQ(7, x);
        EXPECT_EQ(3, y);
    } END

    if (finished < 4) return end();
    TEST(Lab1_c, sum) {
        const int arr[] = {1, 2, 3, 4, 5};
        const int result = sum_c(arr, 5);
        EXPECT_EQ(15, result);
    } END

    if (finished < 5) return end();
    TEST(Lab1_c, move_point) {
        Point p = {2, 3};
        const int x=5, y=7;
        move_point_c(&p, x, y);
        EXPECT_EQ(x, p.x);
        EXPECT_EQ(y, p.y);
    } END

    // ===== C++ TESTS =====

    if (finished < 6) return end();
    TEST(Lab1_cpp, change) {
        int x = 10;
        change_cpp(x);
        EXPECT_EQ(20, x);
    } END

    if (finished < 7) return end();
    TEST(Lab1_cpp, swap) {
        int x = 4, y = 9;
        swap_cpp(x, y);
        EXPECT_EQ(9, x);
        EXPECT_EQ(4, y);
    } END

    if (finished < 8) return end();
    TEST(Lab1_cpp, move_point) {
        Point p = {1, 1};
        const int x = 3, y = 4;
        move_point_cpp(p, x, y);
        EXPECT_EQ(x, p.x);
        EXPECT_EQ(y, p.y);
    } END

    if (finished < 9) return end();
    TEST(Lab1_cpp, square) {
        const int a = 6;
        const int result = square(a);
        EXPECT_EQ(36, result);
    } END

    return end();
}