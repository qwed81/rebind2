#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <X11/Xlib.h>
#include <string.h>
#include "keymap.h"

#define LOG 0

KeyMap lookup_table[STATE_LEN][KEY_LEN] = { 0 };
static int (*real_XNextEvent)(Display* display, XEvent* event_return) = NULL;

static void init(void) {
    real_XNextEvent = dlsym(RTLD_NEXT, "XNextEvent");
    if (real_XNextEvent == NULL) {
        printf("Error in `dlsym`: %s\n", dlerror());
    }

	char* mapping_path = getenv("MAPPINGS");
	if (mapping_path == NULL) {
		return;
	}

	FILE* file_ptr = fopen(mapping_path, "r");  
	if (file_ptr == NULL) {
		return;
	}

	fseek(file_ptr, 0, SEEK_END);          
	long file_len = ftell(file_ptr);             

	// if it is not eq to the size of the lookup table
	// then it should end up just returning as it is not the correct format
	if (file_len != sizeof(lookup_table)) {
		return;
	}
	rewind(file_ptr);                      

	// read from the file into the lookup table
	fread(&lookup_table, file_len, 1, file_ptr); 
	fclose(file_ptr); 
}

int XNextEvent(Display* display, XEvent* event_return) {
    if(real_XNextEvent == NULL) {
        init();
    }

	// call the actual function
    int result = real_XNextEvent(display, event_return);

	// if it is not one of these events, it does not need to be processed
	if (event_return->type != KeyPress && event_return->type != KeyRelease) {
		return result;
	}

	// process the return value and change if it needs to be
	XKeyEvent* ev = (XKeyEvent*)event_return;

	// it is out of bounds of our array, return as if nothing
	// happened
	if (ev->state >= STATE_LEN || ev->keycode >= KEY_LEN) {
		return result;
	}

	unsigned int old_key = ev->keycode;
	unsigned int old_state = ev->state;

	KeyMap mapping = lookup_table[ev->state][ev->keycode];
	// it is not mapped so don't do anything
	if (mapping.mapped == false) {
		return result;
	}

	ev->keycode = mapping.to_keycode;
	ev->state = mapping.to_state;

#if LOG
	if (event_return->type == KeyPress) {
		printf("pressed key: %d state: %d\n", old_key, old_state);
	}

	if (event_return->type == KeyRelease) {
		printf("released key: %d state: %d\n", old_key, old_state);
	}

	printf("sent key: %d state: %d\n", ev->keycode, ev->state);
#endif

	return result;
}
