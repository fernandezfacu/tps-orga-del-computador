/*
 * CS:APP Data Lab
 *
 * <Please put your name and userid here>
 *
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code
  must conform to the following style:

  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>

  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.


  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce
 *      the correct answers.
 */


#endif
//1
/*
 * bitAnd - x&y using only ~ and |
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  return ~(~x | ~y);
}
/*
 * bitMatch - Create mask indicating which bits in x match those in y
 *            using only ~ and &
 *   Example: bitMatch(0x7, 0xE) = 0x6
 *   Legal ops: ~ & |
 *   Max ops: 14
 *   Rating: 1
 */
int bitMatch(int x, int y) {
  return (x & y) | (~x & ~y);
}
/*
 * bitNor - ~(x|y) using only ~ and &
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  return (~x)&(~y);
}
/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(x & y) & ~(~x & ~y);
}
//2
/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  // Se crea una máscara que contiene unos en todas las posiciones impares
  int mask = (0xAA << 8) + 0xAA;
  mask = (mask << 16) + mask;

  // Se aplica la máscara al número x para mantener solo los bits en posiciones impares
  // Realiza un XOR con la máscara para verificar si todos los bits impares son unos
  return !((x & mask) ^ mask);
}
/*
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
  
  // Se crea una máscara que contiene unos en todas las posiciones pares
  int mask = 0x55 + (0x55 << 8) + (0x55 << 16) + (0x55 << 24);

  // Se aplica la máscara al número x para mantener bits en posiciones pares.
  return !(!(x & mask)); 
}

/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {

  int swaped_x = x;
  // Desplaza n y m a la izquierda en 3 bits (*8)
  int shifted_n = n << 3;
  int shifted_m = m << 3;

  // Realiza un XOR entre los bytes que queremos cambiar y toma solo los 8 bits menos significativos de lo que queremos intercambiar
  int y = (x >> shifted_n) ^ (x >> shifted_m);
  y = 0xFF & y;

  // Intercambia los bytes de x
  swaped_x = swaped_x ^ (y << shifted_n);
  swaped_x = swaped_x ^ (y << shifted_m);  

  return swaped_x;
}
/*
 * fitsBits - return 1 if x can be represented as an
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {

  //Shifteo n-1 veces x a la derecha 
  int y = x >> (n + ~0);     

  //Si es positivo y quedan todos ceros, se puede representar en n bits. 
  //Si es negativo y quedan todos unos, se puede representar en n bits.
  return !y | !(~y);
}
/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1;
}
/*
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 *  Examples: sign(130) = 1
 *            sign(-23) = -1
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 10
 *  Rating: 2
 */
int sign(int x) {
  return (x >> 31) | !(!x); 
}
//3
/*
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1,
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  int sum = x + y;
  // Toma los signos
  int signX = x >> 31;
  int signY = y >> 31;
  int signSum = sum >> 31;

  // Overflow: signos de x,y iguales y signo sum distinto.
  int overflow = (signX ^ signSum) & (signY ^ signSum);
  // 1: no hay | 0: hay
  return !overflow;
}
/*
 * bitMask - Generate a mask consisting of all 1's
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
  
  int high_ones = ~0 << highbit << 1; //Máscara con unos hasta la posición de highbit
  int low_ones = ~0 << lowbit;        //Máscara con unos hasta la posición de highbit

  return (high_ones ^ low_ones) & low_ones; // Realiza un XOR con las máscaras para guardar los unos entre lowbit y highbit y descarta unos si lowbit > highbit.
}
/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  
  int x_value = !!x; //Es 0 si x == 0 
  int mask = ~x_value + 1; //Máscara con todos ceros si x == 0 o todos unos si x!=0
  
  return (mask & y) | (~mask & z);
}

/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {

  // Si 0x30 <= x <= 0x39  ==> -0x30+x >= 0 y -0x39+x <= 0 
  int y = (~0x30 + 1) + x; 
  int z = (~0x3A + 1) + x;

  int y_sgn = y >> 31; 
  int z_sgn = z >> 31; 

  return (!y_sgn) & z_sgn; //Devuelve 1 si y es positivo y z es negativo.
}
/*
 * isGreater - if x > y  then return 1, else return 0
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
  
  int x_sgn = x >> 31;
  int y_sgn = y >> 31;

  int z = x + (~y + 1); //z = x-y
  int z_sgn = z >> 31;

  int same_sign_condition = !!( ~(x_sgn ^ y_sgn) & ~z_sgn & z); //Es 1 si x e y tienen el mismo signo, x-y es positivo y distinto de cero.

  int diff_sign_condition = (!x_sgn) & y_sgn; //Es 1 si x es positivo e y es negativo.

  return same_sign_condition | diff_sign_condition;
} 
/*
 * replaceByte(x,n,c) - Replace byte n in x with c
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: replaceByte(0x12345678,1,0xab) = 0x1234ab78
 *   You can assume 0 <= n <= 3 and 0 <= c <= 255
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 3
 */
int replaceByte(int x, int n, int c) {
  
  int n_mask = 0xFF << (n << 3); //000000000001111000000000
  int mask = ~ n_mask; //1111111111111111111110000111111111

  return (x & mask) | (c << (n << 3));
}

//4
/*
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  
  int neg_x = ~x + 1;

  int x_sgn = x >> 31;
  int neg_x_sgn = neg_x >> 31;

  return (x | x_sgn) & (neg_x | neg_x_sgn);
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int bang(int x) {
  
  //Si x es distinto de 0, x | -x va a tener un 1 como bit de signo.   
  int neg_x = ~x + 1;

  return ((x | neg_x) >> 31) + 1;
}
/*
 * isNonZero - Check whether x is nonzero using
 *              the legal operators except !
 *   Examples: isNonZero(3) = 1, isNonZero(0) = 0
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int isNonZero(int x) {
  
  //Si x es distinto de 0, x | -x va a tener un 1 como bit de signo.   
  int neg_x = ~x + 1;

  return ~((x | neg_x) >> 31) + 1; 
}
/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
  
  int neg_x = ~x + 1;

  return ((x | neg_x) >> 31) + 1;
}
//float
/*
 * floatAbsVal - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument..
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatAbsVal(unsigned uf) {
  unsigned mask = ~(1 << 31);   // Mascara con 0 en el bit de signo y todos 1
  unsigned exp = (uf >> 23) & 0xff;   // Los bits de exp en los primeros 8
  unsigned frac = uf << 9;  // Los bits de frac en los ultimos 23

  if(exp == 0xff && frac != 0)   // Se chequea si es NaN (exp sean todos 1 y frac no sean todos 0).
    return uf;
  return uf & mask;
}
/*
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
  unsigned exp_uf = (uf >> 23) & 0xff;
  unsigned frac_uf = uf << 9;  
  unsigned exp_ug = (ug >> 23) & 0xff;
  unsigned frac_ug = ug << 9;
  
  if(!((uf | ug) << 1))  // Si ambos son 0 devuelve 1
    return 1;
  if((exp_uf == 0xff && frac_uf != 0) || (exp_ug == 0xff && frac_ug != 0)) // Se devuelve 0 si uf o ug son Nan
    return 0;
  return (uf == ug);
}
/*
 * floatNegate - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned floatNegate(unsigned uf) {
  unsigned mask = 8 << 28;   // Mascara con 1 en el bit de signo y todos 0
  unsigned exp = (uf >> 23) & 0xff;
  unsigned frac = uf << 9;
  
  if(exp == 0xff && frac != 0)   // Se chequea si es NaN (exp sean todos 1 y frac no sean todos 0).
    return uf;
  return uf ^ mask;
}
/*
 * floatIsLess - Compute f < g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 3
 */
int floatIsLess(unsigned uf, unsigned ug) {
  unsigned exp_uf = (uf >> 23) & 0xff;
  unsigned frac_uf = uf << 9;  
  unsigned s_uf = uf >> 31;
  unsigned exp_ug = (ug >> 23) & 0xff;
  unsigned frac_ug = ug << 9;
  unsigned s_ug = ug >> 31;

  
  if((exp_uf == 0xff && frac_uf != 0) || (exp_ug == 0xff && frac_ug != 0) || !((uf | ug) << 1)) // Se devuelve 0 si alguno es Nan o ambos 0
    return 0;

  if(s_uf^s_ug) // Si tienen distinto s, uf debe ser negativo
    return s_uf == 1; 

  if(exp_uf ^ exp_ug) // Si tienen distinto exp, devuelve 1 cuando exp de uf es menor y uf es positivo ó exp de uf es mayor y uf negativo
    return (exp_uf<exp_ug)^(s_uf);
  if(frac_uf ^ frac_ug)
    return (frac_uf<frac_ug)^(s_uf); //Igual exp pero distinto frac, ídem a lo anterior con frac.
  return 0; //Igual s, exp y frac es igualdad.
}
/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  int sign = (uf >> 31) & 0x1;
  int e = ((uf >> 23) & 0xFF) - 127; //exp - 127
  int frac = uf & 0x7FFFFF;
  int result = 0;

  // Si el exponente es negativo, devuelve 0.
  if (e < 0) {
    return 0;
  }

  // Si el exponente es mayor o igual a 31, esta fuera de rango y devuelve 0x80000000u.
  // Tambien incluye Nan e Infinito (exp = 128)
  if (e >= 31) {
    return 0x80000000u;
  }

  // Agrega el bit implícito (1) a la mantisa y ajusta por el exponente.
  result = frac | 0x800000;
  if (e >= 23) 
    result = result << (e - 23);
  else 
    result = result >> (23 - e);
  
  // Aplica el signo.
  if (sign) 
    result = -result;
  
  return result;
}
/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  
  int bias = 127;
  int exp = x + bias;

  if (exp >= 255)
    return 0x7f800000;
  if (exp <= 0)
    return 0;

  return exp << 23;
}

