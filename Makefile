all:
	gcc -lX11 -O3 -shared -fpic -o rebind_preload.so rebind_preload.c
	gcc -o create_mapping create_mapping.c
