/**
 *	\file shell.c
 *
 *		\brief The shell process module.
 *
 *		\author Luciano Zemin, Nicolás Magni, Nicolás Purita
 *
 */

#include "shell.h"
static char * prompt = "root@flyingHighOS$>";
char bufferA[MAX_LINE];
char bufferB[MAX_LINE];

static dataSlotShell tableDataShell[MAX_TTY];

int initGlobalDataShell(){
	int i;
	
	for( i = 0; i < MAX_TTY; i++){
		tableDataShell[i].pid = 0;
	}
	return 0;
}

static int initDataShell(){
	int i;
	pid_t pid ;
	pid = getpid();
	
	if(pid > 0){
		for( i = 0; i < MAX_TTY; i++){
			if(tableDataShell[i].pid == 0){
				tableDataShell[i].pid = pid;
				tableDataShell[i].status = RUNNING;
				tableDataShell[i].index = 0;
				tableDataShell[i].lineBuffer = malloc(sizeof(char)* MAX_LINE);
				tableDataShell[i].enteredCommand = malloc(sizeof(char)* MAX_LINE);
				return TRUE;	
			}
		}
	}
	return FALSE;
}

static int getGlobalDataIndex(){
	int i;
	pid_t pid;
	
	pid = getpid();
	
	for( i = 0; i < MAX_TTY; i++){
		if(tableDataShell[i].pid == pid){
			return i;
		}
	}
	/*Exit with error, no a valid pid, or No sta en la tabla*/
	return -1;	
}

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

static int videoColor ( char * value )
{
	int color;

	if ( getColor(value, &color) )
	{
		color &= 0xF;
		/*setPointerVisibility(0);*/
		setVideoColor(color);
		updateColor();
		refreshScreen();
		/*setPointerVisibility(1);*/
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

static propertyT properties[] = {
	{"video-color", videoColor, "Changes the video color. Valid colors: BLUE, GREEN, CYAN, RED and MAGENTA."},
	{"tab-stop", tabStop, "Changes amount of spaces a tab stop inserts into a line."},
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
static void startTop(char *);
static void startKill(char *);
static void startPrintA(char *);
static void startPrintB(char *);
static void startNothing(char *);
static void startPageFault(char *);
static void changeSchedule( char *);
static void startBattleShip(char *args);

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
	{"top", startTop, "Runs the top process."},
	{"kill",startKill, "Takes the pid of the standar input, and Kill the process associated."},
	{"printA", startPrintA, "Prints 'A' in the stdout, could be running in background."},
	{"printB", startPrintB, "Prints 'B' in the stdout, could be running in background."},
	{"nothing", startNothing, "An idle process, could be running in background."},
	{"pageFault", startPageFault, "Force a page fault exception."},
	{"scheduling", changeSchedule, "Change the Schedule Algorithm, if it's running Round Robin, it changes to RPG and viceversa"},
	{"battleship", startBattleShip, "Starts Battleship"},
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
	puts("jejejeje...are you crazy!!!???\n");
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
	clearTTYScreen();
	setTTYCursorPosition(0, 0);
}

static void echo ( char * args )
{
	puts(args);
	putchar('\n');
}

void logout ( char * args )
{
	int indexDT;
	
	indexDT = getGlobalDataIndex();	
	if(indexDT == -1){
		/*el pid ingresado no es valido hay que salir con error*/
		return;
	}
	
	puts("[logout]\n");
	puts("Exiting Shell...\n");
	tableDataShell[indexDT].status = DEAD;
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

static void startTop(char *args){
	pid_t pid, ownPid;
	int i;
	int status;
	process_t *processTable;
	
	ownPid = getpid();
	processTable = getProcessTable();
	
	setProcessAtomicity(ownPid, ATOMIC);
	
	for (i = 0 ; i < MAX_PROCESS ; ++i){
		if (!strcmp(processTable[i].name, "top") && processTable[i].pid > 0){
			puts("El proceso top ya se encuentra corriendo en la tty ");
			puti(processTable[i].tty + 1);
			putchar('\n');
			setProcessAtomicity(ownPid, UNATOMIC);
			return;
		}
	}
	
	if ((pid = createProcess("top", (void(*)(void *))top, NULL, FOREGROUND)) == -1 ) {
		puts("ERROR: Top could not be created.\n");
	}
	setProcessAtomicity(ownPid, UNATOMIC);
	waitpid(pid, &status);
	clearTTYScreen();
	
	return;		
}

static void startKill(char *args){
	int pid;
	
	pid = toInt(args);	
	if (kill((pid_t)pid) == 0){
		puts("Killing process ");
		puts(getProcessTable()[pid].name);
		puts("...\n");
	}
	else
		puts("No process for the given pid\n");
}

static void startBattleShip(char *args){
	int status;
	pid_t pid, ownPid;
	
	ownPid = getpid();
	setProcessAtomicity(ownPid, ATOMIC);
	
	if ((pid = createProcess("battleship", (void(*)(void *))battleship, NULL, FOREGROUND)) == -1 ) {
		puts("ERROR: Battleship could not be created.\n");
	}
	setPriority(pid, 4);
	setProcessAtomicity(ownPid, UNATOMIC);
	waitpid(pid, &status);
	//clearTTYScreen();
	
}

static void startPrintA(char *args){
	pid_t pid, ownPid;
	int status;
	int mode;
	
	ownPid = getpid();
	setProcessAtomicity(ownPid, ATOMIC);
	mode = *args =='&' ? BACKGROUND:FOREGROUND;
	
	if ((pid = createProcess("printA", (void(*)(void *))printA, NULL, mode)) == -1 ) {
		puts("ERROR: printA could not be created.\n");
	}
	setPriority(pid, 4);
	setProcessAtomicity(ownPid, UNATOMIC);
	if(mode == FOREGROUND){
		waitpid(pid, &status);
	}
	return;
}

static void startPrintB(char *args){
	pid_t pid, ownPid;
	int status;
	int mode;
	
	ownPid = getpid();
	setProcessAtomicity(ownPid, ATOMIC);
	mode = *args =='&' ? BACKGROUND:FOREGROUND;
	
	if ((pid = createProcess("printB", (void(*)(void *))printB, NULL, mode)) == -1 ) {
		puts("ERROR: printB could not be created.\n");
	}
	setPriority(pid, 2);
	setProcessAtomicity(ownPid, UNATOMIC);
	if(mode == FOREGROUND){
		waitpid(pid, &status);
	}
	return;
}

static void startNothing(char *args){
	pid_t pid, ownPid;
	int status;
	int mode;
	
	mode = (*args == '&') ? BACKGROUND : FOREGROUND;
	ownPid = getpid();
	setProcessAtomicity(ownPid, ATOMIC);
	
	if ((pid = createProcess("nothing", (void(*)(void *))nothing, NULL, mode)) == -1 ) {
		puts("ERROR: nothing could not be created.\n");
	}
	if (mode == FOREGROUND){
		setTTYMode(pid, TTY_CANONICAL);
	}
	setProcessAtomicity(ownPid, UNATOMIC);
	
	
	if(mode == FOREGROUND){
		waitpid(pid, &status);
	}
	return;
}

static void startPageFault(char *args){
	pid_t pid;
	int status;
	
	if ((pid = createProcess("nothing", (void(*)(void *))pageFault, NULL, FOREGROUND)) == -1 ) {
		puts("ERROR: nothing could not be created.\n");
	}
	waitpid(pid, &status);

	return;
}

static void changeSchedule( char * args ){
	pid_t pid;
	
	pid = getpid();
	setProcessAtomicity(pid, ATOMIC);
	changeAlgorithimSchedule();
	setProcessAtomicity(pid, UNATOMIC);
	
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
	int indexDT;
	
	indexDT = getGlobalDataIndex();
	if(indexDT == -1){
		/*EL pid ingresado es invalido,*/
		return;
	}
	
	putchar('\n');
	if ( tableDataShell[indexDT].index == 0 )
	{
		printPrompt();
		return;
	}
	tableDataShell[indexDT].lineBuffer[tableDataShell[indexDT].index] = '\0';
	strcpy(tableDataShell[indexDT].enteredCommand, tableDataShell[indexDT].lineBuffer);
	command = strtok(tableDataShell[indexDT].enteredCommand, " ");
	args = strtok(NULL, "");
	if ( (cmd = getCommand(command)) != NULL )
		cmd->func(args);
	else
	{
		puts("shell: ");
		puts(tableDataShell[indexDT].lineBuffer);
		puts(": Invalid Command\n");
	}

	tableDataShell[indexDT].index = 0;
	if ( tableDataShell[indexDT].status == RUNNING ) printPrompt();
}

int shell ( void )
{
	int c;
	unsigned char uc;
	int indexDT, status;
	pid_t pid;
	
	pid = getpid();
	setProcessAtomicity(pid, ATOMIC);
	status = initDataShell();
	setProcessAtomicity(pid, UNATOMIC);

	if(status == FALSE){
		return -1;
	}
	
	indexDT = getGlobalDataIndex();
	puts("Starting Shell #");
	puti(runningProcess->tty + 1);
	puts("...\n");
	puts("\tEnter 'help' for a list of commands.\n");
	puts("\tEnter 'help cmd' for the help message of 'cmd'.\n\n");
	printPrompt();
	
	while (tableDataShell[indexDT].status == RUNNING){
		c = getchar();
		if ( c == EOF ){
			asm volatile("hlt");
			waitTty(getTty(pid));
			continue;
		}
		uc = c;
		if ( uc == '\n' ){
			parseCommand();
		}
		else if ( tableDataShell[indexDT].index < MAX_LINE  - 1){
			tableDataShell[indexDT].lineBuffer[tableDataShell[indexDT].index++] = uc;
		}
	}
	
	return tableDataShell[indexDT].status;
}
