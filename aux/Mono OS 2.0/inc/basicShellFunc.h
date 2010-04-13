#define TAG_SEP_AS_STRING "*"
#define TAG_SEP_AS_CHAR '*' 
#define FILE_SEP_AS_CHAR ':'
#define FILE_SEP_AS_STRING ":"

#define BUILD_REAL_PATH_SIZE_ERROR -1


/**
 *	@fn int isTagPathValid(char *tagPath)
 *	@brief	Checks if the given tagPath is valid by checking that the last tag added does
 *			not appear in the rest of the tag path
 */
int isTagPathValid(char *tagPath);


int isTagNameValid(char *tagName);


int buildFileWithContext(char *string, int maxLengthOfString);


int buildRealPath(char *string, int maxLengthOfString);
