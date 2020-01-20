#include <stdio.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <unistd.h> // for usleep

#define RED_CHAR "\e[31m%c\e[0m"
#define GREEN_CHAR "\e[32m%c\e[0m"
#define MOVE_CURSOR_UP printf("\033[A")

char *tree[255];
int tree_size = 0;
bool is_light = false;
bool is_random = false;

void load_tree()
{
	std::ifstream file("tree.txt");
	char buf[255];
	while (! file.eof()){
		tree[tree_size] = new char[255];
		file.getline(tree[tree_size++], 255);
	}
	file.close();
}

void print_tree()
{
	for (int i = 0; i < tree_size; ++i) {
		int j = 0;
		char c;
		while (c = tree[i][j++]) {
			switch (c) {
				case '*': printf(GREEN_CHAR, c); break;

				case 'o': printf((is_random ? (rand() % 2) : is_light) ? RED_CHAR : "%c", c); break;

				default: printf("%c", c);
			}
		}

		printf("                 \n");
	}

	printf("\r"); // goto first line in row
	int t = tree_size;
	while (t--) {
		MOVE_CURSOR_UP; // move cursor up
	}
}

void listen_switch_mode()
{
	std::thread t1([] () {
		while (true) {
			if (std::cin.get() == '\n') {
				MOVE_CURSOR_UP;
			}
			is_random = ! is_random;
		}	
	});

	t1.detach();
}

int main(int argc, char const *argv[])
{
	load_tree();

	printf("Press Enter to change mode.\n");
	listen_switch_mode();

	while (true) {
		print_tree();
		is_light = ! is_light;
		usleep(500 * 1000);
	}
	

	return 0;
}