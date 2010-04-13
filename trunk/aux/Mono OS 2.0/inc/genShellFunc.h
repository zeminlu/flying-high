/**
 *	@file genShellFunc.h
 *	@brief Defines the basic shell functions
 */
#ifndef GENSHELLFUNC_H_
#define GENSHELLFUNC_H_

#include "cmdDispatch.h"

void parseScreensaverOpt(int paramFd);

void showHelp(int paramFd);


void showAuthors(int paramFd);

void clearScreenWrap(int paramFd);


void fillScreen(int paramFd);

#endif
