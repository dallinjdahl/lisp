void parse(struct cell **loc);
uint8_t parselist(struct cell *loc);
enum op parseop();
uint8_t getnum(int16_t *n);
void eatspace();
uint8_t instr(char c, char *s);

static char input[2048] = {0};
uint16_t cursor = 0;

void parse(struct cell **loc) {
	int16_t i = 0;
	eatspace();
	if(getnum(&i)) {
		(*loc)->disc = num;
		(*loc)->u.num = i;
		return;
	}
	if(input[cursor] == '(') {
		cursor++;
		if(parselist(*loc)) {
			memfree(*loc);
			*loc = disk;
		}
		return;
	}
	(*loc)->disc = op;
	(*loc)->u.op = parseop();
	return;
}

uint8_t parselist(struct cell *loc) {
	eatspace();
	if(input[cursor] == ')') {
		cursor++;
		return 1;
	}
	loc->disc = list;
	struct cell *car = memget();
	loc->u.l.car = car;
	parse(&car);
	struct cell *cdr = memget();
	loc->u.l.cdr = cdr;
	if(parselist(cdr)) {
		memfree(cdr);
		loc->u.l.cdr = disk;
	}
	return 0;
}

enum op parseop() {
	switch(input[cursor++]) {
	case '+':
		return add;
	case '-':
	 	return sub;
	case '*':
		return mult;
	case '/':
		return divd;
	}
	fputs("Expected op:\n", stderr);
	fputs(input, stderr);
	for(uint8_t i = 0; i < cursor; i++) {
		fputs(" ", stderr);
	}
	fputs("^\n",stderr);
	exit(1);
}

uint8_t getnum(int16_t *n) {
	int8_t mult = 1;
	uint16_t oc = cursor;
	if(input[cursor] == '-') {
		mult = -1;
		cursor++;
	}
	if('0' > input[cursor] || input[cursor] > '9') {
		cursor = oc;
		return 0;
	}
	while('0' <= input[cursor] && input[cursor] <= '9') {
		*n *= 10;
		*n += input[cursor] - '0';
		cursor++;
	}

	if(instr(input[cursor], " ()\n")){
		*n *= mult;
		return 1;
	}

	cursor = oc;
	return 0;
}

void eatspace() {
	while(cursor < 2047 && input[cursor] == ' ') {
		cursor++;
	}
}

uint8_t instr(char c, char *s) {
	while(*s && c != *s) {
		s++;
	}
	return *s;
}

