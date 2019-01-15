#include "textgui.h"
#include "../log.h"
#include "../memory.h"

void CLIGUI::prepareMenu() {
	for (int i = 0; i < numButtons; i++) {
		delete buttons[i];
	}

	numButtons = 0;

	logLn ("Making GUI");

	string *line1 = new string (80);
	char *d = line1->getData();
	for (int c = 0; c < 80; c++) {
		d[c] = ' ';
		if (c == 0 || c == 79) {
			d[c] = '|';
		}
	}

	for (int r = 0; r < Terminal::VGA_HEIGHT; r++) {
		t->setChars(0, r, line1->getData(), t->make_color(COLOR_WHITE, COLOR_BLACK));
	}
	currentlyOverlyedStyleFlag = t->make_color(COLOR_WHITE, COLOR_BLACK);


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
	setEnableStreaming(true);
	setResolution(0x00); // 1 count per mm
	setScaling(false);   // Linear scaling
	p = new MousePacket;
	t = new Terminal ();

	buttons = (CLIButton **)calloc(sizeof(CLIButton*) * 10); // Space for ten button pointers
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
