#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


//defined variables that can be changed depending on the case you intend to use them
#define ENDING_MARK "<end_init>\n"
#define BUFFER_SIZE 64
#define STRING_SIZE 131072
#define ARCHIVATION_FOLDER "D:\\programs\\programe saves\\save Code Blocks\\saves\\binary_files\\archive_me"
#define ARCHIVATION_NAME "C:\\Users\\PREDATOREL\\Desktop\\archived.tar"
#define DEZARCHIVATION_FOLDER "C:\\Users\\PREDATOREL\\Desktop\\dezarchived_me"


///UNarchive the specified path, 1st parameter is the  archived file name path, 2nd parameter is the folder path where it need to be UNarchived
int unarchive(char *path, char *dezarchivation_path)
{
    //create the new folder / directory
    int check = mkdir(dezarchivation_path);
    if (!check) printf("Directory created*\n");
    else printf("Unable to create directory or it already exists\n");

    //needed variables
    char buffer[BUFFER_SIZE];
    FILE *archive_file;
    char file_name[BUFFER_SIZE][BUFFER_SIZE];
    int count_files = 0;

    if (!(archive_file = fopen(path , "rb" )))
    {
        printf("Failed to open RB source.\n");
        exit(1);
    }

    //save the names of the files and consume them
    while(1)
    {
        //read a new line and check if it is the end of the initializations
        fgets(buffer, BUFFER_SIZE, archive_file);
        if(strcmp(buffer, ENDING_MARK) == 0) break;

        //save the names
        for(int i=0; i<strlen(buffer)-1; i++)
        {
            file_name[count_files][i] = buffer[i];
        }

        //eliminate the last (\n) new line characters
        file_name[count_files][strlen(buffer)-1] = '\0';
        count_files++;
    }

    //go through every file
    for(int i=0; i<count_files; i++)
    {
        printf("%s\n",file_name[i]);

        //here we create a new file for every file we have archived
        char *location = (char*)malloc(sizeof(char) * (strlen(dezarchivation_path) + strlen(file_name[i]) + 2));
        strcpy(location, dezarchivation_path);
        location[strlen(dezarchivation_path)+1] = '\0';
        location[strlen(dezarchivation_path)] = '\\';
        strcat(location, file_name[i]);
        location[strlen(location)] = '\0';

        //new buffer where we save all our values
        char save_buffer[STRING_SIZE];
        int current_buffer_size = 0;

        printf("%s\n",location);

        FILE *output_file = fopen(location, "w");
        if (output_file == NULL)
        {
            printf("W opening error\n");
            return -1;
        }

        //while running variable is true we did not find the ending mark
        int running = 1;
        //length of the ending mark
        int matches_ending_mark = 0;

        //go through file and find the separator ending mark
        while(!feof(archive_file) && running)
        {
            //we read character by character
            int nr = fread(buffer, 1 ,1, archive_file);
            printf("%c", buffer[nr-1]);

            //we check if we reached our terminator sign
            if(buffer[nr-1] == ENDING_MARK[matches_ending_mark])
            {
                matches_ending_mark ++;
                if(matches_ending_mark == strlen(ENDING_MARK))
                {
                    printf("~ENDING FOUND~\n\n");
                    running = 0;
                    break;
                }
            }
            else matches_ending_mark = 0;

            //save the characters in the new buffer
            if(matches_ending_mark == 0)
            {
                save_buffer[current_buffer_size] = buffer[nr-1];
                current_buffer_size ++;
            }
        }

        //check for errors -> if any ending terminator was wrong or modified
        //if the buffer of strings ended without finding a terminator => error somewhere
        if(running != 0)
        {
            printf("\nWARNING: .tar file was corrupted.\n");
            //close everything that was left opened
            fclose(output_file);
            fclose(archive_file);

            //delete any already written files
            for(int i=0; i<count_files; i++)
            {
                //here we create a new file for every file we have archived
                char *location = (char*)malloc(sizeof(char) * (strlen(dezarchivation_path) + strlen(file_name[i]) + 2));
                strcpy(location, dezarchivation_path);
                location[strlen(dezarchivation_path)+1] = '\0';
                location[strlen(dezarchivation_path)] = '\\';
                strcat(location, file_name[i]);
                location[strlen(location)] = '\0';

                //remove every specific path
                if (remove(location) == 0) printf("%s was deleted successfully -path: %s\n", file_name[i], location);
                else printf("WARNING: Unable to delete the file: %s\n", location);
            }

            //delete the entire folder / directory
            int status = rmdir(dezarchivation_path);
            if(!status)printf("FOLDER path: %s was deleted successfully\n", dezarchivation_path);
            else printf("WARNING: Unable to delete the folder: %s\n", dezarchivation_path);

            return -5;
        }

        //save the entire text
        fwrite(save_buffer, 1, current_buffer_size, output_file);
        //close the file
        fclose(output_file);
    }

    //close the binary file
    fclose(archive_file);
    return 0;
}


///archive the specified path, 1st parameter is the folder path that needs to be archived, 2nd parameter is the new archived file name path
int archive(char *path, char *archivation_path)
{
    //open the current directory
    struct dirent *dp;
    DIR *dfd;
    //save the current path of the files
    char *dir;
    dir = path;

    //save the file content to the new file
    char new_string[STRING_SIZE];
    int current_buffer_size = 0;


    //open the directory
    if ((dfd = opendir(dir)) == NULL)
    {
        fprintf(stderr, "Can't open %s\n", dir);
        return -1;
    }

    //open and create the new binary file and check for opening errors
    FILE *output_file = fopen(archivation_path, "wb");
    if (output_file == NULL)
    {
        printf("WB opening error\n");
        return -1;
    }

    //go through every file from the folder, assuming we'll only have normal files so we won't have to check if we have other directories
    while ((dp = readdir(dfd)) != NULL)
    {
        //skip the hidden files
        if(dp->d_name[0] == '.') continue;

        //here we will save the actual texts from every individual files
        char *name = dp->d_name;
        printf("%s\n",name);

        //create the new file path for every file in the specified directory
        char *new_path = malloc(sizeof(char) * (strlen(path) + strlen(dp->d_name) + 2));

        strcpy(new_path, path);
        new_path[strlen(new_path)+1] = '\0';    //not necessarily but just to be safe
        new_path[strlen(new_path)] = '\\';
        strcat(new_path, dp->d_name);
        new_path[strlen(new_path)] = '\0';

        //open every file you need to read
        FILE *input_file = fopen(new_path,"r");
        if (input_file == NULL)
        {
            printf("R opening error\n");
            return -2;
        }

        //save the name to file
        fwrite(dp->d_name, 1, strlen(dp->d_name), output_file);

        char buffer[BUFFER_SIZE];
        int count_size = 0;

        while (!feof(input_file))
        {
            int nr = fread(buffer, 1 ,BUFFER_SIZE, input_file);
            //fwrite(buffer, 1, nr, output_file);

            for(int i=0; i<nr; i++)
            {
                new_string[current_buffer_size] = buffer[i];
                current_buffer_size ++;
                count_size ++;
            }
        }

        //add a new ending mark after each file text to delimit them
        for(int i=0; i<strlen(ENDING_MARK); i++)
        {
            new_string[current_buffer_size] = ENDING_MARK[i];
                current_buffer_size ++;
        }

        //add a newline
        char c = '\n';
        fwrite(&c, sizeof(char), 1, output_file);

        //close the current file
        fclose(input_file);
    }

    //mark the ending of the headers
    fwrite(ENDING_MARK, 1, strlen(ENDING_MARK), output_file);
    //save the entire text
    fwrite(new_string, 1, current_buffer_size, output_file);

    //close the archived .tar file
    fclose(output_file);
    return 0;
}


int main()
{
    printf("Program starting ..\n");

    printf("\narchiving...\n\n");
    archive(ARCHIVATION_FOLDER, ARCHIVATION_NAME);

    printf("\nunarchiving...\n\n");
    unarchive(ARCHIVATION_NAME, DEZARCHIVATION_FOLDER);

    return 0;
}
