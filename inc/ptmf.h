//---------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief Pointer To Member Functions.
*/
//---------------------------------------------------------------------------------------------------------------------
#ifndef MX_PTMF_H_
#define MX_PTMF_H_

/* -- Includes ------------------------------------------------------------ */


/* -- Defines ------------------------------------------------------------- */


/* -- Types --------------------------------------------------------------- */
namespace mx
{


//compile-time convertion of pointer-to-member-function (ptmf) to function-pointer (fp).
//as a ptmf consists of "two pointers", parameter i specifies which one is desired, to be returned.
//i = 0:
// -if ptmf points to a non-virtual member-function -> get code address of member-function
// -if ptmf points to a virtual member-function -> get offset into objects vtable, to find the entry that points to the respective member-function
//i = 1
// -> the ptrdiff_t value to be added to the object (used to make the ptmf call) to perform the respective up-cast
//
//Usage-Example:
//-------------
// typedef void (*fp_t)(void *);
// static const fp_t handler[] =
// {
//    (fp_t)mx::ptmf<PtmfX>::fp(&PtmfX::learnA, 0), //<- get code address of member-function PtmfX::learnA
//    (fp_t)mx::ptmf<PtmfX>::fp(&PtmfX::learnA, 1)  //<- get value to be added to an object of type PtmfX (as specified by the template parameter) to call member-function PtmfX::learnA
// };
//
//Note: You need at least C++14 (-std=c++14), and maybe a higher optimization level (-O1) .. (-O3)
template<typename T> //T shall be the class name (of the object used) whose member-function pointer is requested
struct ptmf
{
   typedef void (T::*ptmf_t)();
   typedef void (*fp_t)(void *);

   typedef union
   {
      ptmf_t ptmf;
      fp_t fp[2];
   } ptr_u;

   template<typename U> //U shall be a (full qualified) pointer-to-member-function
   static constexpr fp_t fp(U ptmf, unsigned int i = 0)
   {
      ptr_u u{ .ptmf = (ptmf_t)ptmf };
      return u.fp[i];
   }
};





} //namespace mx


/* -- Prototypes ---------------------------------------------------------- */


/* -- Global Variables ----------------------------------------------------- */


/* -- Implementation ------------------------------------------------------ */



#endif

