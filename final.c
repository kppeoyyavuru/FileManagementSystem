#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct Node {
    char name[100];
    int isFile;
    struct Node* children;
    struct Node* sibling;
} Node;

Node* createNode(const char* name, int isFile) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        strncpy(newNode->name, name, sizeof(newNode->name));
        newNode->isFile = isFile;
        newNode->children = NULL;
        newNode->sibling = NULL;
    }
    return newNode;
}

// Function to add a child (file or folder) to a parent node
void addChild(Node* parent, Node* child) {
    if (parent == NULL || child == NULL) {
        return;
    }
    child->sibling = parent->children;
    parent->children = child;
}

// Function to print the file-folder structure using a recursive approach
void printStructure(Node* root, int level) {
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    if (root->isFile) {
        printf("- %s (File)\n", root->name);
    } else {
        printf("- %s (Folder)\n", root->name);
        Node* child = root->children;
        while (child != NULL) {
            printStructure(child, level + 1);
            child = child->sibling;
        }
    }
}



// Function to change the current folder (similar to cd and cd.. in Linux)
Node* changeDirectory(Node* currentFolder, const char* folderName, Node* root) {
    if (currentFolder == NULL) {
        printf("Error: Current folder is not set.\n");
        return currentFolder;
    }

    if (strcmp(folderName, "..") == 0) {
        // Move up to the parent folder
        if (currentFolder != root) {
            Node* parent = root;
            Node* child = root->children;
            while (child != NULL) {
                if (child->children == currentFolder) {
                    return parent;
                }
                parent = child->children;
                child = child->sibling;
            }
            return root;  // Already at the root folder
        } else {
            printf("Error: Already at the root folder.\n");
            return currentFolder;
        }
    } else if (strcmp(folderName, ".") == 0) {
        // Stay in the current folder (no change)
        return currentFolder;
    } else {
        // Move to a child folder with the specified name
        Node* child = currentFolder->children;
        while (child != NULL) {
            if (strcmp(child->name, folderName) == 0 && !child->isFile) {
                return child;
            }
            child = child->sibling;
        }
        printf("Error: Folder '%s' not found.\n", folderName);
        return currentFolder;
    }
}

void freeStructure(Node* root) {
    if (root == NULL) {
        return;
    }

    Node* child = root->children;
    while (child != NULL) {
        Node* next = child->sibling;
        freeStructure(child);
        child = next;
    }

    free(root);
}

void listFilesAndFolders(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullpath[1024]; // Adjust the size as needed
        struct stat info;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (lstat(fullpath, &info) == -1) {
            perror("lstat");
            continue;
        }

        if (S_ISDIR(info.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                printf("Folder: %s\n", entry->d_name);
            }
        } else if (S_ISREG(info.st_mode)) {
            printf("File: %s\n", entry->d_name);
        }
    }
    printf("\n");
    closedir(dir);
}

void createFile(const char *path, const char *filename) {
    char fullpath[1024]; // Adjust the size as needed
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, filename);
    FILE *file = fopen(fullpath, "w");
    if (file != NULL) {
        fclose(file);
        printf("File created: %s\n", fullpath);
    } else {
        perror("fopen");
    }
}

void createFolder(const char *path, const char *foldername) {
    char fullpath[1024]; // Adjust the size as needed
    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, foldername);
    if (mkdir(fullpath, 0755) == 0) {
        printf("Folder created: %s\n", fullpath);
    } else {
        perror("mkdir");
    }
}



void Option1() {
 
  Node* root = createNode("Root", 0);

  Node* currentFolder = root;

  printf("Welcome to File Folder Structure made to manage files and folders using Trees as a data structure in C\n");
  printf("We are on the Root Directory now\n\n");
  int choice;

  do {
    printf("1. Add a file to the current folder\n");
    printf("2. Add a sub folder inside the current folder\n");
    printf("3. Change the Directory (Equivalent to 'cd' or 'cd ..' in Unix)\n");
    printf("4. Print the current File Structure from root\n");
    printf("5. Exit (Free Structure)\n\n"); 
    printf("Enter your choice:");
    scanf("%d",&choice); printf("\n");
    switch (choice) {
      case 1:
        printf("Enter the Name of the File (Less than 100 characters)\n");
        char name[100];
        scanf("%s",name);
        Node* file = createNode(name, 1);
        addChild(currentFolder, file);
        printf("The file has been added to the current folder\n\n");
        break;

      case 2:
        printf("Enter the name of the folder (Less than 100 characters)\n");
        char names[100];
        scanf("%s",names);
        Node* folder = createNode(names, 0);
        addChild(currentFolder, folder);
        printf("The folder has been added\n\n"); break;
      
      case 3:
        printf("Enter the folder name or '..' to move up\n"); 
        char namef[100];
        scanf("%s",namef);
        currentFolder = changeDirectory(currentFolder, namef, root);
        printf("In folder: %s", currentFolder->name);
        printf("\n\n"); break;

      case 4:
        printf("The Current file-folder structure is:\n\n");
        printStructure(root, 0); 
        printf("\n\n");
        break;
      
      case 5:
        freeStructure(root);
        exit(0);
      printf("\n\n");
    }
  }while (choice < 6); 
}

void Option2(){
 char currentDirectory[1024];
    getcwd(currentDirectory, sizeof(currentDirectory));

    printf("Welcome to the File and Folder System using C!\n");
    printf("Any files and folders made using this program will be reflected in your actual machine OS");
    printf("\nCurrent Directory: %s\n\n", currentDirectory);

    while (1) {
        printf("This is the List of Functions that can be performed:\n");
        printf("1. List files and folders\n");
        printf("2. Create a new file\n");
        printf("3. Create a new folder\n");
        printf("4. Change directory (cd)\n");
        printf("5. Go up one level (cd ..)\n");
        printf("6. Exit\n\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice); printf("\n");
        while (getchar() != '\n'); // Clear the input buffer

        switch (choice) {
            case 1:
                printf("The List of files and folders in the current Directory are:\n");
                listFilesAndFolders(currentDirectory);
                break;
            case 2:
                printf("Enter the name of the file to create: ");
                char newFile[256];
                fgets(newFile, sizeof(newFile), stdin);
                newFile[strcspn(newFile, "\n")] = '\0';
                createFile(currentDirectory, newFile);
                break;
            case 3:
                printf("Enter the name of the folder to create: ");
                char newFolder[256];
                fgets(newFolder, sizeof(newFolder), stdin);
                newFolder[strcspn(newFolder, "\n")] = '\0';
                createFolder(currentDirectory, newFolder);
                break;
            case 4:
                printf("Enter the name of the folder to enter: ");
                char folderToEnter[256];
                fgets(folderToEnter, sizeof(folderToEnter), stdin);
                folderToEnter[strcspn(folderToEnter, "\n")] = '\0';
                if (chdir(folderToEnter) == 0) {
                    getcwd(currentDirectory, sizeof(currentDirectory));
                    printf("Folder has been changed\n");
                } else {
                    perror("chdir");
                }
                break;
            case 5:
                if (chdir("..") == 0) {
                    getcwd(currentDirectory, sizeof(currentDirectory));
                    printf("The command 'cd..' has been executed");
                } else {
                    perror("chdir");
                }
                break;
            case 6:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please choose a valid option.\n");
        }

        printf("\n");
    }


}



int main(){
  printf("Welcome to the File-Folder System Made in C\n");
  printf("You have 2 Options:\n");
  printf("1. Create an entire new folder system using C\n");
  printf("2. Make Changes inside the File-Folder system of the current OS your machine is running\n");
  int choice;
  printf("\nEnter your choice:"); scanf("%d",&choice);
  
  switch (choice) {
    case 1:
      Option1();
      break;
    case 2:
      Option2();
      break;
    default:
      printf("None of the above choices have been selected, Please try again\n");

  }
}