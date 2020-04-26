//-----------------------------------------------------------------------------
/*!
   \file
   \brief Playing around with lambdas.

   https://medium.com/software-design/c-lambda-under-the-hood-9b5cd06e550a

   https://shaharmike.com/cpp/lambdas-and-functions/
   https://shaharmike.com/cpp/naive-std-function/

   https://stackoverflow.com/questions/18453145/how-is-stdfunction-implemented
   https://www.heise.de/developer/artikel/C-Core-Guidelines-Type-Erasure-4158206.html
   https://www.heise.de/developer/artikel/C-Core-Guidelines-Type-Erasure-mit-Templates-4164863.html
   https://www.heise.de/developer/artikel/C-Core-Guidelines-Regeln-zu-Templates-und-generischer-Programmierung-4152964.html
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------ */
#include <stdint.h>
#include <iostream>

/* -- Defines ------------------------------------------------------------- */
using std::cout;
using std::endl;


/* -- Types --------------------------------------------------------------- */

class FlexibleCallback
{
   typedef int (*Fp_t)(void * arg, char * buffer, int size); //function pointer
   typedef struct
   {
      Fp_t fp;    //first:  function pointer
      void * arg; //second: argument
   } Callback_t;
   Callback_t cb;


public:
#if 0 //support for NULL
   FlexibleCallback(long nul)
   {
      (void)nul; //not used
      this->cb.fp = nullptr;
      this->cb.arg = nullptr;
   }
#endif

   //support for nullptr
   FlexibleCallback(nullptr_t nul)
   {
      (void)nul; //not used
      cout << "FlexibleCallback construction using nullptr_t" << endl;
      this->cb.fp = nullptr;
      this->cb.arg = nullptr;
   }

   template<typename T>
   FlexibleCallback(const T& t)
   {
      typedef int (T::*Ptmf_t)(char *, int) const; //pointer to member function
      union
      {
         Ptmf_t ptmf;
         Callback_t cb;
         void * debug[2];
      } u;
      cout << "FlexibleCallback construction using T&" << endl;
      cout << "- sizeof(T): " << sizeof(T) << endl;
      //here we use the union to perform the type erasure
      u.ptmf = &T::operator(); //get pointer to member function (ptmf). Attention: there are two pointers inside. (1st: address of member function "operator()"; 2nd: offset to be added to the instance pointer (to perform an optional upcast))
      cout << "- &operator() [0]: " << u.debug[0] << endl;
      cout << "- &operator() [1]: " << u.debug[1] << endl;
      cout << "- &t: " << (void *)&t << endl;
      u.cb.arg = (void *)((uintptr_t)&t + (uintptr_t)u.cb.arg); //add the offset information from the ptmf to the instance pointer t.
      this->cb = u.cb; //store/copy ABI compatible C representation to call a C++ member function. some persons would say that's dirty!?
   }

   FlexibleCallback(Fp_t cb)
   {
      cout << "FlexibleCallback construction using Fp_t" << endl;
      this->cb.fp = cb;
      this->cb.arg = nullptr;
   }

   explicit FlexibleCallback(Fp_t cb, void * arg)
   {
      cout << "FlexibleCallback construction using <Fp_t, void*>" << endl;
      this->cb.fp = cb;
      this->cb.arg = arg;
   }

   int operator()(char * buffer, int size) const
   {
      if (cb.fp == nullptr) return 0; //do nothing if no function pointer is set. return 0!
      return cb.fp(cb.arg, buffer, size); //call the function with the given argument - As i am ABI compatible, i can also call C++ member functions this way *hurrah*
   }
};




/* -- (Module) Global Variables ------------------------------------------- */


/* -- Implementation ------------------------------------------------------ */




int send_cpplamba(const FlexibleCallback& handler)
{
   char buffer[128];
   return handler(buffer, sizeof(buffer));
}



int a_c_send_function(void * arg, char * buffer, int size)
{
   cout << "a_c_send_function: Bufsize = " << size << endl;
   if (arg != NULL)
   {
      int * status = (int *)arg;
      cout << "Status: " << *status << endl;
   }
   buffer[0] = size;
   return size;
}



int main(int argc, char * argv[])
{
   int status;
   int capture = 0;

   // status = send_cpplamba(NULL);
   cout << "1)----------------" << endl;
   status = send_cpplamba(nullptr);


   cout << "\n2)----------------" << endl;
   status = send_cpplamba(&a_c_send_function);


#if 0 //doesn't work!!!
   //doesn't work - because it invokes FlexibleCallback(const T& t) constructor
   //instead of FlexibleCallback(Fp_t cb) constructor :-(
   status = send_cpplamba([](void * arg, char * buffer, int size) {
      return size;
   });
#endif


   cout << "\n3)----------------" << endl;
   status = send_cpplamba(FlexibleCallback([](void * arg, char * buffer, int size) {
      cout << "a_c_lambda: Bufsize = " << size << endl;
      int *capture = (int *)arg;
      cout << "Capture: " << *capture << endl;
      buffer[0] = size;
      return size;
   }, &capture)); //explicit using FlexibleCallback(Fp_t cb, void * arg) constructor works!


   cout << "\n4)-----------------" << endl;
   status = 7;
   status = send_cpplamba(FlexibleCallback(&a_c_send_function, &status));


   cout << "\n5)-----------------" << endl;
   status = send_cpplamba([](char * buffer, int size){
      cout << "C-Lambda: Bufsize = " << size << endl;
      buffer[0] = size;
      return size;
   });


   cout << "\n6)-----------------" << endl;
   capture = 42;
   status = send_cpplamba([&capture](char * buffer, int size){
      cout << "Object-Lambda: Bufsize = " << size << endl;
      cout << "Capture: " << capture << endl;
      ++capture;
      buffer[0] = size;
      return size;
   });

   cout << "\n7)-----------------" << endl;
   status = send_cpplamba([&capture](char * buffer, int size){
      cout << "Object-Lambda: Bufsize = " << size << endl;
      cout << "Capture: " << capture << endl;
      ++capture;
      buffer[0] = size;
      return size;
   });


   cout << "\n8)-----------------" << endl;
   status = send_cpplamba([capture](char * buffer, int size){
      cout << "Object-Lambda: Bufsize = " << size << endl;
      cout << "Capture: " << capture << endl;
      buffer[0] = size;
      return size;
   });

   return 0;
}

