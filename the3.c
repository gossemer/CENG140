#include <stdio.h>
#include <stdlib.h>
#include "the3.h"
#include <string.h>

vp DFS(vp graph, char *wanted, char mark)
{
    int i;
    vp w;
    TAG(graph) = mark;
    for (i = 0; EDGE(graph)[i] != NULL; i++)
    {
        w = EDGE(graph)[i];
        if (!strcmp(LABEL(w), wanted))
        {
            DFS(graph, "", 'q');
            return w;
        }
        if (TAG(w) != mark)
        {
            TAG(w) = mark;
            return DFS(w, wanted, mark);
        }
    }
    return NULL;
}

vp DFT(vp graph1, char mark)
{
    int i;
    vp w, merged;

    for (i = 0; EDGE(graph1)[i] != NULL; i++)
        ;
    merged = malloc(sizeof(vertex) + (i + 1) * sizeof(vp));

    LABEL(merged) = strdup(LABEL(graph1));
    TAG(merged) = mark;
    for (i = 0; EDGE(graph1)[i] != NULL; i++)
    {
        w = EDGE(graph1)[i];
        if (TAG(w) != mark)
        {
            TAG(w) = mark;
            EDGE(merged)
            [i] = DFT(w, mark);
        }
        else
        {
            EDGE(merged)
            [i] = w;
        }
    }
    EDGE(merged)
    [i] = NULL;
    return merged;
}
vp dft(vp graph1, vp graph2, char mark)
{
    int i, j, k, cmp;
    vp v, w, merged, search;
    search = DFS(graph2, LABEL(graph1), mark + 1);
    for (i = 0; EDGE(graph1)[i] != NULL; i++)
        ;
    j = 0;
    if (search)
    {
        for (j = 0; EDGE(search)[j] != NULL; j++)
            ;
    }
    merged = malloc(sizeof(vertex) + (i + j + 1) * sizeof(vp));

    LABEL(merged) = strdup(LABEL(graph1));
    TAG(merged) = mark;
    TAG(graph1) = mark;
    if (search)
    {
        TAG(search) = mark;
        for (i = j = k = 0; EDGE(graph1)[i] != NULL || EDGE(search)[j] != NULL; k++)
        {
            v = EDGE(graph1)[i];
            w = EDGE(search)[j];
            if (v && w)
            {
                cmp = strcmp(LABEL(v), LABEL(w));
                if (cmp < 0)
                {
                    if (TAG(v) != mark)
                    {
                        TAG(v) = mark;
                        EDGE(merged)
                        [k] = dft(v, graph2, mark);
                    }
                    else
                    {
                        EDGE(merged)
                        [k] = v;
                    }
                    i++;
                }
                else
                {
                    if (TAG(w) != mark)
                    {
                        TAG(w) = mark;
                        EDGE(merged)
                        [k] = dft(w, graph1, mark);
                    }
                    else
                    {
                        EDGE(merged)
                        [k] = w;
                    }
                    j++;
                }
            }
            else if (v)
            {
                if (TAG(v) != mark)
                {
                    TAG(v) = mark;
                    EDGE(merged)
                    [k] = dft(v, graph2, mark);
                }
                else
                {
                    EDGE(merged)
                    [k] = v;
                }
                i++;
            }
            else if (w)
            {
                if (TAG(w) != mark)
                {
                    TAG(w) = mark;
                    EDGE(merged)
                    [k] = dft(w, graph1, mark);
                }
                else
                {
                    EDGE(merged)
                    [k] = w;
                }
                j++;
            }
            else
                break;
        }
        EDGE(merged)
        [k] = NULL;
    }
    else
    {
        DFT(graph1, mark);
    }
    return merged;
}

vp merge(vp graph1, vp graph2)
{
    vp merged = dft(graph1, graph2, '!');
    return merged;
}
