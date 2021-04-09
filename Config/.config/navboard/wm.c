#include <navboard/navboard.h>
Key keys_wm[] = {
    {XK_Hyper_L}, {XK_q}, {XK_w}, {XK_e},
    {0},
    {XK_Shift_L}, {XK_a}, {XK_s}, {XK_d},
    {0},
    {XK_h}, {XK_j}, {XK_k}, {XK_l},
    {0},
    {XK_Control_L}, {XK_Super_L}, {XK_space}, {XK_Alt_L}
};
REGISTER(wm, keys_wm);
