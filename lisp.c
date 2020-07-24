#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "mem.h"
#include "parse.h"

int main(int argc, char **argv) {
	puts("Lisp 0.0.1");
	puts("Press ctrl+c to exit");

	meminit();
	while(1) {
		fputs("\nl> ", stdout);
		fgets(input, 2048, stdin);
		cursor = 0;
		struct cell *head = memget();
		parse(&head);
		printcell(head);
	}
	return 0;
}
