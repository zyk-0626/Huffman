#include "huffman.h"

// 统计文本中字符的出现频率
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

// 从文件读取内容
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

int main()
{
    char filename[256];
    printf("请输入要压缩的文件名：");
    scanf("%255s", filename);

    char *text = readFileContent(filename);
    if (text == NULL)
    {
        return 1;
    }

    printf("待压缩文本：\n%s\n", text);

    int freq[256] = {0};
    char chars[256];
    int charCount;
    countFrequency(text, freq, chars, &charCount);

    Queue q;
    initQueue(&q);
    for (int i = 0; i < charCount; i++)
    {
        enqueue(&q, createleafNode(chars[i], freq[i]));
    }

    Tree *Huffman = buildhuffmanTree(&q);

    char outFilename[266];
    snprintf(outFilename, sizeof(outFilename), "huffman_%s", filename);
    FILE *outFile = fopen(outFilename, "w");
    if (outFile == NULL)
    {
        printf("错误：无法创建输出文件 %s\n", outFilename);
        free(text);
        return 1;
    }

    fprintf(outFile, "原始文本：\n%s\n\n", text);

    char code[max];
    printf("\n哈夫曼编码如下：\n");
    Huffmancode(Huffman, code, 0);

    fprintf(outFile, "哈夫曼编码如下：\n");
    HuffmancodeToFile(Huffman, code, 0, outFile);

    fclose(outFile);
    printf("\n结果已保存到文件：%s\n", outFilename);

    free(text);
    return 0;
}