#include "textgui.h"
#include "../log.h"
#include "../memory.h"

void CLIGUI::prepareMenu() {
	for (int i = 0; i < numButtons; i++) {
		delete buttons[i];
	}

	numButtons = 0;

	logLn ("Making GUI");

	char *d = new char[80];
	for (int c = 0; c < 80; c++) {
		d[c] = ' ';
		if (c == 0 || c == 79 || c == 17) {
			d[c] = '|';
		}
	}

	for (int r = 0; r < Terminal::VGA_HEIGHT; r++) {
		t->setChars(0, r, d, t->make_color(COLOR_WHITE, COLOR_BLACK));
	}
	currentlyOverlyedStyleFlag = t->make_color(COLOR_WHITE, COLOR_BLACK);

	CLIButton *informationBut = new CLIButton ("Information    >", 1, 0, this);
	CLIButton *toolsBut =       new CLIButton ("Tools          >", 1, 1, this);
	CLIButton *filesystemBut =  new CLIButton ("Filesystem     >", 1, 2, this);
	CLIButton *shutdownBut =    new CLIButton ("Shutdown       >", 1, 3, this);

	informationBut->setFunction(&CLIGUI::showInformation);
	addButton(informationBut);
	addButton(toolsBut);
	addButton(filesystemBut);
	addButton(shutdownBut);

	CLILabel *lineA =            new CLILabel ("----------------", 1, 4);
	CLILabel *lineB =            new CLILabel ("Recent Files    ", 1, 5);
	CLILabel *lineC =            new CLILabel ("----------------", 1, 6);

	addLabel(lineA);
	addLabel(lineB);
	addLabel(lineC);
}

void CLIGUI::showInformation () {
	int firstCol = 18;
	char *information =
 R"(======== Neutron OS ========

		Neutron OS is a toy operating system created by Jake Costen
		(aka JkyProgrammer).
		See below for a usage manual. The menu (on the left)
		contains several items:
		* Information - displays this info page.
		* Tools - TODO (will show a collection of built-in tools).
		* Filesystem - TODO (will show a filesystem explorer, when
			there is a filesystem).
		* Shutdown - unsurprisingly, shuts the system down.

		Help also TODO

		Copyright (C) JkyProgrammer 2019
	)";

	char *link = information;
	int len = strlen (information);
	// while (*link != '\0') {
	// 	int leng = 79-firstCol;
	// 	while (*(link+len) == '\0') {
	// 		len--;
	// 	}
	// 	t->setChars(firstCol, 0, link, , len);
	// 	link += len;
	// }
	char sf = t->make_color(COLOR_WHITE, COLOR_BLACK);

	int i = 0;
	int terminal_row = 0;
	int terminal_column = firstCol;
	unsigned short *videoMemStart = (unsigned short *)0xB8000;
	int place = (Terminal::VGA_WIDTH * terminal_row) + terminal_column;
	while (i < len) { // FIXME
		if (link[i] == '\n' || terminal_column >= Terminal::VGA_WIDTH-1) {
			terminal_row++;
			terminal_column = firstCol;
			int place = (Terminal::VGA_WIDTH * terminal_row) + terminal_column;
			if (link[i] != '\n') i--;
		} else if (link[i] != '\b' && link[i] != '\t') {
			videoMemStart[place] = t->makeVGA(sf, link[i]);
			place++;
			terminal_column++;
		}
		i++;
	}
	//clearViewPane();
}

void CLIGUI::clearViewPane () { // FIXME
	int firstCol = 18;
	char sf = t->make_color(COLOR_WHITE, COLOR_BLACK);
	unsigned short *videoMemStart = (unsigned short *)0xB8000;
	for (int i = firstCol; i < (Terminal::VGA_HEIGHT * Terminal::VGA_WIDTH)-1; i++) {
		if (i == Terminal::VGA_WIDTH-1) {
			i += firstCol;
		} else {
			videoMemStart[i] = t->makeVGA(sf, ' ');
		}
	}
}

void CLIGUI::handleMouseUp (int button) {
	for (int i = 0; i < numButtons; i++) {
		CLIButton *b = buttons[i];
		if (mouseX >= b->getX() && mouseY >= b->getY()) {
			if (mouseX < b->getX()+b->getWidth() && mouseY < b->getY()+b->getHeight()) {
				b->callFunction();
			}
		}
	}
}

void CLIGUI::handleMouseDown (int button) {

}

void CLIGUI::handleMouseMoved () {

}

void CLIGUI::addButton (CLIButton *b) {
	t->setChars(b->getX(), b->getY(), b->getText(), t->make_color(COLOR_BLACK, COLOR_LIGHT_CYAN));
	buttons[numButtons] = b;
	numButtons++;
}

void CLIGUI::addLabel (CLILabel *l) {
	t->setChars(l->getX(), l->getY(), l->getText(), t->make_color(COLOR_WHITE, COLOR_BLACK));
}

void CLIGUI::continueClicked () {
	logLn ("We're going on!");
	prepareMenu();
}

void CLIGUI::quitClicked () {
	logLn ("Oh well... they must be a PHP programmer.");
	logLn ("Exiting.");
	this->shouldContinue = false;
}

void CLIGUI::setup () {
	logLn ("Initialising required things...      ");
	initMouse();
	p = new MousePacket;
	t = new Terminal ();
	t->disableTextCursor();
	buttons = (CLIButton **)calloc(sizeof(CLIButton*) * 30); // Space for 30 button pointers
	numButtons = 0;
	logLn ("Done.");

	log ("Clearing terminal...                 ");
	t->resetTerminal();
	logLn ("Done.");

	mouseX = 0;
	mouseY = 0;

	shouldContinue = true;
	leftButton = false;
	rightButton = false;
	middleButton = false;

	char *l =  "[ Warning! This OS melts PHP programmers. Continue with care. ]";

	int len = strlen (l);

	CLILabel *warninglab = new CLILabel (l, (80/2)-(len/2), (24/2)-1);

	CLIButton *contbut = new CLIButton ("[ Continue ]", (80/2)-(len/2), (24/2), this);
	contbut->setFunction(&CLIGUI::continueClicked);

	CLIButton *quitbut = new CLIButton ("[ Quit now ]", contbut->getX() + contbut->getWidth() + 1, contbut->getY(), this);
	quitbut->setFunction(&CLIGUI::quitClicked);

	addButton(contbut);
	addButton(quitbut);
	addLabel(warninglab);

	currentlyOverlyedStyleFlag = t->getStyleFlag (mouseX, mouseY);
}

void CLIGUI::tidyup() {
	delete t;
	delete p;
	for (int i = 0; i < numButtons; i++) {
		delete buttons[i];
	}
	delete [] buttons;
	return;
}

int CLIGUI::main () {
	while (shouldContinue) {
		runMouseUpdate ();
	}
	tidyup();
	return 0;
}

int CLIGUI::processedMotion (int input) {
	if (input > 20) return input/16;
	if (input > 10) return input/8;
	if (input > 5) return input/4;
	return input/2;
}

void CLIGUI::runMouseUpdate () {
	if (isPacketAvailable()) {
		poll (p);
		if (p->xMovement || p->yMovement) {
			t->setStyleFlag(mouseX, mouseY, currentlyOverlyedStyleFlag);
			if (!p->xIsNegative) {
				mouseX += processedMotion(p->xMovement);
			} else {
				mouseX -= processedMotion(p->xMovement);
			}
			if (mouseX < 0) mouseX = 0;
			else if (mouseX > Terminal::VGA_WIDTH-1) mouseX = Terminal::VGA_WIDTH-1;
			if (!p->yIsNegative) {
				mouseY -= processedMotion(p->yMovement) / 2;
			} else {
				mouseY += processedMotion(p->yMovement) / 2;
			}
			if (mouseY < 0) mouseY = 0;
			else if (mouseY > Terminal::VGA_HEIGHT-1) mouseY = Terminal::VGA_HEIGHT-1;
			currentlyOverlyedStyleFlag = t->getStyleFlag (mouseX, mouseY);
			t->setStyleFlag(mouseX, mouseY, t->make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
			handleMouseMoved();
		}


		if (leftButton && !p->leftButton) {handleMouseUp(0);}
		if (rightButton && !p->rightButton) {handleMouseUp(1);}
		if (middleButton && !p->middleButton) {handleMouseUp(2);}

		if (!leftButton && p->leftButton) {handleMouseDown(0);}
		if (!rightButton && p->rightButton) {handleMouseDown(1);}
		if (!middleButton && p->middleButton) {handleMouseDown(2);}

		leftButton = p->leftButton;
		rightButton = p->rightButton;
		middleButton = p->middleButton;
	}
}
