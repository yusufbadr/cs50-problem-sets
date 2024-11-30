// Implements a dictionary's functionality

#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
// choosing N to be a prime number
const unsigned int N = 97;

// Hash table
node *table[N];

int count;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hashVal = hash(word);
    if (table[hashVal] == NULL)
    {
        return false;
    }
    else
    {
        node *curr = table[hashVal];
        while (curr != NULL)
        {
            if (strcasecmp(curr->word, word) == 0)
            {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int len = strlen(word);
    char word_copy[len + 1];
    strcpy(word_copy, word);

    for (int i = 0; i < len; i++)
    {
        word_copy[i] = tolower(word_copy[i]);
    }
    int sum = 0;

    for (int i = 0; i < len; i++)
    {
        sum += word_copy[i];
    }

    return sum % N;
}

// // Hashes word to a number
// unsigned int hash(const char *word)
// {
//     // TODO: Improve this hash function

//     return (word[0]-'a')%26;
// }

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Error in opening the file\n");
        return false;
    }

    char str[LENGTH + 1];
    while (fgets(str, LENGTH + 2, file))
    {
        int len = strlen(str);
        if (len > 0)
        {
            str[len - 1] = '\0';
        }
        int hashedVal = hash(str);
        node *newNode = (node *) malloc(sizeof(node));
        if (newNode == NULL)
        {
            printf("Erro allocating memory");
            return false;
        }

        strcpy(newNode->word, str);
        newNode->next = NULL;

        if (table[hashedVal] == NULL)
        {
            table[hashedVal] = newNode;
        }
        else
        {
            node *tmp = table[hashedVal];
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }

            tmp->next = newNode;
        }
        count++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            node *currNode = table[i];
            while (currNode != NULL)
            {
                node *tmp = currNode;
                currNode = currNode->next;
                free(tmp);
            }
        }
    }
    return true;
}

// void print(){
//     node *tmp;
//     for (int i = 0; i < N; i++){
//         printf("%d=>", i);
//         if (table[i]!=NULL){
//             tmp = table[i];
//             while (tmp!=NULL){
//                 printf("%s  ", tmp->word);
//                 tmp=tmp->next;
//             }
//             printf("\n");
//         }
//     }
// }
