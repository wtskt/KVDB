###

#### 哈希算法
##### FNV-1a算法
> FNV-1a算法是一种非常流行的非加密哈希函数.
FNV-1a算法的一些主要特点包括:
1. 快速计算:它只进行了乘法和异或操作,速度很快。
2. 低碰撞率:经过验证,FNV-1a在实际应用中的碰撞率很低。
3. 从串首计算:它是从左到右依次处理每个字节,这可以支持串切片计算。
4. 简单实现:算法逻辑清晰,代码实现简短。
5. 多平台兼容:FNV-1a的计算过程与平台无关,在不同系统上计算结果相同。
6. 公开标准:FNV-1a定义明确,任何编程语言都可以实现。

> 具体算法步骤为:
1. 给定初始基数h=2166136261
2. 对每个字节b: h = h XOR b
3. h = h * 16777619
4. 返回最终h值

##### n-gram字符串哈希
> n-gram字符串哈希的主要步骤是:
1. 设置n-gram的长度n
这个n决定了字符串将被分成多长的片段。一般取2-5。
2. 对字符串进行n-gram切割
以n为步长,分别取字符串连续的子串组成n-gram序列。
3. 计算每个n-gram的哈希值
使用性能好的哈希算法(如FNV)对每个n-gram子串计算哈希。
4. 将n-gram哈希值求和
将所有n-gram的哈希值利用累加的方式求和。
5. 对求和结果取模
将上一步的和值对哈希表的大小取模,得到索引位置。
6. 插入kv对
将<原字符串,值>键值对插入到索引位置在表中的槽位。
7. 查找过程
搜索时重复上述步骤1-5,获取匹配索引位置, dann在其槽位上查找。

> 总结:
1. n-gram切割
2. 每段n-gram哈希
3. 求和
4. 取模(索引)
5. 插入/查找

#### redis持久化策略
redis开始是也在内存中的，但后它有两种持久化策略： RDB和AOF，可以将数据持久化。 --法海