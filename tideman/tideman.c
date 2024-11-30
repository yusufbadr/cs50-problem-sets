#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = i + 1; j < MAX; j++)
        {
            pair pair;
            if (preferences[i][j] == preferences[j][i])
            {
                continue;
            }
            else if (preferences[i][j] - preferences[j][i] < 0)
            {
                pair.winner = j;
                pair.loser = i;
            }
            else
            {
                pair.winner = i;
                pair.loser = j;
            }
            pairs[pair_count] = pair;
            pair_count++;
        }
    }
}

int compare(const void *p1, const void *p2)
{
    if (preferences[(*(pair *) p1).winner][(*(pair *) p1).loser] < preferences[(*(pair *) p2).winner][(*(pair *) p2).loser])
    {
        return 1;
    }
    else if (preferences[(*(pair *) p1).winner][(*(pair *) p1).loser] > preferences[(*(pair *) p2).winner][(*(pair *) p2).loser])
    {
        return -1;
    }
    else if (preferences[(*(pair *) p1).winner][(*(pair *) p1).loser] == preferences[(*(pair *) p2).winner][(*(pair *) p2).loser])
    {
        return 0;
    }
    return 0;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    qsort(pairs, pair_count, sizeof(pair), compare);
}

bool checkCyclicRec(int node, bool visited[], bool inStack[])
{
    if (inStack[node])
    {
        return true;
    }
    if (visited[node])
    {
        return false;
    }

    inStack[node] = true;
    visited[node] = true;

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[node][i] && checkCyclicRec(i, visited, inStack))
        {
            return true;
        }
    }

    inStack[node] = false;
    return false;
}

bool checkCyclicUtil(int node)
{
    bool visited[candidate_count];
    bool inStack[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        visited[i] = false;
        inStack[i] = false;
    }

    if (checkCyclicRec(node, visited, inStack))
    {
        return true;
    }

    return false;
}

bool checkCyclic(pair pair)
{
    locked[pair.winner][pair.loser] = true;
    int startNode = pair.winner;
    if (checkCyclicUtil(startNode))
    {
        locked[pair.winner][pair.loser] = false;
        return true;
    }

    locked[pair.winner][pair.loser] = false;
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (checkCyclic(pairs[i]))
        {
            continue;
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{

    // for each candidate
    //      check if no arrow pointing towards candidate then print candidate
    bool winner[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        winner[i] = true;
        ;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                winner[i] = false;
                break;
            }
        }
    }

    for (int i = 0; i < candidate_count; i++)
    {
        if (winner[i])
        {
            printf("%s\n", candidates[i]);
        }
    }
}
