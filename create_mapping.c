#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keymap.h"

int main(int argc, char** argv) {
	if (argc < 3) {
		printf("usage - create_mapping [input path] [output path]\n");
		return -1;
	}

	FILE* input = fopen(argv[1], "rb");
	if (input == NULL) {
		printf("could not open input file\n");
		return - 1;
	}

	FILE* output = fopen(argv[2], "w+");
	if (output == NULL) {
		printf("could not open output file\n");
		return - 1;
	}

	char line_buf[400];
	KeyMap lookup_table[STATE_LEN][KEY_LEN] = { 0 };
	int line_counter = 0;
	while (true) {
		line_counter += 1;
		char* output = fgets(line_buf, sizeof(line_buf), input);
		if (output == NULL) {
			break;
		}

		// ignore lines that start with #
		if (output[0] == '#' || output[0] == '\n') {
			continue;
		}

		char* token = strtok(line_buf, " ");

		// read in the next base 16 input
		long int results[4] = { 0 };
		char* used[4] = { 0 };
		for (int i = 0; i < 4; i += 1) {
			if (token == NULL) {
				printf("error on line %d, expected 4 numbers\n", line_counter);
				return -1;
			}

			results[i] = strtol(token, &used[i], 10);
			token = strtok(NULL, " ");
		}

		for (int i = 0; i < 4; i += 1) {
			// if the value is not a valid integer
			if (results[i] == 0 && used[i] == NULL) {
				printf("error on line %d, expected 4 numbers\n", line_counter);
				return -1;
			}
		}

		if (results[1] >= STATE_LEN || results[0] >= KEY_LEN) {
			printf("invalid range on line %d", line_counter);
			return -1;
		}

		KeyMap mapping = {
			.mapped = true,
			.to_keycode = results[2],
			.to_state = results[3]
		};

		lookup_table[results[1]][results[0]] = mapping;
		mapping = lookup_table[results[1]][results[0]];  

		printf("on line %d added mapping: %d %d to %d %d\n", line_counter,
			results[0], results[1], mapping.to_keycode, mapping.to_state);
	}

	fclose(input);

	// write out the full lookup table to a pointer
	fwrite(&lookup_table, sizeof(lookup_table), 1, output);
	fclose(output);

	return 0;
}
