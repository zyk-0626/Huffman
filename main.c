#include "huffman.h"

int main()
{
    // 1. 获取用户输入的文件名
    char filename[256];
    printf("请输入要压缩的文件名：");
    scanf("%255s", filename);

    // 2. 读取文件内容
    char *text = readFileContent(filename);
    if (text == NULL)
    {
        return 1;
    }

    // 3. 统计字符频率
    int freq[256] = {0};
    char chars[256];
    int charCount;
    countFrequency(text, freq, chars, &charCount);

    // 4. 初始化优先队列并添加叶子节点
    Queue q;
    initQueue(&q);
    for (int i = 0; i < charCount; i++)
    {
        enqueue(&q, createleafNode(chars[i], freq[i]));
    }

    // 5. 构建哈夫曼树
    Tree *Huffman = buildhuffmanTree(&q);

    // 6. 准备输出文件
    char outFilename[266];
    snprintf(outFilename, sizeof(outFilename), "huffman_%s", filename);
    FILE *outFile = fopen(outFilename, "w");
    if (outFile == NULL)
    {
        printf("错误：无法创建输出文件 %s\n", outFilename);
        free(text);
        return 1;
    }

    // 7. 写入原始文本和哈夫曼编码
    fprintf(outFile, "原始文本：\n%s\n\n", text);
    fprintf(outFile, "哈夫曼编码如下：\n");
    
    char code[max];
    HuffmancodeToFile(Huffman, code, 0, outFile);

    // 8. 清理资源
    fclose(outFile);
    free(text);
    
    printf("结果已保存到文件：%s\n", outFilename);
    return 0;
}