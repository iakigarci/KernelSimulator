/*═════════════════════════════════════════════════════════════════════════════ 
 *   heracles.c 
 *
 *      ./heracles --program=prog000.elf
 *
 *════════════════════════════════════════════════════════════════════════════*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <string.h>

#define PROG_NAME_DEFAULT     "prog000.elf"
#define MAX_LINE_LENGTH       80    // Tamaño máximo de una línea de código

char *prog_name;

void set_configuration(int argc, char *argv[]);
void ___error_message(int cod, char *s);
void ___display_header(int cod);

int main(int argc, char *argv[]) {
    FILE            *fd;
    char            line[MAX_LINE_LENGTH];
    char            cod_op[16][5] =  {"ld",   "st",   "add",  "sub",  
                                      "mul",  "div",  "and",  "or",   
                                      "xor",  "mov",  "cmp",  "b", 
                                      "beq",  "bgt",  "blt",  "exit"};
    unsigned int    code_start, data_start;
    unsigned int    i, number_of_lines;
    unsigned int    binary, datum, v_address, addr;
    unsigned char   op, reg1, reg2, reg3;

    set_configuration(argc, argv);   // tomar parámentros

    ___display_header(0);

    if((fd = fopen(prog_name, "r")) == NULL){
        ___error_message(0,"Error while opening file");
    }

    printf("\n╔═══════════════════════════════════════════════════\n"); 
    printf("║ Program name: %s\n", prog_name);
    printf("╟───────────────────────────────────────────────────\n");
    
    if (fscanf(fd, "%s %X", line, &code_start) == EOF) {
        ___error_message(0,"Error: Unexpected End of File");
    }
    if (strncmp(line, ".text",5)) { // code
        ___error_message(0,"Error: .text line missing");
    }
    printf("║ .text                               @[0x%06X]\n", code_start);
    printf("╟───────────────────────────────────────────────────\n");

    if (fscanf(fd, "%s %X", line, &data_start) == EOF) {
        ___error_message(0,"Error: Unexpected End of File");
    }
    if (strncmp(line, ".data",5)) { // data
        ___error_message(0,"Error: .data line missing");
    }
    
    number_of_lines = (data_start - code_start) >> 2;

    addr = code_start;
    for(i = 0; i < number_of_lines; i++){
        if (fscanf(fd, "%08X", &binary) == EOF) {
            ___error_message(0,"Error: Unexpected End of File");
        }
        op        = (binary >> 28) & 0x0F;
        reg1      = (binary >> 24) & 0x0F; 
        reg2      = (binary >> 20) & 0x0F; 
        reg3      = (binary >> 16) & 0x0F;
        v_address = binary & 0x00FFFFFF;             
        switch (op){
            case 0: // ld
            case 1: // st
                    printf("║ 0x%06X: [%08X] %-5s r%d,0x%06X\n", addr, binary, 
                                        cod_op[op], reg1, v_address);
                    break;
            case 2: // add
            case 3: // sub
            case 4: // mul
            case 5: // div
            case 6: // and
            case 7: // or
            case 8: // xor
                    printf("║ 0x%06X: [%08X] %-5s r%d,r%d,r%d\n", addr, binary, 
                                        cod_op[op], reg1, reg2, reg3);
                    break; 
            case 9:  // mov
            case 10: // cmp
                    printf("║ 0x%06X: [%08X] %-5s r%d,r%d\n", addr, binary, 
                                        cod_op[op], reg1, reg2);
                    break;
            case 11: // b
            case 12: // beq
            case 13: // bgt
            case 14: // blt
                    printf("║ 0x%06X: [%08X] %-5s 0x%06X\n", addr, binary, 
                                        cod_op[op], v_address);
                    break;
            case 15: // exit
                    printf("║ 0x%06X: [%08X] %-5s\n", addr, binary, cod_op[op]);
                    break;
            default:
                    break;
        } // switch
        addr = addr + 4;
    } // for i

    printf("╟───────────────────────────────────────────────────\n");
    printf("║ .data                               @[0x%06X]\n", data_start);
    printf("╟───────────────────────────────────────────────────\n");

    addr = data_start;
    while (fscanf(fd, "%8X", &datum) != EOF) {
        printf("║ 0x%06X: [%08X] %d\n",addr , datum, datum);
        addr = addr + 4;
    } // while

    printf("╚═══════════════════════════════════════════════════\n\n");

    fclose(fd);
}


void set_configuration(int argc, char *argv[]){ // tomar parámentros

    int opt, long_index;
    static struct option long_options[] = {
        {"help",       no_argument,       0,  'h' },
        {"program",    required_argument, 0,  'p' },
        {0,            0,                 0,   0  }
    };

    prog_name = PROG_NAME_DEFAULT;

    long_index =0;
    while ((opt = getopt_long(argc, argv,":hp:", 
                        long_options, &long_index )) != -1) {
      switch(opt) {
        case 'h':   // -h or --help
        case '?':
            printf ("Uso %s [OPTIONS]\n", argv[0]);
            printf ("  -h, --help\t\t"
                "Ayuda\n");
            printf ("  -p  --program=SSS\t"
                "Nombre del programa [%s]\n", PROG_NAME_DEFAULT);
            exit (2);
        case 'p':   // -p or --program 
            prog_name = optarg;
            break; 
        default:
            ___error_message(0, "Unknown argument option"); 
      } 
    } 

    srand (time(NULL));
} // set_configuration

/*----------------------------------------------------------------------------- 
 *   Mensajes del programa
 *----------------------------------------------------------------------------*/

void ___display_header(int cod) {
  time_t rawtime;
  struct tm *ptm;
  
  switch (cod) {
    case 0: 
            rawtime = time(NULL);
            ptm = localtime(&rawtime);

            printf("╔═══════════════════════════════════╗\n");
            printf("║  SE·SO          %02d/%02d/%04d  %02d:%02d ║\n",
                                                    ptm->tm_mday, 
                                                    ptm->tm_mon+1, 
                                                    ptm->tm_year+1900, 
                                                    ptm->tm_hour, 
                                                    ptm->tm_min);
            printf("║          ☼☼☼ Heracles ☼☼☼         ║\n");
            printf("║ Nombre del programa:              ║\n");
            printf("║   ├ %-30s║\n", prog_name);
            printf("╚═══════════════════════════════════╝\n\n");
            break; // 0
    default: 
            break; // default
  } // switch
} // ___display_header

void ___error_message(int cod, char *s) {
  switch (cod) {
    case 0: // texto
            printf("☼☼☼☼☼☼☼☼ %s ☼☼☼☼☼☼☼☼\n", s);
            break; //0
    default: 
            break; // default
  } // switch
  exit(-1);
} // ___error_message