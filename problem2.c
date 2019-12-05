#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct coord
{
    int x;
    int y;
    int l;
} coord;

int distance(struct coord a);
char ** split(char buffer[]);
struct coord * trace(char **instruction);
int uniqcoord (struct coord a[], struct coord b);
struct coord * intersect(struct coord a[], struct coord b[]);

int main(int argc, char const *argv[])
{
    char buffer1[2000], buffer2[2000];
    struct coord *line1, *line2;

    FILE * file;
    file = fopen("input", "r");
    
    fgets(buffer1, 2000, file); strtok(buffer1, "\n");
    struct coord *path1 = trace(split(buffer1));
    
    fgets(buffer2, 2000, file); strtok(buffer2, "\n");
    struct coord *path2 = trace(split(buffer2));

    struct coord *intersection = intersect(path1, path2);
    printf("(%d, %d)\n", intersection[0].x, intersection[0].y);

    int min = __INT_MAX__;
    for (size_t i = 0; intersection[i].x != 0 || intersection[i].y != 0; i++)
    {
        if (intersection[i].l < min) { min = intersection[i].l; }
        printf("(%d, %d) @ %d -> %d\n", intersection[i].x, intersection[i].y, \
            intersection[i].l, distance(intersection[i]));
    }

    printf("Distance of shortest path to intersection: %d\n", min);

    return 0;
}

char ** split(char buffer[])
{
    char *buffer_ = strdup(buffer);

    int n = 1;
    for (size_t i = 0; buffer_[i]; i++)
    {
        if (buffer_[i] == ',') { n++; }
    }
    
    char **tokens;
    tokens = malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) { tokens[i] = malloc(6 * sizeof(char *)); }

    char *tmp;
    tmp = strtok(buffer_, ",");
    for (size_t i = 0; tmp != NULL; i++)
    {
        tokens[i] = tmp;
        tmp = strtok(NULL, ",");
        if (tmp == NULL) { tokens[i + 1] = NULL; } else { tmp += '\0'; }
    }
    
    return tokens;
}

int uniqcoord (struct coord a[], struct coord b)
{
    for (size_t i = 0; a[i].x != 0 || a[i].y != 0; i++)
    {
        if (a[i].x == b.x && a[i].y == b.y) { return 0; }
    }

    return 1;
}

struct coord * trace(char **instruction)
{
    int n = 1;
    for (size_t i = 0; instruction[i]; i++) { n++; }
    struct coord *path;
    path = malloc((n * 2000) * sizeof(struct coord));   
    
    size_t index = 0;
    for (size_t i = 0; instruction[i]; i++)
    {
        char direction = instruction[i][0];
        int value = 0;
        for (size_t j = 0; instruction[i][j]; j++)
        {
            if (isdigit(instruction[i][j]))
            {
                value = (value * 10) + (instruction[i][j] - '0');
            }
        }

        for (size_t i = 0; i < value; i++)
        {
            struct coord current;
            struct coord new;

            if (index == 0) { current.x = 0; current.y = 0; current.l = 0; }
            else if (path[index - 1].x == 0 && path[index - 1].y == 0)
            {
                current = path[index - 1];
                index--;
            } else {
                current = path[index - 1];
            }

            switch (direction)
            {
                case 'R':
                    new.x = current.x + 1;
                    new.y = current.y;
                    new.l = current.l + 1;
                    break;

                case 'L':
                    new.x = current.x - 1;
                    new.y = current.y;
                    new.l = current.l + 1;

                    break;

                case 'U':
                    new.x = current.x;
                    new.y = current.y + 1;
                    new.l = current.l + 1;

                    break;

                case 'D':
                    new.x = current.x;
                    new.y = current.y - 1;
                    new.l = current.l + 1;

                    break;

                default:
                    break;
            }
            path[index] = new;
            index++;
        }
    }


    path[index + 1].x = 0;
    path[index + 1].y = 0;
    return path;
}

struct coord * intersect(struct coord a[], struct coord b[])
{
    struct coord *intersection;
    intersection = calloc(100000000, sizeof(struct coord));
    size_t index = 0;

    for (size_t i = 0; a[i].x != 0 || a[i].y != 0; i++)
    {
        for (size_t j = 0; b[j].x != 0 || b[j].y != 0; j++)
        {
            if (a[i].x == b[j].x && a[i].y == b[j].y)
            {
                intersection[index].x = a[i].x;
                intersection[index].y = a[i].y;
                intersection[index].l = a[i].l + b[j].l;
                index++;
            }
        }
    }
    
    return intersection;
}

int distance(struct coord a) { return abs(a.x) + abs(a.y); }
