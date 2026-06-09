#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define queue_size 100
#define max 20

// 定义结构体
// 哈夫曼树节点结构
typedef struct node
{
    char ch;      //字符 叶子节点才有
    int weight;    //权值
    struct node *left;
    struct node *right;
}Node,Tree;

// 队列结构
typedef struct queue
{
    Node* data[queue_size];
    int size;
}Queue;

// 函数声明

// 队列相关的
// 初始化队列
void initQueue(Queue* q);
// 入队(权值最小的先入队)
void enqueue(Queue* q,Node* node);
// 出队
Node* dequeue(Queue* q);

// 哈夫曼树相关
// 创建叶子节点
Node* createleafNode(char ch,int weight);
// 构造哈夫曼树
Tree* buildhuffmanTree(Queue* q);

// 得到哈夫曼编码
void Huffmancode(Node* root,char code[],int depth);
// 得到哈夫曼编码（输出到文件）
void HuffmancodeToFile(Node* root, char code[], int depth, FILE *outFile);

// 统计字符频率
void countFrequency(const char *text, int freq[], char chars[], int *charCount);
// 从文件读取内容
char* readFileContent(const char *filename);