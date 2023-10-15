//
// Created by 王童 on 2023/10/14.
//

#ifndef UNTITLED_HASHTABLE_H
#define UNTITLED_HASHTABLE_H

#define MAX_HASHTABLE_SIZE 100000
#define MAX_KEY_LEN 8
#define MAX_VALUE_LEN 8

struct KvData {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    struct KvData * next;
};

// 哈希表结构
typedef struct HashTable {
    int capacity; // 表容量
    int size;     // 表大小

    struct KvData** buckets; // 槽位数组, 每个槽位为链表
} HashTable;


/**
 * 哈希函数,这里用FNV-1a算法
 * @param key
 * @return
 */
unsigned long hash(const char* str) {
    unsigned long h = 2166136261;
    for (int i = 0; str[i] != '\0'; i++) {
        h = (h^str[i]) * 16777619;
    }
    return h % MAX_HASHTABLE_SIZE;
}

/**
 * n-gram字符串哈希
 * @param str
 * @param n
 * @return
 */
unsigned long ngrams_hash(char* str, int n) {
    unsigned long hashValue = 0;
    int len = (int) strlen(str);

    for (int i = 0; i < len-n+1; i++) {
        char ngram[n+1];
        for (int j = 0; j < n; j++) {
            ngram[j] = str[i+j];
        }
        ngram[n] = '\0';

        unsigned long h = hash(ngram);

        hashValue += h;
    }
    return hashValue;
}

/**
 * 创建哈希表
 * @param capacity
 * @return
 */
HashTable * create(int capacity) {
    HashTable * table = malloc(sizeof (HashTable));
    table->capacity = capacity;
    table->size = 0;

    table->buckets = malloc(capacity * sizeof (struct KvData));

    for (int i = 0; i < capacity; i++) {
        table->buckets[i] = NULL;
    }

    return table;
}

/**
 * 插入节点，使用链地址法解决哈希冲突
 * @param table
 * @param key
 * @param value
 */
void insert(HashTable* table, const char *key, const char *value) {
    int index = (int)hash(key);

    struct KvData* node =  malloc(sizeof(struct KvData));
    strcpy(node->key, key);
    strcpy(node->value, value);

    /* 头插法 */
    node->next = table->buckets[index];
    table->buckets[index] = node;
    table->size++;
}

/**
 * 搜索节点
 * @param table
 * @param key
 * @return
 */
struct KvData* search(HashTable* table, const char* key) {
    int index = (int)hash(key);
    struct KvData* cur = table->buckets[index];

    while (cur != NULL) {
        if (strcmp(cur->key, key) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}
#endif //UNTITLED_HASHTABLE_H
