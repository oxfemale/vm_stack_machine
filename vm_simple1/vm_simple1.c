// vm_simple1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <Windows.h>

#include "simple-vm.h"


static void hehe(void);

void error(char *msg)
{
    fprintf(stderr, "ERROR running script - %s\n", msg);
    exit(1);
}


int getlength(void *funcaddress);

int getlength(void *funcaddress)
{
    int length = 0;
    for (length = 0; *((UINT32 *)(&((unsigned char *)funcaddress)[length])) != 0xCCCCCCCC; ++length);
    return length;
}

int run_file2(int instructions)
{
    //int vm_code = (*(int(*)()) hehe)();
    void* vm_code = *hehe;

    int size = getlength((void*)hehe);
    unsigned char *code = (unsigned char *)malloc(size);
    memset(code, '\0', size);    
    memcpy(code, vm_code, size);

    printf("Code size: %d\r\n", size);
    svm_t *cpu = svm_new(code, size);
    if (!cpu)
    {
        printf("Failed to create virtual machine instance.\n");
        return 1;
    }

    /**
    * Set the error-handler.
    */
    svm_set_error_handler(cpu, &error);


    /**
    * Run the bytecode.
    */
    svm_run_N_instructions(cpu, instructions);


    /**
    * Dump?
    */

    /**
    * Cleanup.
    */
    svm_free(cpu);
    free(code);
    return 0;
}

int run_file(const char *filename, int instructions)
{
    struct stat sb;

    if (stat(filename, &sb) != 0)
    {
        printf("Failed to read file: %s\n", filename);
        return 1;
    }

    int size = sb.st_size;

    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Failed to open program-file %s\n", filename);
        return 1;
    }

    unsigned char *code = (unsigned char *)malloc(size);
    if (!code)
    {
        printf("Failed to allocate RAM for program-file %s\n", filename);
        fclose(fp);
        return 1;
    }
    memset(code, '\0', size);

    /**
    * Abort on a short-read, or error.
    */
    size_t read = fread(code, 1, size, fp);
    if (read < 1 || (read < (size_t)size))
    {
        fprintf(stderr, "Failed to wholly read input file\n");
        fclose(fp);
        return 1;
    }
    fclose(fp);

    svm_t *cpu = svm_new(code, size);
    if (!cpu)
    {
        printf("Failed to create virtual machine instance.\n");
        return 1;
    }

    /**
    * Set the error-handler.
    */
    svm_set_error_handler(cpu, &error);


    /**
    * Run the bytecode.
    */
    svm_run_N_instructions(cpu, instructions);


    

    /**
    * Cleanup.
    */
    svm_free(cpu);
    free(code);
    return 0;
}

void op_custom(struct svm *svm)
{
    printf("\nCustom Handling Here\n");
    printf("\tOur bytecode is %d bytes long\n", svm->size);

    /* handle the next instruction */
    svm->ip += 1;
}

unsigned char bytecode[] =
"\x30\x01\x0C\x00\x49\x20\x6C\x69\x6B\x65\x20\x6C\x6F\x6F\x70\x73\x31\x01\x10\x00\x00";

int run_vm()
{/*
    void* vm_code = *hehe;
    
    int size = getlength((void*)hehe);
    unsigned char *code = (unsigned char *)malloc(size);
    memset(code, '\0', size);
    memcpy(code, vm_code, size);
    svm_t *cpu = svm_new(code, size);
    */

    int size = sizeof(bytecode) / sizeof(bytecode[0]);

    svm_t *cpu = svm_new(bytecode, size);
    if (!cpu)
    {
        printf("Failed to create virtual machine instance.\n");
        return 1;
    }

    /**
    * Allow our our custom handler to be called via opcode 0xCD.
    */
    //cpu->opcodes[0xCD] = op_custom;

    /**
    * Run the bytecode.
    */
    svm_run(cpu);

    /**
    * Cleanup.
    */
    svm_free(cpu);
    return 0;
}

/**
* Simple driver to launch our virtual machine.
*
* Given a filename parse/execute the opcodes contained within it.
*
*/
int main(int argc, char **argv)
{
    int max_instructions = 0;
    //push(1);
    //run_file2(0);
    //exit(0);

    //run_vm();
    
    if (argc < 2)
    {
        printf("Usage: %s input-file\n", argv[0]);
        return 0;
    }

    if (argc > 2)
        max_instructions = atoi(argv[2]);

    return (run_file(argv[1], max_instructions));
    
    
    return 9;
}

static void hehe(void)
{
    MessageBoxA(0, "a", "b", 0);
    int Zabor = 1;
    Zabor = Zabor + 2;
    printf("hello world: %d\r\n", Zabor);
    printf("\t=====================\t\r\n");
    return;
}
