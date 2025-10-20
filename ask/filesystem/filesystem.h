#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#define FILE_PATH_SIZE 256u
typedef char file_path_t[FILE_PATH_SIZE];

/** Resolves to "$HOME/.ask" */
extern file_path_t ROOT_DIR;
extern unsigned int ROOT_DIR_LENGTH;

/** Resolves to "$HOME/.ask/saved" */
extern file_path_t SAVE_DIR;
extern unsigned int SAVE_DIR_LENGTH;

/** Actual size in bytes */
extern unsigned int MAX_SUBDIRECTORY_SIZE;
/** Actual size in bytes */
extern unsigned int MAX_CONVERSATION_SIZE;

int filesystem_resolve_constants();
int filesystem_resolve_path(const char* rel_path, file_path_t* output);
int filesystem_is_allowed_path(file_path_t* p);

#endif