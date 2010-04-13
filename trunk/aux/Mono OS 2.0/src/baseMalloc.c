#include "baseMalloc.h"
#include "string.h"
#include "debug.h"

/*
 * 	Defines
 * 	=======
 */
#define NALLOC 100


typedef long Align;
union header {
	struct{
	    union header *ptr;
	    unsigned size;
	} s;
	Align x;
};

typedef union header Header;

typedef struct {
    Header base;
    Header *freep;
    void *point;
} memAreaHeader_t;

static void printHeader(memArea_t memArea){
    memAreaHeader_t memHeader;
    memcpy(&memHeader, memArea.address, sizeof(memAreaHeader_t));
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "Header: ", TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "(addres of memory area: ", FALSE);
	hexa_debug(BASE_MALLOC_DEBUG, NULL, memArea.address, TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "base: ", FALSE);
	if( memHeader.base.s.ptr == NULL )
		hexa_debug(BASE_MALLOC_DEBUG, NULL, 0, TRUE);
	else
		hexa_debug(BASE_MALLOC_DEBUG, NULL, *((int *)memHeader.base.s.ptr), TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "size: ", FALSE);
	int_debug(BASE_MALLOC_DEBUG, NULL, memHeader.base.s.size, TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "freep: ", FALSE);
	if( memHeader.freep == NULL )
		hexa_debug(BASE_MALLOC_DEBUG, NULL, 0, TRUE);
	else
		hexa_debug(BASE_MALLOC_DEBUG, NULL, *((int *) memHeader.freep), TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "pointer: ", TRUE);
	hexa_debug(BASE_MALLOC_DEBUG, __FUNCTION__, 	memHeader.point, TRUE);
}



/*
 * 	Private Functions
 * 	=================
 */

static void *memAdmin(int increment, memArea_t memArea){

    printHeader(memArea);
    //puts((unsigned char *) "En mem admin, hay debug \n");
    //_debug();
    ((memAreaHeader_t *) (memArea.address))->point += increment;
  /*  puts("\n");
   putx( ((memAreaHeader_t *) (memArea.address))->point );
    puts("\n mem size: ");
    puti(memArea.size);
    puts("\n");
    puts("cuenta: ");
    puti( ((memAreaHeader_t *) (memArea.address))->point - memArea.address  );
    puts("increment: ");
    puti( increment );
    puts("\n");
    */
    if( ((memAreaHeader_t *) (memArea.address))->point - memArea.address < memArea.size){
	   /**/
	//   puts("\n Devolviendo para poner: \n");
	  // putx( (((memAreaHeader_t *) (memArea.address))->point - increment));
	  // puts("\n");
	   //_debug();
	   return (char *) (((memAreaHeader_t *) (memArea.address))->point - increment);
    }
    /**/
    return NULL;
}

static void baseFree(void *ap, memArea_t memArea/*Header **freep*/){
   // memAreaHeader_t memHeader;
   // memcpy(&memHeader, memArea.address, sizeof(memAreaHeader_t));

    Header *bp, *p;

    printHeader(memArea);
   // puts((unsigned char *) "En base free, sige un debug \n");
    //_debug();

    bp = (Header *)ap -1;
    for( p = /*freep*/   ((memAreaHeader_t *) (memArea.address))->freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr )
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
   /* freep */  ((memAreaHeader_t *) (memArea.address))->freep= p;

	/**/
}


static Header *baseMoreCore(unsigned nu, void *(*memAdmin)(int, memArea_t), memArea_t memArea){
    char *cp;
    Header *up;

    printHeader(memArea);
   // puts((unsigned char *) "En base more core, hay un debug \n");
   // _debug();

    if( nu < NALLOC )
	   nu = NALLOC;
    //puti(nu);
    //_debug();
    //puti(sizeof(Header));
    //_debug();
    cp = memAdmin(nu * sizeof(Header), memArea);
//    puts((unsigned char *) "En base more core, hay un debug \n");
    printHeader(memArea);
    //_debug();
    if( cp == /*(char *) - 1*/ NULL )
	   return NULL;
    up = (Header *) cp;
    up->s.size = nu;
    baseFree((void *) (up + 1), memArea);
    return /*freepmemHeader */ ((memAreaHeader_t *) (memArea.address))->freep;
}

static void *baseMalloc(size_t nbytes, void *(*memAdmin)(int, memArea_t), memArea_t memArea){

  //  Header base = memHeader.base;
   // Header *freep = memHeader.freep;
    Header *p, *prevp;
    Header *morecore(unsigned);
    unsigned nunits;

    nunits = (nbytes + sizeof(Header)-1)/sizeof(Header) + 1;
    if( (prevp = /*freep*/ ((memAreaHeader_t *) (memArea.address))->freep) == NULL){
	   /*base*/ ((memAreaHeader_t *) (memArea.address))->base.s.ptr = /*freep*/  ((memAreaHeader_t *) (memArea.address))->freep = prevp = /*&base*/ memArea.address;
	   /*base*/((memAreaHeader_t *) (memArea.address))->base.s.size = 0;
    	//	memcpy(memArea.address, &memHeader, sizeof(memAreaHeader_t));
    }
    for( p=prevp->s.ptr; ; prevp = p, p = p->s.ptr ){
	   if( p->s.size >= nunits ){
		  if( p->s.size == nunits )
			 prevp->s.ptr = p->s.ptr;
		  else{
		 	 p->s.size -= nunits;
		 	 p += p->s.size;
			 p->s.size = nunits;
			((memAreaHeader_t *) (memArea.address))->freep  = prevp;
		//	memHeader.freep/*freep*/ = prevp;
			 /**/

    			//memcpy(memArea.address, &memHeader, sizeof(memAreaHeader_t));
		 	 return (void *)(p+1);
	   	}
	   }
	   if(p ==/* freep*/ ((memAreaHeader_t *) (memArea.address))->freep){
    			//memcpy(memArea.address, &memHeader, sizeof(memAreaHeader_t));
		  if((p=baseMoreCore(nunits, memAdmin, memArea)) == NULL){
			 /**/

    			//memcpy(memArea.address, &memHeader, sizeof(memAreaHeader_t));
			 return NULL;
		  }
	   }
    }
}


/*
 * 	Public Functions
 * 	================
 */
void sysFree(void *ap, memArea_t (*getMemoryArea)()){
    baseFree(ap, getMemoryArea());
}

void *sysRealloc(void *ptr, size_t size, memArea_t (*getMemoryArea)()){
	void *ret;

	if ( ptr == NULL ) {
		*((char *)(0x0B8000)) = 'J';
		//_debug();
		return sysMalloc(size, getMemoryArea);
	}
		*((char *)(0x0B8000)) = 'H';
		//_debug();
	if( (ret=sysMalloc(size, getMemoryArea)) != NULL ){
	    memcpy(ret, ptr, size);
	    sysFree(ptr, getMemoryArea);
	}
	return ret;
}

void *sysMalloc(size_t nbytes, memArea_t (*getMemoryArea)()){
 	memArea_t memArea= getMemoryArea();
	memAreaHeader_t memHeader;
	if( ((memAreaHeader_t *) (memArea.address))->freep == NULL ){
		((memAreaHeader_t *) (memArea.address))->point = memArea.address + sizeof(memAreaHeader_t);
	}
	memcpy(&memHeader, memArea.address, sizeof(memAreaHeader_t));

	/*
	*((Header **) memArea.address) = memArea.address;// sets the first header space 
	*((Header **) (memArea.address + sizeof(Header))) = NULL; // sets the free list pointer to NULL 
	*((Header **) (memArea.address + sizeof(Header) + sizeof(Header *))) = memArea.address + sizeof(Header) + 2*sizeof(Header *);
	*/
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "Setting header to: ", TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "(addres of memory area: ", FALSE);
	hexa_debug(BASE_MALLOC_DEBUG, NULL, memArea.address, TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "base: ", FALSE);
	if( memHeader.base.s.ptr == NULL )
		hexa_debug(BASE_MALLOC_DEBUG, NULL, 0, TRUE);
	else
		hexa_debug(BASE_MALLOC_DEBUG, NULL, *((int *)memHeader.base.s.ptr), TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "size: ", FALSE);
	int_debug(BASE_MALLOC_DEBUG, NULL, memHeader.base.s.size, TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "freep: ", FALSE);
	if( memHeader.freep == NULL )
		hexa_debug(BASE_MALLOC_DEBUG, NULL, 0, TRUE);
	else
		hexa_debug(BASE_MALLOC_DEBUG, NULL, *((int *) memHeader.freep), TRUE);
	str_debug(BASE_MALLOC_DEBUG, __FUNCTION__, "pointer: ", TRUE);
	hexa_debug(BASE_MALLOC_DEBUG, __FUNCTION__, 	memHeader.point, TRUE);

	return baseMalloc(nbytes, memAdmin, memArea);
}
