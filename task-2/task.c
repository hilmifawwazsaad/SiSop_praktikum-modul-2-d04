#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>

char *download_file(char *url, char *output_name)
{
    char *argz[] = {"wget", "--no-check-certificate", "-O", output_name, url, NULL};

    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        // Child process: execute wget command
        execv("/usr/bin/wget", argz);
        perror("wget failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process: wait for the child to finish
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            printf("Download successful\n");
        }
        else
        {
            printf("Download failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    // Return the output filename
    return strdup(output_name);
}

void create_directory(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1)
    {
        // Directory does not exist, create it
        if (mkdir(path, 0777) == -1)
        {
            perror("Failed to create directory");
            exit(EXIT_FAILURE);
        }
        printf("Created directory: %s\n", path);
    }
    else
    {
        printf("Directory already exists: %s\n", path);
    }
}

void unzip_file(char *filename, char *path)
{
    // Check if the directory already exists
    struct stat st;
    if (stat(path, &st) == -1)
    {
        // Directory does not exist, create it
        if (mkdir(path, 0777) == -1)
        {
            perror("Failed to create directory");
            exit(EXIT_FAILURE);
        }
        printf("Created directory: %s\n", path);
    }

    char *arguments[] = {"unzip", "-o", "-q", filename, "-d", path, NULL};

    pid_t pid;
    pid = fork();

    if (pid == 0)
    {
        // Child process: execute unzip command
        execv("/usr/bin/unzip", arguments);
        perror("unzip failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        // Parent process: wait for the child to finish
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            printf("Unzip successful\n");
        }
        else
        {
            printf("Unzip failed\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
}

void search_txt_files(const char *dir_path)
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(dir_path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            // Skip "." and ".." entries
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
            {
                char full_path[PATH_MAX];
                snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, ent->d_name);

                // If directory, recursively search
                if (ent->d_type == DT_DIR)
                {
                    search_txt_files(full_path);
                }
                else
                {
                    // Check if the file ends with ".txt"
                    if (strstr(ent->d_name, ".txt") != NULL)
                    {
                        printf("Found TXT file: %s\n", ent->d_name);
                    }
                }
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    char *url = "https://dl.dropboxusercontent.com/scl/fi/gmy0qvbysh3h7vdexso9k/task_sisop.zip?rlkey=ism9qfjh82awg2tbtzfbrylg4&dl=0";
    char *output_filename = "task_sisop.zip";
    char *path = "task";

    // Download the file
    char *downloaded_file = download_file(url, output_filename);

    // Unzip the downloaded file into a directory named "task"
    create_directory(path);
    unzip_file(downloaded_file, path);

    // Clean up: remove the downloaded ZIP file
    if (remove(downloaded_file) != 0)
    {
        perror("Failed to remove downloaded file");
    }
    else
    {
        printf("Removed downloaded file: %s\n", downloaded_file);
    }

    // Search for TXT files in the extracted directory
    search_txt_files(path);

    // Free allocated memory
    free(downloaded_file);

    return 0;
}
