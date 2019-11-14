//---------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief Learning C++, Pointer To Member Functions.
*/
//---------------------------------------------------------------------------------------------------------------------
#ifndef POINTER_TO_MEMBER_FUNCTION_H_
#define POINTER_TO_MEMBER_FUNCTION_H_

/* -- Includes ------------------------------------------------------------ */


/* -- Defines ------------------------------------------------------------- */


/* -- Types --------------------------------------------------------------- */
class PtmfA
{
public:
   int a1;
   int a2;
   void * a3;
   void learnA();
};

class PtmfB
{
public:
   int b1;
   int b2;
   void * b3;
   void learnB();
   void learnB1();
};

class PtmfC
{
public:
   int c1;
   int c2;
   void * c3;
   void learnC();
   virtual void learnC1();
   virtual void learnC2();
   virtual void learnC3() = 0;
};

// class PtmfD
// {
// public:
//    int d1;
//    int d2;
//    void * d3;
//    void learnD();
//    virtual void learnD1() = 0;
// };



class PtmfX : public PtmfA, public PtmfB, public PtmfC /*, public PtmfD*/
{
public:
   int x1;
   int x2;
   void * x3;

   void learnB1(); //overwrite
   void learnC2(); //virtual overwrite
   void learnC3(); //virtual definition

   void learnX();
   virtual void learnX1();

   int callX();
};


/* -- Prototypes ---------------------------------------------------------- */


/* -- Global Variables ----------------------------------------------------- */


/* -- Implementation ------------------------------------------------------ */



#endif

