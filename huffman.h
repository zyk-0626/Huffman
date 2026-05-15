// 哈夫曼编码：文本压缩
// 文档：包含什么数据结构，调度方式，怎么运行

// 数据结构：队列、二叉树
//
// 步骤：
//
// 1.为每一个字符创建一个叶子节点，叶子节点包含字符和其权值
//
// 2.将权值放入最小优先队列中，权值就会从小到大，依次排序，
//   保证了每次取出来的两个权值是最小的，和次小的
//
// 3.循环，每次从队列取出的两个权值最小的节点，创建一个新的父节点，
//   父节点的权值是这两个子节点的权值之和，将取出来的两个节点，
//   分别为左节点和右节点。新生成的父节点再插入队列里面。
//   新节点放回队列，直到队列只剩下一个节点，也就是根节点
//
// 4.遍历这棵哈夫曼树，左0右1，生成最终编码

// 运行流程：建叶子 → 优先队列排序 → 循环合并，建立哈夫曼树 → 遍历生成编码

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











