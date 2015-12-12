/* Author : Teja Sasank Gorthi
   Email  : jet.sasank@gmail.com */


/*Problem : Assume that a system has a 32-bit virtual address with a 4-KB page size. 
Write a C program that is passed a virtual address (in decimal) on the command line and have it output the page number and offset for the given address.*/

#include <stdio.h>

int main(int argc, char *argv[])
{
unsigned long page;
unsigned long offset;
unsigned long address;


address= atoll(argv[1]);
/* Page Number =  quotient of  address / 4KB and offset = remainder*/
/*Below is the faster method of calculating the same*/

page = address >> 12; /* Since page size is 4KB => 12 bits holding the virtual address*/
offset = address & 0xfff;

printf("The address %lu contains: \n", address);
printf("page number = %lu\n",page);
printf("offset = %lu\n", offset);


return 0;
}