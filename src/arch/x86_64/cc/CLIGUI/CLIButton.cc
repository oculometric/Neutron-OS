#include "textgui.h"
#include "../log.h"

CLIButton::CLIButton (char *c, int xx, int yy, CLIGUI *cc) {
	x = xx;
	y = yy;

	width = strlen (c);
	height = 1;

	text = c;

	container = cc;
	return;
}

void CLIButton::callFunction () {
	if (isInstanceFunc) {
		((*container).*memFunction)();
	} else function();
}
void CLIButton::setFunction (void (*f)(void)) {
	function = f;
	isInstanceFunc = false;
}

void CLIButton::setFunction (void (CLIGUI::*f)(void)) {
	memFunction = f;
	isInstanceFunc = true;
}

int CLIButton::getX () {return x;}
int CLIButton::getY () {return y;}
int CLIButton::getWidth () {return width;}
int CLIButton::getHeight () {return height;}
char * CLIButton::getText () {return text;}
