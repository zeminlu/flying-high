/*
*	Standard Library
*
*	Similar to the C Standard Library but has a simpler 
*	implementation of the dynamic memory allocation module
*
*	Author: Santiago Perez De Rosso
*/

/*
*	Includes
*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//#include "baseMalloc.h"

/*
*	Defines
*/
#define BUFF_SIZE 10000

#ifndef NULL 
#define NULL 0
#endif



/*
*	Public functions
*/
/*
typedef long Align;
union header {
	struct{
	    union header *ptr;
	    unsigned size;
	} s;
	Align x;
};

typedef union header Header;

static Header base;
static Header *freep = NULL;

static char myTempPage[4096];
static char *bufPoint=myTempPage;

char *sbrk(int increment){
    bufPoint+=increment;
    if( bufPoint - myTempPage < 4096 ){
	   return (char *) bufPoint-increment;
    }
    return NULL;
}

static Header *morecore(unsigned nu){
    char *cp, *sbrk(int);
    Header *up;

    if( nu < NALLOC )
	   nu = NALLOC;
    cp = sbrk(nu * sizeof(Header));
    if( cp == (char *) - 1 )
	   return NULL;
    up = (Header *) cp;
    up->s.size = nu;
    free((void *) (up + 1));
    return freep;
}

void *realloc(void *ptr, size_t size){
	void *ret;
	if( (ret=malloc(size)) != NULL ){
	    memcpy(ret, ptr, size);
	    free(ptr);
	}
	return ret;
}



void free(void *ap){
    Header *bp, *p;

    bp = (Header *)ap -1;
    for( p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr )
	   if( p >= p->s.ptr && (bp > p || bp < p->s.ptr))
		  break;
    if( bp + bp->s.size == p->s.ptr ){
	   bp->s.size += p ->s.ptr->s.size;
	   bp->s.ptr = p->s.ptr->s.ptr;
    }
    else
	   bp->s.ptr = p->s.ptr;
    if(p + p->s.size == bp ){
	   p->s.size += bp->s.size;
	   p->s.ptr = bp -> s.ptr;
    }
    else
	   p->s.ptr = bp;
    freep = p;
}

void *malloc(size_t nbytes){
    Header *p, *prevp;
    Header *morecore(unsigned);
    unsigned nunits;

    nunits = (nbytes + sizeof(Header)-1)/sizeof(Header) + 1;
    if( (prevp =freep) == NULL){
	   base.s.ptr = freep = prevp = &base;
	   base.s.size = 0;
    }
    for( p=prevp->s.ptr; ; prevp = p, p = p->s.ptr ){
	   if( p->s.size >= nunits ){
		  if( p->s.size == nunits )
			 prevp->s.ptr = p->s.ptr;
	   }
	   else{
		  p->s.size -= nunits;
		  p += p->s.size;
		  p->s.size = nunits;
		  freep = prevp;
		  return (void *)(p+1);
	   }
	   if(p == freep)
		  if((p=morecore(nunits)) == NULL)
			 return NULL;
    }
}
*/
/*
static unsigned char mallocBuf[BUFF_SIZE];
static unsigned char *bufPoint=mallocBuf;

void *calloc(size_t nmemb, size_t size){
	void *ret;
	if( nmemb*size != 0 &&  mallocBuf + BUFF_SIZE- bufPoint > nmemb*size ){
		bufPoint += nmemb*size;
		for( ret=(((unsigned char*)bufPoint) - nmemb * size); ((unsigned char *)ret)<bufPoint; ++ret)
			*(unsigned char *)ret=0;
	}
	else                                                                       
                ret=NULL;
	return ret;
}

void
*malloc(size_t size){
	void *ret;
	if( size != 0 &&  mallocBuf + BUFF_SIZE- bufPoint > size ){
		bufPoint += size;
		ret = bufPoint- size;
	}
	else
		ret=NULL;
	return ret;
}

void
free(void *ptr){
 	unsigned char *q;
	if(ptr != NULL){
        q = ptr;
        if( q >= mallocBuf && q < mallocBuf +  BUFF_SIZE )
		bufPoint=q;
	}

}
*/

double
atof(const unsigned char *nptr){
	double val, power;
	int i, sign;

	for(i=0; isspace(nptr[i]); i++);
	sign=(nptr[i]=='-') ? -1:1;
	if(nptr[i]=='+' || nptr[i]=='-' )
		i++;
	for(val=0.0; isdigit(nptr[i]); i++){
		val=10.0*val+(nptr[i]-'0');
	}
	if(nptr[i]=='.')
		i++;
	for(power=1.0; isdigit(nptr[i]); i++){
		val=10.0*val+(nptr[i]-'0');
		power *= 10.0;
	}
	return sign*val/power;
}

int
atoi(const char *nptr){
	int val, i;
	int len=strlen(nptr);
	for(i=0, val=0; i<4 && i<len && isdigit(nptr[i]); i++)
		val=10*val+(nptr[i]-'0');
	return val;
}
