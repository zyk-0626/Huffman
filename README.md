# 项目名称

**哈夫曼编码文本压缩**

---

## 算法背景

### 0.1 算法历史

哈夫曼编码是由美国计算机科学家 **David A. Huffman** 于 1952 年在麻省理工学院攻读博士学位期间发明的。

| 科学家 | 国籍 | 贡献 |
|--------|------|------|
| **David A. Huffman** | 美国 | 提出哈夫曼编码算法，发表于《A Method for the Construction of Minimum-Redundancy Codes》 |

**发表时间**：1952年

**历史意义**：
- 这是第一个实现最优前缀编码的贪心算法
- 开创了数据压缩领域的先河，成为无损压缩的基础算法
- 至今仍被广泛应用于各种压缩格式（如ZIP、JPEG、MP3等）

### 0.2 算法命名由来

哈夫曼编码（Huffman Coding）以其发明者 **David A. Huffman** 的名字命名。

### 0.3 算法设计思想

哈夫曼编码的核心思想源于信息论中的 **香农编码定理**，该定理证明了对于信源符号的最优编码长度下界。哈夫曼通过贪心策略实现了这一下界：

**核心原理**：
- **频率优先**：出现频率越高的字符，分配越短的编码
- **前缀编码**：任意字符的编码都不是其他字符编码的前缀，保证解码的唯一性
- **最优性证明**：哈夫曼编码在给定字符频率下，能产生最小的平均编码长度

---

## 一、需求分析

### 1.1 项目背景

哈夫曼编码广泛应用于：

- **数据压缩**：ZIP、GZIP、DEFLATE 等压缩格式
- **图像压缩**：JPEG 图像的熵编码阶段
- **音频压缩**：MP3、AAC 等音频格式
- **通信领域**：数据传输中的高效编码

### 1.2 功能需求

本项目实现一个完整的哈夫曼编码文本压缩工具，支持以下核心功能：

| 功能 | 说明 |
|------|------|
| **字符频率统计** | 遍历输入文本，统计每个字符的出现次数 |
| **哈夫曼树构建** | 基于字符频率，使用贪心算法构建最优哈夫曼树 |
| **编码生成** | 遍历哈夫曼树，为每个字符生成唯一的二进制编码 |
| **编码输出** | 展示每个字符及其对应的哈夫曼编码 |
| **压缩演示** | 提供示例文本的完整压缩流程演示 |

### 1.3 性能需求

| 指标 | 要求 |
|------|------|
| **时间复杂度** | O(n log n)，其中 n 为不同字符的数量 |
| **空间复杂度** | O(n) 用于存储哈夫曼树和编码表 |
| **压缩率** | 对于字符频率分布不均匀的文本，可实现 30%-70% 的压缩率 |

---

## 二、核心数据结构设计

### 2.1 整体文件结构

| 文件 | 职责 |
|------|------|
| **main.c** | 程序入口，负责用户交互和整体流程控制 |
| **huffman.h** | 定义数据结构和函数接口声明 |
| **huffman.c** | 实现所有核心功能（队列操作、树构建、编码生成、频率统计、文件读取） |

**职责划分**：
- **main.c**：仅负责流程编排和用户交互，不包含业务逻辑
- **huffman.c**：包含所有哈夫曼编码相关的核心算法实现
- **huffman.h**：提供对外暴露的数据结构和函数接口

### 2.2 哈夫曼树节点

```c
typedef struct node
{
    char ch;
    int weight;
    struct node *left;
    struct node *right; 
} Node, Tree;
```

**节点类型说明**：

| 节点类型 | ch 字段 | weight 字段 | left/right |
|----------|---------|-------------|------------|
| **叶子节点** | 存储原始字符 | 字符出现频率 | NULL |
| **中间节点** | `'\0'`（空字符） | 左右子节点权值之和 | 指向子节点 |

**设计要点**：
- 使用二叉链表结构存储哈夫曼树
- 左子树路径对应编码位 `0`，右子树路径对应编码位 `1`
- 叶子节点存储实际字符，中间节点仅用于构建树结构

### 2.3 优先队列

```c
typedef struct queue
{
    Node* data[queue_size];
    int size;
} Queue;
```

**数据结构特性**：

| 属性 | 说明 |
|------|------|
| **存储方式** | 数组实现的顺序队列 |
| **排序规则** | 按节点权值升序排列 |
| **核心作用** | 每次取出权值最小的两个节点用于构建哈夫曼树 |

**核心操作**：

| 操作 | 功能描述 | 实现方式 |
|------|----------|----------|
| `initQueue` | 初始化队列 | 设置 size 为 0 |
| `enqueue` | 按权值有序插入 | 插入排序思想，保持队列升序 |
| `dequeue` | 取出队首元素 | 返回 data[0]，后续元素前移 |
| `isEmpty` | 判断队列是否为空 | 判断 size == 0 |

---

## 三、核心算法设计

### 3.1 文件读取算法

```c
char* readFileContent(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("错误：无法打开文件 %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(fileSize + 1);
    if (content == NULL)
    {
        printf("错误：内存分配失败\n");
        fclose(file);
        return NULL;
    }

    size_t bytesRead = fread(content, 1, fileSize, file);
    content[bytesRead] = '\0';

    fclose(file);
    return content;
}
```

**算法设计思路**：
1. 使用 `fopen` 打开文件，检查文件是否成功打开
2. 使用 `fseek` 将文件指针移到末尾，通过 `ftell` 获取文件大小
3. 根据文件大小动态分配内存
4. 使用 `fread` 读取文件内容到内存缓冲区
5. 在内容末尾添加字符串结束符 `'\0'`

**时间复杂度**：O(n)，其中 n 为文件大小
**空间复杂度**：O(n)，需要分配与文件大小相同的内存空间

**错误处理**：
- 文件不存在或无法打开时返回 NULL
- 内存分配失败时返回 NULL

---

### 3.2 字符频率统计算法

```c
void countFrequency(const char *text, int freq[], char chars[], int *charCount)
{
    int len = strlen(text);
    *charCount = 0;
    for (int i = 0; i < len; i++)
    {
        int found = 0;
        char ch = text[i];
        for (int j = 0; j < *charCount; j++)
        {
            if (chars[j] == ch)
            {
                freq[j]++;
                found = 1;
                break;
            }
        }
        if (found == 0)
        {
            chars[*charCount] = ch;
            freq[*charCount] = 1;
            (*charCount)++;
        }
    }
}
```

**算法设计思路**：
1. 遍历输入文本的每个字符
2. 对每个字符，检查是否已在 `chars` 数组中
3. 如果已存在，对应的频率计数加 1
4. 如果不存在，将字符添加到 `chars` 数组末尾，频率设为 1

**时间复杂度**：O(n^2)（n 为文本长度，双重循环统计）
**空间复杂度**：O(k)，其中 k 为不同字符的数量

**输入输出示例**：
- 输入：`text = "abracadabra"`
- 输出：`chars = ['a', 'b', 'r', 'c', 'd'], freq = [5, 2, 2, 1, 1], charCount = 5`

---

### 3.3 优先队列入队算法

```c
void enqueue(Queue *q, Node *node)
{
    if (q->size >= queue_size)
    {
        return;
    }
    int i = q->size - 1;
    while (i >= 0 && q->data[i]->weight > node->weight)
    {
        q->data[i + 1] = q->data[i];
        i--;
    }
    q->data[i + 1] = node;
    q->size++;
}
```

**算法设计思路**：
1. 检查队列是否已满
2. 使用插入排序思想，从队列末尾向前遍历
3. 将权值大于新节点的元素向后移动
4. 将新节点插入到合适位置，保持队列升序排列

**时间复杂度**：O(n)，其中 n 为队列当前元素个数
**空间复杂度**：O(1)，原地操作

**设计要点**：
- 队列按节点权值升序排列
- 队首元素始终是权值最小的节点
- 为哈夫曼树构建提供高效的最小节点获取

---

### 3.4 优先队列出队算法

```c
Node *dequeue(Queue *q)
{
    if (q->size == 0)
        return NULL;
    int i = 1;
    Node *tmp = q->data[0];
    while (i < q->size)
    {
        q->data[i - 1] = q->data[i];
        i++;
    }
    q->size--;
    return tmp;
}
```

**算法设计思路**：
1. 检查队列是否为空
2. 保存队首元素（权值最小的节点）
3. 将所有后续元素向前移动一位
4. 队列大小减 1，返回保存的队首元素

**时间复杂度**：O(n)，其中 n 为队列当前元素个数
**空间复杂度**：O(1)，原地操作

---

### 3.5 哈夫曼树构建算法

```c
Tree* buildhuffmanTree(Queue* q)
{
    while (q->size > 1)
    {
        Node *left = dequeue(q);
        Node *right = dequeue(q);
        int dad_weight = left->weight + right->weight;
        Node *dad = (Node *)malloc(sizeof(Node));
        dad->left = left;
        dad->right = right;
        dad->ch = '\0';
        dad->weight = dad_weight;

        enqueue(q, dad);  
    }
    return dequeue(q);
}
```

**算法设计思路（贪心算法）**：
1. **初始化**：将所有叶子节点按权值升序加入优先队列
2. **合并操作**：取出队首两个权值最小的节点
3. **创建中间节点**：权值为两个子节点权值之和
4. **插入队列**：将新节点按权值插入优先队列
5. **终止条件**：重复步骤 2-4，直到队列中只剩一个节点（根节点）

**时间复杂度**：O(n log n)，其中 n 为不同字符的数量
- 每个节点入队/出队操作：O(n)
- 共有 2n-1 个节点操作

**空间复杂度**：O(n)，存储哈夫曼树的所有节点

**算法正确性证明**：
- 哈夫曼算法是贪心算法的经典应用
- 每次合并权值最小的两个节点保证了全局最优性
- 生成的哈夫曼树具有最小加权路径长度

---

### 3.6 编码输出到文件算法

```c
void HuffmancodeToFile(Node* root, char code[], int depth, FILE *outFile)
{
    if (root == NULL) return;
    if (root->left == NULL && root->right == NULL)
    {
        code[depth] = '\0';
        fprintf(outFile, "该字符%c的编码是：%s\n", root->ch, code);
        return;
    }

    code[depth] = '0';
    HuffmancodeToFile(root->left, code, depth + 1, outFile);

    code[depth] = '1';
    HuffmancodeToFile(root->right, code, depth + 1, outFile);
}
```

**算法设计思路**：
- 与 `Huffmancode` 算法逻辑相同
- 将输出目标从控制台改为文件
- 使用 `fprintf` 将编码结果写入指定文件

**时间复杂度**：O(n)，其中 n 为哈夫曼树的节点数

---

## 四、业务流程设计

### 4.1 整体流程图

```
┌─────────────┐     ┌─────────────┐     ┌─────────────────┐
│ 输入文件      │ --> │ 频率统计     │ --> │ 构建优先队列      │
└─────────────┘     └─────────────┘     └─────────────────┘
                                              │
                                              ▼
┌─────────────┐     ┌─────────────┐     ┌─────────────────┐
│ 输出结果文件  │ <-- │ 生成编码     │ <-- │ 构建哈夫曼树      │
└─────────────┘     └─────────────┘     └─────────────────┘
```

**流程概述**：
```
输入文件 → 读取内容 → 频率统计 → 初始化队列 → 构建哈夫曼树 → 生成编码 → 输出文件
```

### 4.2 详细流程说明

| 步骤 | 操作 | 函数调用 | 输入 | 输出 |
|------|------|----------|------|------|
| 1 | 获取文件名 | 用户输入 | 用户输入的文件名 | `filename` 字符串 |
| 2 | 读取文件内容 | `readFileContent(filename)` | 文件名 | 文件内容字符串 `text` |
| 3 | 字符频率统计 | `countFrequency(text, freq, chars, &charCount)` | 文本内容 | `chars[]` 字符数组、`freq[]` 频率数组、`charCount` 字符种类数 |
| 4 | 初始化优先队列 | `initQueue(&q)` | 队列表头指针 | 空队列 |
| 5 | 创建叶子节点 | `createleafNode(ch, weight)` | 字符和频率 | 叶子节点指针 |
| 6 | 入队操作 | `enqueue(&q, node)` | 队列和节点 | 有序队列 |
| 7 | 构建哈夫曼树 | `buildhuffmanTree(&q)` | 优先队列 | 哈夫曼树根节点 |
| 8 | 生成编码并输出 | `HuffmancodeToFile(root, code, 0, outFile)` | 根节点、输出文件 | 编码写入文件 |
| 9 | 释放资源 | `free(text)` | 动态分配的文本 | 内存释放 |

---

## 五、难点与解决方法

### 5.1 优先队列的有序插入与出队

**难点**：保证每次取出的都是权值最小的节点

**解决方法**：
- `enqueue`：采用插入排序的思想，将新节点按权值从后往前插入到合适位置，保持队列升序
- `dequeue`：取出队首元素后，将后续元素整体前移一位

### 5.2 哈夫曼树的构建逻辑

**难点**：中间节点的权值合并与队列维护

**解决方法**：合并两个节点后，生成新的中间节点，再调用 `enqueue` 按权值插入队列，保证队列始终有序

### 5.3 递归生成哈夫曼编码

**难点**：如何记录当前路径的编码序列

**解决方法**：用 `code` 数组存储当前路径的编码，`depth` 记录当前节点的深度，递归返回时自动回溯修改编码（`code[depth]` 会被后续递归覆盖）
