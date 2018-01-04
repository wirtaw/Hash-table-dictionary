#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLESIZE 101

// Связанный список
typedef struct node
{
  char *data;
  struct node *next;
} node;

// Хэш-функция: возвращаемое значение будет остатком от деления разности между 
// значением первого символа строки и значением первого строкового символа
// таблицы ASCII на переданное значение размера таблицы.
unsigned int hash(const char *str, int tablesize)
{
    int value;

    // Получение значения первого символа строки
    value = toupper(str[0]) - 'A';

    return value % tablesize;
}

static int lookup(node *table[], const char *key)
{
    unsigned index = hash(key, TABLESIZE);
    const node *it = table[index];

    // Попытка установить наличие соответствующего ключа в связанном списке
    while(it != NULL && strcmp(it->data, key) != 0)
    {
        it = it->next;
    }
    return it != NULL;
}

int insert(node *table[], char *key)
{
    if( !lookup(table, key) )
    {
        // Поиск необходимого связанного списка
        unsigned index = hash(key, TABLESIZE);
        node *new_node = malloc(sizeof *new_node);

        if(new_node == NULL)
            return 0;

        new_node->data = malloc(strlen(key)+1);

        if(new_node->data == NULL)
            return 0;

        // Добавление нового ключа и обновление указателя на начало связанного списка
        strcpy(new_node->data, key);
        new_node->next = table[index];
        table[index] = new_node;
        return 1;
    }
    return 0;
}

// Заполнение хэш-таблицы
// Первый параметр: Переменная хэш-таблицы
// Второй параметр: Структура файла со словами
int populate_hash(node *table[], FILE *file)
{
    char word[50];
    char c;

    do {
        c = fscanf(file, "%s", word);
        // Важно: следует удалить символ перехода на следующую строку
        size_t ln = strlen(word) - 1;
        if (word[ln] == '\n')
            word[ln] = '\0';

        insert(table, word);
    } while (c != EOF);

    return 1;
}

int main(int argc, char **argv)
{
    char word[50];
    char c;
    int found = 0;

    // Инициализация хэш-таблицы
    node *table[TABLESIZE] = {0};

    FILE *INPUT;
    INPUT = fopen("INPUT", "r");
    // Заполнение хэш-таблицы
    populate_hash(table, INPUT);
    fclose(INPUT);
    printf("Хэш-таблица заполнена!\n");

    int line = 0;
    FILE *CHECK;
    CHECK = fopen("CHECK", "r");

    do {
        c = fscanf(CHECK, "%s", word);

        // Важно: следует удалить символ перехода на следующую строку
        size_t ln = strlen(word) - 1;
        if (word[ln] == '\n')
            word[ln] = '\0';

        line++;
        if( lookup(table, word) )
        {
            found++;
        }
    } while (c != EOF);

    printf("В хэш-таблице обнаружено %d слов!\n", found);

    fclose(CHECK);
    return 0;
}