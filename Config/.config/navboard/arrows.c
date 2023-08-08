#include <navboard/navboard.h>
Key keys_arrows[] = {
    {XK_Control_L}, {XK_Alt_L}, {XK_Shift_L},
    {0},
    {XK_Page_Up}, {XK_Up}, {XK_Page_Down},
    {0},
    {XK_Left}, {XK_Down}, {XK_Right},
    {0},
    {XK_space},
};
REGISTER(arrows, keys_arrows);
