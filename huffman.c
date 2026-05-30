#include "huffman.h"

// 队列相关的
// 初始化队列
void initQueue(Queue *q)
{
    q->size = 0;
}

// 入队(权值最小的先入队)
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

// 出队
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

// 哈夫曼树相关
// 创建叶子节点
Node *createleafNode(char ch, int weight)
{
    Node *leaf = (Node *)malloc(sizeof(Node));
    leaf->ch = ch;
    leaf->weight = weight;
    leaf->right = NULL;
    leaf->left = NULL;
    return leaf;
}

// 构造哈夫曼树
Tree *buildhuffmanTree(Queue *q)
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

        enqueue(q,dad);  
    }
    return dequeue(q);
}

// 得到哈夫曼编码
void Huffmancode(Node* root,char code[],int depth)
{
    if (root==NULL) return;
    if (root->left==NULL&&root->right==NULL)
    {
        code[depth]='\0';
        printf("该字符%c的编码是：%s\n",root->ch,code);
        return;
    }

    code[depth]='0';
    Huffmancode(root->left,code,depth+1);

    code[depth]='1';
    Huffmancode(root->right,code,depth+1);
}