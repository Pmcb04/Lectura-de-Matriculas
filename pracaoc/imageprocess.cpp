/***************************************************************************
 *   Copyright (C) 2019 by Paco Andrés and Pilar Bachiller                 *
 *   pacoan@unex.es pilarb@unex.es                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "imageprocess.h"



void imageprocess::copiar(uchar * imgO, uchar * imgD, int tam)
{

    asm volatile(
            "movq %0, %%rsi;"
            "movq %1, %%rdi;"
            "movsxd %2, %%rcx;"
            "rep movsb;"

            :
            : "m" (imgO),	"m" (imgD), "m"(tam)
            : "%rcx", "%rsi", "%rdi", "memory"
    );

}

void imageprocess::borrar(uchar * imgD, int w, int h)
{

    asm volatile(
        "movq %0, %%rsi;"
        "movsxd %1, %%rcx;"
        "movsxd %2, %%rax;"
        "imul %%rax, %%rcx;"
        "mov $0, %%rax;"

        "bucleBorrar:"
           "mov %%al, (%%rsi);"
           "inc %%rsi;"
        "loop bucleBorrar;"

        :
        : "m" (imgD),"m" (w), "m" (h)
        : "%rsi", "%rcx", "%rax" , "memory"
    );

}

void imageprocess::invertir(uchar * imgO, uchar * imgD, int w, int h)
{
    asm volatile(
        "movq %0, %%rsi;"
        "movq %1, %%rdi;"
        "movsxd %2, %%rcx;"
        "movsxd %3, %%rax;"
        "imul %%rax, %%rcx;"

        "bucleInvertir:"
           "mov $255, %%rax;"
           "sub (%%rsi), %%rax;"
           "mov %%rax, (%%rdi);"
           "inc %%rsi;"
           "inc %%rdi;"
        "loop bucleInvertir;"

        :
        : "m" (imgO),	"m" (imgD), "m" (w), "m" (h)
        : "%rsi", "%rdi", "%rax", "%rcx", "memory"
    );

}

void imageprocess::umbralizar(uchar * imgD, int w, int h)
{
    asm volatile(
        "movq %0, %%rdi;"
        "movsxd %1, %%rcx;"
        "movsxd %2, %%rax;"
        "imul %%rax, %%rcx;"

        "bucleUmbralizar:"
            "mov (%%rdi), %%al;"
            "cmp $128, %%al;"
            "jbe elseUmbralizar;"

            "mov $255, %%al;"
            "jmp finUmbralizar;"

            "elseUmbralizar:;"
              "mov $0, %%al;"

          "finUmbralizar:;"
               "mov %%al, (%%rdi);"
               "inc %%rdi;"

        "loop bucleUmbralizar;"

        :
        : "m" (imgD), "m" (w), "m" (h)
        : "%rdi", "%rax", "%rcx", "memory"
    );
}

void imageprocess::eliminarRuido_F1(uchar * imgO, uchar * imgD, int w, int h)
{
    asm volatile(
        "mov %0, %%rsi;" // dirOrig = imgO
        "mov %1, %%rdi;" // dirDest = imgD
        "movsxd %2, %%rbx;" // rbx = w
        "dec %%rbx;" // w-1
        "movsxd %3, %%rcx;" // rcx = h
        "dec %%rcx;" // h-1

        "mov $1, %%r8;" // f = 1
        "jmp comprobacionEliminarRuidoF11;"
        "bucleEliminarRuidoF11:;"

            "mov $1, %%r9;" // c = 1
            "jmp comprobacionEliminarRuidoF12;"
            "bucleEliminarRuidoF12:;"
                "mov $-1, %%r10;" // df = -1
                "mov $1, %%r11;" // tBlancos = true

                "jmp comprobacionEliminarRuidoF13;"
                "bucleEliminarRuidoF13:;"
                    "mov $-1, %%r12;" // dc = -1
                    "jmp comprobacionEliminarRuidoF14;"
                    "bucleEliminarRuidoF14:;"
                        "mov %%r8, %%r13;" //f
                        "add %%r10, %%r13;" // f+df
                        "movsxd %2, %%rax;"
                        "imul %%rax, %%r13;" // (f+df)*w
                        "add %%r9, %%r13;" //(f+df)*w +c
                        "add %%r12, %%r13;" //(f+df)*w +c+dc
                        "mov (%%rsi, %%r13), %%al;"
                        "cmp $0, %%al;"  // ¿[dirOrig+posPixel] == 0?
                        "jne saltartBlancosF1;"
                            "mov $0, %%r11;" // tBlancos = false
                        "saltartBlancosF1:;"
                        "inc %%r12;" // dc++
                    "comprobacionEliminarRuidoF14:;"
                        "cmp $1, %%r12;" // dc <= 1
                        "jg finBucleF14;"
                        "cmp $1, %%r11;" // ¿tBlancos = true?
                        "je bucleEliminarRuidoF14;"
                    "finBucleF14:;"

                    "inc %%r10;" // df++
                "comprobacionEliminarRuidoF13:;"
                    "cmp $1, %%r10;" // df <= 1
                    "jg finBucleF13;"
                    "cmp $1, %%r11;" // ¿tBlancos = true?
                    "je bucleEliminarRuidoF13;"

              "finBucleF13:;"
                "movsxd %2, %%rax;"
                "imul %%r8, %%rax;" // f*w
                "add %%r9, %%rax;" // f*w+c
                "cmp $1, %%r11;" // ¿tBlancos = true?
                "jne elseEliminarRuidoF1;"
                    "mov $255, %%r13;"
                    "jmp finEliminarRuidoF1;"
                "elseEliminarRuidoF1:;"
                    "mov $0, %%r13;"
                "finEliminarRuidoF1:;"
                    "mov %%r13, (%%rax,%%rdi);"

                "inc %%r9;" // c++
            "comprobacionEliminarRuidoF12:;"
                "cmp %%rbx, %%r9;"  // c < w-1
                "jl bucleEliminarRuidoF12;"

             "inc %%r8;" // f++
        "comprobacionEliminarRuidoF11:;"
            "cmp %%rcx, %%r8;" // f < h-1
            "jl bucleEliminarRuidoF11;"

        :
        : "m" (imgO),	"m" (imgD), "m" (w), "m" (h)
        : "%rsi", "%rdi", "%rax", "%rbx","%rcx","%r8","%r9", "%r10", "%11", "%r12", "%r13", "memory"
    );

}

void imageprocess::eliminarRuido_F2(uchar * imgO, uchar * imgD, int w, int h)
{
    asm volatile(
        "mov %0, %%rsi;" // dirOrig = imgO
        "mov %1, %%rdi;" // dirDest = imgD
        "movsxd %2, %%rbx;" // rbx = w
        "dec %%rbx;" // w-1
        "movsxd %3, %%rcx;" // rcx = h
        "dec %%rcx;" // h-1

        "mov $1, %%r8;" // f = 1
        "jmp comprobacionEliminarRuidoF21;"
        "bucleEliminarRuidoF21:;"

            "mov $1, %%r9;" // c = 1
            "jmp comprobacionEliminarRuidoF22;"
            "bucleEliminarRuidoF22:;"
                "mov $-1, %%r10;" // df = -1
                "mov $0, %%r11;" // aBlancos = false

                "jmp comprobacionEliminarRuidoF23;"
                "bucleEliminarRuidoF23:;"
                    "mov $-1, %%r12;" // dc = -1
                    "jmp comprobacionEliminarRuidoF24;"
                    "bucleEliminarRuidoF24:;"
                        "mov %%r8, %%r13;" //f
                        "add %%r10, %%r13;" // f+df
                        "movsxd %2, %%rax;"
                        "imul %%rax, %%r13;" // (f+df)*w
                        "add %%r9, %%r13;" //(f+df)*w +c
                        "add %%r12, %%r13;" //(f+df)*w +c+dc
                        "mov (%%rsi, %%r13), %%al;"
                        "cmp $255, %%al;"  // ¿[dirOrig+posPixel] == 255?
                        "jne saltartBlancosF2;"
                            "mov $1, %%r11;" // aBlanco = true
                        "saltartBlancosF2:;"
                        "inc %%r12;" // dc++
                    "comprobacionEliminarRuidoF24:;"
                        "cmp $1, %%r12;" // dc <= 1
                        "jg finBucleF24;"
                        "cmp $0, %%r11;" // ¿aBlanco = false?
                        "je bucleEliminarRuidoF24;"
                    "finBucleF24:;"

                    "inc %%r10;" // df++
                "comprobacionEliminarRuidoF23:;"
                    "cmp $1, %%r10;" // df <= 1
                    "jg finBucleF23;"
                    "cmp $0, %%r11;" // ¿aBlanco = false?
                    "je bucleEliminarRuidoF23;"
                "finBucleF23:;"

                "movsxd %2, %%rax;"
                "imul %%r8, %%rax;" // f*w
                "add %%r9, %%rax;" // f*w+c
                "cmp $1, %%r11;" // ¿aBlanco = true?
                "jne elseEliminarRuidoF2;"
                    "mov $255, %%r13;"
                    "jmp finEliminarRuidoF2;"
                "elseEliminarRuidoF2:;"
                    "mov $0, %%r13;"
                "finEliminarRuidoF2:;"
                    "mov %%r13, (%%rax,%%rdi);"

                "inc %%r9;" // c++
            "comprobacionEliminarRuidoF22:;"
                "cmp %%rbx, %%r9;"  // c < w-1
                "jb bucleEliminarRuidoF22;"

             "inc %%r8;" // f++
        "comprobacionEliminarRuidoF21:;"
            "cmp %%rcx, %%r8;" // f < h-1
            "jb bucleEliminarRuidoF21;"

        :
        : "m" (imgO),	"m" (imgD), "m" (w), "m" (h)
        :"%rsi", "%rdi", "%rax", "%rbx","%rcx","%r8","%r9", "%r10", "%11", "%r12", "%r13", "memory"
    );

}


int imageprocess::detectarV_min(uchar *imgD, int U){

    int min=0;

	asm volatile(

        "mov %1, %%rdi;" // dirDest = imgD
        "mov $0, %%r8;" // f = 0
        "xor %%r9, %%r9;" // cont = 0
        "jmp condWhileMin;"

            "whileDetectarMin:;"
               "mov $320, %%rcx;"
               "xor %%r9, %%r9;" // cont = 0

                 "bucleDetectarMin:;"
                    "mov (%%rdi), %%al;"
                    "cmp $255, %%al;" // [dirDest] == 255
                    "jne saltarIncrementoMin;"
                         "inc %%r9;" // cont++
                    "saltarIncrementoMin:;"
                        "inc %%rdi;" // dirDest++
                 "loop bucleDetectarMin;"

              "inc %%r8;" // f++

          "condWhileMin:;"
            "cmp $100, %%r8;"
            "jae finWhileMin;"
            "movsxd %2, %%rax;"
            "cmp %%rax, %%r9;"
            "jbe whileDetectarMin;"

        "finWhileMin:;"
        "dec  %%r8;"
        "mov %%r8d, %0;"

        : "=m" (min)
        : "m" (imgD), "m" (U)
        : "%rdi", "%rax", "%rcx", "%r8", "%r9", "memory"
	);

    return min;
}

int imageprocess::detectarV_max(uchar *imgD, int U){

    int max=0;

    asm volatile(

        "mov $(320*100-1), %%rax;"
        "mov %1, %%rdi;"
        "add %%rax, %%rdi;" // rdi = imgD + 320*100-1
        "mov $99, %%r8;" // f = 99
        "xor %%r9, %%r9;" // cont = 0
        "jmp condWhileMax;"

            "whileDetectarMax:;"

               "mov $320, %%rcx;"
               "xor %%r9, %%r9;" // cont = 0

                 "bucleDetectarMax:;"
                    "mov (%%rdi), %%al;"
                    "cmp $255, %%al;"
                    "jne saltarIncrementoMax;"
                        "inc %%r9;"
                    "saltarIncrementoMax:;"
                        "dec %%rdi;"
                 "loop bucleDetectarMax;"

              "dec %%r8;"

        "condWhileMax:;"
            "cmp $0, %%r8;"
            "jb finWhileMax;"
            "movsxd %2, %%rax;"
            "cmp %%rax, %%r9;"
            "jbe whileDetectarMax;"

        "finWhileMax:;"
        "inc %%r8;"
        "mov %%r8d, %0;"

        : "=m" (max)
        : "m" (imgD), "m" (U)
        : "%rdi", "%rax", "%rcx", "%r8", "%r9", "memory"
    );

    return max;

}


void imageprocess::detectarH_F1(uchar *imgD, uchar * VA, uchar U){

    asm volatile(
        "mov %1, %%rsi;" // dirVA = VA
        "xor %%r8, %%r8;" // c = 0
        "jmp comprobarDetectarF11;"
        "bucleDetectarF11:;"
            "mov %0, %%rdi;" // dirDest = imgD
            "xor %%rbx, %%rbx;" // cont = 0

             "mov $100, %%rcx;"
                "bucleDetectarF12:;"
                    "mov (%%rdi,%%r8), %%al;"
                    "cmp $255, %%al;"
                    "jne saltarIncrementoCont;"
                        "inc %%rbx;" // cont++
                    "saltarIncrementoCont:;"
                    "add $320, %%rdi;"
                "loop bucleDetectarF12;"

            "xor %%rax, %%rax;"
            "mov %2, %%al;" // U
            "cmp %%rax, %%rbx;"
            "jbe elseDetectarF1;"
                "mov $1, %%rax;"
                "jmp finDetectarF1;"
            "elseDetectarF1:;"
                "mov $0, %%rax;"
            "finDetectarF1:;"
                "mov %%rax, (%%rsi,%%r8);"

            "inc %%r8;" // c++
        "comprobarDetectarF11:;"
            "cmp $320, %%r8;"
            "jl bucleDetectarF11;"

        :
        : "m" (imgD), "m" (VA),"m" (U)
        : "%rsi", "%rdi", "%rax", "%rbx", "%rcx","%r8","memory"
	);
}

void imageprocess::detectarH_F2(uchar * VA, int * Vh){

    asm volatile(
        "mov %0, %%rsi;" //dirVA = VA
        "mov %1, %%rdi;" // dirVh  = Vh
        "xor %%r8, %%r8;" // c = 0
        "xor %%r9, %%r9;" // nC = 0

        "jmp comprobarBucleDetectarF2;"
        "bucleDetectarF2:;"
            "mov (%%rsi, %%r8), %%al;"
            "cmp $1, %%al;"
            "jne saltoIf;"
                "mov %%r8, %%r10;" // iC = c

                    "whileDetectarF2:;"
                        "inc %%r10;"

                    "cmp $320, %%r10;"
                    "jae finWhileDetectarF2;"
                    "mov (%%rsi, %%r10), %%al;"
                    "cmp $1, %%al;"
                    "je whileDetectarF2;"

                "finWhileDetectarF2:;"
                    "push %%r10;"
                    "sub %%r8, %%r10;" // iC - c
                    "mov %%r10, %%r11;" //tamC = iC - c
                    "mov %%r11, %%r12;" // tamC en %%r12
                    "pop %%r10;"
                    "shr $1, %%r11;" // tamC/2
                    "add %%r8, %%r11;" // tamC/2 + iC
                    "cmp $150, %%r10;"
                    "jae saltarAmpliacion;"
                    "cmp $20, %%r12;"
                    "jae saltarAmpliacion;"
                    "sub $10, %%r11;"
                    "saltarAmpliacion:;"
                    "mov %%r11, (%%rdi);"
                    "add $4, %%rdi;" // dirVh = dirVh + 4
                    "inc %%r9;" // nC++
                    "dec %%r10;" // iC-1
                    "mov %%r10, %%r8;" // c = iC - 1
            "saltoIf:;"
                "inc %%r8;" // c++

        "comprobarBucleDetectarF2:;"
            "cmp $320, %%r8;"
            "jae finBucleDetectar;"
            "cmp $7, %%r9;"
            "jb bucleDetectarF2;"

        "finBucleDetectar:;"

        :
        : "m" (VA),"m"(Vh)
        : "%rsi", "%rdi","%rax", "%r8","%r9", "%r10", "%r11","%r12", "memory"
    );

}


void imageprocess::recortar(uchar *imgO, uchar *imgD,int x, int y, int w, int h){

	asm volatile(

        "movsxd %3, %%rax;" // y
        "imul $320, %%rax;" // 320*y
        "movsxd %2, %%rcx;" // x
        "add %%rax, %%rcx;" // 320*y + x
        "mov %0, %%rsi;" // img0
        "add %%rcx, %%rsi;" // imgO + 320*y + x
        "mov %1, %%rdi;"

        "movsxd %5, %%rcx;"
        "bucleRecortar1:;"
          "push %%rcx;"
          "movsxd %4, %%rcx;"

            "bucleRecortar2:;"
                "mov (%%rsi), %%al;"
                "mov %%al, (%%rdi);"
                "inc %%rsi;"
                "inc %%rdi;"
            "loop bucleRecortar2;"

          "pop %%rcx;"
          "add $320, %%rsi;"
          "movsxd %4, %%rax;"
          "sub %%rax, %%rsi;"
       "loop bucleRecortar1"

        :
        : "m" (imgO),"m"(imgD),"m" (x),"m" (y),"m" (w),"m"(h)
        : "%rsi", "%rdi", "%rax", "%rcx",  "memory"
    );

}

int imageprocess::matching(uchar *caracM, uchar *patrones[31], int ini, int tam){
    int iMax=0;

    asm volatile(
        "mov %2, %%rdi;" // dirPatrones = patrones
        "movsxd %3, %%rsi;" // iP = ini
        "mov $-1000, %%r8;" // maxSim = -1000
        "movsxd %4, %%rcx;" // tam

        "bucleMatching1:;"
            "mov (%%rdi, %%rsi, 8), %%r9;" // dirP = [dirPatrones + posPatron]
            "mov %1, %%r10;" // dirCarac = caracM
            "xor %%r11, %%r11;" // sim = 0

            "push %%rcx;"
              "mov $(32*55), %%rcx;"
                  "bucleMatching2:;"
                     "mov (%%r10), %%al;" //[dirCarac]
                     "cmp %%al, (%%r9);" // ¿[dirCarac] == [dirP] ?
                     "jne saltarIfMatching;"
                        "cmp $255, %%al;" // ¿[dirCarac] = 255?
                        "jne saltarIfMatching2;"
                            "add $3, %%r11;" // sim = sim + 3
                            "jmp finMatching2;"
                        "saltarIfMatching2:;"
                            "inc %%r11;" // sim = sim + 1
                            "jmp finMatching2;"
                     "saltarIfMatching:;"
                        "dec %%r11;" // sim = sim -1
                    "finMatching2:;"
                    "inc %%r10;" // dirCarac++
                    "inc %%r9;" // dirP++
                  "loop bucleMatching2;"
              "pop %%rcx;"

              "cmp %%r8, %%r11;" //¿sim>maxSim?
              "jle saltarIfMatching3;"
                "mov %%r11, %%r8;" // maxSim = sim
                "mov %%esi, %0;" // iMax = iP
              "saltarIfMatching3:;"
                "inc %%rsi;" // iP++
        "loop bucleMatching1;"

        : "=m" (iMax)
        : "m" (caracM),"m"(patrones),"m" (ini),"m" (tam)
        : "%rsi", "%rdi", "%rax","%rcx","%r8","%r9", "%r10", "%r11", "memory"
    );

    return iMax;
}
