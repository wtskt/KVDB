//
// Created by 王童 on 2023/10/11.
//

#include <stdio.h>
#include "string.h"
#include "stdlib.h"
#include "hashTable.h"

#ifndef UNTITLED_KVDB_H
#define UNTITLED_KVDB_H

/**
 * 数据库管理对象
 */
struct DataBase {
    HashTable * table;
    HashTable * newData;

    char * fileName;
    int dataSize;
    int count;
} KVDB;

/**
 * 打开数据库
 * @param fileName
 * @return
 */
int open(const char * fileName);

/**
 * 关闭数据库
 */
void close();

/**
 * 根据key查找值
 * @param key
 * @return
 */
char* getStr(const char * key);

/**
 * 存储kv对
 * @param key
 * @param value
 */
void setStr(const char * key, const char * value);

/**
 * 删除kv对 - 未实现
 * @param key
 * @return
 */
int removeStr(const char * key);

/**
 * 初始化数据库
 * @param fileName
 */
void initDB(const char * fileName);



void initDB(const char * fileName) {
    KVDB.table = create(MAX_HASHTABLE_SIZE);
    KVDB.newData = create(MAX_HASHTABLE_SIZE);

    KVDB.dataSize = sizeof (char) * (MAX_KEY_LEN + MAX_VALUE_LEN); /* 读&写的数据大小 */
    KVDB.count = 0;

    KVDB.fileName = (char *)malloc(sizeof (char) * strlen(fileName) + 1);
    strcpy(KVDB.fileName, fileName);    /* 标记数据文件 */
}

int open(const char *fileName) {
    initDB(fileName);

    FILE* fp = fopen(fileName, "rb");

    if (fp == NULL) {
        fopen(fileName, "w");
        return 1;
    }

    while (!feof(fp)) {
        struct KvData * node = malloc(sizeof (struct KvData)); /* 需要先分配好空间 */

        if (node == NULL) {
            printf("分配内存失败");
            fclose(fp);
            return 1;
        }

        size_t len = fread(node, KVDB.dataSize, 1, fp);

        if (len == 0) {
            break;
        }

        insert(KVDB.table, node->key, node->value);

        KVDB.count++;
    }

    fclose(fp);
    return 0;
}

void close() {
    FILE * fp = fopen(KVDB.fileName, "ab");

    // 遍历，将新数据写入文件
    for (int i = 0; i < KVDB.newData->capacity; i++) {
        struct KvData * cur = KVDB.newData->buckets[i];

        if (cur != NULL) {
            cur = cur->next;
        }

        while (cur != NULL) {
            fwrite(cur, KVDB.dataSize, 1, fp);
            cur = cur->next;
        }
    }

    fclose(fp);
}

char *getStr(const char *key) {
    return search(KVDB.table, key)->value;
}

void setStr(const char* key, const char* value) {
    insert(KVDB.newData, key, value);
    insert(KVDB.table, key, value);
    KVDB.count++;
}

int removeStr(const char * key) {
    return 0;
}
#endif //UNTITLED_KVDB_H

