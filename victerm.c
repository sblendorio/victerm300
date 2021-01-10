// cl65 -t vic20 --config vic20-unexpanded.cfg -Cl -O -o victerm.prg victerm.c
// cl65 -t vic20 --config vic20-32k.cfg -Cl -O -o victerm.prg victerm.c
#pragma charmap(147, 147)
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
#include <cbm.h>
#include <vic20.h>

#define BUFFER_SIZE 32

char ch;
char *open_string="x";
char buffer[BUFFER_SIZE];
int len;
int i;

void print(char* str);
void put_char(char ch);
char kb_hit(void);
char getkey(void);
char get(void);

char baud[] = {6, 7, 8, 10}; // 300, 600, 1200, 2400
char choice;
char f[] = {
      0,   0,   0, 137,   0,   0,   0,   0,  20,   0,   0,   0, 147,  13,   0,   0,
    146, 134,   0, 138,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
     48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
     64, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,  91,  92,  93,  94,  95,
      0,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
     80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,   0,   0,   0,   0,   0,
      0,   0,   0, 137,   0,   0,   0,   0,  20,   0,   0,   0, 147,  13,   0,   0,
    146, 134,   0, 138,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
     32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
     48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
     64, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
    208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,  91,  92,  93,  94,  95,
      0,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
     80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,   0,   0,   0,   0,   0
};
char t[] = {
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  13,   0,   0, 
      0,   0,   0,   0,   8,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
     32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47, 
     48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63, 
     64,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122,  91,  92,  93,  94,  95, 
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,  16,  17,   0,   0,   3,  19,   0,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
      0,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79, 
     80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

void main(void) {
    print("\223\016\010Choose modem speed\n\n"
        "1- 300 BAUD\n"
        "2- 600 BAUD\n"
        "3- 1200 BAUD\n"
        "4- 2400 BAUD");
    do {
        ch = getkey();
    } while (ch < '1' || ch > '4');
    choice = ch - '1';
    open_string[0] = baud[choice];
    print("\223VIC Terminal\n\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300");
    i = cbm_open(5,2,0,open_string);
    put_char(i==0?'0':'1');
    for (;;) {
        put_char('p');
        len = cbm_read(5, buffer, BUFFER_SIZE);
        put_char('u');
        for (i=0; i<len; ++i) put_char(buffer[i]);
        ch = get();
        if (ch != 0) {
            buffer[0] = ch;
            cbm_write(5, buffer, 1);
        }
    }

}

void put_char(char ch) {
    __A__ = ch;
    asm("jsr $ffd2");
}

char get(void) {
    if (kb_hit()) {
        asm("jsr $e5cf");
        return  __A__;
    }
    return 0;
}

char getkey(void) {
    while (1) {
        if (kb_hit()) {
            asm("jsr $e5cf");
            return __A__;
        }
    }
}

void print(char *str) {
    while (*str) {
        __A__ = *str++;
        asm("jsr $ffd2");
    }
}

char kb_hit(void) {
    asm("ldx #$00");
    asm("lda $c6");
    asm("beq %g", out);
    asm("lda #$01");
out:
    return __A__;
}


void cursor_on(void) {
	asm("ldy #$00");
	asm("sty $cc");
}

void cursor_off(void) {
	asm("ldy $cc");
	asm("bne %g", exitloop);
	asm("ldy #$01");
	asm("sty $cd");
loop:
	asm("ldy $cf");
	asm("bne %g", loop);
exitloop:
	asm("ldy #$ff");
	asm("sty $cc");
}
