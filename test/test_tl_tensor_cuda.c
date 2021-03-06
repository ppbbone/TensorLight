/*
 * Copyright (c) 2018 Zhao Zhixu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
c */

#ifdef TL_CUDA

#include "test_tensorlight.h"
#include "../src/tl_tensor.h"
#include "../src/tl_check.h"

#define ARR(type, varg...) (type[]){varg}

static void setup(void)
{
}

static void teardown(void)
{
}

START_TEST(test_tl_tensor_zeros_cuda)
{
     tl_tensor *t;
     int dims[3] = {1, 2, 3};
     int32_t data[6] = {1, 2, 3, 4, 5, 6};
     void *data_h, *data_d;
     int i;

     t = tl_tensor_zeros_cuda(3, (int[]){1, 2, 3}, TL_DOUBLE);
     data_h = tl_clone_d2h(t->data, sizeof(double)*t->len);
     ck_assert_int_eq(t->ndim, 3);
     ck_assert_int_eq(t->dtype, TL_DOUBLE);
     ck_assert_int_eq(t->len, 6);
     for (i = 0; i < t->ndim; i++)
          ck_assert(t->dims[i] == dims[i]);
     for (i = 0; i < t->len; i++)
          ck_assert(((double *)data_h)[i] == 0);
     tl_tensor_free_data_too_cuda(t);
     tl_free(data_h);

     t = tl_tensor_zeros_cuda(3, dims, TL_FLOAT);
     data_h = tl_clone_d2h(t->data, sizeof(float)*t->len);
     ck_assert_int_eq(t->ndim, 3);
     ck_assert_int_eq(t->dtype, TL_FLOAT);
     ck_assert_int_eq(t->len, 6);
     for (i = 0; i < t->ndim; i++)
          ck_assert(t->dims[i] == dims[i]);
     for (i = 0; i < t->len; i++)
          ck_assert(((float *)data_h)[i] == 0);
     tl_tensor_free_data_too_cuda(t);
     tl_free(data_h);

     data_d = tl_clone_h2d(data, sizeof(int32_t)*6);
     t = tl_tensor_create(data_d, 3, dims, TL_INT32);
     data_h = tl_clone_d2h(t->data, sizeof(int32_t)*t->len);
     ck_assert_int_eq(t->ndim, 3);
     ck_assert_int_eq(t->dtype, TL_INT32);
     ck_assert_int_eq(t->len, 6);
     for (i = 0; i < t->ndim; i++)
          ck_assert(t->dims[i] == dims[i]);
     for (i = 0; i < t->len; i++)
          ck_assert(((int32_t *)data_h)[i] == data[i]);
     tl_tensor_free(t);
     tl_free(data_h);

     t = tl_tensor_create(data_d, 3, dims, TL_INT32);
     data_h = tl_clone_d2h(t->data, sizeof(int32_t)*t->len);
     ck_assert_int_eq(t->ndim, 3);
     ck_assert_int_eq(t->dtype, TL_INT32);
     ck_assert_int_eq(t->len, 6);
     for (i = 0; i < t->ndim; i++)
          ck_assert(t->dims[i] == dims[i]);
     for (i = 0; i < t->len; i++)
          ck_assert(((int32_t *)data_h)[i] == data[i]);
     tl_tensor_free(t);
     tl_free(data_h);
}
END_TEST

START_TEST(test_tl_tensor_free_data_too_cuda)
{
}
END_TEST

START_TEST(test_tl_tensor_clone_h2d)
{
     tl_tensor *t1, *t2;
     int dims[3] = {1, 2, 3};
     int32_t data[6] = {1, 2, 3, 4, 5, 6};
     int32_t *data_h;
     int i;

     t1 = tl_tensor_create(data, 3, dims, TL_INT32);
     t2 = tl_tensor_clone_h2d(t1);
     data_h = tl_clone_d2h(t2->data, sizeof(int32_t)*t2->len);
     ck_assert_int_eq(t2->ndim, 3);
     ck_assert_int_eq(t2->dtype, TL_INT32);
     ck_assert_int_eq(t2->len, 6);
     for (i = 0; i < t2->ndim; i++)
          ck_assert(t2->dims[i] == dims[i]);
     for (i = 0; i < t2->len; i++)
          ck_assert(data_h[i] == data[i]);
     tl_tensor_free(t1);
     tl_tensor_free_data_too_cuda(t2);
     tl_free(data_h);
}
END_TEST

START_TEST(test_tl_tensor_clone_d2h)
{
     tl_tensor *t1, *t2;
     int dims[3] = {1, 2, 3};
     int32_t data[6] = {1, 2, 3, 4, 5, 6};
     int32_t *data_d;
     int i;

     data_d = tl_clone_h2d(data, sizeof(int32_t)*6);
     t1 = tl_tensor_create(data_d, 3, dims, TL_INT32);
     t2 = tl_tensor_clone_d2h(t1);
     ck_assert_int_eq(t2->ndim, 3);
     ck_assert_int_eq(t2->dtype, TL_INT32);
     ck_assert_int_eq(t2->len, 6);
     for (i = 0; i < t2->ndim; i++)
          ck_assert(t2->dims[i] == dims[i]);
     for (i = 0; i < t2->len; i++)
          ck_assert(((int32_t *)t2->data)[i] == data[i]);
     tl_tensor_free(t1);
     tl_tensor_free_data_too(t2);
     tl_free_cuda(data_d);
}
END_TEST

START_TEST(test_tl_tensor_clone_d2d)
{
     tl_tensor *t1, *t2, *t3;
     int dims[3] = {1, 2, 3};
     int32_t data[6] = {1, 2, 3, 4, 5, 6};
     int32_t *data_h;
     int i;

     t1 = tl_tensor_create(data, 3, dims, TL_INT32);
     t2 = tl_tensor_clone_h2d(t1);
     t3 = tl_tensor_clone_d2d(t2);
     data_h = tl_clone_d2h(t3->data, sizeof(int32_t)*t3->len);
     ck_assert_int_eq(t3->ndim, 3);
     ck_assert_int_eq(t3->dtype, TL_INT32);
     ck_assert_int_eq(t3->len, 6);
     for (i = 0; i < t3->ndim; i++)
          ck_assert(t3->dims[i] == dims[i]);
     for (i = 0; i < t3->len; i++)
          ck_assert(data_h[i] == data[i]);
     tl_tensor_free(t1);
     tl_tensor_free_data_too_cuda(t2);
     tl_tensor_free_data_too_cuda(t3);
     tl_free(data_h);
}
END_TEST

START_TEST(test_tl_tensor_fprint_cuda)
{
     tl_tensor *t;
     FILE *fp;
     char s[BUFSIZ];

     t = tl_tensor_zeros_cuda(3, (int[]){1, 2, 3}, TL_FLOAT);

     fp = tmpfile();
     ck_assert_ptr_ne(fp, NULL);
     tl_tensor_fprint_cuda(fp, t, NULL);
     rewind(fp);
     ck_assert_ptr_ne(fgets(s, 100, fp), NULL);
     ck_assert_ptr_ne(fgets(s+strlen(s), 100, fp), NULL);
     ck_assert_str_eq(s, "[[[0.000 0.000 0.000]\n"
                      "  [0.000 0.000 0.000]]]\n");
     fclose(fp);

     fp = tmpfile();
     ck_assert_ptr_ne(fp, NULL);
     tl_tensor_fprint_cuda(fp, t, "%.4f");
     rewind(fp);
     ck_assert_ptr_ne(fgets(s, 100, fp), NULL);
     ck_assert_ptr_ne(fgets(s+strlen(s), 100, fp), NULL);
     ck_assert_str_eq(s, "[[[0.0000 0.0000 0.0000]\n"
                      "  [0.0000 0.0000 0.0000]]]\n");
     fclose(fp);

     tl_tensor_free_data_too_cuda(t);
}
END_TEST

START_TEST(test_tl_tensor_print_cuda)
{
}
END_TEST

START_TEST(test_tl_tensor_save_cuda)
{
     tl_tensor *t;
     FILE *fp;
     char s[BUFSIZ];

     t = tl_tensor_zeros_cuda(3, (int[]){1, 2, 3}, TL_FLOAT);

     tl_tensor_save_cuda("__test_tensor_save_tmp", t, NULL);
     fp = fopen("__test_tensor_save_tmp", "r");
     ck_assert_ptr_ne(fp, NULL);
     ck_assert_ptr_ne(fgets(s, 100, fp), NULL);
     ck_assert_ptr_ne(fgets(s+strlen(s), 100, fp), NULL);
     ck_assert_str_eq(s, "[[[0.000 0.000 0.000]\n"
                      "  [0.000 0.000 0.000]]]\n");
     fclose(fp);
     ck_assert_int_eq(remove("__test_tensor_save_tmp"), 0);

     /* ck_assert_int_lt(tl_tensor_save("__non_exist_dir/tmp", t, NULL), 0); */
     tl_tensor_free_data_too_cuda(t);
}
END_TEST

START_TEST(test_tl_tensor_zeros_slice_cuda)
{
     tl_tensor *t1, *t2;
     void *data_h;
     int i;

     t1 = tl_tensor_zeros_cuda(1, (int[]){1}, TL_INT8);
     t2 = tl_tensor_zeros_slice_cuda(t1, 0, 1, TL_INT8);
     data_h = tl_clone_d2h(t2->data, tl_size_of(t2->dtype)*t2->len);
     ck_assert_int_eq(t2->ndim, 1);
     ck_assert_int_eq(t2->dtype, TL_INT8);
     ck_assert_int_eq(t2->len, 1);
     ck_assert(t2->dims[0] == 1);
     for (i = 0; i < t2->len; i++)
          ck_assert(((int8_t *)data_h)[i] == 0);
     tl_tensor_free_data_too_cuda(t1);
     tl_tensor_free_data_too_cuda(t2);
     tl_free(data_h);

     t1 = tl_tensor_zeros_cuda(3, (int[]){1, 2, 3}, TL_INT16);
     t2 = tl_tensor_zeros_slice_cuda(t1, 2, 2, TL_UINT8);
     data_h = tl_clone_d2h(t2->data, tl_size_of(t2->dtype)*t2->len);
     ck_assert_int_eq(t2->ndim, 3);
     ck_assert_int_eq(t2->dtype, TL_UINT8);
     ck_assert_int_eq(t2->len, 4);
     ck_assert(t2->dims[0] == 1);
     ck_assert(t2->dims[1] == 2);
     ck_assert(t2->dims[2] == 2);
     for (i = 0; i < t2->len; i++)
          ck_assert(((uint8_t *)data_h)[i] == 0);
     tl_tensor_free_data_too_cuda(t1);
     tl_tensor_free_data_too_cuda(t2);
     tl_free(data_h);
}
END_TEST

START_TEST(test_tl_tensor_slice_cuda)
{
     tl_tensor *t1, *t2;
     int ndim = 3;
     int dims[3] = {1, 2, 3};
     uint16_t data[6] = {1, 2, 3, 4, 5, 6};
     uint16_t data_slice1[4] = {2, 3, 5, 6};
     uint16_t data_slice2[3] = {1, 2, 3};
     void *data_h, *data_d;
     int i;

     data_d = tl_clone_h2d(data, sizeof(uint16_t)*6);
     t1 = tl_tensor_create(data_d, ndim, dims, TL_UINT16);
     t2 = tl_tensor_slice_cuda(t1, NULL, 2, 1, 2);
     data_h = tl_clone_d2h(t2->data, tl_size_of(t2->dtype)*t2->len);
     ck_assert_int_eq(t2->ndim, 3);
     ck_assert_int_eq(t2->dtype, TL_UINT16);
     ck_assert_int_eq(t2->len, 4);
     ck_assert(t2->dims[0] == 1);
     ck_assert(t2->dims[1] == 2);
     ck_assert(t2->dims[2] == 2);
     for (i = 0; i < t2->len; i++)
          ck_assert(((uint16_t *)data_h)[i] == data_slice1[i]);
     tl_tensor_free(t1);
     tl_tensor_free_data_too_cuda(t2);
     tl_free_cuda(data_d);
     tl_free(data_h);

     data_d = tl_clone_h2d(data, sizeof(uint16_t)*6);
     t1 = tl_tensor_create(data_d, ndim, dims, TL_UINT16);
     t2 = tl_tensor_zeros_slice_cuda(t1, 1, 1, TL_UINT16);
     t2 = tl_tensor_slice_cuda(t1, t2, 1, 0, 1);
     data_h = tl_clone_d2h(t2->data, tl_size_of(t2->dtype)*t2->len);
     ck_assert_int_eq(t2->ndim, 3);
     ck_assert_int_eq(t2->dtype, TL_UINT16);
     ck_assert_int_eq(t2->len, 3);
     ck_assert(t2->dims[0] == 1);
     ck_assert(t2->dims[1] == 1);
     ck_assert(t2->dims[2] == 3);
     for (i = 0; i < t2->len; i++)
          ck_assert(((uint16_t *)data_h)[i] == data_slice2[i]);
     tl_tensor_free(t1);
     tl_tensor_free_data_too_cuda(t2);
     tl_free_cuda(data_d);
     tl_free(data_h);
}
END_TEST

START_TEST(test_tl_tensor_maxreduce_cuda)
{
     tl_tensor *src, *dst, *arg;
     int dims[3] = {2, 3, 2};
     int32_t data[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
     int32_t dst_data1[4] = {5, 6, 11, 12};
     int32_t dst_data2[6] = {7, 8, 9, 10, 11, 12};
     int32_t dst_data3[6] = {2, 4, 6, 8, 10, 12};
     int32_t arg_data[6] = {1, 1, 1, 1, 1, 1};
     int32_t *data_d, *data_h;
     int i;

     data_d = tl_clone_h2d(data, sizeof(int32_t)*12);
     src = tl_tensor_create(data_d, 3, dims, TL_INT32);
     dst = tl_tensor_maxreduce_cuda(src, NULL, NULL, 1);
     data_h = tl_clone_d2h(dst->data, tl_size_of(dst->dtype)*dst->len);
     ck_assert_int_eq(dst->ndim, 3);
     ck_assert_int_eq(dst->dtype, TL_INT32);
     ck_assert_int_eq(dst->len, 4);
     ck_assert(dst->dims[0] == 2);
     ck_assert(dst->dims[1] == 1);
     ck_assert(dst->dims[2] == 2);
     for (i = 0; i < dst->len; i++)
          ck_assert(((int32_t *)data_h)[i] == dst_data1[i]);
     tl_tensor_free_data_too_cuda(dst);
     tl_free(data_h);

     dst = tl_tensor_zeros_slice_cuda(src, 0, 1, TL_INT32);
     dst = tl_tensor_maxreduce_cuda(src, dst, NULL, 0);
     data_h = tl_clone_d2h(dst->data, tl_size_of(dst->dtype)*dst->len);
     ck_assert_int_eq(dst->ndim, 3);
     ck_assert_int_eq(dst->dtype, TL_INT32);
     ck_assert_int_eq(dst->len, 6);
     ck_assert(dst->dims[0] == 1);
     ck_assert(dst->dims[1] == 3);
     ck_assert(dst->dims[2] == 2);
     for (i = 0; i < dst->len; i++)
          ck_assert(((int32_t *)data_h)[i] == dst_data2[i]);
     tl_tensor_free_data_too_cuda(dst);
     tl_free(data_h);

     dst = tl_tensor_zeros_slice_cuda(src, 2, 1, TL_INT32);
     arg = tl_tensor_zeros_slice_cuda(src, 2, 1, TL_INT32);
     dst = tl_tensor_maxreduce_cuda(src, dst, arg, 2);
     ck_assert_int_eq(dst->ndim, 3);
     ck_assert_int_eq(dst->dtype, TL_INT32);
     ck_assert_int_eq(dst->len, 6);
     ck_assert(dst->dims[0] == 2);
     ck_assert(dst->dims[1] == 3);
     ck_assert(dst->dims[2] == 1);
     data_h = tl_clone_d2h(dst->data, tl_size_of(dst->dtype)*dst->len);
     for (i = 0; i < dst->len; i++)
          ck_assert(((int32_t *)data_h)[i] == dst_data3[i]);
     ck_assert_int_eq(arg->ndim, 3);
     ck_assert_int_eq(arg->dtype, TL_INT32);
     ck_assert_int_eq(arg->len, 6);
     ck_assert(arg->dims[0] == 2);
     ck_assert(arg->dims[1] == 3);
     ck_assert(arg->dims[2] == 1);
     data_h = tl_clone_d2h(arg->data, tl_size_of(arg->dtype)*arg->len);
     for (i = 0; i < arg->len; i++)
          ck_assert(((int32_t *)data_h)[i] == arg_data[i]);
     tl_free(data_h);
     tl_tensor_free_data_too_cuda(dst);
     tl_tensor_free_data_too_cuda(arg);

     tl_free_cuda(data_d);
     tl_tensor_free(src);
}
END_TEST

START_TEST(test_tl_tensor_elew_cuda)
{
     tl_tensor *src1, *src2, *dst;
     int8_t src1_data[6] = {1, 1, 2, 2, 3, 3};
     int8_t src2_data[6] = {1, 2, 3, 4, 5, 6};
     int8_t dst_data[6] = {1, 2, 6, 8, 15, 18};
     void *data_d1, *data_d2, *data_h;
     int dims[2] = {2, 3};
     int i;

     data_d1 = tl_clone_h2d(src1_data, sizeof(int8_t)*6);
     data_d2 = tl_clone_h2d(src2_data, sizeof(int8_t)*6);

     src1 = tl_tensor_create(data_d1, 2, dims, TL_INT8);
     src2 = tl_tensor_create(data_d2, 2, dims, TL_INT8);
     dst = tl_tensor_elew_cuda(src1, src2, NULL, TL_MUL);
     data_h = tl_clone_d2h(dst->data, tl_size_of(dst->dtype)*dst->len);
     ck_assert_int_eq(dst->ndim, 2);
     ck_assert_int_eq(dst->dtype, TL_INT8);
     ck_assert_int_eq(dst->len, 6);
     ck_assert(dst->dims[0] == 2);
     ck_assert(dst->dims[1] == 3);
     for (i = 0; i < dst->len; i++)
          ck_assert(((int8_t *)data_h)[i] == dst_data[i]);
     tl_tensor_free_data_too_cuda(dst);
     tl_free(data_h);

     src1 = tl_tensor_create(data_d1, 2, dims, TL_INT8);
     src2 = tl_tensor_create(data_d2, 2, dims, TL_INT8);
     dst = tl_tensor_zeros_cuda(2, dims, TL_INT8);
     dst = tl_tensor_elew_cuda(src1, src2, dst, TL_MUL);
     data_h = tl_clone_d2h(dst->data, tl_size_of(dst->dtype)*dst->len);
     ck_assert_int_eq(dst->ndim, 2);
     ck_assert_int_eq(dst->dtype, TL_INT8);
     ck_assert_int_eq(dst->len, 6);
     ck_assert(dst->dims[0] == 2);
     ck_assert(dst->dims[1] == 3);
     for (i = 0; i < dst->len; i++)
          ck_assert(((int8_t *)data_h)[i] == dst_data[i]);
     tl_tensor_free_data_too_cuda(dst);
     tl_free(data_h);

     tl_tensor_free(src1);
     tl_tensor_free(src2);
     tl_free_cuda(data_d1);
     tl_free_cuda(data_d2);
}
END_TEST

START_TEST(test_tl_tensor_convert_cuda)
{
     float data_f[5] = {-1, 0, 1, 255, 256};
     uint8_t data_ui8[5] = {0, 0, 1, 255, 255};
     void *data_d, *data_h;
     tl_tensor *t1, *t2;

     data_d = tl_clone_h2d(data_f, sizeof(float)*5);
     t1 = tl_tensor_create(data_d, 1, (int[]){5}, TL_FLOAT);

     t2 = tl_tensor_convert_cuda(t1, NULL, TL_UINT8);
     ck_assert_int_eq(t2->ndim, 1);
     ck_assert_int_eq(t2->dtype, TL_UINT8);
     ck_assert_int_eq(t2->len, t1->len);
     ck_assert(t2->dims[0] == t1->dims[0]);
     data_h = tl_clone_d2h(t2->data, tl_size_of(t2->dtype)*t2->len);
     for (int i = 0; i < 5; i++)
          ck_assert_uint_eq(((uint8_t*)data_h)[i], data_ui8[i]);
     tl_tensor_free_data_too_cuda(t2);
     tl_free(data_h);

     t2 = tl_tensor_zeros_cuda(1, (int[]){5}, TL_UINT8);
     t2 = tl_tensor_convert_cuda(t1, t2, TL_UINT8);
     ck_assert_int_eq(t2->ndim, 1);
     ck_assert_int_eq(t2->dtype, TL_UINT8);
     ck_assert_int_eq(t2->len, t1->len);
     ck_assert(t2->dims[0] == t1->dims[0]);
     data_h = tl_clone_d2h(t2->data, tl_size_of(t2->dtype)*t2->len);
     for (int i = 0; i < 5; i++)
          ck_assert_uint_eq(((uint8_t*)data_h)[i], data_ui8[i]);
     tl_tensor_free_data_too_cuda(t2);
     tl_free(data_h);

     tl_tensor_free(t1);
}
END_TEST

START_TEST(test_tl_tensor_transpose_cuda)
{
     tl_tensor *src, *dst;
     int dims1[3] = {2, 3, 2};
     int dims2[3] = {3, 2, 2};
     uint8_t data[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
     int axes1[3] = {0, 2, 1};
     uint8_t dst_data1[12] = {1, 3, 5, 2, 4, 6, 7, 9, 11, 8, 10, 12};
     int axes2[3] = {1, 2, 0};
     uint8_t dst_data2[12] = {1, 7, 2, 8, 3, 9, 4, 10, 5, 11, 6, 12};
     void *data_d, *data_h;
     int i;

     data_d = tl_clone_h2d(data, sizeof(uint8_t)*12);
     src = tl_tensor_create(data_d, 3, dims1, TL_UINT8);

     dst = tl_tensor_transpose_cuda(src, NULL, axes1);
     ck_assert_int_eq(dst->ndim, 3);
     ck_assert_int_eq(dst->dtype, TL_UINT8);
     ck_assert_int_eq(dst->len, 12);
     ck_assert(dst->dims[0] == 2);
     ck_assert(dst->dims[1] == 2);
     ck_assert(dst->dims[2] == 3);
     data_h = tl_clone_d2h(dst->data, tl_size_of(dst->dtype)*dst->len);
     for (i = 0; i < dst->len; i++)
          ck_assert(((int8_t *)data_h)[i] == dst_data1[i]);
     tl_tensor_free_data_too_cuda(dst);
     tl_free(data_h);

     dst = tl_tensor_zeros_cuda(3, dims2, TL_UINT8);
     dst = tl_tensor_transpose_cuda(src, dst, axes2);
     ck_assert_int_eq(dst->ndim, 3);
     ck_assert_int_eq(dst->dtype, TL_UINT8);
     ck_assert_int_eq(dst->len, 12);
     ck_assert(dst->dims[0] == 3);
     ck_assert(dst->dims[1] == 2);
     ck_assert(dst->dims[2] == 2);
     data_h = tl_clone_d2h(dst->data, tl_size_of(dst->dtype)*dst->len);
     for (i = 0; i < dst->len; i++)
          ck_assert(((int8_t *)data_h)[i] == dst_data2[i]);
     tl_tensor_free_data_too_cuda(dst);
     tl_free(data_h);

     tl_tensor_free(src);
     tl_free_cuda(data_d);
}
END_TEST

START_TEST(test_tl_tensor_resize_cuda)
{
     float src_data_h[] = {1, 2, 3, 4};
     float true_data[] = {1, 1, 2, 2, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 4, 4};
     float dst_data_h[16] = {0};
     float *src_data_d, *dst_data_d;
     tl_tensor *src, *dst;

     src_data_d = tl_clone_h2d(src_data_h, sizeof(float)*4);
     src = tl_tensor_create(src_data_d, 2, ARR(int,2,2), TL_FLOAT);

     dst = tl_tensor_resize_cuda(src, NULL, ARR(int,4,4), TL_NEAREST);
     ck_assert_int_eq(dst->ndim, 2);
     ck_assert_int_eq(dst->dtype, TL_FLOAT);
     tl_memcpy_d2h(dst_data_h, dst->data, tl_size_of(TL_FLOAT)*16);
     for (int i = 0; i < dst->len; i++)
          ck_assert_float_eq_tol(((float *)dst_data_h)[i], true_data[i], 1e-5);
     tl_tensor_free_data_too_cuda(dst);

     dst_data_d = tl_clone_h2d(dst_data_h, tl_size_of(TL_FLOAT)*16);
     dst = tl_tensor_create(dst_data_d, 2, ARR(int,4,4), TL_FLOAT);
     dst = tl_tensor_resize_cuda(src, dst, ARR(int,4,4), TL_NEAREST);
     tl_memcpy_d2h(dst_data_h, dst->data, tl_size_of(TL_FLOAT)*16);
     for (int i = 0; i < dst->len; i++)
          ck_assert_float_eq_tol(((float *)dst_data_h)[i], true_data[i], 1e-5);
     tl_tensor_free(dst);

     tl_tensor_free(src);
     tl_free_cuda(src_data_d);
     tl_free_cuda(dst_data_d);
}
END_TEST
/* end of tests */

Suite *make_tensor_cuda_suite(void)
{
     Suite *s;
     TCase *tc_tensor_cuda;

     s = suite_create("tensor_cuda");
     tc_tensor_cuda = tcase_create("tensor_cuda");
     tcase_add_checked_fixture(tc_tensor_cuda, setup, teardown);

     tcase_add_test(tc_tensor_cuda, test_tl_tensor_zeros_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_free_data_too_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_clone_h2d);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_clone_d2h);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_clone_d2d);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_fprint_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_print_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_save_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_zeros_slice_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_slice_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_maxreduce_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_elew_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_convert_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_transpose_cuda);
     tcase_add_test(tc_tensor_cuda, test_tl_tensor_resize_cuda);
     /* end of adding tests */

     suite_add_tcase(s, tc_tensor_cuda);

     return s;
}

#endif /* TL_CUDA */

