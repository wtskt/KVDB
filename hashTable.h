//
// Created by 王童 on 2023/10/14.
//

#ifndef UNTITLED_HASHTABLE_H
#define UNTITLED_HASHTABLE_H

#define MAX_HASHTABLE_SIZE 100000 // 最大存储对数
#define MAX_KEY_LEN 8 // 最大键长
#define MAX_VALUE_LEN 8 // 最大值长

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
    return h;
}

/**
 * n-gram字符串哈希
 * @param str
 * @param n
 * @return
 */
unsigned long ngrams_hash(const char* str, int n) {
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
    return hashValue % MAX_HASHTABLE_SIZE;
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
    int index = (int) ngrams_hash(key, 2);

    struct KvData* node =  malloc(sizeof(struct KvData));
    strcpy(node->key, key);
    strcpy(node->value, value);

    /* 头插法 */
    struct KvData **head = &(table->buckets[index]);
    if ((*head) == NULL) {
        (*head) = malloc(sizeof (struct KvData));
        (*head)->next = NULL;
    }

    node->next = (*head)->next;
    (*head)->next = node;
    table->size++;
}

/**
 * 搜索节点
 * @param table
 * @param key
 * @return
 */
struct KvData* search(HashTable* table, const char* key) {
    int index = (int) ngrams_hash(key, 2);
    struct KvData* cur = table->buckets[index];

    while (cur != NULL) {
        if (strcmp(cur->key, key) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

/**
 * 删除节点
 * @param table
 * @param key
 */
int delete(HashTable * table, const char * key) {
    int index = (int) ngrams_hash(key, 2);
    struct KvData* cur = table->buckets[index];
    while (cur->next != NULL) {
        if (strcmp(cur->next->key, key) == 0) {
            cur->next = cur->next->next;
            return 1;
        }
    }
    return 0;
}
#endif //UNTITLED_HASHTABLE_H
