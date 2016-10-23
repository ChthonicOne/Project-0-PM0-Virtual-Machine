#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

typedef struct
{
    int op;
    int l;
    int m;
} instruction;

int stack[MAX_STACK_HEIGHT];

int execute_instruction(instruction next, int *pc, int *bp, int *sp);   // This is my completed function prototype
int base(int l, int base);                                              // This is the prototype for the base function if Kika wants to make it

void print_a(FILE *ofp, int line, int op, int l, int m, char *opcodes);                                     //Jacob can use these three or change them as needed
void print_e (FILE *ofp, instruction *array, int pc_a, int pc_b, int bp, int sp, int *stack, int * start);
void print_s (FILE *ofp, int *stack, int sp, int *index);


char *opcodes[] = {"", "LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO", "SIO", "SIO"};

int main(int argc, char * argv[])
{

    int bp = 1;             //base pointer
    int sp = 0;             //stack pointer
    int pc = 0;             //program counter
    int ir = 0;             //instruction register
    int halted = 0;

    instruction * array = malloc(sizeof(instruction) * MAX_CODE_LENGTH);


    if(argc != 2)           // i.e. if there isn't a file passed to Main
    {
        printf("\nError while opening the input file, EXITING PROGRAM\n");
        return -1;
    }

    FILE* ifp = fopen(argv[1], "r");        // input file
    FILE* ofp = fopen("trace.txt", "w");    // output file

    int i = 0;
    int j = 0;
    int ic;

    fprintf(ofp, "Line\tOP\t\tL\t\tM\n");

	while(fscanf(ifp, "%d %d %d", &array[i].op, &array[i].l, &array[i].m) != EOF)
	{
		if (opcodes[array[i].op] != '\0')
			print_a(ofp, i, array[i].op, array[i].l, array[i].m, opcodes[array[i].op]);
		i++;

	}

	ic = i;

	fprintf(ofp, "\n\t\t\t\t\t\t\t\tPC\t\tBP\t\tSP\t\tstack\n");
	fprintf(ofp, "Initial Values\t\t\t\t\t\t\t%d\t\t%d\t\t%d\n", pc, bp, sp);

	/*
        Here is the Main Instruction -> Print stack loop
        Above here should be all of the code loading the program into the array
	*/

    while (!halted)
    {
        halted = execute_instruction(array[pc], &pc, &bp, &sp);

        /*
            Jacob, this is where you will put the calls to your print function for the stack trace
        */

    }

	fclose(ifp);

	if (halted == 2)    // here we check to see if we exited the program abnormally to give an appropriate error.
    {
        printf("Invalid modifier for an Op 2 instruction!\n");
        return 1;
    } else if (halted == 3)
    {
        printf("Invalid modifier for an Op 9 instruction!\n");
        return 1;
    } else if (halted == 4)
    {
        printf("Invalid Op code on this instruction!\n");
        return 1;
    }

	return 0;
}

int execute_instruction(instruction next, int *pc, int *bp, int *sp)
{
    *pc++;
    switch (next.op)
    {
    case 1: *sp++;
        stack[*sp] = next.m;
        break;
    case 3: *sp++;
        stack[*sp] = stack[base(next.l, *bp) + next.m];
        break;
    case 4: stack[base(next.l,*bp) + next.m] = stack[*sp];
        *sp--;
        break;
    case 5: stack[*sp+1]=0;
        stack[*sp+2]=base(next.l, *bp);
        stack[*sp+3]=*bp;
        stack[*sp+4]=*pc;
        *bp=*sp+1;
        *pc=next.m;
        break;
    case 6: *sp+=next.m;
        break;
    case 7: *pc=next.m;
        break;
    case 8: if (stack[*sp]==0)
            *pc=next.m;
        break;
    case 2: switch(next.m)
        {
        case 0: *sp = *bp-1;
            *pc = stack[*sp+4];
            *bp = stack[*sp+3];
            break;
        case 1: stack[*sp] = -stack[*sp];
            break;
        case 2: *sp--;
            stack[*sp] = stack[*sp] + stack[*sp+1];
            break;
        case 3: *sp--;
            stack[*sp] = stack[*sp] - stack[*sp+1];
            break;
        case 4: *sp--;
            stack[*sp] = stack[*sp] * stack[*sp+1];
            break;
        case 5: *sp--;
            stack[*sp] = stack[*sp] / stack[*sp+1];
            break;
        case 6: stack[*sp] = stack[*sp] & 1;
            break;
        case 7: *sp--;
            stack[*sp] = stack[*sp] % stack[*sp+1];
            break;
    	case 8: *sp--;
            stack[*sp] = stack[*sp] == stack[*sp+1];
            break;
        case 9: *sp--;
            stack[*sp] = stack[*sp] != stack[*sp+1];
            break;
        case 10: *sp--;
            stack[*sp] = stack[*sp] < stack[*sp+1];
            break;
        case 11: *sp--;
            stack[*sp] = stack[*sp] <= stack[*sp+1];
            break;
        case 12: *sp--;
            stack[*sp] = stack[*sp] > stack[*sp+1];
            break;
        case 13: *sp--;
            stack[*sp] = stack[*sp] >= stack[*sp+1];
            break;
        default: return 2;                          //Invalid Op 2 modifier
        }
        break;
    case 9: switch(next.m)
        {
        case 0: printf("%d\n", stack[*sp]);
            *sp--;
            break;
        case 1: *sp++;
            printf("./");
            scanf("%d", stack[*sp]);
            break;
        case 2: return 1;                           // Halted normally
        default: return 3;                          // Invalid Op 9 modifier
        }
        break;
    default: return 4;                              // Invalid Op
    }
    return 0;
}
