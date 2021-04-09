#include <navboard/navboard.h>
Key keys_colors[] = {
    {.label="R", .foreground = 0xFF0000},
    {.label="G", .foreground = 0x00FF00},
    {.label="B", .foreground = 0x0000FF},
    {0},
    {.label="RR", .background = { [0] = 0xFF0000}},
    {.label="GR", .background = { [0] = 0x00FF00}},
    {.label="BR", .background = { [0] = 0x0000FF}},
    {0},
    {.label="RG", .background = { [1] = 0xFF0000}},
    {.label="GG", .background = { [1] = 0x00FF00}},
    {.label="BG", .background = { [1] = 0x0000FF}},
};
REGISTER(colors, keys_colors);
