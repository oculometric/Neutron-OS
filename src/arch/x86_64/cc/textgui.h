#include "terminal.h"
#include "mouse.h"

#ifndef CLIGUIDEF
#define CLIGUIDEF
class CLIGUI;

class CLIButton {
public:
	CLIButton (char *c, int xx, int yy, CLIGUI *cc);
	void callFunction ();
	void setFunction (void (*f)(void));
	void setFunction (void (CLIGUI::*f)(void));

	int getX ();
	int getY ();
	int getWidth ();
	int getHeight ();
	char * getText ();

private:
	char *text;
	CLIGUI *container;
	void (*function)(void);
	void (CLIGUI::*memFunction)(void);
	bool isInstanceFunc;
	int x;
	int y;
	int width;
	int height;
};

class CLILabel {
public:
	CLILabel (char *c, int xx, int yy);

	int getX ();
	int getY ();
	int getWidth ();
	int getHeight ();
	char * getText ();

private:
	char *text;
	int x;
	int y;
	int width;
	int height;
};

class CLIGUI {
public:
	int main ();
	void setup ();
	void tidyup ();
	void runMouseUpdate ();
	void addButton (CLIButton *b);
	void addLabel (CLILabel *l);
private:

	CLIButton **buttons;
	int numButtons;

	MousePacket *p;
	signed int mouseX;
	signed int mouseY;
	Terminal *t;
	bool shouldContinue;
	bool leftButton, rightButton, middleButton;
	char currentlyOverlyedStyleFlag;
	int processedMotion (int input);

	void handleMouseUp (int button);
	void handleMouseDown (int button);
	void handleMouseMoved ();

	// NON-CORE
	void continueClicked ();
	void quitClicked ();
};
#endif
