#include "poem_collector.h"

static const char* MENU_TEXT =  "--------------------------\n"
                                "| [1] Add a new poem     |\n"
                                "| [2] Print a poem       |\n"
                                "| [3] Print poems' title |\n"
                                "| [4] Delete a poem      |\n"
                                "| [5] Modify a poem      |\n"
                                "| [6] Exit               |\n"
                                "--------------------------\n"
                                "Write here your command: ";

void run(const char* filename) {
    bool running = true;
    const char* const separator = "-----";
    const char* const separatorWithNewLines = "\n-----\n";

    while(running) {
        printf("%s", MENU_TEXT);

        Poem* poem;
        int id;
        int inputValue = readNumberFromConsole();
        switch(inputValue) {
            case 1:
                printf("I am waiting for your poem (to stop it, press @ or EOF):\n");
                poem = readPoemFromConsole();
                if(poem == NULL) MemoryAllocationException();
                if(safeToFile(filename, poem, separatorWithNewLines)) {freePoem(poem); IOException(); }
                freePoem(poem);
            break;
            case 2:
                printf("Enter the id of the selected poem: ");
                id = readNumberFromConsole();
                poem = readFromFile(filename, id, separator);
                if (poem == NULL) IOException();
                if (strlen(poem->title) == 0) {
                    printf("The ID doesn't exists!\n");
                }
                else {
                    printPoem(poem);
                }

                freePoem(poem);
            break;
            case 3:
                id = 1;
                poem = readFromFile(filename, id, separator);
                if(poem == NULL) IOException();

                printf("titles:\n");
                while(strlen(poem->title) != 0) {
                    printf("%d: %s\n", id, poem->title);
                    freePoem(poem);

                    id += 1;
                    poem = readFromFile(filename, id, separator);
                    if(poem == NULL) IOException();
                }
                freePoem(poem);
            break;
            case 4:
                printf("Enter the id of the selected poem: ");
                id = readNumberFromConsole();
                freePoem(deleteFromFile(filename, id, separator));
            break;
            case 5:
                printf("Enter the id of the selected poem: ");
                id = readNumberFromConsole();
                poem = deleteFromFile(filename, id, separator);
                if (strlen(poem->title) == 0) {
                    printf("The ID doesn't exists!\n");
                }
                else {
                    printPoem(poem);

                    printf("Write here your new Poem: ");
                    poem = readPoemFromConsole();
                    if(poem == NULL) MemoryAllocationException();
                    if(safeToFile(filename, poem, separatorWithNewLines)) {freePoem(poem); IOException(); }
                }
                
                freePoem(poem);
            break;
            case 6:
                running = false;
            break;
        }
        
        printf("\n");
    }
}

Poem* readPoemFromConsole() {
    Poem* poem = (Poem*)malloc(sizeof(Poem));
    if(poem == NULL) return NULL;

    printf("title - ");
    poem->title = readStringFromConsole('\n');
    if(poem->title == NULL) { freePoem(poem); return NULL; }
    
    printf("content:\n");
    poem->content = readStringFromConsole('@');
    if(poem->content == NULL) { freePoem(poem); return NULL; }

    trim(poem->title);
    trim(poem->content);

    return poem;
}

char* readStringFromConsole(char end) {
    int buffer = 2;

    char* storage = (char*)malloc(buffer * sizeof(char));
    if(storage == NULL) { return NULL; }

    int i = 0;
    for(char c = getchar(); c != EOF && c != end; i += 1, c = getchar()) {
        if(i >= buffer-2) {
            buffer *= 2;
            storage = (char*)realloc(storage, buffer * sizeof(char));
            if(storage == NULL) { free(storage); return NULL; }
        }
        storage[i] = c;
    }
    storage[i] = '\0';

    return storage;
}

int readNumberFromConsole() {
    char line[16];
    fgets(line, sizeof(line), stdin);

    return atoi(line);
}

void trim(char* str) {
    int start = 0;
    while (isspace(str[start])) {
        start += 1;
    }

    if (str[start] == '\0') {
        str[0] = '\0';
        return;
    }

    int end = strlen(str)-1;
    while (isspace(str[end])) {
        end -= 1;
    }
    str[end + 1] = '\0';

    if (start > 0) {
        memmove(str, str + start, end - start + 2);
    }
}

// error functions

void MemoryAllocationException() {
    printf("Memory allocation error!");
    exit(-1);
}

void IOException() {
    printf("Input/Output error!");
    exit(-1);
}