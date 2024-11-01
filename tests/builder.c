#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>

#include "..\src\log.h"
#include "..\src\utils.h"

#define BUFFER_SIZE 2048
#define LINE_MAX_LENGTH 128
#define MAX_ARG_SIZE 512
#define MAX_FILES 32
#define MAX_UNIT_TESTS_PER_FILE 64
#define SEARCH_DEPTH 8

typedef struct CommandLineArgs {
    char template_file_path[MAX_ARG_SIZE];
    char tests_folder[MAX_ARG_SIZE];
    char output_file_path[MAX_ARG_SIZE];
} CommandLineArgs;

typedef struct FileList {
    char file_paths[MAX_FILES][BUFFER_SIZE];
    size_t nfiles;
} FileList;

typedef struct UnitTests {
    char names[MAX_UNIT_TESTS_PER_FILE][BUFFER_SIZE];
    size_t ntests;
} UnitTests;

void get_tests_from_file(char *filename, UnitTests *file_tests) {
    char line_buffer[LINE_MAX_LENGTH];
    FILE *file;
    char prefix[] = "void ";

    if ((file = fopen(filename, "r")) == NULL) {
        LOG_ERROR("Could not open file '%s'", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(line_buffer, LINE_MAX_LENGTH, file) != NULL) {
        if (strncmp(prefix, line_buffer, strlen(prefix)) == 0) {
            size_t bracket_index = 0;

            for (size_t i = strlen(prefix); i < strlen(line_buffer); i++) {
                if (line_buffer[i] == '(') {
                    bracket_index = i;
                    break;
                }
            }

            strncpy_s(file_tests->names[file_tests->ntests++], BUFFER_SIZE,
                      line_buffer + strlen(prefix),
                      bracket_index - strlen(prefix));
        }
    }

    fclose(file);
}

void walk_folder(char *folder_name, FileList *files, int depth) {
    if (depth < 0) {
        return;
    }

    LOG_INFO("Walking folder %s\n", folder_name);
    char current_path[BUFFER_SIZE];
    HANDLE find_handle;
    WIN32_FIND_DATAA find_file_data;

    snprintf(current_path, BUFFER_SIZE, "%s\\*", folder_name);

    find_handle = FindFirstFileA(current_path, &find_file_data);
    if (find_handle == INVALID_HANDLE_VALUE) {
        LOG_ERROR("FindFirstFileA function failed with error code: %lu",
                  GetLastError());
        exit(EXIT_FAILURE);
    }

    do {
        snprintf(current_path, BUFFER_SIZE, "%s\\%s", folder_name,
                 find_file_data.cFileName);
        if (strcmp(find_file_data.cFileName, ".") != 0 &&
            strcmp(find_file_data.cFileName, "..") != 0) {
            if (find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                walk_folder(current_path, files, depth - 1);
            } else if (strncmp(find_file_data.cFileName, "test_", 5) == 0) {
                strncpy_s(files->file_paths[files->nfiles], BUFFER_SIZE,
                          current_path, strlen(current_path));
                files->nfiles++;
            }
        }
    } while (FindNextFileA(find_handle, &find_file_data));

    FindClose(find_handle);
}

void print_usage(char *exe, int exit_code) {
    if (exit_code == EXIT_FAILURE) {
        printf("Error while parsing args\n\n");
    }

    printf("Usage: %s [tests folder] -t [template file] -o [output file]\n",
           exe);
    exit(exit_code);
}

void parse_args(int argc, char *argv[], CommandLineArgs *out) {
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0], EXIT_SUCCESS);
    } else if (argc == 6) {
        for (size_t i = 2; i < (argc - 2); i = i + 2) {
            if (argv[i][0] != '-' || argv[i + 1][0] == '-') {
                print_usage(argv[0], EXIT_FAILURE);
            }
            switch (argv[i][1]) {
            case 't':
                strncpy_s(out->template_file_path, MAX_ARG_SIZE, argv[i + 1],
                          MAX_ARG_SIZE);
                break;
            case 'o':
                strncpy_s(out->output_file_path, MAX_ARG_SIZE, argv[i + 1],
                          MAX_ARG_SIZE);
                break;
            default:
                print_usage(argv[0], EXIT_FAILURE);
            }
        }
    } else {
        print_usage(argv[0], EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {

    CommandLineArgs cl_args = {0};

    parse_args(argc, argv, &cl_args);

    FileList *files = SAFE_MALLOC(sizeof(*files));
    files->nfiles = 0;

    walk_folder("tests", files, SEARCH_DEPTH);

    for (size_t i = 0; i < files->nfiles; i++) {
        LOG_INFO("Found test file: %s\n", files->file_paths[i]);
    }

    UnitTests *tests = SAFE_MALLOC(sizeof(*tests) * files->nfiles);

    for (size_t i = 0; i < files->nfiles; i++) {
        get_tests_from_file(files->file_paths[i], &tests[i]);
    }

    for (size_t i = 0; i < files->nfiles; i++) {
        for (size_t j = 0; j < tests[i].ntests; j++) {
            printf("File %s: found %s\n", files->file_paths[i],
                   tests[i].names[j]);
        }
    }

    free(files);
    free(tests);

    return EXIT_SUCCESS;
}