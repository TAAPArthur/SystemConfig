#include <X11/X.h>
#include <X11/XF86keysym.h>
#include <X11/keysym.h>
#include <div/div.h>
#include <div/functions.h>
#include <div/image_view.h>
#include <div/x.h>
#include <img_loader/img_loader.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <xcb/xcb.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>


void windowTitle() {
    static char buffer[255];
    const char* pwd = getenv("PWD");
    const char* home = getenv("HOME");
    const char* prefix = "";
    const char* name = image_holders->name;
    if(pwd && name && strncmp(pwd, name, strlen(pwd)) ==0)
        name += strlen(pwd) + 1;
    if(home && name && strncmp(home, name, strlen(home)) ==0) {
        name += strlen(home);
        prefix = "~";
    }
    if(getNumActiveImages() == 1)
        snprintf(buffer, sizeof(buffer) - 1, "%s %s%s %d/%d", state.user_title, prefix, name, state.file_index + 1, state.num_files);
    else
        snprintf(buffer, sizeof(buffer) - 1, "%s %s%s %d-%d of %d", state.user_title, prefix, name, state.file_index + 1, state.file_index + 1 + getNumActiveImages() - 1, state.num_files);
    setWindowTitle(buffer);
}

void sort(int arg) {
    image_loader_sort(state.image_context, arg);
    open_images();
}

extern xcb_generic_event_t* event;
/**
 * Print and copy the selected image's name
 */
void printSelectedImage() {
    static char buffer[] = "printf %s \"$FILENAME\" | xsel -b";
    xcb_button_press_event_t* bevent = (xcb_button_press_event_t*)event;
    for (int i = 0; i < getNumActiveImages(); i++) {
        if(image_holders[i].geometry.x < bevent->event_x && bevent->event_x < image_holders[i].geometry.x + image_holders[i].geometry.width )
            if(image_holders[i].geometry.y < bevent->event_y && bevent->event_y < image_holders[i].geometry.y + image_holders[i].geometry.height ) {
                printf("%s\n", image_holders[i].name);
                setenv("FILE_NAME", image_holders[i].name, 1);
                system(buffer);
                return;
            }
    }
}

/**
 * Goto the next page depending in the click happened on the right/left edge. The exact translation depends on
 * state.right_to_left.
 * For example, clicking on the left edge, when reading right-to-left, would progress to the next page but if reading
 * left-to-right, it would go back to the previous page
 */
void buttonNavigate() {
    xcb_button_press_event_t* bevent = (xcb_button_press_event_t*)event;
    int index = state.file_index;
    if(bevent->event_x < state.win_width / 3 )
        next_page(state.right_to_left ? 1 : -1);
    if(bevent->event_x > state.win_width * 2 / 3)
        next_page(!state.right_to_left ? 1 : -1);
    if(state.file_index && index == state.file_index) {
        exit(0);
    }
}

Binding user_bindings[] = {
    // Navigate with volume buttons; useful on mobile
    {0, XF86XK_AudioRaiseVolume, next_image, +1},
    {0, XF86XK_AudioLowerVolume, next_image, -1},

    // Basic sorting on file properties
    {0, XK_s, sort, IMG_SORT_SIZE},
    {ShiftMask, XK_s, sort, -IMG_SORT_SIZE},
    {0, XK_n, sort, IMG_SORT_NAME},
    {ShiftMask, XK_n, sort, -IMG_SORT_NAME},
    {ShiftMask, XK_a, sort, IMG_SORT_MOD},
    {0, XK_a, sort, -IMG_SORT_MOD},

    {0, Button1, printSelectedImage, .type=XCB_BUTTON_PRESS},
    {0, Button1, buttonNavigate, .type=XCB_BUTTON_RELEASE},
    {0}
};

void customScaleFunc(const char* buf, uint32_t original_width, uint32_t original_height, char* out_buf, uint32_t width, uint32_t height, int num_channels) {
    // Use a higher quality scale function when viewing few images
    if(getNumActiveImages() < 4) {
        stbir_resize_uint8(buf, original_width, original_height, 0, out_buf, width, height, 0, num_channels);
        return;
    }
    nearestNeighbourScale(buf, original_width,original_height,out_buf,width,height,num_channels);
}

void customOpenImages() {
    static char largePage;
    if(largePage) {
        state.cols = 2;
        largePage = false;
    }
    defaultOpenImages();
    // If we are in a dual-page view and see a relatively large image, assume that image is dual image
    // let it take up the entire window
    if(getNumActiveImages() == 2) {
        for (int i = 0; i < getNumActiveImages(); i++) {
            if(image_holders[i].image_data && image_holders[i].image_width >= 1024 && image_holders[i].image_width > image_holders[i].image_height) {
                largePage = true;
                state.cols = 1;
            }
        }
    }
}

void onStartup() {
    state.xevent_mask |= XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE;
    events[SET_TITLE] = windowTitle;
    events[XCB_BUTTON_RELEASE] = events[XCB_KEY_PRESS];
    events[XCB_BUTTON_PRESS] = events[XCB_KEY_PRESS];
    events[XCB_KEY_RELEASE] = events[XCB_KEY_PRESS];
    events[OPEN_IMAGES] = customOpenImages,
    scaleFunc = customScaleFunc;
}
