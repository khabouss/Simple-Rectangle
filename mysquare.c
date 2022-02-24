#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
# define success 0
# define fail 1
# define ERROR_MSG "Error\n"
# define ERROR_LEN 6

int draw(FILE *file)
{
    int i, x, y, bg_width, bg_height;
    char *image, bg_char, rect_char, rect_type;
    float rect_x, rect_y, rect_width, rect_height;
    //         -----       ------       ---------

    if (fscanf(file, "%d %d %c\n", &bg_width, &bg_height, &bg_char) != 3)
        return (fail);
    if ((image = malloc(bg_width * bg_height + 1)) == NULL)
        return (fail);
    if (bg_width <= 0 || bg_height <= 0 || bg_width > 300 || bg_height > 300)
        return (fail);
    i = -1;
    while (++i < bg_width * bg_height)
        image[i] = bg_char;
    image[i] = '\0';

    while (fscanf(file, "%c %f %f %f %f %c\n", &rect_type, &rect_x, &rect_y, &rect_width, &rect_height, &rect_char) == 6)
    {
        int type_condition = rect_type == 'r' || rect_type == 'R';
        int dimentions_condition = rect_width > 0 && rect_height > 0;
        if (type_condition && dimentions_condition)
        {
            y = -1;
            while (++y < bg_height)
            {
                x = -1;
                while (++x < bg_width)
                {
                    int range_condition = (x >= rect_x && x <= rect_x + rect_width && y >= rect_y && y <= rect_y +rect_height);
                    int border_condition = (rect_type == 'r') && (x - rect_x < 1 || rect_x + rect_width - x < 1 || y - rect_y < 1 || rect_y +rect_height - y < 1);
                    int fill_condition = (rect_type == 'R');
                    if (range_condition && (border_condition || fill_condition))
                        image[y * bg_width + x] = rect_char;
                }
            }
        }
    }

    i = -1;
    while (++i < bg_height)
    {
        write (STDOUT_FILENO, &image[i * bg_width], bg_width);
        write (STDOUT_FILENO, "\n", 1);
    }
    fclose(file);
    free(image);
    return (success);
}

int main(int argc, char **argv)
{
    FILE *file;
    if (argc == 2)
    {
        file = fopen(argv[1], "r");
        if (file == NULL)
            return (fail);
        if (draw(file) == fail)
            return (fail);
        return (success);
    }
    write (STDOUT_FILENO, ERROR_MSG, ERROR_LEN);
    return (fail);
}