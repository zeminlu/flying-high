/**
 *	@file genShellFunc.c
 *	@brief Implements the general shell functions
 */

/*
*	Includes
*	========
*/
#include "cmdDispatch.h"
#include "string.h"
#include "screenSaver.h"
#include "videoDriver.h"
#include "stdio.h"
#include "stdlib.h"

/*
*	Public Functions
*	================
*/
void
parseScreensaverOpt(int paramFd){
	int i=0;
	char opt[MAX_OPT_SIZE];
	read(paramFd, opt, sizeof(opt));
	int len=strlen((char *)opt);
	int num;
	puts((unsigned char*)"<<Screensaver>> \n");

	while(isspace(opt[i]) && i<len)
		++i;
	if(i==len){
		puts((unsigned char*)"For setting the time use -dX where X is the time in seconds \n");
		return;
	}
	for( ; i+2<len; ++i)
		if(opt[i]=='-'){
			if(opt[i+1]=='d'){
				num=atoi(opt+i+2);	
				if(num <= 0  || strlen((char *)opt+i+2) > 5) 
					puts((unsigned char*)"Invalid time. Time should be greater than 0 but less than 9999 \n");
				else{
					setScreenSaverTime((unsigned int) num); 
					puts((unsigned char*)"The time was set \n");
				}
				break;
			}
			else{
				puts((unsigned char *) "Unrecognized option \n");
			}
		}
		else{
			puts((unsigned char*)"Invalid option \n");
			break;
		}
		
	return;
}

void
showHelp(int paramFd){
	puts((unsigned char *) "For activating the screensaver use \n >screensaver \n for setting the amount of seconds till activating use \n >screensaver -dx \n where x is the desired amount of seconds \n");	
	puts((unsigned char *) "For printing a file content to standard output use : cat \n");
	puts((unsigned char *) "For printing a content to a file use echo fileName content \n");
	puts((unsigned char *) "The ls command lists the tag contents \n");
	puts((unsigned char *) "The detail comand details a given filename \n");
	puts((unsigned char *) "There are also commands for making and removing tags, files, etc (mktag, rm, rmtag...) \n");
}

void
showAuthors(int paramFd){
	puts((unsigned char *) "Santiago Bermudez \nSantiago Perez De Rosso \nLucas Pikzagalli \n Guido Maruli \n");
}

void
clearScreenWrap(int paramFd){
	clearScreen(stdout);
	return;
}

void
fillScreen(int paramFd){
/*	puts((unsigned char *) "Fill screen!");*/


puts((unsigned char*)"Linux is an operating system that was initially created as a hobby by a young student, Linus Torvalds, at the University of Helsinki in Finland. Linus had an interest in Minix, a small UNIX system, and decided to develop a system that exceeded the Minix standards. He began his work in 1991 when he released version 0.02 and worked steadily until 1994 when version 1.0 of the Linux Kernel was released. The kernel, at the heart of all Linux systems, is developed and released under the GNU General Public License and its source code is freely available to everyone. Many have helped in its development. It is this kernel that forms the base around which a Linux operating system is developed. There are now literally hundreds of companies and organizations and an equal number of individuals that have released their own versions of operating systems based on the Linux kernel. More information on the kernel can be found at our sister site, LinuxHQ and at the official Linux Kernel Archives. The current full-featured version is 2.6 (released December 2003) and development continues. \n");

puts((unsigned char *)"Apart from the fact that it's freely distributed, Linux's functionality, adaptability and robustness, has made it the main alternative for proprietary Unix and Microsoft operating systems. IBM, Hewlett-Packard and other giants of the computing world have embraced Linux and support its ongoing development. Well into its second decade of existence, Linux has been adopted worldwide primarily as a server platform. Its use as a home and office desktop operating system is also on the rise. The operating system can also be incorporated directly into microchips in a process called embeddin and is increasingly being used this way in appliances and devices. \n");

}


