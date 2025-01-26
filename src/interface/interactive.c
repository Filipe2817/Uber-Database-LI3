#include <curses.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <unistd.h>
#include "../includes/interface/interactive.h"
#include "../includes/parser.h"
#include "../includes/utils.h"
#include "../includes/users-catalog.h"
#include "../includes/drivers-catalog.h"
#include "../includes/rides-catalog.h"
#include "../includes/interface/chars.h"

#define MAX_QUERY_ARGS 4

bool file_exists(const char *filename)
{
    return access(filename, F_OK) == 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(local_win);
    return local_win;
}

void print_in_window_centered(WINDOW *win, char *str, int standout)
{
    if (standout == 1)
    {
        wattron(win, A_STANDOUT);
    }
    int lines, columns;
    getmaxyx(win, lines, columns);
    mvwprintw(win, lines / 2, columns / 2 - strlen(str) / 2, str);
    wrefresh(win);
    wattroff(win, A_STANDOUT);
}

void delete_window(WINDOW *win)
{
    wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}

char *get_dataset() // starts ncurses on main
{
    initscr();
    keypad(stdscr, TRUE);

    int starty_title = 9;
    int startx_title = COLS / 2 - (strlen("Dateset folder path") * 8) / 2; // 7 is the width of each char + 1(space added after each char printed)

    print_string_big_chars(stdscr, "Dateset folder path", starty_title, startx_title);

    WINDOW *win = create_newwin(LINES / 8, COLS / 2, LINES / 2 - LINES / 16, COLS / 2 - COLS / 4);

    int input_lines = getmaxy(win);
    wmove(win, input_lines / 2, 3);

    char *path_buffer = malloc(PATH_MAX * sizeof(char));
    char *input = malloc(PATH_MAX * sizeof(char));

    wgetstr(win, input);

    char *path = realpath(input, path_buffer);

    clear();
    delete_window(win);
    free(input);

    return path;
}

void print_waiting_on_catalogs() // sets noecho()
{
    curs_set(0);
    noecho();
    print_string_big_chars(stdscr, "Building Catalogs", LINES / 2 - 4, COLS / 2 - (strlen("Building Catalogs") * 8) / 2); // 4 because the length of a big char is 7 and 4 is close to half of that, leaving it somewhat centered
}

//---------------------------------------------------------------------QUERIES------------------------------------------------------------------------------------------

void print_page_queries(WINDOW *win, int starty, int startx, int output_lines, char *str)
{
    char *ptr = str;
    char *line = malloc(100 * sizeof(char));

    while (starty < 0)
    {
        if (ptr[0] == '\n')
            starty++;
        ptr++;
    }

    while (sscanf(ptr, "%[^\n]\n", line) == 1 && starty <= output_lines)
    {
        mvwaddstr(win, starty, startx, line);
        wrefresh(win);
        starty++;
        ptr += strlen(line) + 1;
    }
    free(line);
}

void display_query_results(WINDOW *border_win, char *input, ...)
{
    noecho();
    va_list args;
    va_start(args, input);

    int border_lines, border_cols;
    getmaxyx(border_win, border_lines, border_cols);

    WINDOW *win = newwin(border_lines - 2, border_cols - 2, LINES / 4 + 1, COLS / 4 + 1); // border starting position + 1

    int output_lines = getmaxy(win);

    Output_Type output_struct = parse_query_interactive(input, MAX_QUERY_ARGS, args);

    setlocale(LC_ALL, "");

    int lines = 0;
    for (int i = 0; i < (int)strlen(output_struct->Type.str); i++)
    {
        if (output_struct->Type.str[i] == '\n')
            lines++;
    }
    int n_pages = ceil((double)lines / output_lines);

    print_page_queries(win, 0, 0, output_lines, output_struct->Type.str);
    mvprintw(LINES / 4 - 1, COLS / 4, "1 of %d", n_pages);
    mvprintw(LINES / 4 - 1, 3 * COLS / 4 - strlen("Use the arrow keys"), "Use the arrow keys");

    int y_setter = 0; // this multiplies by (-output_lines) in order to have the starting y of the string placed in a way that the window and the correct chunk of the string match each other
    int cycle_flag = 1;
    while (cycle_flag)
    {
        int choice = getch();
        switch (choice)
        {
        case KEY_RIGHT:
            y_setter++;
            if (y_setter >= n_pages - 1)
            {
                y_setter = n_pages - 1;
            }
            wclear(win);
            print_page_queries(win, (-output_lines) * y_setter, 0, output_lines, output_struct->Type.str);

            move(LINES / 4 - 1, COLS / 4);
            clrtoeol();
            mvprintw(LINES / 4 - 1, COLS / 4, "%d of %d", y_setter + 1, n_pages);
            mvprintw(LINES / 4 - 1, 3 * COLS / 4 - strlen("Use the arrow keys"), "Use the arrow keys");
            refresh();
            break;
        case KEY_LEFT:
            y_setter--;
            if (y_setter <= 0)
            {
                y_setter = 0;
            }
            wclear(win);
            print_page_queries(win, (-output_lines) * y_setter, 0, output_lines, output_struct->Type.str);

            move(LINES / 4 - 1, COLS / 4);
            clrtoeol();
            mvprintw(LINES / 4 - 1, COLS / 4, "%d of %d", y_setter + 1, n_pages);
            mvprintw(LINES / 4 - 1, 3 * COLS / 4 - strlen("Use the arrow keys"), "Use the arrow keys");
            refresh();
            break;
        case 10:
            move(LINES / 4 - 1, COLS / 4);
            clrtoeol();
            wclear(win);
            wrefresh(win);
            refresh();
            cycle_flag = 0;
            break;
        }
    }
    va_end(args);
    free(output_struct->Type.str);
    free(output_struct);
}

void launch_queries_interface(Users_Catalog users_catalog, Drivers_Catalog drivers_catalog, Rides_Catalog rides_catalog)
{
    clear();
    print_string_big_chars(stdscr, "Queries", 1, COLS / 2 - (strlen("Queries") * 8) / 2);

    mvprintw(9, COLS / 2 - strlen("Type 'exit' to quit.") / 2, "Type 'exit' to quit.");
    move(LINES - LINES / 16, COLS / 4);
    refresh();

    WINDOW *input_window = newwin(LINES / 16, COLS / 2, LINES - LINES / 16, COLS / 4);
    WINDOW *output_window = create_newwin(LINES / 1.5, COLS / 2, LINES / 4, COLS / 4); // dividing by 1.5 and 2 so that it's a rectangle which takes up most of the window and allows for some limited freedom in resolution, dividing by four so that it's centered (starty and startx are the top left corner)

    int query_number;
    char remaining_input;
    char *str = malloc(100 * sizeof(char));
    do
    {
        echo();
        curs_set(1);
        wgetstr(input_window, str);
        curs_set(0);

        if (sscanf(str, "%d %c", &query_number, &remaining_input) == 2)
        {
            display_query_results(output_window, str, users_catalog, drivers_catalog, rides_catalog);
        }

        wclear(input_window);
    } while (strcmp(str, "exit"));

    delwin(input_window);
    delete_window(output_window);
    clear();
    free(str);
}

//---------------------------------------------------------------------DATASETS-----------------------------------------------------------------------------------

void print_page_dataset(WINDOW *win, int starty, int startx, int output_lines, char *path)
{
    FILE *fp = fopen(path, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    nread = getline(&line, &len, fp);

    while ((nread = getline(&line, &len, fp)) != -1 && starty <= output_lines)
    {
        if (starty >= 0)
        {
            mvwaddstr(win, starty, startx, line);
            wrefresh(win);
        }
        starty++;
    }

    free(line);
    fclose(fp);
}

void print_dataset(char *path)
{
    clear();
    refresh();

    WINDOW *border_win = create_newwin(LINES - LINES / 12, COLS - COLS / 12, LINES / 24, COLS / 24);

    int border_lines, border_cols;
    getmaxyx(border_win, border_lines, border_cols);

    WINDOW *output_window = newwin(border_lines - 2, border_cols - 2, LINES / 24 + 1, COLS / 24 + 1);

    FILE *fp = fopen(path, "r");

    char *field_names = NULL;
    size_t len = 0;
    ssize_t nread;
    nread = getline(&field_names, &len, fp);

    if (nread == -1)
        return;

    field_names[strcspn(field_names, "\n")] = '\0';

    int output_lines = getmaxy(output_window);
    int lines = 0;
    for (char c = getc(fp); c != EOF; c = getc(fp))
    {
        if (c == '\n')
            lines++;
    }

    int n_pages = ceil((double)lines / output_lines);

    fclose(fp);

    setlocale(LC_ALL, "");

    print_page_dataset(output_window, 0, 0, output_lines, path);
    mvprintw(LINES / 24 - 1, COLS / 24, "1 of %d", n_pages);
    mvprintw(LINES / 24 - 1, COLS - COLS / 24 - strlen("Use the arrow keys"), "Use the arrow keys");
    mvprintw(LINES / 24 - 1, COLS / 2 - strlen(field_names) / 2, field_names);

    int y_setter = 0;
    int cycle_flag = 1;
    while (cycle_flag)
    {
        int choice = getch();
        switch (choice)
        {
        case KEY_RIGHT:
            y_setter++;
            if (y_setter >= n_pages - 1)
            {
                y_setter = n_pages - 1;
            }
            wclear(output_window);
            print_page_dataset(output_window, (-output_lines) * y_setter, 0, output_lines, path);

            move(LINES / 24 - 1, COLS / 24);
            clrtoeol();
            mvprintw(LINES / 24 - 1, COLS / 24, "%d of %d", y_setter + 1, n_pages);
            mvprintw(LINES / 24 - 1, COLS - COLS / 24 - strlen("Use the arrow keys"), "Use the arrow keys");
            mvprintw(LINES / 24 - 1, COLS / 2 - strlen(field_names) / 2, field_names);
            refresh();
            break;
        case KEY_LEFT:
            y_setter--;
            if (y_setter <= 0)
            {
                y_setter = 0;
            }
            wclear(output_window);
            print_page_dataset(output_window, (-output_lines) * y_setter, 0, output_lines, path);

            move(LINES / 24 - 1, COLS / 24);
            clrtoeol();
            mvprintw(LINES / 24 - 1, COLS / 24, "%d of %d", y_setter + 1, n_pages);
            mvprintw(LINES / 24 - 1, COLS - COLS / 24 - strlen("Use the arrow keys"), "Use the arrow keys");
            mvprintw(LINES / 24 - 1, COLS / 2 - strlen(field_names) / 2, field_names);
            refresh();
            break;
        case 10:
            move(LINES / 24 - 1, COLS / 24);
            clrtoeol();
            wclear(output_window);
            wrefresh(output_window);
            refresh();
            cycle_flag = 0;
            break;
        }
    }

    clear();
    free(field_names);
}

void launch_datasets_interface(char *users_path, char *drivers_path, char *rides_path)
{
    clear();

    char *curr_path = NULL;
    int highlight = 0;
    int cycle_flag = 1;
    while (cycle_flag)
    {

        print_string_big_chars(stdscr, "Choose a group", 1, COLS / 2 - (strlen("Choose a group") * 8) / 2);

        WINDOW *drivers_option = create_newwin(LINES / 8, COLS / 4, LINES / 2 - LINES / 8, COLS / 2 - COLS / 8);
        WINDOW *users_option = create_newwin(LINES / 8, COLS / 4, LINES / 2, COLS / 2 - COLS / 8);
        WINDOW *rides_option = create_newwin(LINES / 8, COLS / 4, LINES / 2 + LINES / 8, COLS / 2 - COLS / 8);
        WINDOW *quit_option = create_newwin(LINES / 8, COLS / 4, LINES / 2 + 2 * LINES / 8, COLS / 2 - COLS / 8);

        print_in_window_centered(drivers_option, "Drivers", 0);
        print_in_window_centered(users_option, "Users", 0);
        print_in_window_centered(rides_option, "Rides", 0);
        print_in_window_centered(quit_option, "Quit", 0);

        if (highlight == 0)
        {
            print_in_window_centered(drivers_option, "Drivers", 1);
            curr_path = drivers_path;
        }
        else if (highlight == 1)
        {
            print_in_window_centered(users_option, "Users", 1);
            curr_path = users_path;
        }
        else if (highlight == 2)
        {
            print_in_window_centered(rides_option, "Rides", 1);
            curr_path = rides_path;
        }
        if (highlight == 3)
        {
            print_in_window_centered(quit_option, "Quit", 1);
        }

        int choice = getch();

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight <= 0)
            {
                highlight = 0;
            };
            delete_window(drivers_option);
            delete_window(users_option);
            delete_window(rides_option);
            delete_window(quit_option);
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight >= 3)
            {
                highlight = 3;
            };
            delete_window(drivers_option);
            delete_window(users_option);
            delete_window(rides_option);
            delete_window(quit_option);
            break;
        case 10:
            if (highlight == 3)
                cycle_flag = 0;
            else
            {
                print_dataset(curr_path);
            }
            delete_window(drivers_option);
            delete_window(users_option);
            delete_window(rides_option);
            delete_window(quit_option);
            break;
        }
    };

    clear();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void launch_main_interface(int n_args, ...)
{
    clear();
    keypad(stdscr, true);

    va_list args;
    va_start(args, n_args);

    Users_Catalog users_catalog = va_arg(args, Users_Catalog);
    Drivers_Catalog drivers_catalog = va_arg(args, Drivers_Catalog);
    Rides_Catalog rides_catalog = va_arg(args, Rides_Catalog);
    char *users_file = va_arg(args, char *);
    char *drivers_file = va_arg(args, char *);
    char *rides_file = va_arg(args, char *);

    int highlight = 0;
    int cycle_flag = 1;
    while (cycle_flag)
    {

        print_string_big_chars(stdscr, "Uber Database", 1, COLS / 2 - (strlen("Uber Database") * 8) / 2);
        print_string_big_chars(stdscr, "LI3", LINES - 9, COLS / 2 - (strlen("LI3") * 8) / 2);

        WINDOW *queries_option = create_newwin(LINES / 8, COLS / 4, LINES / 2 - LINES / 8, COLS / 2 - COLS / 8); // using the macros instead of simple numbers in order to allow some limited resizing
        WINDOW *datasets_option = create_newwin(LINES / 8, COLS / 4, LINES / 2, COLS / 2 - COLS / 8);
        WINDOW *quit_option = create_newwin(LINES / 8, COLS / 4, LINES / 2 + LINES / 8, COLS / 2 - COLS / 8);

        print_in_window_centered(queries_option, "Execute Queries", 0);
        print_in_window_centered(datasets_option, "Print Datasets", 0);
        print_in_window_centered(quit_option, "Quit", 0);

        refresh();

        if (highlight == 0)
        {
            print_in_window_centered(queries_option, "Execute Queries", 1);
            refresh();
        }
        else if (highlight == 1)
        {
            print_in_window_centered(datasets_option, "Print Datasets", 1);
            refresh();
        }
        else if (highlight == 2)
        {
            print_in_window_centered(quit_option, "Quit", 1);
            refresh();
        }

        int choice = getch();

        switch (choice)
        {
        case KEY_UP:
            highlight--;
            if (highlight <= 0)
            {
                highlight = 0;
            };
            delete_window(queries_option);
            delete_window(datasets_option);
            delete_window(quit_option);
            break;
        case KEY_DOWN:
            highlight++;
            if (highlight >= 2)
            {
                highlight = 2;
            };
            delete_window(queries_option);
            delete_window(datasets_option);
            delete_window(quit_option);
            break;
        case 10:
            if (highlight == 0)
            {
                launch_queries_interface(users_catalog, drivers_catalog, rides_catalog);
            }
            else if (highlight == 1)
            {
                launch_datasets_interface(users_file, drivers_file, rides_file);
            }
            else if (highlight == 2)
            {
                cycle_flag = 0;
            }
            break;
        }
    }

    va_end(args);
    endwin();
}