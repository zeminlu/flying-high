/*
 *	Function: ssize_t write(int fd, const void *buf, size_t count) 
 *
 *	 Description:
 *		Writes on File Descriptor the buffer
 *	Parameters:
 *		fd = File Descriptor (fd)
 *		buf=  buffer [memory address]
 *		count= size of buffer [bytes]
 * 	Returns:	
 *		amount of bytes written
 *	
 *	Programmer:
 *		Bermudez, Perez De Rosso, Pizzagalli
 *
 *	Version:	
 *	23.05.09.15.29
 */
ssize_t write(int fd, const void *buf, size_t count);

/*
 * Function: ssize_t read(int fd, const void *buf, size_t count) 
 *
 *	Description:
 *		Reads on File Descriptor the buffer
 *
 *	Parameters:
 *		fd = File Descriptor (fd)
 *		buf=  buffer [memory address]
 *		count= size of buffer [bytes]
 * 	Returns:	
 *		amount of bytes read 
 *		
 *	Programmer:
 *		Bermudez, Perez De Rosso, Pizzagalli
 *
 *	Version:	
 *		23.05.09.15.29
 */
ssize_t read(int fd, const void *buf, size_t count);

