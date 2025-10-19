#ifndef UTILITY_H_
#define UTILITY_H_

#define _STRINGIFY(x) #x
#define _VARNAME_TO_STRING(x) _STRINGIFY(x)

#define _resolve_root_dir(varname, rpath, how_to_exit) \
    file_path_t varname; \
    if (snprintf(varname, FILE_PATH_SIZE, "%s/%s", CORRECT_ROOT_DIR, rpath) < 0) { \
        printf("resolve_root_dir: snprintf failed on variable named \"%s\".", _VARNAME_TO_STRING(varname)); \
        how_to_exit; \
    }

#define resolve_root_dir(varname, rpath) _resolve_root_dir(varname, rpath, FAIL() << "snprintf failed")

#define _resolve_project_dir(varname, rpath, how_to_exit) \
    file_path_t varname; \
    if (snprintf(varname, FILE_PATH_SIZE, "%s/%s", PROJECT_DIR, rpath) < 0) { \
        printf("resolve_project_dir: snprintf failed on variable named \"%s\".", _VARNAME_TO_STRING(varname)); \
        how_to_exit; \
    }

#define resolve_project_dir(varname, rpath) _resolve_project_dir(varname, rpath, FAIL() << "snprintf failed")

#define create_string_of_size(varname,size) \
    char *varname = (char*)malloc(size); \
    if (varname == nullptr) { \
        printf("create_string_of_size: failed to malloc string variable named \"%s\".", _VARNAME_TO_STRING(varname)); \
        FAIL() << "malloc failed"; \
    } \
    for (int i=0; i<size-1; ++i) \
        varname[i] = 'a'; \
    varname[size-1] = '\0'

#endif