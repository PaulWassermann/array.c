#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#define LOG_LEVEL ERROR_LEVEL

#include "..\src\log.h"
#include "..\src\utils.h"

#define XL_BUFFER_SIZE 8192
#define L_BUFFER_SIZE 2048
#define M_BUFFER_SIZE 512
#define S_BUFFER_SIZE 256
#define XS_BUFFER_SIZE 128
#define MAX_FILES 32
#define MAX_UNIT_TESTS_PER_FILE 64
#define SEARCH_DEPTH 8

char error_msg[S_BUFFER_SIZE] = {'\0'};

#define MIN(a, b) (a <= b) ? a : b
#define PRINT_USAGE_FAILURE(error_msg_fmt, ...)                                \
    snprintf(error_msg, S_BUFFER_SIZE, error_msg_fmt, __VA_ARGS__);            \
    print_usage(argv[0], error_msg, EXIT_FAILURE);
#define PRINT_USAGE_SUCCESS() print_usage(argv[0], NULL, EXIT_SUCCESS);

typedef struct CommandLineArgs {
    char template_file_path[M_BUFFER_SIZE];
    char tests_folder[M_BUFFER_SIZE];
    char output_file_path[M_BUFFER_SIZE];
} CommandLineArgs;

typedef struct FileList {
    char file_paths[MAX_FILES][L_BUFFER_SIZE];
    size_t nfiles;
} FileList;

typedef struct UnitTests {
    char names[MAX_UNIT_TESTS_PER_FILE][L_BUFFER_SIZE];
    size_t ntests;
} UnitTests;

// TESTS BUILDING FUNCTIONS
void get_tests_from_file(char *filename, UnitTests *file_tests);
void fill_template(char *buffer, size_t buffer_size, char *template,
                   FileList *files, UnitTests *tests);
void get_relative_path(char *dest, char *src, char *base);
void get_relative_paths(FileList *dest, FileList *src, char *base);
void read_file(char *buffer, size_t buffer_size, char *filename);
FILE *safe_open_file(char *filename, char *mode);
void walk_folder(char *folder_name, FileList *files, int depth);
void write_to_file(char *buffer, char *filename, char *mode);

// COMMAND LINE PARSING FUNCTIONS
void print_usage(char *exe, char *error_msg, int exit_code);
void parse_args(int argc, char *argv[], CommandLineArgs *out);

// TESTS BUILDING FUNCTIONS
void get_tests_from_file(char *filename, UnitTests *file_tests) {
    char line_buffer[XS_BUFFER_SIZE];
    char prefix[] = "void ";

    FILE *file = safe_open_file(filename, "r");

    while (fgets(line_buffer, XS_BUFFER_SIZE, file) != NULL) {
        if (strncmp(prefix, line_buffer, strlen(prefix)) == 0) {
            size_t bracket_index = 0;

            for (size_t i = strlen(prefix); i < strlen(line_buffer); i++) {
                if (line_buffer[i] == '(') {
                    bracket_index = i;
                    break;
                }
            }

            strncpy_s(file_tests->names[file_tests->ntests++], L_BUFFER_SIZE,
                      line_buffer + strlen(prefix),
                      bracket_index - strlen(prefix));
        }
    }

    fclose(file);
}

void fill_template(char *buffer, size_t buffer_size, char *template,
                   FileList *files, UnitTests *tests) {
    char *includes_buffer =
        SAFE_MALLOC(sizeof(*includes_buffer) * L_BUFFER_SIZE);
    char *tests_buffer = SAFE_MALLOC(sizeof(*tests_buffer) * XL_BUFFER_SIZE);
    char temp_buffer[M_BUFFER_SIZE];

    includes_buffer[0] = '\0';
    tests_buffer[0] = '\0';

    for (size_t i = 0; i < files->nfiles; i++) {
        snprintf(temp_buffer, M_BUFFER_SIZE, "#include \"%s\"\n",
                 files->file_paths[i]);
        strncat_s(includes_buffer, L_BUFFER_SIZE, temp_buffer,
                  L_BUFFER_SIZE - strlen(includes_buffer));
        for (size_t j = 0; j < tests[i].ntests; j++) {
            snprintf(temp_buffer, M_BUFFER_SIZE, "TEST(%s)\n    ",
                     tests[i].names[j]);
            strncat_s(tests_buffer, XL_BUFFER_SIZE, temp_buffer,
                      XL_BUFFER_SIZE - strlen(tests_buffer));
        }
    }

    snprintf(buffer, buffer_size, template, includes_buffer, tests_buffer);

    free(includes_buffer);
    free(tests_buffer);
}

void get_relative_path(char *dest, char *src, char *base) {
    char base_absolute[M_BUFFER_SIZE];
    char src_absolute[M_BUFFER_SIZE];

    char base_buffer[M_BUFFER_SIZE] = {'\0'};
    char src_buffer[M_BUFFER_SIZE] = {'\0'};

    GetFullPathNameA(base, M_BUFFER_SIZE, base_absolute, NULL);
    GetFullPathNameA(src, M_BUFFER_SIZE, src_absolute, NULL);

    size_t i;
    size_t offset;

    for (i = 0; i < MIN(strlen(base_absolute), strlen(src_absolute)); i++) {
        base_buffer[i] = base_absolute[i];
        src_buffer[i] = src_absolute[i];

        if (base_absolute[i] == '\\' || src_absolute[i] == '\\') {

            if (strcmp(base_buffer, src_buffer) != 0) {
                break;
            } else {
                offset = i + 1;
            }
        }
    }

    size_t nfolders = 0;
    for (size_t i = offset; i < strlen(base_absolute); i++) {
        if (base_absolute[i] == '\\') {
            nfolders++;
        }
    }

    dest[0] = '\0';

    for (size_t i = 0; i < nfolders; i++) {
        strncat_s(dest, M_BUFFER_SIZE, "..\\", M_BUFFER_SIZE - strlen(dest));
    }

    strncat_s(dest, M_BUFFER_SIZE, src_absolute + offset,
              M_BUFFER_SIZE - strlen(dest));
}

void get_relative_paths(FileList *dest, FileList *src, char *base) {

    for (size_t i = 0; i < src->nfiles; i++) {
        get_relative_path(dest->file_paths[i], src->file_paths[i], base);
    }
}

void read_file(char *buffer, size_t buffer_size, char *filename) {
    size_t read_size;
    FILE *template_file = safe_open_file(filename, "r");

    read_size = fread(buffer, sizeof(char), XL_BUFFER_SIZE, template_file);

    fclose(template_file);

    if (read_size >= XL_BUFFER_SIZE - 1) {
        LOG_ERROR("Template test file %s is to voluminous and couldn't be read "
                  "entirely",
                  filename);
        exit(EXIT_FAILURE);
    }

    buffer[read_size] = '\0';
}

FILE *safe_open_file(char *filename, char *mode) {
    FILE *file;
    if ((file = fopen(filename, mode)) == NULL) {
        LOG_ERROR("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }

    return file;
}

void walk_folder(char *folder_name, FileList *files, int depth) {
    if (depth < 0) {
        return;
    }

    LOG_DEBUG("Walking folder %s\n", folder_name);
    char current_path[L_BUFFER_SIZE];
    HANDLE find_handle;
    WIN32_FIND_DATAA find_file_data;

    snprintf(current_path, L_BUFFER_SIZE, "%s\\*", folder_name);

    find_handle = FindFirstFileA(current_path, &find_file_data);
    if (find_handle == INVALID_HANDLE_VALUE) {
        LOG_ERROR("FindFirstFileA function failed with error code: %lu",
                  GetLastError());
        exit(EXIT_FAILURE);
    }

    do {
        snprintf(current_path, L_BUFFER_SIZE, "%s\\%s", folder_name,
                 find_file_data.cFileName);
        if (strcmp(find_file_data.cFileName, ".") != 0 &&
            strcmp(find_file_data.cFileName, "..") != 0) {
            if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                walk_folder(current_path, files, depth - 1);
            } else if (strncmp(find_file_data.cFileName, "test_", 5) == 0) {
                strncpy_s(files->file_paths[files->nfiles], L_BUFFER_SIZE,
                          current_path, strlen(current_path));
                files->nfiles++;
            }
        }
    } while (FindNextFileA(find_handle, &find_file_data));

    FindClose(find_handle);
}

void write_to_file(char *buffer, char *filename, char *mode) {
    FILE *dest_file = safe_open_file(filename, mode);

    if (fputs(buffer, dest_file) == EOF) {
        LOG_ERROR("Could not write buffer to file '%s'", filename);
        exit(EXIT_FAILURE);
    }

    fclose(dest_file);
}

// COMMAND LINE PARSING FUNCTIONS
void print_usage(char *exe, char *error_msg, int exit_code) {
    if (exit_code == EXIT_FAILURE) {
        printf("Error: %s\n\n", error_msg);
    }

    printf("Usage: %s [tests folder] -t [template file] -o [output file]\n",
           exe);
    exit(exit_code);
}

void parse_args(int argc, char *argv[], CommandLineArgs *out) {
    char prev_flag;
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        PRINT_USAGE_SUCCESS();
    } else if (argc == 6) {

        strncpy_s(out->tests_folder, M_BUFFER_SIZE, argv[1], M_BUFFER_SIZE);

        for (size_t i = 2; i <= (argc - 2); i = i + 2) {
            if (argv[i][0] != '-') {
                PRINT_USAGE_FAILURE("argument passed without flag %s", argv[i]);
            }
            if (argv[i + 1][0] == '-') {
                PRINT_USAGE_FAILURE("no argument specified for flag %s",
                                    argv[i]);
            }
            switch (argv[i][1]) {
            case 't':
                if (prev_flag == 't') {
                    PRINT_USAGE_FAILURE("flag -t has already been passed",
                                        NULL);
                }
                strncpy_s(out->template_file_path, M_BUFFER_SIZE, argv[i + 1],
                          M_BUFFER_SIZE);
                break;
            case 'o':
                if (prev_flag == 'o') {
                    PRINT_USAGE_FAILURE("flag -o has already been passed",
                                        NULL);
                }
                strncpy_s(out->output_file_path, M_BUFFER_SIZE, argv[i + 1],
                          M_BUFFER_SIZE);
                break;
            default:
                PRINT_USAGE_FAILURE("unknown option %s", argv[i]);
            }
            prev_flag = argv[i][1];
        }
    } else {
        PRINT_USAGE_FAILURE(
            "wrong number of arguments; expected 5, received %d", argc - 1);
    }
}

int main(int argc, char *argv[]) {

    CommandLineArgs cl_args = {0};

    parse_args(argc, argv, &cl_args);

    FileList *files = SAFE_MALLOC(sizeof(*files));
    files->nfiles = 0;

    walk_folder(cl_args.tests_folder, files, SEARCH_DEPTH);

    for (size_t i = 0; i < files->nfiles; i++) {
        LOG_DEBUG("Found test file: %s\n", files->file_paths[i]);
    }

    UnitTests *tests = SAFE_MALLOC(sizeof(*tests) * files->nfiles);

    for (size_t i = 0; i < files->nfiles; i++) {
        get_tests_from_file(files->file_paths[i], &tests[i]);
    }

    for (size_t i = 0; i < files->nfiles; i++) {
        for (size_t j = 0; j < tests[i].ntests; j++) {
            LOG_DEBUG("File %s: found %s\n", files->file_paths[i],
                      tests[i].names[j]);
        }
    }

    FileList *relative_files = SAFE_MALLOC(sizeof(*relative_files));
    relative_files->nfiles = files->nfiles;

    get_relative_paths(relative_files, files, cl_args.output_file_path);

    for (size_t i = 0; i < files->nfiles; i++) {
        LOG_DEBUG("Given path: %s\n", files->file_paths[i]);
        LOG_DEBUG("Relative path: %s\n", relative_files->file_paths[i]);
    }

    char *template_string =
        SAFE_MALLOC(sizeof(*template_string) * L_BUFFER_SIZE);
    read_file(template_string, L_BUFFER_SIZE, cl_args.template_file_path);

    char *filled_template =
        SAFE_MALLOC(sizeof(*filled_template) * XL_BUFFER_SIZE);
    fill_template(filled_template, XL_BUFFER_SIZE, template_string,
                  relative_files, tests);

    write_to_file(filled_template, cl_args.output_file_path, "w");

    free(files);
    free(filled_template);
    free(template_string);
    free(tests);

    return EXIT_SUCCESS;
}