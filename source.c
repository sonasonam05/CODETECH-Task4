//This is a simple program to compress and decompress text file using the RLE(Run Length Encoding) algorithm.it is a lossless algortihm for compression. it is best effective when characters are repetative.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILENAME 100

// function to get file size in bytes
long getFileSize(const char* filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return -1;
    }
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

// function to compress a file (Using RLE algorith)
void compressFile(const char* inputFile, const char* outputFile) {
    FILE *infile = fopen(inputFile, "r");

    if (!infile) {
        printf("Error: Could not open input file.\n");
        return;
    }

    FILE *outfile = fopen(outputFile, "w");
    if (!outfile) {
        fclose(infile);
        printf("Error: Could not create output file.\n");
        return;
    }
    char current, previous;
    int count = 1;

    previous = fgetc(infile);
    if (previous == EOF) {
        printf("Input file is empty.\n");
        fclose(infile);
        fclose(outfile);
        return;
    }

    while ((current = fgetc(infile)) != EOF) {
        if (current == previous) {
            count++;
        } else {
            fprintf(outfile, "%c%d", previous, count);
            previous = current;
            count = 1;
        }
    }
    fprintf(outfile, "%c%d", previous, count); // final write

    fclose(infile);
    fclose(outfile);

    long originalSize = getFileSize(inputFile);
    long compressedSize = getFileSize(outputFile);

    if (originalSize <= compressedSize) {
        remove(outputFile);
        printf("\nCompression Ineffective. Original file is smaller or equal in size.\n");
        printf("Original Size: %ld bytes\n", originalSize);
        printf("Compressed file was discarded.\n\n");
    } else {
        printf("\nFile Compressed Successfully!\n");
        printf("Original Size   : %ld bytes\n", originalSize);
        printf("Compressed Size : %ld bytes\n", compressedSize);
        printf("Compression Ratio: %.2f\n", (1.0 *  originalSize / compressedSize));
        printf("Output File     : %s\n\n", outputFile);
    }
}

// function to decompress file
void decompressFile(const char* inputFile, const char* outputFile) {
    FILE *infile = fopen(inputFile, "r");
    FILE *outfile = fopen(outputFile, "w");
    char ch;
    while ((ch = fgetc(infile)) != EOF) {
        if (!isalpha(ch) && !ispunct(ch)) continue; // Skip anything unexpected

        char countBuffer[20] = {0};
        int i = 0;

        char nextChar;
        while ((nextChar = fgetc(infile)) != EOF && isdigit(nextChar)) {
            if (i < sizeof(countBuffer) - 1)
                countBuffer[i++] = nextChar;
        }

        int count = atoi(countBuffer);
        for (int j = 0; j < count; j++) {
            fputc(ch, outfile);
        }

        if (nextChar != EOF)
            ungetc(nextChar, infile); // put back non-digit for next char
    }

    fclose(infile);
    fclose(outfile);
    printf("\nFile Decompressed Successfully!\n");
    printf("Output File: %s\n\n", outputFile);
}


// Main Menu
int main() {
    int choice;
    char inputFile[MAX_FILENAME], outputFile[MAX_FILENAME];

    printf("\n\t\tCompression Tool\n");
    printf("=============================================\n");
    printf("[1] Compress a File\n");
    printf("[2] Decompress a File\n");
    printf("[0] Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter input file path: ");
            scanf("%s", inputFile);
            printf("Enter output file path (e.g., compressed.txt): ");
            scanf("%s", outputFile);
            compressFile(inputFile, outputFile);
            break;

        case 2:
            printf("Enter compressed file path: ");
            scanf("%s", inputFile);
            printf("Enter output (decompressed) file path: ");
            scanf("%s", outputFile);
            decompressFile(inputFile, outputFile);
            break;

        case 0:
            printf("Goodbye!\n");
            break;

        default:
            printf("Invalid choice. Please try again.\n");
    }

    return 0;
}
