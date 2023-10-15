#include <stdio.h>
#include "kvdb.h"
#include "windows.h"
#include "time.h"

int seed = 1;

char *keys[10] = {NULL};

char* getRandomString(int length);

void test1 ();

void test2();

void testGet();

void testSet();

void testSetOne();

void testTimeByMs(void (*ptr)());   /* 毫秒级 */

void testTime(void (*ptr)());       /* 微妙级 */

int main() {
    testTime(&testSet);
//    testTime(&testGet);
//    testSetOne();
    return 0;
}

void testTimeByMs(void (*ptr)()) {
    clock_t start, stop;
    start = clock();

    open("data");
    ptr();
    close();

    stop = clock();
    //计算以秒为单位的运行时间，（结束时间-开始时间）/CLK_TCK
    printf("耗费时间为%f秒\n", ((double)(stop - start)) / CLK_TCK);
}

void testTime(void (*ptr)()) {
    open("data");

    LARGE_INTEGER freq;
    LARGE_INTEGER start, stop;
    double exe_time;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);


    ptr();


    QueryPerformanceCounter(&stop);
    exe_time = 1e3*(stop.QuadPart - start.QuadPart) / freq.QuadPart / 1000.0;
    printf("耗费时间为%lf秒\n", exe_time);

    close();
}

void testGet() {
    char *value = getStr("BQK0");
    printf("%s\n", value);
}

void testSet() {
    for (int i = 0; i < 5; i++) {
        char *key = getRandomString(5);
        char *value = getRandomString(5);
        setStr(key, value);
    }
    printf("当前数据总量: %d\n", KVDB.count);
}

void testSetOne() {
    open("data");

    char *key =getRandomString(5);
    char *value = getRandomString(5);
    setStr(key, value);

    close();
}

char* getRandomString(int length) { // 生成length长度的随机字符串
    int flag, i;
    char* string;
    srand(seed++);
    if ((string = (char*) malloc(length)) == NULL ) {
        return NULL ;
    }

    for (i = 0; i < length - 1; i++) {
        flag = rand() % 3;
        switch (flag) {
            case 0:
                string[i] = 'A' + rand() % 26;
                break;
            case 1:
                string[i] = 'a' + rand() % 26;
                break;
            case 2:
                string[i] = '0' + rand() % 10;
                break;
            default:
                string[i] = 'x';
                break;
        }
    }
    string[length - 1] = '\0';
    return string;
}

void test1 () { /* 二进制读写测试 */
    FILE *fp = fopen("data", "ab");
    char * key = "hello";
    char buf[128] = "";

    fwrite(key, sizeof (char), 5, fp);
    fclose(fp);

    fp = fopen("data", "rb");
    fread(buf, sizeof (char), 5, fp);
    printf("%s", buf);

    fclose(fp);
}

void test2() {
    open("data");
    setStr("wt", "hello");
    char *string = getStr("wt");
    printf("%s", string);
    close();
}