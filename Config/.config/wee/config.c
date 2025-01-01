#include "buffer.h"
#include "context.h"
#include "defaults.h"
#include "events.h"
#include "functions.h"


static KeyBinding defaultBindings[] = {
    {MODE_INSERT, .str = "jj", .func = setMode,         .arg = {.i = MODE_NORMAL}},
};

STATIC_BUFFER(defaultBindingsBuffer, defaultBindings);

int onStartup(Context* context) {
    addDefaults(context);
    addStateBuffer(context, BINDING_INDEX, &defaultBindingsBuffer);
    Settings* settings = getGlobalSettings(context);
    setEnabled(settings, statusbarstr_index, 1);
    setEnabled(settings, dock_linenumber_index, 1);
    setEnabled(settings, dock_statusbar_index, 1);
    setStringSettings(settings, linenumberf_index, "%03d ", 0);
    return 0;
}
