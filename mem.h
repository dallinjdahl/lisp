enum op {add, sub, mult, divd};
enum disc {num, op, list};

struct cell {
	enum disc disc;
	union {
		uint32_t num;
		enum op op;
		struct {
			struct cell *car;
			struct cell *cdr;
		} l;
	} u;
};

void meminit();
void memfree(struct cell *x);
struct cell *memget();
void printcell(struct cell *x);
void printop(enum op o);

struct cell disk[2048];
uint16_t end = 1;

void meminit() {
	disk[0].u.l.car = disk;
}

void memfree(struct cell *x) {
	if(x == disk) {
		fputs("error: freeing block 0\n", stderr);
		exit(1);
	}
	x->u.l.car = disk[0].u.l.car;
	disk[0].u.l.car = x;
}

struct cell *memget() {
	if(disk->u.l.car == disk) {
		return disk+(end++);
	}
	struct cell *res = disk[0].u.l.car;
	disk[0].u.l.car = res->u.l.car;
	return res;
}

void printcell(struct cell *x) {
	if(x == disk) {
		fputs("nil", stdout);
		return;
	}
	switch (x->disc) {
	case num:
		printf("%d", x->u.num);
		return;
	case op:
		printop(x->u.op);
		return;
	case list:
		fputs("(", stdout);
		printcell(x->u.l.car);
		x = x->u.l.cdr;
		while(x != disk) {
			fputs(" ", stdout);
			printcell(x->u.l.car);
			x = x->u.l.cdr;
		}
		fputs(")", stdout);
	}
}

void printop(enum op o) {
	switch(o) {
	case add:
		fputs("+",stdout);
		return;
	case sub:
		fputs("-",stdout);
		return;
	case mult:
		fputs("*",stdout);
		return;
	case divd:
		fputs("/",stdout);
		return;
	}
}

