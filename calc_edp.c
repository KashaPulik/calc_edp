#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int read_file(const char *filename, size_t size, int graph[size][size])
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return 1;

    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++)
        {
            if (fscanf(file, "%d", &graph[i][j]) == -1)
                return 1;
        }
    return 0;
}

int check_connectivity(size_t size, int graph[size][size], int visit_graph[size][size], size_t n, size_t m)
{
    size_t flag = 0;
    if (graph[n][m])
        flag = 1;
    else
        for (size_t i = 0; i < size; i++)
        {
            if (graph[n][i] && !visit_graph[n][i])
            {
                visit_graph[n][i] = 1;
                flag += check_connectivity(size, graph, visit_graph, i, m);
            }
        }
    return flag ? 1 : 0;
}

size_t count_pairs(size_t size, int graph[size][size])
{
    int visit_graph[size][size];
    size_t amount = 0;
    for (size_t i = 0; i < size; i++)
        for (size_t j = i + 1; j < size; j++)
        {
            for (size_t n = 0; n < size; n++)
                for (size_t m = 0; m < size; m++)
                    visit_graph[n][m] = 0;
            amount += !check_connectivity(size, graph, visit_graph, i, j);
        }
    return amount;
}

size_t get_edp(size_t n, size_t size, int graph[size][size])
{
    if (n == 0)
        return count_pairs(size, graph);
    size_t amount = 0;
    for (size_t i = 0; i < size; i++)
        for (size_t j = i + 1; j < size; j++)
        {
            if (graph[i][j] == 1)
            {
                graph[i][j] = 0;
                graph[j][i] = 0;
                amount += get_edp(n - 1, size, graph);
                graph[i][j] = n;
                graph[j][i] = n;
            }
        }
    for (size_t i = 0; i < size; i++)
        for (size_t j = i + 1; j < size; j++)
            if (graph[i][j] == n)
            {
                graph[i][j] = 1;
                graph[j][i] = 1;
            }

    return amount;
}

size_t count_edges(size_t size, int graph[size][size])
{
    size_t n = 0;
    for (size_t i = 0; i < size; i++)
        for (size_t j = i + 1; j < size; j++)
            n += graph[i][j];
    return n;
}

int main(int argc, char *argv[])
{
    clock_t start = clock();
    if (argc != 3)
    {
        fprintf(stderr, "Необходимо указать размер графа и имя файла\n");
        fprintf(stderr, "Usage: calc_edp <n_vertex> <filename>\n");
        return 1;
    }

    size_t size = atoi(argv[1]);
    int graph[size][size];
    if (read_file(argv[2], size, graph))
    {
        fprintf(stderr, "Ошибка чтения файла\n");
        return 1;
    }

    size_t n_edges = count_edges(size, graph);
    n_edges++;
    size_t ks[n_edges];

    for (size_t i = 0; i < n_edges; i++)
    {
        ks[i] = get_edp(i, size, graph);
        for (size_t n = 0; n < size; n++)
            for (size_t m = 0; m < size; m++)
                if (graph[n][m] == 2)
                    graph[n][m] = 1;
    }
    double t = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf("EDP был вычислен за %f секунды\n", t);

    int output_flag = 0;

    for (size_t i = 0; i < n_edges; i++)
    {
        if (ks[i] == 0)
            continue;
        if (i != 0 && ks[i] > 0 && output_flag != 0)
            printf(" + ");
        printf("%ldp^%ldq^%ld", ks[i], n_edges - i - 1, i);
        output_flag = 1;
    }
    printf("\n");
}