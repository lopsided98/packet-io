#pragma once

#include <unity.h>
#include "mock_print.h"

inline void DumpStream(const uint8_t* buf, size_t n) {
    for(size_t i = 0; i < n; i++) {
        UnityPrintNumberHex(buf[i], 2);
        if(i != n-1) UnityPrint(" ");
    }
}

inline void DumpStream(const MockBuffer& s) {
    DumpStream(s.get_buffer(), s.get_len());
}

template<int N>
void DumpStream(const char (&s)[N]) {
    DumpStream((const uint8_t*)s, N-1);
}

template<int N>
void AssertEqualStream(const char (&expected)[N],
                       const MockBuffer& actual,
                       const UNITY_LINE_TYPE lineNumber)
{
    size_t         le = N - 1;
    const uint8_t* be = (const uint8_t*) expected;
    size_t         la = actual.get_len();
    const uint8_t* ba = actual.get_buffer();

    bool match = false;
    if(le == la) {
        match = true;
        for(size_t i = 0; i < le; i++)
            match &= (ba[i] == be[i]);
    }
    if(!match) {
        UNITY_OUTPUT_CHAR('\n');
        UnityPrint("  Expected: ");
        DumpStream(expected);
        UNITY_OUTPUT_CHAR('\n');
        UnityPrint("  Actual  : ");
        DumpStream(actual);
        UNITY_OUTPUT_CHAR('\n');
        UNITY_TEST_FAIL(lineNumber, "Streams do not match");
    }
}

#define TEST_ASSERT_EQUAL_STREAM(expected, actual) AssertEqualStream((expected), (actual), (__LINE__))

inline void write_retry(Print& p, const char* data, size_t n) {
    while(n) {
        size_t amt = p.write(data, n);
        n -= amt;
        data += amt;
    }
}