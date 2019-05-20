#ifndef MAIN_H
#define MAIN_H

struct Input_interface {
	int a;
	int b;
	float c;
	int enable;
};

struct Output_interface {
	int a;
	int b;
	float c;
};

void printff(struct Input_interface *input, struct Output_interface *output);

#endif // MAIN_H
