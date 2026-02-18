#ifndef GTEST_LITE_H
#define GTEST_LITE_H

/**
 * \file gtest_lite.h  (v2)
 *
 * A testing framework similar to Google gtest.
 * Sz.I. 2015., 2016., 2017. (_Has_X)
 * Sz.I. 2018 (template), ENDM, ENDMsg
 *
 * Provides the most basic testing functions and macros.
 * Not thread-safe.
 *
 * All test cases must be implemented in a single file!
 * Only this file should include gtest_lite.h
 * To bypass this limitation, a singleton would be needed to implement
 * the test class properly.
 *
 * Free to use and extend.
 *
 * Usage example:
 *   Test the function f(x) = 2*x:
 * int f(int x) { return 2*x; }
 *
 * int main() {
 *   TEST(TestCaseName, TestName)
 *     EXPECT_EQ(0, f(0));
 *     EXPECT_EQ(4, f(2)) << "Function returned incorrect result" << std::endl;
 *     ...
 *   END
 * ...
 *
 * Understanding the implementation details is an optional exercise.
 */

#include <iostream>
#include <cassert>
#include <cmath>
#include <cstring>
#include <limits>
#include <string>
#include <fstream>
#ifdef MEMTRACE
#include "memtrace.h"
#endif

// Two macros before and after each test:
// They create a new block with braces, so local names
// avoid collisions.

/// Start of a test. The macro parameters are similar
/// to gtest parameters. This allows easy migration
/// to the gtest framework.
/// @param C - test case name (separate for gtest compatibility)
/// @param N - test name
#define TEST(C, N) { gtest_lite::test.begin(#C"."#N);

/// End of test case
#define END gtest_lite::test.end(); }

/// End of test case with comparison of allocated blocks
/// This check is not foolproof.
#define ENDM gtest_lite::test.end(true); }

/// End of test case with comparison of allocated blocks
/// If there is an error, it prints the message.
#define ENDMsg(t) gtest_lite::test.end(true) << t << std::endl; }

// Macros to help check results.
// Parameters and functions are similar to gtest.

/// Successful test macro
#define SUCCEED() gtest_lite::test.expect(true, __FILE__, __LINE__, "SUCCEED()", true)

/// Failed test macro
#define FAIL() gtest_lite::test.expect(false, __FILE__, __LINE__, "FAIL()", true)

/// Macro to expect equality
#define EXPECT_EQ(expected, actual) gtest_lite::EXPECT_(expected, actual, gtest_lite::eq, __FILE__, __LINE__, "EXPECT_EQ(" #expected ", " #actual ")" )

/// Macro to expect inequality
#define EXPECT_NE(expected, actual) gtest_lite::EXPECT_(expected, actual, gtest_lite::ne, __FILE__, __LINE__, "EXPECT_NE(" #expected ", " #actual ")", "expected" )

/// Macro to expect less than or equal
#define EXPECT_LE(expected, actual) gtest_lite::EXPECT_(expected, actual, gtest_lite::le, __FILE__, __LINE__, "EXPECT_LE(" #expected ", " #actual ")", "expected" )

/// Macro to expect less than
#define EXPECT_LT(expected, actual) gtest_lite::EXPECT_(expected, actual, gtest_lite::lt, __FILE__, __LINE__, "EXPECT_LT(" #expected ", " #actual ")", "expected" )

/// Macro to expect greater than or equal
#define EXPECT_GE(expected, actual) gtest_lite::EXPECT_(expected, actual, gtest_lite::ge, __FILE__, __LINE__, "EXPECT_GE(" #expected ", " #actual ")", "expected" )

/// Macro to expect greater than
#define EXPECT_GT(expected, actual) gtest_lite::EXPECT_(expected, actual, gtest_lite::gt, __FILE__, __LINE__, "EXPECT_GT(" #expected ", " #actual ")", "expected" )

/// Macro to expect true
#define EXPECT_TRUE(actual)  gtest_lite::EXPECT_(true, actual,  gtest_lite::eq, __FILE__, __LINE__, "EXPECT_TRUE(" #actual ")" )

/// Macro to expect false
#define EXPECT_FALSE(actual) gtest_lite::EXPECT_(false, actual, gtest_lite::eq, __FILE__, __LINE__, "EXPECT_FALSE(" #actual ")" )

/// Macro to expect floating point equality
#define EXPECT_FLOAT_EQ(expected, actual)  gtest_lite::EXPECT_(expected, actual, gtest_lite::almostEQ, __FILE__, __LINE__, "EXPECT_FLOAT_EQ(" #expected ", " #actual ")" )

/// Macro to expect double equality
#define EXPECT_DOUBLE_EQ(expected, actual) gtest_lite::EXPECT_(expected, actual, gtest_lite::almostEQ, __FILE__, __LINE__, "EXPECT_DOUBLE_EQ(" #expected ", " #actual ")" )

/// Macro to test C strings (const char *) equality
#define EXPECT_STREQ(expected, actual) gtest_lite::EXPECTSTR(expected, actual, gtest_lite::eqstr, __FILE__, __LINE__, "EXPECT_STREQ(" #expected ", " #actual ")" )

/// Macro to test C strings (const char *) inequality
#define EXPECT_STRNE(expected, actual) gtest_lite::EXPECTSTR(expected, actual, gtest_lite::nestr, __FILE__, __LINE__, "EXPECT_STRNE(" #expected ", " #actual ")", "expected" )

/// Expect exception
#define EXPECT_THROW(statement, exception_type) try { gtest_lite::test.tmp = false; statement; } \
    catch (exception_type) { gtest_lite::test.tmp = true; } \
    catch (...) { } \
    EXPECTTHROW(statement, "expected exception.", "did not throw '"#exception_type"' exception.")

/// Expect any exception
#define EXPECT_ANY_THROW(statement) try { gtest_lite::test.tmp = false; statement; } \
    catch (...) { gtest_lite::test.tmp = true; } \
    EXPECTTHROW(statement, "expected exception.", "did not throw any exception.")

/// Expect no exception
#define EXPECT_NO_THROW(statement) try { gtest_lite::test.tmp = true; statement; } \
    catch (...) { gtest_lite::test.tmp = false; }\
    EXPECTTHROW(statement, "did not throw exception.", "threw exception.")

/// Expect exception and rethrow -- not in gtest
#define EXPECT_THROW_THROW(statement, exception_type) try { gtest_lite::test.tmp = false; statement; } \
    catch (exception_type) { gtest_lite::test.tmp = true; throw; } \
    EXPECTTHROW(statement, "expected exception.", "did not throw '"#exception_type"' exception.")

/// Helper macro to test existence of a member or method at runtime
/// Idea:
/// https://cpptalk.wordpress.com/2009/09/12/substitution-failure-is-not-an-error-2
/// Usage:
/// CREATE_Has_(size)
/// ... if (Has_size<std::string>::member)...
#define CREATE_Has_(X) \
template<typename T> struct _Has_##X {  \
    struct Fallback { int X; };         \
    struct Derived : T, Fallback {};    \
    template<typename C, C> struct ChT; \
    template<typename D> static char (&f(ChT<int Fallback::*, &D::X>*))[1]; \
    template<typename D> static char (&f(...))[2]; \
    static bool const member = sizeof(f<Derived>(0)) == 2; \
};

/// Helper template to check type conversion at runtime
template <typename F, typename T>
struct _Is_Types {
    template<typename D> static char (&f(D))[1];
    template<typename D> static char (&f(...))[2];
    static bool const convertable = sizeof(f<T>(F())) == 1;
};

/// -----------------------------------
/// Internal macros and classes
/// Not recommended to use or modify directly
/// -----------------------------------

/// EXPECTTHROW: exception handling
#define EXPECTTHROW(statement, exp, act) gtest_lite::test.expect(gtest_lite::test.tmp, __FILE__, __LINE__, #statement) \
    << "** Statement " << (act) \
    << "\n** Expected: " << (exp) << std::endl

#ifdef CPORTA
#define GTINIT(is)  \
    int magic;      \
    is >> magic;
#else
#define GTINIT(IS)
#endif // CPORTA

#ifdef CPORTA
#define GTEND(os)  \
    os << magic << (gtest_lite::test.fail() ? " NO" : " OK?") << std::endl;
#else
#define GTEND(os)
#endif // CPORTA

/// gtest_lite namespace for framework functions and objects
namespace gtest_lite {

/// Class storing test status
/// Only a single static instance is created, whose
/// destructor is called at the end of execution.
struct Test {
    int sum;            ///< total tests
    int failed;         ///< failed tests
    int ablocks;        ///< allocated blocks
    bool status;        ///< current test status
    bool tmp;           ///< temp for exception handling
    std::string name;   ///< current test name
    std::fstream null;  ///< sink if nothing needs to be printed

    Test() :sum(0), failed(0), status(false), null("/dev/null") {}
    /// Start test
    void begin(const char *n) {
        name = n; status = true;
#ifdef MEMTRACE
        ablocks = memtrace::allocated_blocks();
#endif
#ifndef CPORTA
        std::cerr << "\n---> " << name << std::endl;
#endif // CPORTA
        ++sum;
    }
    /// End test
    std::ostream& end(bool memchk = false) {
#ifdef MEMTRACE
        if (memchk && ablocks != memtrace::allocated_blocks()) {
            status = false;
            return std::cerr << "** Possible memory not freed! **" << std::endl;
        }
#endif
#ifdef CPORTA
        if (!status)
#endif // CPORTA
            std::cerr << (status ? "     SUCCESS" : "** FAILED ****") << "\t" << name << " <---" << std::endl;
        if (!status)
            return std::cerr;
        else
            return null;
    }

    bool fail() { return failed; }

    /// Register result: true is good
    std::ostream& expect(bool st, const char *file, int line, const char *expr, bool pr = false) {
        if (!st) {
            ++failed;
            status = false;
        }
        if (!st || pr) {
            std::string str(file);
            size_t i = str.rfind("\\");
            if (i == std::string::npos) i = str.rfind("/");
            if (i == std::string::npos) i = 0; else i++;
            return std::cerr << "\n**** " << &file[i] << "(" << line << "): " << expr << " ****" << std::endl;
        }
        return null;
    }

    /// Destructor
    ~Test() {
#ifdef CPORTA
        if (failed)
#endif // CPORTA
            std::cerr << "\n==== END OF TEST ==== FAILED/TOTAL: " << failed << "/" << sum << std::endl;
    }
};

/// Single static instance. (singleton would be nicer)
static Test test;

/// General template for expected values
template <typename T>
std::ostream& EXPECT_(T exp, T act, bool (*pred)(T, T), const char *file, int line,
                      const char *expr, const char *lhs = "expected", const char *rhs = "actual") {
    return test.expect(pred(exp, act), file, line, expr)
        << "** " << lhs << ": " << std::boolalpha << exp
        << "\n** " << rhs << ": " << std::boolalpha << act << std::endl;
}

/// Pointer specialization
template <typename T>
std::ostream& EXPECT_(T* exp, T* act, bool (*pred)(T*, T*), const char *file, int line,
                      const char *expr, const char *lhs = "expected", const char *rhs = "actual") {
    return test.expect(pred(exp, act), file, line, expr)
        << "** " << lhs << ": " << (void*) exp
        << "\n** " << rhs << ": " << (void*) act << std::endl;
}

/// Compare strings
inline
std::ostream& EXPECTSTR(const char *exp, const char *act, bool (*pred)(const char*, const char*), const char *file, int line,
                      const char *expr, const char *lhs = "expected", const char *rhs = "actual") {
    return test.expect(pred(exp, act), file, line, expr)
        << "** " << lhs << ": " << (exp == NULL ? "NULL pointer" : std::string("\"") + exp + std::string("\""))
        << "\n** " << rhs << ": " << (act == NULL ? "NULL pointer" : std::string("\"") + act + std::string("\"")) << std::endl;
}

/// Relation helpers
template <typename T>
bool eq(T a, T b) { return a == b; }

inline
bool eqstr(const char *a, const char *b) {
    if (a != NULL && b != NULL)
        return strcmp(a, b) == 0;
    return false;
}

template <typename T>
bool ne(T a, T b) { return a != b; }

inline
bool nestr(const char *a, const char *b) {
    if (a != NULL && b != NULL)
        return strcmp(a, b) != 0;
    return false;
}

template <typename T>
bool le(T a, T b) { return a <= b; }

template <typename T>
bool lt(T a, T b) { return a < b; }

template <typename T>
bool ge(T a, T b) { return a >= b; }

template <typename T>
bool gt(T a, T b) { return a > b; }

/// Helper for comparing floating point numbers
template <typename T>
bool almostEQ(T a, T  b) {
    // eps: relative or absolute error smaller than this is accepted
    T eps = 10 * std::numeric_limits<T>::epsilon(); // 10 times the smallest value
    if (a == b) return true;
    if (fabs(a - b) < eps)
       return true;
    double aa = fabs(a);
    double ba = fabs(b);
    if (aa < ba) {
        aa = ba;
        ba = fabs(a);
    }
    return (aa - ba) < aa * eps;
}

} // namespace gtest_lite

#endif // GTEST_LITE_H