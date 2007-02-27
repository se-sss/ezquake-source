/*
	Options Menu module
	$Id: menu_options.h,v 1.2 2007-01-11 15:33:12 johnnycz Exp $
*/

// <interface for menu.c>
// initializes the "Demo" menu on client startup
void Menu_Options_Init(void);

// process key press that belongs to demo menu
void Menu_Options_Key(int key, int unichar);

// process request to draw the demo menu
void Menu_Options_Draw (void);
// </interface>