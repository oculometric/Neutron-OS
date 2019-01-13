#include "textgui.h"
#include "log.h"

CLILabel::CLILabel (char *c, int xx, int yy) {
	x = xx;
	y = yy;

	width = strlen (c);
	height = 1;

	text = c;
	return;
}
int CLILabel::getX () {return x;}
int CLILabel::getY () {return y;}
int CLILabel::getWidth () {return width;}
int CLILabel::getHeight () {return height;}
char * CLILabel::getText () {return text;}
