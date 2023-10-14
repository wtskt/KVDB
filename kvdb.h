//
// Created by ��ͯ on 2023/10/11.
//

#include <stdio.h>
#include "string.h"
#include "stdlib.h"

#ifndef UNTITLED_KVDB_H
#define UNTITLED_KVDB_H

#define MAX_KV_DATA 1000010
#define MAX_KEY_LEN 8
#define MAX_VALUE_LEN 8

struct KvData {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    struct KvData * next;
};

struct DataBase {
    struct KvData * myDataStart;
    struct KvData * myDataEnd;
    struct KvData * newDataStart;

    char * fileName;
    int dataSize;
    int count;
    int newCount;
} KVDB;

/**
 * �����ݿ�
 * @param fileName
 * @return
 */
int open(const char * fileName);

/**
 * �ر����ݿ�
 */
void close();

/**
 * ����key����ֵ
 * @param key
 * @return
 */
char* getStr(const char * key);

/**
 * �洢kv��
 * @param key
 * @param value
 */
void setStr(const char * key, const char * value);

/**
 * ��ʼ�����ݿ�
 */
void initDB(const char * fileName);



void initDB(const char * fileName) {
    KVDB.dataSize = sizeof (char) * (MAX_KEY_LEN + MAX_VALUE_LEN); /* ��&д�����ݴ�С */
    KVDB.count = 0;
    KVDB.newCount = 0;
    KVDB.fileName = (char *)malloc(sizeof (char) * strlen(fileName) + 1);
    strcpy(KVDB.fileName, fileName);    /* ��������ļ� */

    KVDB.myDataStart = malloc(KVDB.dataSize);
    KVDB.myDataEnd = KVDB.myDataStart;
    KVDB.newDataStart = KVDB.myDataEnd;

    KVDB.myDataStart->next = NULL;                    /* ��Ϊ�� */
    memset(KVDB.myDataStart->key, '\0', sizeof (char) * 8);
}

int open(const char *fileName) {
    initDB(fileName);

    FILE* fp = fopen(fileName, "rb");

    if (fp == NULL) {
        fopen(fileName, "w");
        return 1;
    }

    while (!feof(fp)) {
        struct KvData * node = malloc(sizeof (struct KvData)); /* ��Ҫ�ȷ���ÿռ� */

        if (node == NULL) {
            printf("�����ڴ�ʧ��");
            fclose(fp);
            return 1;
        }

        size_t len = fread(node, KVDB.dataSize, 1, fp);

        if (len == 0) {
            break;
        }

        KVDB.myDataEnd->next = node;
        KVDB.myDataEnd = node;
        node->next = NULL;

        KVDB.count++;
    }

    KVDB.newDataStart = KVDB.myDataEnd; /* ���������ݳ�ʼλ�� */
    fclose(fp);
    return 0;
}

void close() {
    FILE * fp = fopen(KVDB.fileName, "ab");
    struct KvData * pData = KVDB.newDataStart->next; /* ��һλ���������ݵĳ�ʼ��ַ */
    while (pData != NULL) {
        if (strcmp(pData->key, "") != 0) {
            fwrite(pData, KVDB.dataSize, 1, fp);
        }
        pData = pData->next;
    }

    // ToDo
//    free(KVDB.myDataStart);
//    free(KVDB.myDataEnd);
//    free(KVDB.newDataStart);
//    free(KVDB.newDataEnd);
    fclose(fp);
}

char *getStr(const char *key) {
    struct KvData *pData = KVDB.myDataStart;
    while (pData != NULL) {
        if (strcmp(pData->key, key) == 0) {
            return pData->value;
        }
        pData = pData->next;
    }
    return NULL;
}

void setStr(const char* key, const char* value) {

    struct KvData * node = malloc(sizeof (struct KvData));
    memset(node->key, '\0', sizeof (char) * 8);
    memset(node->value, '\0', sizeof (char) * 8);
    strcpy(node->key, key);
    strcpy(node->value, value);
    node->next = NULL;

    KVDB.myDataEnd->next = node;
    KVDB.myDataEnd = node;

    KVDB.newCount++;
    KVDB.count++;
}



#endif //UNTITLED_KVDB_H

