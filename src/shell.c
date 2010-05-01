/*
 *	File shell.c
 *	
 *		Brief 
 *
 *		Author Guido Marucci Blas, Nicolas Purita & Luciano Zemin
 *
 */
#include "shell.h"

static int firstRun = 1;
static int status = DEAD;
static char lineBuffer[MAX_LINE];
static char enteredCommand[MAX_LINE];
static int index = 0;
static char * prompt = "root@ArkOS$>";

typedef void (*shellFuncT)(char *);

typedef struct {
	char * command;
	shellFuncT func;
	char * helpMsg;
} commandT;

typedef int (*pFuncT)(char*);

typedef struct {
	char * name;
	pFuncT func;
	char * helpMsg;
} propertyT;

/* SYSTEM PROPERTIES */
static int getColor(char *value, int * color)
{
	toLowerCase(value);
	if ( strcmp(value, "black") == 0  )
		*color = setColor(BLACK,BLACK);
	else if ( strcmp(value, "blue") == 0 )
		*color = setColor(BLUE,BLUE);
	else if ( strcmp(value, "green") == 0 )
		*color = setColor(GREEN,GREEN);
	else if ( strcmp(value, "cyan") == 0 )
		*color = setColor(CYAN,CYAN);
	else if ( strcmp(value, "red") == 0 )
		*color = setColor(RED,RED);
	else if ( strcmp(value, "magenta") == 0 )
		*color = setColor(MAGENTA,MAGENTA);
	else if ( strcmp(value, "brown") == 0 )
		*color = setColor(BROWN,BROWN);
	else if ( strcmp(value, "white") == 0 )
		*color = setColor(WHITE,WHITE);
	else
		return 0;
	return 1;
}

static int pointerColor ( char * value )
{
	int color, row, col;

	if ( getColor(value, &color) )
	{
		setPointerColor(color);
		getPointerPosition(&row, &col);
		setPointerPosition(row, col);
		refreshScreen();
	}
	else
	{
		puts("Error: Invalid Color - ");
		puts(value);
		puts(".\n");
		return 0;
	}
	return 1;
}

static int videoColor ( char * value )
{
	int color;

	if ( getColor(value, &color) )
	{
		color &= 0xF;
		setPointerVisibility(0);
		setVideoColor(color);
		updateColor();
		refreshScreen();
		setPointerVisibility(1);
	}
	else
	{
		puts("Error: Invalid Color - ");
		puts(value);
		puts(".\n");
		return 0;
	}
	return 1;
}

static int tabStop (char * value)
{
	int ts = toInt(value);
	if ( setVideoTabStop(ts) != ts )
	{
		puts("Error: Invalid value - ");
		puts(value);
		puts(".\n");
		return 0;
	}
	return 1;
}

/*static int mouseRate (char *value)
{
	int rate = toInt(value);
	setMouseRate(rate);
	return 1;
}

static int mouseEnable (char *value)
{
	toLowerCase(value);
	if (strcmp(value, "true") == 0)
	{
		setPointerVisibility(1);
		setMouseState(ENABLED);
	}
	else if (strcmp(value, "false") == 0)
	{
		setPointerVisibility(0);
		setMouseState(DISABLED);
	}
	else
	{
		puts("Error: Invalid value - ");
		puts(value);
		puts(".\n");
		return 0;	
	}
	refreshScreen();
	return 1;
}*/

static int screenSaverTime (char *value)
{
	int time;
	if ( isAllNumb(value) )
	{
		time = toInt(value) * TICKS_PER_SECOND;
		setScreenSaverTime(time);
	}
	else
	{
		puts("Error: Invalid value - ");
		puts(value);
		puts(".\n");
		return 0;	
	}
	return 1;
}

static propertyT properties[] = {
	{"pointer-color", pointerColor, "Changes the mouse pointer's color. Valid colors: BLUE, GREEN, CYAN, RED and MAGENTA."},
	{"video-color", videoColor, "Changes the video color. Valid colors: BLUE, GREEN, CYAN, RED and MAGENTA."},
	{"tab-stop", tabStop, "Changes amount of spaces a tab stop inserts into a line."},
	/*{"mouse-rate", mouseRate, "Changes the mouse rate. The amount of ticks before the mouse refreshes it's position."},
	{"mouse-enable", mouseEnable, "Turn on/off the mouse. Valid values 'true' or 'false'"},*/
	{"screensaver-time", screenSaverTime, "Changes the non-activity time required by the screensaver to be launched."},
	{"", NULL}
};

static int setProperty(char *property, char *value)
{
	int i;
	
	for ( i = 0; properties[i].func != NULL; ++i )
		if ( strcmp(properties[i].name, property) == 0 )
			return properties[i].func(value);
	puts("Error: Invalid property - ");
	puts(property);
	puts(".\n");
	return 0;
}

static void printProperties ( char *value )
{
	int i;
	puts("System Properties:\n");
	for ( i = 0; properties[i].func != NULL; ++i )
	{
		puts("\t - ");
		puts(properties[i].name);
		puts(": ");
		puts(properties[i].helpMsg);
		puts("\n\n");
	}
}

/* END SYSTEM PROPERTIES */

static commandT * getCommand ( char * command );

/* SHELL COMMANDS */
static void clear(char *);
static void echo(char *);
static void logout(char *);
static void help(char *);
static void set(char *);
static void reboot(char *);
static void shutdown(char *);
static void mkdir(char *);
static void printPhrasing(char *);

static commandT commands[] = {
	{"clear", clear, "Clears Screen."},
	{"echo", echo, "Prints the arguments on the standard output."},
	{"logout", logout, "Terminates the shell application."},
	{"help", help, "Prints this list of commands."},
	{"set", set, "Sets a system property. Use: set property=value."},
	{"reboot", reboot, "Reboots the system."},
	{"shutdown", shutdown, "Shuts the system down."},
	{"mkdir", mkdir, "Makes a new directory."},
	{"print-sysproperties", printProperties, "Prints the lisf of system properties that can be set by the set command."},
	{"print-statement", printPhrasing, "Prints the statment of the project."},
	{"", NULL, ""}
};

static void printPhrasing(char * value)
{
	static char * msg1 = "\nObjetivo\n\tRealizar un programa que muestre algunas de las caracteristicas del Modo protegido de los microprocesadores de Intel.\n\tEnunciado\n\tEl trabajo consta de un diskete booteable con GRUB instalado, el cual debe obtener de disco una imagen binaria que sera el programa que corra en Modo protegido.\n\tDicho programa debera contener lo siguiente:\n\t\ta) Shell: El sistema debe proveer al usuario de un shell, para poder ejecutar comandos.\n\t\tb) INT 80h: Crear las primitivas read() y write() ";
	static char * msg2 = "para centralizar la lectura y la escritura de los dispositivos. Dichas primitivas deben invocar la INT 80h.\n\t\tc) Driver de Mouse: El programa debera poder manejar un cursor de texto en pantalla. Con el botón izquierdo del Mouse presionado se podra seleccionar cualquier parte de la pantalla (copiar) y con el boton derecho ";
	static char * msg3 = "se podra “pegar” lo seleccionado donde se encuentre el cursor para ingreso de datos.\n\t\td) Salva pantalla: Por línea de comando se podra programar la cantidad de segundos en la que se activa el salva pantalla. El mismo dejara de funcionar ante la presión de una tecla o el movimiento del Mouse.\n\n";
	puts(msg1);
	puts(msg2);
	puts(msg3);
}

static void mkdir(char *value)
{
	puts("jejejeje are you crazy!!!");
}

static void shutdown(char *value)
{
	puts("Shutting down the system...\n");
	puts("...oops!!! function not implemented...Push the power button.\n");
}

static void reboot(char *value)
{
	rebootSystem();
}

static void clear ( char * args )
{
	wait(500);
	flushStdScreen();
	setPointerVisibility(0);
	clearScreen();
	setCursorPosition(0, 0);
	setPointerVisibility(1);
}

static void echo ( char * args )
{
	puts(args);
	putchar('\n');
}

void logout ( char * args )
{
	puts("[logout]\n");
	puts("Exiting Shell...\n");
	status = DEAD;
}

static void help ( char * args )
{
	int i;
	char * command;
	commandT * cmd;

	if ( (command = strtok(args, " ")) == NULL )
	{
		puts("Command list:\n");
		for ( i = 0; commands[i].func != NULL; ++i )
		{
			putchar('\t');
			puts(commands[i].command);
			puts(" - ");
			puts(commands[i].helpMsg);
			putchar('\n');
		}
	}
	else if ( (cmd = getCommand(command)) != NULL )	
	{
		puts(command);
		puts(": ");
		puts(cmd->helpMsg);
		putchar('\n');
	}
	else
		puts("Command not found\n");
}

static void set ( char * args )
{
	char * property, * value;

	if ( (property = strtok(args, "=")) == NULL )
	{
		puts("Error: Invalid System Property\n");
		return;
	}
	if ( (value = strtok(NULL, "=")) == NULL )
	{
		puts("Error: Invalid Property Value\n");
		return;
	}
	setProperty(property, value);
}

/* END SHELL COMMANDS */

static commandT * getCommand ( char * command )
{
	int i;

	for ( i = 0; commands[i].func != NULL; ++i )
		if ( strcmp(commands[i].command, command) == 0 )
			return &(commands[i]);
	return NULL;
}

static void printPrompt()
{
	puts(prompt);
}

static void parseCommand ( void )
{
	commandT * cmd;
	char *command, *args;

	putchar('\n');
	if ( index == 0 )
	{
		printPrompt();
		return;
	}
	lineBuffer[index] = '\0';
	strcpy(enteredCommand, lineBuffer);
	command = strtok(enteredCommand, " ");
	args = strtok(NULL, "");
	if ( (cmd = getCommand(command)) != NULL )
		cmd->func(args);
	else
	{
		puts("shell: ");
		puts(lineBuffer);
		puts(": Invalid Command\n");
	}

	index = 0;
	if ( status == LIVE ) printPrompt();
}

static void putBackspace ( void )
{
	if ( index == 0 )
		return;
	putchar('\b');
	--index;
}

static void parseBackspace ( void )
{
	int i, ts;

	if ( lineBuffer[index] == '\t' )
		for ( i = 0, ts = getVideoTabStop(); i < ts; ++i)
			putBackspace();
	else
		putBackspace();
}

static void autoComplete ( void )
{
	/*
	 * Code for autocompliting command
	 */

	/* If command could not be autocompleted print tab*/
	index += getVideoTabStop();
	lineBuffer[index] = '\t';
	putchar('\t');
}

int shell ( void )
{
	int c;
	unsigned char uc, att;

	if ( firstRun )
	{
		firstRun = 0;
		status = LIVE;
		puts("Starting Shell...\n");
		puts("\tEnter 'help' for a list of commands.\n");
		puts("\tEnter 'help cmd' for the help message of 'cmd'.\n\n");
		printPrompt();
	}

	c = getchar();
	att = fgetc(inatt);

	if ( c == EOF )
		return status;
	uc = c;
	if ( uc == '\n' )
		parseCommand();
	else if ( uc == '\b' ) 
		parseBackspace();
	else if ( uc == '\t' )
		autoComplete();
	else if ( index < MAX_LINE  - 1)
	{
		lineBuffer[index++] = uc;
		putchar(uc);
		/*fputc(att, outatt);*/
	}

	return status;
}
