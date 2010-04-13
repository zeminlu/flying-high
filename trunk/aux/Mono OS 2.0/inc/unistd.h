#ifndef _ASM_X86_UNISTD_32_H
#define _ASM_X86_UNISTD_32_H

/*
 * This file contains the system call numbers.
 */

#define ID_restart_syscall      	0
#define ID_EXIT			 		 	1
#define ID_FORK			  			2
#define ID_READ			  			3
#define ID_WRITE		  			4
#define ID_OPEN		 	 			5
#define ID_CLOSE		  			6
#define ID_WAITPID		  			7
#define ID_CREATE		  			8
#define ID_RMFILE		 			9
#define ID_READTAG		 			10
#define ID_MKTAG		 			11
#define ID_RMTAG		 			12
#define ID_OPENTAG					13
#define ID_FILEINFO					14

#define ID_SET_TTY					30
#define ID_GET_TTY					31
#define ID_GET_PID					32
#define ID_SET_PID					33
#define ID_GET_PRIO					34
#define ID_SET_PRIO					35
#define ID_GET_LEVEL				36
#define ID_SET_LEVEL				37
#define ID_DUP						38
#define ID_DUP_2					39
#define ID_WAIT_PID					40
#define ID_KILL						41

#endif /* _ASM_X86_UNISTD_32_H */


