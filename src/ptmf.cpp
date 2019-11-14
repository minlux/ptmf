//---------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief obj C++, Pointer To Member Functions.
*/
//---------------------------------------------------------------------------------------------------------------------


/* -- Includes ------------------------------------------------------------ */
#include <string.h>
#include <stdio.h>
#include "ptmf.h"



/* -- Defines ------------------------------------------------------------- */


/* -- Types --------------------------------------------------------------- */

/* -- (Module-)Global Variables ------------------------------------------- */

/* -- Function Prototypes ------------------------------------------------- */

/* -- Implementation ------------------------------------------------------ */

void PtmfA::learnA()
{
   printf("__________PtmfA__________\n");
   printf("this => %p\n", this);
   printf("sizeof(PtmfA) => %d\n", sizeof(PtmfA));
   printf("----------DATA-----------\n");
   printf("&this->a1 => %p (this + %d)\n", &this->a1, ((int)&this->a1 - (int)this));
   printf("&this->a2 => %p (this + %d)\n", &this->a2, ((int)&this->a2 - (int)this));
   printf("&this->a3 => %p (this + %d)\n", &this->a3, ((int)&this->a3 - (int)this));
   printf("----------PTMF-----------\n");

   //i need the union to examine the pointer to member function ...
   union
   {
      void (PtmfA::*ptmf)(); //a pointer to a member function is twice a "normal" pointer
      void * p[2];
      unsigned int u[2];
   } helper;
   helper.ptmf = &PtmfA::learnA;
   printf("&PtmfA::learnA => %p:%p\n", helper.p[1], helper.p[0]);
}


void PtmfB::learnB()
{
   printf("__________PtmfB__________\n");
   printf("this => %p\n", this);
   printf("sizeof(PtmfA) => %d\n", sizeof(PtmfB));
   printf("----------DATA-----------\n");
   printf("&this->b1 => %p (this + %d)\n", &this->b1, ((int)&this->b1 - (int)this));
   printf("&this->b2 => %p (this + %d)\n", &this->b2, ((int)&this->b2 - (int)this));
   printf("&this->b3 => %p (this + %d)\n", &this->b3, ((int)&this->b3 - (int)this));
   printf("----------PTMF-----------\n");

   //i need the union to examine the pointer to member function ...
   union
   {
      void (PtmfB::*ptmf)(); //a pointer to a member function is twice a "normal" pointer
      void * p[2];
      unsigned int u[2];
   } helper;
   helper.ptmf = &PtmfB::learnB;
   printf("&PtmfB::learnB  => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfB::learnB1;
   printf("&PtmfB::learnB1 => %p:%p\n", helper.p[1], helper.p[0]);
}

void PtmfB::learnB1()
{
   this->b1 = 0xb;
}


void PtmfC::learnC()
{
   printf("__________PtmfC__________\n");
   printf("this => %p\n", this);
   printf("sizeof(PtmfA) => %d\n", sizeof(PtmfC));
   printf("----------DATA-----------\n");
   printf("&this->c1 => %p (this + %d)\n", &this->c1,  ((int)&this->c1  - (int)this));
   printf("&this->c2 => %p (this + %d)\n", &this->c2,  ((int)&this->c2  - (int)this));
   printf("&this->c3 => %p (this + %d)\n", &this->c3,  ((int)&this->c3  - (int)this));
   printf("----------PTMF-----------\n");

   //i need the union to examine the pointer to member function ...
   union
   {
      void (PtmfC::*ptmf)(); //a pointer to a member function is twice a "normal" pointer
      void * p[2];
      unsigned int u[2];
   } helper;
   helper.ptmf = &PtmfC::learnC;
   printf("&PtmfC::learnC  => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfC::learnC1;
   printf("&PtmfC::learnC1 => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfC::learnC2;
   printf("&PtmfC::learnC2 => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfC::learnC3;
   printf("&PtmfC::learnC3 => %p:%p\n", helper.p[1], helper.p[0]);
}


void PtmfC::learnC1()
{
   this->c1 = this->c2;
}


void PtmfC::learnC2()
{
   this->c2 = this->c1;
}



void PtmfX::learnB1()
{
   this->b1 = 0xbb;
}


void PtmfX::learnC2()
{
   this->c2++;
}


void PtmfX::learnC3()
{
   this->c3 = nullptr;
}



void PtmfX::learnX()
{
   printf("__________PtmfX__________\n");
   printf("this => %p\n", this);
   printf("sizeof(PtmfX) => %d\n", sizeof(PtmfX));
   printf("----------OFFSETS-----------\n");
   printf("(PtmfA *)this => %p (this + %d)\n", (PtmfA *)this, ((int)((PtmfA *)this) - (int)this));
   printf("(PtmfB *)this => %p (this + %d)\n", (PtmfB *)this, ((int)((PtmfB *)this) - (int)this));
   printf("(PtmfC *)this => %p (this + %d)\n", (PtmfC *)this, ((int)((PtmfC *)this) - (int)this));
   printf("----------DATA-----------\n");
   printf("&this->a1 => %p (this + %d)\n", &this->a1, ((int)&this->a1 - (int)this));
   printf("&this->a2 => %p (this + %d)\n", &this->a2, ((int)&this->a2 - (int)this));
   printf("&this->a3 => %p (this + %d)\n", &this->a3, ((int)&this->a3 - (int)this));
   printf("&this->b1 => %p (this + %d)\n", &this->b1, ((int)&this->b1 - (int)this));
   printf("&this->b2 => %p (this + %d)\n", &this->b2, ((int)&this->b2 - (int)this));
   printf("&this->b3 => %p (this + %d)\n", &this->b3, ((int)&this->b3 - (int)this));
   printf("&this->c1 => %p (this + %d)\n", &this->c1, ((int)&this->c1 - (int)this));
   printf("&this->c2 => %p (this + %d)\n", &this->c2, ((int)&this->c2 - (int)this));
   printf("&this->c3 => %p (this + %d)\n", &this->c3, ((int)&this->c3 - (int)this));
   printf("&this->x1 => %p (this + %d)\n", &this->x1, ((int)&this->x1 - (int)this));
   printf("&this->x2 => %p (this + %d)\n", &this->x2, ((int)&this->x2 - (int)this));
   printf("&this->x3 => %p (this + %d)\n", &this->x3, ((int)&this->x3 - (int)this));
   printf("----------CODE-----------\n");

   //i need the union to examine the pointer to member function ...
   union
   {
      void (PtmfX::*ptmf)(); //a pointer to a member function is twice a "normal" pointer
      void * p[2];
      unsigned int u[2];
   } helper;

   helper.ptmf = &PtmfX::learnA;
   printf("&PtmfX::learnA  => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfX::learnB;
   printf("&PtmfX::learnB  => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfB::learnB1;
   printf("&PtmfB::learnB1 => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfX::learnB1;
   printf("&PtmfX::learnB1 => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfX::learnC;
   printf("&PtmfX::learnC  => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfX::learnC1;
   printf("&PtmfX::learnC1 => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfX::learnC2;
   printf("&PtmfX::learnC2 => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfX::learnC3;
   printf("&PtmfX::learnC3 => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfX::learnX;
   printf("&PtmfX::learnX  => %p:%p\n", helper.p[1], helper.p[0]);
   helper.ptmf = &PtmfX::learnX1;
   printf("&PtmfX::learnX1 => %p:%p\n", helper.p[1], helper.p[0]);
}


void PtmfX::learnX1()
{
   this->x1 = 0;
}

#if 0
void PtmfX::callX()
{
   void (PtmfX::*ptmf)(); //define a local variable, of type "pointer to member function"

   //call by function pointer
   ptmf = &PtmfX::learnA;
   (this->*ptmf)(one, two, three);
   //call by function pointer
   ptmf = &PtmfX::learnB;
   (this->*ptmf)(one, two, three);
   //call by function pointer
   ptmf = &PtmfX::learnC;
   (this->*ptmf)(one, two, three);
   //call by function pointer
   ptmf = &PtmfX::learnC1;
   (this->*ptmf)(one, two, three);
   //call by function pointer
   ptmf = &PtmfX::learnX;
   (this->*ptmf)(one, two, three);

   return 0;
}
#endif
