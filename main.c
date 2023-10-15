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

void testTimeByMs(void (*ptr)());   /* ���뼶 */

void testTime(void (*ptr)());       /* ΢� */

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
    //��������Ϊ��λ������ʱ�䣬������ʱ��-��ʼʱ�䣩/CLK_TCK
    printf("�ķ�ʱ��Ϊ%f��\n", ((double)(stop - start)) / CLK_TCK);
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
    printf("�ķ�ʱ��Ϊ%lf��\n", exe_time);

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
    printf("��ǰ��������: %d\n", KVDB.count);
}

void testSetOne() {
    open("data");

    char *key =getRandomString(5);
    char *value = getRandomString(5);
    setStr(key, value);

    close();
}

char* getRandomString(int length) { // ����length���ȵ�����ַ���
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

void test1 () { /* �����ƶ�д���� */
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