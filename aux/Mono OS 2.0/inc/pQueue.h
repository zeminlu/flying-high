#ifndef _PQUEUE_H_
#define _PQUEUE_H_

typedef struct pQueueCDT *pQueueADT;

#define ERROR 0
#define OK 1


/**
 *  * 	\fn		pQueueADT newPQueue(void (*freeData)(void *), void *(*cpyData)(void *))
 *   * 	\brief	Creates a new priority queue, \a freeData and \a cpyData are function pointers for
 *    * 			freeing and copying the data in use in the priority queue
 *     *
 *      * 	\pre	\a freeData and \a cpyData should be valid function pointers
 *       * 	\return	a heap or \c NULL if some error was encountered
 *        *
 *         */
pQueueADT newPQueue(void (*freeData)(void *), void *(*cpyData)(void *));


/**
 *  * 	\fn		void freePQueue(pQueueADT pQueue)
 *   * 	\brief	frees the priority queue passed as parameter
 *    * 	\pre	\a pQueue should correspond to a valid priority queue
 *     * 	\post	all the memory used by the queue is freed
 *      */
void freePQueue(pQueueADT pQueue);

/**
 *  * 	\fn		int pEnqueueElem(pQueueADT pQueue, void *data, float priority)
 *   * 	\brief	Enqueues the \a data passed as parameter in a position of the
 *    * 			\a pQueue depending on its \a priority. Low priorities will be
 *     * 			dequeued first.				\n
 *      * 			Use:
 *       * 			\code
 *        * 					pQueueADT pq=newPQueue(freeInt, cpyInt);
 *         * 					int a=2, b=3, c=2;
 *          * 					pEnqueueElem(pq, &a, 32.112);
 *           * 					pEnqueueElem(pq, &b, 32.120);
 *            * 					pEnqueueElem(pq, &a, 35);
 *             * 			\endcode
 *              * 	\pre	\a pQueue should be a valid queue
 *               * 	\post	the \a data is correctly enqueued in the priority queue depending on its priority
 *                * 	\return	\c OK or \c ERROR if some error was encountered
 *                 */
int pEnqueueElem(pQueueADT pQueue, void *data, float priority);


/**
 *  * 	\fn		void *pDequeue(pQueueADT pQueue, int *stat)
 *   * 	\brief	dequeues the first element from the queue \a pQueue and notifies in \a stat if an error has occured by saving \c ERROR
 *    * 			in it or \c OK otherwise. WARNING: this function returns a cpy that should be freed after use.  \n
 *     * 			Use:
 *      * 			\code
 *       * 					pQueueADT pQueue=newPQueue(freeData, cpyData);
 *        * 					int a=2, *ret, stat;
 *         * 					pEnqueue(pQueue, &a);
 *          * 					ret=pDequeue(pQueue, &stat);
 *           * 					if( *stat != ERROR )
 *            * 						printf("%d \n", *ret);
 *             * 			\endcode
 *              * 	\pre	\a pQueue should be a valid priority queue
 *               * 	\return	the element dequeued or \c NULL. The \c NULL value could occurr because the queue is empty or because an error
 *                * 			occurred. If an error occurs \c ERROR is saved in stat or \c OK otherwise
 *                 */
void *pDequeue(pQueueADT pQueue, int *error);


void *pDequeueIfEqual(pQueueADT pQueue, int (*cmpFn)(void *data, void *data1), void *data1);
/**
 *  * 	\fn		int pQueueIsEmpty(pQueueADT pQueue)
 *   * 	\brief	indicates wether the given priority queue is empty or not
 *    * 	\pre	\a pQueue should be a valid priority queue
 *     * 	\return	1 if the given priority queue is empty or 0 otherwise
 *      */
int pQueueIsEmpty(pQueueADT);


#endif
