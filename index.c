#include "tests.h"
#include "lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

// Test to add elements at different positions
bool test_add_at() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    ASSERT_CC_OK(cc_array_add_at(a, (void*) 1, 0))
    ASSERT_CC_OK(cc_array_add_at(a, (void*) 2, 1))
    ASSERT_CC_OK(cc_array_add_at(a, (void*) 3, 1))  // Insert in the middle

    ASSERT_EQ(3, cc_array_size(a))

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 1, &get_result))
    ASSERT_EQ(3, (int) get_result)

    cc_array_destroy(a);
    return true;
}

// Test to remove elements and verify array structure
bool test_remove() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    ASSERT_CC_OK(cc_array_add(a, (void*) 1))
    ASSERT_CC_OK(cc_array_add(a, (void*) 2))
    ASSERT_CC_OK(cc_array_add(a, (void*) 3))

    ASSERT_EQ(3, cc_array_size(a))

    void* remove_result;
    ASSERT_CC_OK(cc_array_remove(a, (void*) 2, &remove_result))
    ASSERT_EQ(2, (int) remove_result)
    ASSERT_EQ(2, cc_array_size(a))

    ASSERT_CC_OK(cc_array_get_at(a, 1, &remove_result))
    ASSERT_EQ(3, (int) remove_result)

    cc_array_destroy(a);
    return true;
}

// Test to replace elements at different positions
bool test_replace_at() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    ASSERT_CC_OK(cc_array_add(a, (void*) 1))
    ASSERT_CC_OK(cc_array_add(a, (void*) 2))
    ASSERT_CC_OK(cc_array_add(a, (void*) 3))

    ASSERT_EQ(3, cc_array_size(a))

    void* replace_result;
    ASSERT_CC_OK(cc_array_replace_at(a, (void*) 4, 1, &replace_result))
    ASSERT_EQ(2, (int) replace_result)
    ASSERT_EQ(3, cc_array_size(a))

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 1, &get_result))
    ASSERT_EQ(4, (int) get_result)

    cc_array_destroy(a);
    return true;
}

// Test to trim the capacity of the array
bool test_trim_capacity() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 10; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    ASSERT_EQ(10, cc_array_size(a))
    ASSERT_CC_OK(cc_array_trim_capacity(a))
    ASSERT_EQ(10, cc_array_capacity(a))

    cc_array_destroy(a);
    return true;
}

// Test to copy the array deeply
bool test_copy_deep() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_Array* deep_copy;
    ASSERT_CC_OK(cc_array_copy_deep(a, NULL, &deep_copy))
    ASSERT_EQ(5, cc_array_size(deep_copy))

    void* get_result;
    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_get_at(deep_copy, i, &get_result))
        ASSERT_EQ(i, (int) (intptr_t) get_result)
    }

    cc_array_destroy(a);
    cc_array_destroy(deep_copy);
    return true;
}

// Test to copy the array shallowly
bool test_copy_shallow() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_Array* shallow_copy;
    ASSERT_CC_OK(cc_array_copy_shallow(a, &shallow_copy))
    ASSERT_EQ(5, cc_array_size(shallow_copy))

    void* get_result;
    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_get_at(shallow_copy, i, &get_result))
        ASSERT_EQ(i, (int) (intptr_t) get_result)
    }

    cc_array_destroy(a);
    cc_array_destroy(shallow_copy);
    return true;
}

// Test to sort the array
int compare(const void* a, const void* b) {
    return (int) (intptr_t) a - (int) (intptr_t) b;
}

bool test_sort() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    ASSERT_CC_OK(cc_array_add(a, (void*) 3))
    ASSERT_CC_OK(cc_array_add(a, (void*) 1))
    ASSERT_CC_OK(cc_array_add(a, (void*) 2))

    cc_array_sort(a, compare)

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 0, &get_result))
    ASSERT_EQ(1, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 1, &get_result))
    ASSERT_EQ(2, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 2, &get_result))
    ASSERT_EQ(3, (int) (intptr_t) get_result)

    cc_array_destroy(a);
    return true;
}

// Additional sort test with floating point numbers
int compare_float(const void* a, const void* b) {
    return (*(float*)a > *(float*)b) - (*(float*)a < *(float*)b);
}

bool test_sort_float() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    float values[] = {3.1, 1.2, 2.3};
    for (int i = 0; i < 3; i++) {
        float* num = malloc(sizeof(float));
        *num = values[i];
        ASSERT_CC_OK(cc_array_add(a, num))
    }

    cc_array_sort(a, compare_float)

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 0, &get_result))
    ASSERT_EQ(1.2, *(float*)get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 1, &get_result))
    ASSERT_EQ(2.3, *(float*)get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 2, &get_result))
    ASSERT_EQ(3.1, *(float*)get_result)

    cc_array_destroy(a);
    return true;
}

// Test to filter the array
bool is_even(const void* value) {
    return ((int) (intptr_t) value) % 2 == 0;
}

bool test_filter() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_Array* filtered;
    ASSERT_CC_OK(cc_array_filter(a, is_even, &filtered))
    ASSERT_EQ(3, cc_array_size(filtered))  // 0, 2, 4

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(filtered, 0, &get_result))
    ASSERT_EQ(0, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(filtered, 1, &get_result))
    ASSERT_EQ(2, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(filtered, 2, &get_result))
    ASSERT_EQ(4, (int) (intptr_t) get_result)

    cc_array_destroy(a);
    cc_array_destroy(filtered);
    return true;
}

// Test to reverse the array
bool test_reverse() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    cc_array_reverse(a)

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 0, &get_result))
    ASSERT_EQ(4, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 1, &get_result))
    ASSERT_EQ(3, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 2, &get_result))
    ASSERT_EQ(2, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 3, &get_result))
    ASSERT_EQ(1, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 4, &get_result))
    ASSERT_EQ(0, (int) (intptr_t) get_result)

    cc_array_destroy(a);
    return true;
}

// Test to remove and free all elements from the array
bool test_remove_all_free() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        int* num = malloc(sizeof(int));
        *num = i;
        ASSERT_CC_OK(cc_array_add(a, num))
    }

    cc_array_remove_all_free(a)

    ASSERT_EQ(0, cc_array_size(a))

    cc_array_destroy(a);
    return true;
}

// Test to remove all elements from the array
bool test_remove_all() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    cc_array_remove_all(a)
    ASSERT_EQ(0, cc_array_size(a))

    cc_array_destroy(a);
    return true;
}

// Test iterating and removing elements using iterator
bool test_iter_remove() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, a)

    void* get_result;
    while (cc_array_iter_next(&iter, &get_result) == CC_OK) {
        if ((int) (intptr_t) get_result % 2 == 0) {
            ASSERT_CC_OK(cc_array_iter_remove(&iter, NULL))
        }
    }

    ASSERT_EQ(2, cc_array_size(a))  // Only odd numbers should remain

    cc_array_destroy(a);
    return true;
}

// Test creating subarrays with different ranges
bool test_subarray() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_Array* subarray;
    ASSERT_CC_OK(cc_array_subarray(a, 1, 3, &subarray))
    ASSERT_EQ(3, cc_array_size(subarray))

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(subarray, 0, &get_result))
    ASSERT_EQ(1, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(subarray, 1, &get_result))
    ASSERT_EQ(2, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(subarray, 2, &get_result))
    ASSERT_EQ(3, (int) (intptr_t) get_result)

    cc_array_destroy(a);
    cc_array_destroy(subarray);
    return true;
}

// Test adding elements using iterator
bool test_iter_add() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, a)

    ASSERT_CC_OK(cc_array_iter_next(&iter, NULL))
    ASSERT_CC_OK(cc_array_iter_add(&iter, (void*) (intptr_t) 10))
    ASSERT_EQ(4, cc_array_size(a))

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 1, &get_result))
    ASSERT_EQ(10, (int) (intptr_t) get_result)

    cc_array_destroy(a);
    return true;
}

// Test adding elements using iterator in different scenarios
bool test_iter_add_multiple() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, a)

    ASSERT_CC_OK(cc_array_iter_next(&iter, NULL))
    ASSERT_CC_OK(cc_array_iter_add(&iter, (void*) (intptr_t) 10))
    ASSERT_CC_OK(cc_array_iter_add(&iter, (void*) (intptr_t) 20))

    ASSERT_EQ(5, cc_array_size(a))

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 1, &get_result))
    ASSERT_EQ(10, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 2, &get_result))
    ASSERT_EQ(20, (int) (intptr_t) get_result)

    cc_array_destroy(a);
    return true;
}

// Test replacing elements using iterator
bool test_iter_replace() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, a)

    ASSERT_CC_OK(cc_array_iter_next(&iter, NULL))
    void* replace_result;
    ASSERT_CC_OK(cc_array_iter_replace(&iter, (void*) (intptr_t) 10, &replace_result))
    ASSERT_EQ(0, (int) (intptr_t) replace_result)

    ASSERT_CC_OK(cc_array_get_at(a, 0, &replace_result))
    ASSERT_EQ(10, (int) (intptr_t) replace_result)

    cc_array_destroy(a);
    return true;
}

// Test iterating two arrays in parallel
bool test_zip_iter_next() {
    CC_Array* a1;
    CC_Array* a2;
    ASSERT_CC_OK(cc_array_new(&a1))
    ASSERT_CC_OK(cc_array_new(&a2))

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a1, (void*) (intptr_t) i))
        ASSERT_CC_OK(cc_array_add(a2, (void*) (intptr_t) (i + 3)))
    }

    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, a1, a2)

    void* result1;
    void* result2;
    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(0, (int) (intptr_t) result1)
    ASSERT_EQ(3, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(1, (int) (intptr_t) result1)
    ASSERT_EQ(4, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(2, (int) (intptr_t) result1)
    ASSERT_EQ(5, (int) (intptr_t) result2)

    cc_array_destroy(a1);
    cc_array_destroy(a2);
    return true;
}

// Test iterating two arrays of different sizes in parallel
bool test_zip_iter_next_different_sizes() {
    CC_Array* a1;
    CC_Array* a2;
    ASSERT_CC_OK(cc_array_new(&a1))
    ASSERT_CC_OK(cc_array_new(&a2))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a1, (void*) (intptr_t) i))
    }

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a2, (void*) (intptr_t) (i + 5)))
    }

    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, a1, a2)

    void* result1;
    void* result2;
    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(0, (int) (intptr_t) result1)
    ASSERT_EQ(5, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(1, (int) (intptr_t) result1)
    ASSERT_EQ(6, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(2, (int) (intptr_t) result1)
    ASSERT_EQ(7, (int) (intptr_t) result2)

    enum cc_stat status = cc_array_zip_iter_next(&iter, &result1, &result2)
    ASSERT_EQ(CC_ITER_END, status)

    cc_array_destroy(a1);
    cc_array_destroy(a2);
    return true;
}

// Test adding elements to two arrays in parallel using zip iterator
bool test_zip_iter_add() {
    CC_Array* a1;
    CC_Array* a2;
    ASSERT_CC_OK(cc_array_new(&a1))
    ASSERT_CC_OK(cc_array_new(&a2))

    for (int i = 0; i < 2; i++) {
        ASSERT_CC_OK(cc_array_add(a1, (void*) (intptr_t) i))
        ASSERT_CC_OK(cc_array_add(a2, (void*) (intptr_t) (i + 2)))
    }

    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, a1, a2)

    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, NULL, NULL))
    ASSERT_CC_OK(cc_array_zip_iter_add(&iter, (void*) (intptr_t) 10, (void*) (intptr_t) 20))
    ASSERT_EQ(3, cc_array_size(a1))
    ASSERT_EQ(3, cc_array_size(a2))

    void* result1;
    void* result2;
    ASSERT_CC_OK(cc_array_get_at(a1, 1, &result1))
    ASSERT_EQ(10, (int) (intptr_t) result1)

    ASSERT_CC_OK(cc_array_get_at(a2, 1, &result2))
    ASSERT_EQ(20, (int) (intptr_t) result2)

    cc_array_destroy(a1);
    cc_array_destroy(a2);
    return true;
}

// Test removing elements from two arrays in parallel using zip iterator
bool test_zip_iter_remove() {
    CC_Array* a1;
    CC_Array* a2;
    ASSERT_CC_OK(cc_array_new(&a1))
    ASSERT_CC_OK(cc_array_new(&a2))

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a1, (void*) (intptr_t) i))
        ASSERT_CC_OK(cc_array_add(a2, (void*) (intptr_t) (i + 3)))
    }

    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, a1, a2)

    void* result1;
    void* result2;
    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(0, (int) (intptr_t) result1)
    ASSERT_EQ(3, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_zip_iter_remove(&iter, NULL, NULL))
    ASSERT_EQ(2, cc_array_size(a1))
    ASSERT_EQ(2, cc_array_size(a2))

    ASSERT_CC_OK(cc_array_get_at(a1, 0, &result1))
    ASSERT_EQ(1, (int) (intptr_t) result1)

    ASSERT_CC_OK(cc_array_get_at(a2, 0, &result2))
    ASSERT_EQ(4, (int) (intptr_t) result2)

    cc_array_destroy(a1);
    cc_array_destroy(a2);
    return true;
}

// Test removing elements in parallel using zip iterator with additional checks
bool test_zip_iter_remove_extended() {
    CC_Array* a1;
    CC_Array* a2;
    ASSERT_CC_OK(cc_array_new(&a1))
    ASSERT_CC_OK(cc_array_new(&a2))

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a1, (void*) (intptr_t) i))
        ASSERT_CC_OK(cc_array_add(a2, (void*) (intptr_t) (i + 3)))
    }

    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, a1, a2)

    void* result1;
    void* result2;
    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(0, (int) (intptr_t) result1)
    ASSERT_EQ(3, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_zip_iter_remove(&iter, NULL, NULL))
    ASSERT_EQ(2, cc_array_size(a1))
    ASSERT_EQ(2, cc_array_size(a2))

    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(1, (int) (intptr_t) result1)
    ASSERT_EQ(4, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_zip_iter_remove(&iter, NULL, NULL))
    ASSERT_EQ(1, cc_array_size(a1))
    ASSERT_EQ(1, cc_array_size(a2))

    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(2, (int) (intptr_t) result1)
    ASSERT_EQ(5, (int) (intptr_t) result2)

    cc_array_destroy(a1);
    cc_array_destroy(a2);
    return true;
}

// Test to reduce the array to a single value
void sum(void* a, void* b, void* result) {
    *(int*)result = *(int*)a + *(int*)b;
}

bool test_reduce() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 1; i <= 5; i++) {
        int* num = malloc(sizeof(int));
        *num = i;
        ASSERT_CC_OK(cc_array_add(a, num))
    }

    int result = 0;
    cc_array_reduce(a, sum, &result)
    ASSERT_EQ(15, result)  // 1+2+3+4+5 = 15

    cc_array_destroy(a);
    return true;
}

// Additional reduce test with multiplication
void multiply(void* a, void* b, void* result) {
    *(int*)result = *(int*)a * (*(int*)b ? *(int*)b : 1);
}

bool test_reduce_multiplication() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 1; i <= 5; i++) {
        int* num = malloc(sizeof(int));
        *num = i;
        ASSERT_CC_OK(cc_array_add(a, num))
    }

    int result = 1;
    cc_array_reduce(a, multiply, &result)
    ASSERT_EQ(120, result)  // 1*2*3*4*5 = 120

    cc_array_destroy(a);
    return true;
}

// Additional reduce test with subtraction
void subtract(void* a, void* b, void* result) {
    *(int*)result = *(int*)a - *(int*)b;
}

bool test_reduce_subtraction() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 1; i <= 5; i++) {
        int* num = malloc(sizeof(int));
        *num = i;
        ASSERT_CC_OK(cc_array_add(a, num))
    }

    int result = 0;
    cc_array_reduce(a, subtract, &result)
    ASSERT_EQ(-13, result)  // (((1-2)-3)-4)-5 = -13

    cc_array_destroy(a);
    return true;
}

// Test to map a function over the array
void increment(void* e) {
    int* num = (int*)e;
    (*num)++;
}

bool test_map() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        int* num = malloc(sizeof(int));
        *num = i;
        ASSERT_CC_OK(cc_array_add(a, num))
    }

    cc_array_map(a, increment)

    void* get_result;
    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_get_at(a, i, &get_result))
        ASSERT_EQ(i + 1, *(int*)get_result)
    }

    cc_array_destroy(a);
    return true;
}

// Additional map test with decrement function
void decrement(void* e) {
    int* num = (int*)e;
    (*num)--;
}

bool test_map_decrement() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        int* num = malloc(sizeof(int));
        *num = i;
        ASSERT_CC_OK(cc_array_add(a, num))
    }

    cc_array_map(a, decrement)

    void* get_result;
    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_get_at(a, i, &get_result))
        ASSERT_EQ(i - 1, *(int*)get_result)
    }

    cc_array_destroy(a);
    return true;
}

// Test to check if the array contains a specific element
bool test_contains() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    ASSERT_EQ(3, cc_array_contains(a, (void*) 3))
    ASSERT_EQ(0, cc_array_contains(a, (void*) 5))

    cc_array_destroy(a);
    return true;
}

// Test to check if the array contains duplicated elements
bool test_contains_duplicates() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    ASSERT_CC_OK(cc_array_add(a, (void*) 1))
    ASSERT_CC_OK(cc_array_add(a, (void*) 2))
    ASSERT_CC_OK(cc_array_add(a, (void*) 1))  // Duplicate

    ASSERT_EQ(2, cc_array_contains(a, (void*) 1))
    ASSERT_EQ(1, cc_array_contains(a, (void*) 2))

    cc_array_destroy(a);
    return true;
}

// Comparator function for test_contains_value
int compare_int(const void* a, const void* b) {
    return (int) (intptr_t) a - (int) (intptr_t) b;
}

// Test to check if the array contains a specific value using comparator
bool test_contains_value() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    ASSERT_EQ(1, cc_array_contains_value(a, (void*) 3, compare_int))
    ASSERT_EQ(0, cc_array_contains_value(a, (void*) 5, compare_int))

    cc_array_destroy(a);
    return true;
}

// Test to find the index of a specific element
bool test_index_of() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    size_t index;
    ASSERT_CC_OK(cc_array_index_of(a, (void*) 3, &index))
    ASSERT_EQ(3, index)

    enum cc_stat status = cc_array_index_of(a, (void*) 5, &index)
    ASSERT_EQ(CC_ERR_OUT_OF_RANGE, status)

    cc_array_destroy(a);
    return true;
}

// Test to check the size and capacity of the array after various operations
bool test_size_capacity() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    ASSERT_EQ(0, cc_array_size(a))
    ASSERT_EQ(DEFAULT_CAPACITY, cc_array_capacity(a))

    for (int i = 0; i < 10; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    ASSERT_EQ(10, cc_array_size(a))
    ASSERT_TRUE(cc_array_capacity(a) > 10)

    cc_array_trim_capacity(a)
    ASSERT_EQ(10, cc_array_capacity(a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_remove_at(a, 0, NULL))
    }

    ASSERT_EQ(5, cc_array_size(a))

    cc_array_destroy(a);
    return true;
}

// Test to get elements at different positions, including edge cases
bool test_get_at() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 0, &get_result))
    ASSERT_EQ(0, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 4, &get_result))
    ASSERT_EQ(4, (int) (intptr_t) get_result)

    enum cc_stat status = cc_array_get_at(a, 5, &get_result)
    ASSERT_EQ(CC_ERR_OUT_OF_RANGE, status)

    cc_array_destroy(a);
    return true;
}

// Test to get the last element of the array
bool test_get_last() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    void* last_element;
    ASSERT_CC_OK(cc_array_get_last(a, &last_element))
    ASSERT_EQ(4, (int) (intptr_t) last_element)

    cc_array_destroy(a);
    return true;
}

// Test getting the last element of an empty array
bool test_get_last_empty() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    void* last_element;
    enum cc_stat status = cc_array_get_last(a, &last_element)
    ASSERT_EQ(CC_ERR_VALUE_NOT_FOUND, status)

    cc_array_destroy(a);
    return true;
}

// Test to iterate over all elements of the array
bool test_iterate() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, a)

    void* element;
    int count = 0;
    while (cc_array_iter_next(&iter, &element) == CC_OK) {
        ASSERT_EQ(count, (int) (intptr_t) element)
        count++;
    }

    ASSERT_EQ(5, count)

    cc_array_destroy(a);
    return true;
}

// Test to replace elements in different positions using iterator
bool test_iter_replace_positions() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, a)

    void* replaced_element;
    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_iter_next(&iter, NULL))
        ASSERT_CC_OK(cc_array_iter_replace(&iter, (void*) (intptr_t) (i + 10), &replaced_element))
        ASSERT_EQ(i, (int) (intptr_t) replaced_element)
    }

    for (int i = 0; i < 5; i++) {
        void* element;
        ASSERT_CC_OK(cc_array_get_at(a, i, &element))
        ASSERT_EQ(i + 10, (int) (intptr_t) element)
    }

    cc_array_destroy(a);
    return true;
}

// Test to create subarrays with different ranges
bool test_subarray_ranges() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 10; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_Array* subarray;
    ASSERT_CC_OK(cc_array_subarray(a, 2, 5, &subarray))
    ASSERT_EQ(4, cc_array_size(subarray))  // 2, 3, 4, 5

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(subarray, 0, &get_result))
    ASSERT_EQ(2, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(subarray, 1, &get_result))
    ASSERT_EQ(3, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(subarray, 2, &get_result))
    ASSERT_EQ(4, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(subarray, 3, &get_result))
    ASSERT_EQ(5, (int) (intptr_t) get_result)

    cc_array_destroy(a);
    cc_array_destroy(subarray);
    return true;
}

// Test to remove the last element of the array
bool test_remove_last() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    void* last_element;
    ASSERT_CC_OK(cc_array_remove_last(a, &last_element))
    ASSERT_EQ(4, (int) (intptr_t) last_element)
    ASSERT_EQ(4, cc_array_size(a))

    cc_array_destroy(a);
    return true;
}

// Test to replace elements in parallel using zip iterator
bool test_zip_iter_replace() {
    CC_Array* a1;
    CC_Array* a2;
    ASSERT_CC_OK(cc_array_new(&a1))
    ASSERT_CC_OK(cc_array_new(&a2))

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a1, (void*) (intptr_t) i))
        ASSERT_CC_OK(cc_array_add(a2, (void*) (intptr_t) (i + 3)))
    }

    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, a1, a2)

    void* result1;
    void* result2;
    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, &result1, &result2))
    ASSERT_EQ(0, (int) (intptr_t) result1)
    ASSERT_EQ(3, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_zip_iter_replace(&iter, (void*) 10, (void*) 20, &result1, &result2))
    ASSERT_EQ(0, (int) (intptr_t) result1)
    ASSERT_EQ(3, (int) (intptr_t) result2)

    ASSERT_CC_OK(cc_array_get_at(a1, 0, &result1))
    ASSERT_EQ(10, (int) (intptr_t) result1)

    ASSERT_CC_OK(cc_array_get_at(a2, 0, &result2))
    ASSERT_EQ(20, (int) (intptr_t) result2)

    cc_array_destroy(a1);
    cc_array_destroy(a2);
    return true;
}

// Test to get the index of the last iterated element in parallel using zip iterator
bool test_zip_iter_index() {
    CC_Array* a1;
    CC_Array* a2;
    ASSERT_CC_OK(cc_array_new(&a1))
    ASSERT_CC_OK(cc_array_new(&a2))

    for (int i = 0; i < 3; i++) {
        ASSERT_CC_OK(cc_array_add(a1, (void*) (intptr_t) i))
        ASSERT_CC_OK(cc_array_add(a2, (void*) (intptr_t) (i + 3)))
    }

    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, a1, a2)

    void* result1;
    void* result2;
    size_t index = 0;
    while (cc_array_zip_iter_next(&iter, &result1, &result2) == CC_OK) {
        index = cc_array_zip_iter_index(&iter);
    }

    ASSERT_EQ(2, index)

    cc_array_destroy(a1);
    cc_array_destroy(a2);
    return true;
}

// Test to filter the array mutably
bool test_filter_mut() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 6; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    ASSERT_CC_OK(cc_array_filter_mut(a, is_even))
    ASSERT_EQ(3, cc_array_size(a))  // 0, 2, 4

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 0, &get_result))
    ASSERT_EQ(0, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 1, &get_result))
    ASSERT_EQ(2, (int) (intptr_t) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 2, &get_result))
    ASSERT_EQ(4, (int) (intptr_t) get_result)

    cc_array_destroy(a);
    return true;
}

// Comparator function for test_contains_value
int compare_int(const void* a, const void* b) {
    return (int) (intptr_t) a - (int) (intptr_t) b;
}

// Test to check if the array contains a specific value using comparator
bool test_contains_value() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    ASSERT_EQ(1, cc_array_contains_value(a, (void*) 3, compare_int))
    ASSERT_EQ(0, cc_array_contains_value(a, (void*) 5, compare_int))

    cc_array_destroy(a);
    return true;
}

// Test to swap elements at specific positions
bool test_swap_at() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    ASSERT_CC_OK(cc_array_add(a, (void*) 1))
    ASSERT_CC_OK(cc_array_add(a, (void*) 2))
    ASSERT_CC_OK(cc_array_add(a, (void*) 3))

    ASSERT_EQ(3, cc_array_size(a))

    ASSERT_CC_OK(cc_array_swap_at(a, 0, 2))

    void* get_result;
    ASSERT_CC_OK(cc_array_get_at(a, 0, &get_result))
    ASSERT_EQ(3, (int) get_result)

    ASSERT_CC_OK(cc_array_get_at(a, 2, &get_result))
    ASSERT_EQ(1, (int) get_result)

    cc_array_destroy(a);
    return true;
}

// Test to get the index of the last iterated element using iterator
bool test_iter_index() {
    CC_Array* a;
    ASSERT_CC_OK(cc_array_new(&a))

    for (int i = 0; i < 5; i++) {
        ASSERT_CC_OK(cc_array_add(a, (void*) (intptr_t) i))
    }

    CC_ArrayIter iter;
    cc_array_iter_init(&iter, a)

    void* element;
    size_t index = 0;
    while (cc_array_iter_next(&iter, &element) == CC_OK) {
        index = cc_array_iter_index(&iter);
    }

    ASSERT_EQ(4, index)

    cc_array_destroy(a);
    return true;
}

// Test to add elements to two arrays in parallel using zip iterator
bool test_zip_iter_add_parallel() {
    CC_Array* a1;
    CC_Array* a2;
    ASSERT_CC_OK(cc_array_new(&a1))
    ASSERT_CC_OK(cc_array_new(&a2))

    for (int i = 0; i < 2; i++) {
        ASSERT_CC_OK(cc_array_add(a1, (void*) (intptr_t) i))
        ASSERT_CC_OK(cc_array_add(a2, (void*) (intptr_t) (i + 2)))
    }

    CC_ArrayZipIter iter;
    cc_array_zip_iter_init(&iter, a1, a2)

    ASSERT_CC_OK(cc_array_zip_iter_next(&iter, NULL, NULL))
    ASSERT_CC_OK(cc_array_zip_iter_add(&iter, (void*) (intptr_t) 10, (void*) (intptr_t) 20))
    ASSERT_EQ(3, cc_array_size(a1))
    ASSERT_EQ(3, cc_array_size(a2))

    void* result1;
    void* result2;
    ASSERT_CC_OK(cc_array_get_at(a1, 1, &result1))
    ASSERT_EQ(10, (int) (intptr_t) result1)

    ASSERT_CC_OK(cc_array_get_at(a2, 1, &result2))
    ASSERT_EQ(20, (int) (intptr_t) result2)

    cc_array_destroy(a1);
    cc_array_destroy(a2);
    return true;
}

test_t TESTS[] = {
    &test_add_at,
    &test_remove,
    &test_replace_at,
    &test_trim_capacity,
    &test_copy_deep,
    &test_sort,
    &test_filter,
    &test_reverse,
    &test_remove_all_free,
    &test_iter_remove,
    &test_subarray,
    &test_iter_add,
    &test_iter_replace,
    &test_zip_iter_next,
    &test_zip_iter_add,
    &test_zip_iter_remove,
    &test_reduce,
    &test_map,
    &test_contains,
    &test_index_of,
    &test_size_capacity,
    &test_get_last,
    &test_iterate,
    &test_iter_replace_positions,
    &test_subarray_ranges,
    &test_remove_last,
    &test_zip_iter_replace,
    &test_zip_iter_index,
    &test_filter_mut,
    &test_contains_value,
    &test_copy_shallow,
    &test_swap_at,
    &test_remove_all,
    &test_iter_index,
    &test_zip_iter_add_parallel,
    &test_reduce_multiplication,
    &test_get_at,
    &test_zip_iter_remove_extended,
    &test_contains_duplicates,
    &test_sort_float,
    &test_map_decrement,
    &test_reduce_subtraction,
    &test_iter_add_multiple,
    &test_zip_iter_next_different_sizes,
    &test_get_last_empty,
    NULL
};
