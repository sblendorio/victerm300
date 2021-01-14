#pragma charmap(147, 147)
#pragma charmap(17, 17)
#include <stdio.h>
#include <cbm.h>
#include <peekpoke.h>
#include <vic20.h>

void print(char* str);
void put_char(char ch);
char kb_hit(void);
char getkey(void);
char get(void);
void cursor_on(void);
void cursor_off(void);
void display_petscii_ascii(void);
void beep(void);
int __fastcall__ rs232_read(unsigned char lfn, unsigned char* buffer, unsigned int size);

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

#define BUFFER_SIZE 32
#define COLOR_COMBOS 4

char ch;
char *open_string = "x";
char buffer[BUFFER_SIZE];
int len;
int i;
char is_petscii;
char is_start;
char color_combo;
char border_background[] = {27, 8, 8, 25};
char foreground[] = {31, 30, 5, 144};
char foreground_mem[] = {6, 5, 1, 0};
char baud[] = {6, 7, 8}; // 300, 600, 1200

void main(void) {
    is_start = 1;
    is_petscii = 0;
    color_combo = 0;
    POKE(0x900E, 0);
    for (;;) {
        cursor_off();
        print("\223\016\010Choose modem speed:\n\n"
            "1-  300 BAUD (8n1)\n"
            "2-  600 BAUD (8n1)\n"
            "3- 1200 BAUD (8n1)\n"
        );
        do {
            ch = getkey();
        } while (ch < '1' || ch > '3');
        ch = ch - '1';
        open_string[0] = baud[ch];
        display_petscii_ascii();
        cursor_on();
        cbm_close(1);
        cbm_open(1,2,3,open_string);
        if (is_start) {
            is_start = 0;
            buffer[0] = '\n';
            cbm_write(1, buffer, 1);
        }
        for (;;) {
            ch = get();
            if (ch == 133)  { // F1
                is_petscii = !is_petscii;
                display_petscii_ascii();
            } else if (ch == 134) { // F3
                color_combo = (color_combo + 1) % COLOR_COMBOS;
                POKE(0x900F, border_background[color_combo]);
                put_char(foreground[color_combo]);
                #ifdef EXP8K
                for (i=0x9400; i<=0x95F9; ++i) POKE(i, foreground_mem[color_combo]);
                #else
                for (i=0x9600; i<=0x97F9; ++i) POKE(i, foreground_mem[color_combo]);
                #endif
            } else if (ch == 135) { // F5
                break;
            } else if (ch == 136) { // F7
                cursor_off();
                putchar(foreground[color_combo]);
                print("\223\021\021\021\021\021\021\021\021\021\021"
                      " EXIT: ARE YOU SURE?");
                cursor_on();
                ch = getkey();
                if (ch == 'Y' || ch == 'y') asm("brk");
                display_petscii_ascii();
            } else if (ch != 0) {
                buffer[0] = is_petscii ? ch : t[ch];
                cbm_write(1, buffer, 1);
            }
            len = rs232_read(1, buffer, BUFFER_SIZE);
            if (len > 0) cursor_off();
            for (i=0; i<len; ++i) {
                if (buffer[i] == 7) beep();
                put_char(is_petscii ? buffer[i]: f[buffer[i]]);
                POKE(212, 0);
                POKE(216, 0);
            }
            if (PEEK(204) != 0) cursor_on();
        }
    }
}

void display_petscii_ascii(void) {
    cursor_off();
    putchar(foreground[color_combo]);
    print("\223\022VIC Terminal          \222"
        "\n"
        "F1-PET/ASCII F3-COLOR\n"
        "F5-SET BAUD  F7-EXIT\n"
        "\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300\300");
    print(is_petscii
        ? "* Charset: PETSCII\n\n"
        : "* Charset: ASCII\n\n"
    );
}

int __fastcall__ rs232_read(unsigned char lfn, unsigned char* buffer, unsigned int size)
{
    static unsigned int bytesread;
    if (cbm_k_chkin(lfn)) return -1;
    bytesread = 0;

    while (PEEK(667)!=PEEK(668) && bytesread<size)
        buffer[bytesread++] = cbm_k_getin();

    cbm_k_clrch();
    return bytesread;
}

void put_char(char ch) {
    __A__ = ch;
    asm("jsr $ffd2");
}

char get(void) {
    if (kb_hit()) {
        asm("jsr $e5cf");
        return __A__;
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

void beep(void) {
    static unsigned long j;
    POKE(0x900E, 15);
    POKE(0x900D, 0);
    POKE(0x900C, 230);
    for (j=0; j<1000; ++j) asm("nop");
    POKE(0x900E, 0);
}
