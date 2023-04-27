typedef struct KeyMap {
	bool mapped;
	unsigned int to_keycode;
	unsigned int to_state;
} KeyMap;

#define STATE_LEN 16
#define KEY_LEN 256
