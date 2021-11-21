#include <X11/X.h>
#include <X11/keysym.h>
#include <xcb/xcb.h>
#include <div/div.h>
#include <div/functions.h>
#include <div/image_view.h>
#include <div/x.h>
#include <img_loader/img_loader.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>


int spawn(const char* command) {
    int pid = fork();
    if(pid == 0) {
        const char* const args[] = {"/bin/sh", "-c", command, NULL};
        execv(args[0], (char* const*)args);
        exit(1);
    }
    else if(pid < 0) exit(1);
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : WIFSIGNALED(status) ? WTERMSIG(status) : -1;
}

void windowTitle() {
    static char buffer[255];
    const char* pwd = getenv("PWD");
    const char* name = image_holders->name;
    if(pwd && name && strncmp(pwd, name, strlen(pwd)) ==0)
        name += strlen(pwd) + 1;
    if(getNumActiveImages() == 1)
        snprintf(buffer, sizeof(buffer) - 1, "%s %s %d/%d", state.user_title, name, state.file_index + 1, state.num_files);
    else
        snprintf(buffer, sizeof(buffer) - 1, "%s %s %d-%d of %d", state.user_title, name, state.file_index + 1, state.file_index + 1 + getNumActiveImages() - 1, state.num_files);
    setWindowTitle(buffer);
}

void sort(int arg) {
    sortImages(state.image_context, arg);
    open_images();
    state.dirty=1;
}

extern xcb_generic_event_t* event;
void printSelectedImage() {
    xcb_button_press_event_t* bevent = (xcb_button_press_event_t*)event;
    for (int i = 0; i < getNumActiveImages(); i++) {
        if(image_holders[i].geometry.x < bevent->event_x && bevent->event_x < image_holders[i].geometry.x + image_holders[i].geometry.width )
            if(image_holders[i].geometry.y < bevent->event_y && bevent->event_y < image_holders[i].geometry.y + image_holders[i].geometry.height ) {
                printf("%s\n", image_holders[i].name);
                static char buffer[255];
                snprintf(buffer, sizeof(buffer) - 1, "printf %s | xsel ", image_holders[i].name);
                spawn(buffer);
                return;
            }
    }
}

Binding user_bindings[] = {
    {0, XK_s, sort, IMG_SORT_SIZE},
    {ShiftMask, XK_s, sort, -IMG_SORT_SIZE},
    {0, XK_n, sort, IMG_SORT_NAME},
    {ShiftMask, XK_n, sort, -IMG_SORT_NAME},
    {ShiftMask, XK_a, sort, IMG_SORT_MOD},
    {0, XK_a, sort, -IMG_SORT_MOD},
    {0, Button1, printSelectedImage, .type=-1},
    {0}
};

void customScaleFunc(const char* buf, uint32_t original_width, uint32_t original_height, char* out_buf, uint32_t width, uint32_t height, int num_channels) {
    if(getNumActiveImages() < 4) {
        stbir_resize_uint8(buf, original_width, original_height, 0, out_buf, width, height, 0, num_channels);
        return;
    }
    nearestNeighbourScale(buf, original_width,original_height,out_buf,width,height,num_channels);
}
void onStartup(){
    state.xevent_mask |= XCB_EVENT_MASK_BUTTON_PRESS ;
    events[SET_TITLE] = windowTitle;
    events[XCB_BUTTON_RELEASE] = events[XCB_KEY_PRESS];
    events[XCB_BUTTON_PRESS] = events[XCB_KEY_PRESS];
    events[XCB_KEY_RELEASE] = events[XCB_KEY_PRESS];
    scaleFunc = customScaleFunc;
}
