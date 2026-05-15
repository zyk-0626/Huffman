#include "huffman.h"

// 统计文本中字符的出现频率
void countFrequency(const char *text, int freq[], char chars[], int *charCount)
{
    int len = strlen(text);
    *charCount = 0;
    for (int i = 0; i < len; i++)
    {
        int found = 0; // 记录每种字符出现的次数
        char ch = text[i];
        // 缉拿查是否统计过这个字符
        for (int j = 0; j < *charCount; j++)
        {
            if (chars[j] == ch)
            {
                freq[j]++;
                found = 1;
                break;
            }
        }
        // 没统计过的found+1
        if (found == 0)
        {
            chars[*charCount] = ch;
            freq[*charCount] = 1;
            (*charCount)++;
        }
    }
}

int main()
{
    // 1. 示例文本（也可以改成从文件读取）
    const char *text = "abracadabra";
    printf("待压缩文本：%s\n", text);

    // 2. 统计字符频率
    int freq[256] = {0}; // 记录每个字符出现的频率，eg：freg[0]是记录字符a的次数
    char chars[256];     // 有哪些不同的字符，eg：chars[0]='a' chars[1]='b'
    int charCount;       // 文本一共有几种不同字符 如这个文本有5种 a b r c d
    countFrequency(text, freq, chars, &charCount);

    // 3. 初始化优先队列，加入所有叶子节点
    Queue q;
    initQueue(&q);
    for (int i = 0; i < charCount; i++)
    {
        enqueue(&q,createleafNode(chars[i],freq[i]));
    }
    
    // 4. 构建哈夫曼树
    Tree* Huffman=buildhuffmanTree(&q);
    
    // 5. 生成并打印哈夫曼编码
    char code[max];
    printf("哈夫曼编码如下：\n");
    Huffmancode(Huffman,code,0);
    
    return 0;
}
