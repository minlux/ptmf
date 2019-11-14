# C++ Pointer to Member Function

A challenge for every C++ programmer are pointer to member functions (*ptmf*). *ptmf*s are diffent than their C counterparts - function pointers (*fp*).
Using function pointers in C is a common practice and gives the programmer a powerful technique for a good pice of software. When used correctly, *fp*s can help to:
- improve software's performance
- lead to modulare and reusable components (when using *fp*s to couple components)

Function pointers can also be used in C++. However *fp*s can only be used to point to static class functions. *fp*s can't be used to point to member functions, as a member function must be always called in combination with a object instance.

![Example](doc/example.png)

Using a function pointer to point to `char * Person::race(void)` is possible, as this is a static function, that can be called without an instance of `Person` (*race* of any person is "human" - therfore no *Person* instance is neccessary).
```
char * (*getRace)(void) = &Person::race; //getRace is a fp, that points to a function that takes void and returns char *, now pointing to Persons's static "race" function.
```

However a function pointer can't be used to point to `char * Person::first_name(void)` or  `char * Person::first_last(void)` as a *Person* instance is required to get the name of the respective persion. This is where a C++ pointer to member function *ptmf* comes into play.

## Definition
*ptmf* defines not only the functions signature, it defines also the class they are related. E.g. the following *typedef* defines a *ptmf*, that points to member function of class *Person*, that takes "void" and returns a "char *". The respective typedef is named "GetName_t".
```
typedef char * (Person::*GetName_t)(void);
```

## Usage
To use a *ptmf*, it must first be set to the member function, that shall be called later on, using it. This is done like this:
```
GetName_t getName; //defintion (getName is a ptmf that points to a member function of Person, that takes void and returns a char *)
getName = &Person::first_name; //assign reference to the method that shall be called later on (with this ptmf)
```

To call the member function, assigned to the *ptmf* an instance of *Person* is required. Then the invocation of the *ptmf* is done like this:
```
Person manuel;
...
char * name = (manuel.*getName)(); //use the .* notation for object references
```
Here we are using person instance *manuel* to call the member function that was assigned to the ptmf *getName*. In this example - it will call `manuel.first_name()`.
The parentheses aroung `manuel.*getName` are importent.

To use the *ptmf* in combination with a pointer to *Person* the syntax is like this:
```
Person * manuel;
...
char * name = (manuel->*getName)(); //use the ->* notation for object pointers
```

## How *ptmf*s work
*fp*s and *ptmf*s are incompatible - the reason is that they are made for different techniques. *fp*s are made for C, *ptmf* are made for C++.

### How *fp*s work
Before starting with *ptmf*s lets refresh our knowledge about C function pointers *fp*s:
- A C function pointer is a pointer having the address of a function.
- A C function pointer is a pointer is aware about the signature of the function it is pointing to.

When invoking a function pointer, the compiler set ups the function arguments (and later on the return value) according to the known signature and simply jumps to the address that is stored in the pointer. Thus the addressed function is called. That's it!

### Playint C++ to C compiler
Let's play C++ to C compiler, to get some basic undestanding how C++ works. Therfore lets compile down a C++ class into its C equivalent.

1. Class becomes struct:
```
class Person {
private:
   char fn[32];
   char ln[32];
   int age;
public:
   static char * race(void);
   char * first_name(void);
   char * last_name(void);
   void set_age(int age);
};

-->

struct Person {
   char fn[32];
   char ln[32];
};
```

2. The member functions becomes "normal" functions, each with a `struct Person *` as frist argument (formerly the "invisible" *this* pointer passed into every member function).
```
   char * Person_first_name(struct Person * thiz);
   char * Person_last_name(struct Person * thiz);
   void Person_set_age(struct Person * thiz, int age);
```
Note: The static class functions doesn't get such a `struct Person *` pointer prefixed. Their signatur does not change - as they are already "normal" functions!


### Forceing the address of a member function into a *fp*
Now, that we know that a member function can be considered as a C function with a leading pointer that points to the class object, a clever C/C++ programmer can get the idea to force the address of a member function into a *fp*. The *fp*s signature must surely be the signature of the member functions C-equivalent! For example like that:
```
typedef char * (*PersonGetName_t)(void * thiz); //a typedef for a C function pointer, with a signature, corresponding to Person::last_name's C-equivalent
PersonGetName_t getName = (PersonGetName_t)&Person::first_name; //trying to assign address of member function to *fp* - dosn't work!
```
And if that would work, the "clever programmer" could use this function pointer together with a struct Person pointer, to invoke a C++ member function with C technique.
```
struct Person manuel;
...
char * name = getName(&manuel);
```
However, the C++ compiler doesn't allows the assignment - event not with a cast!!!
Likewise, it does not allow the assignment of a *ptmf* to a *fp*:
```
typedef char * (*PersonGetName_t)(void * thiz); //a typedef for a C function pointer, with a signature, corresponding to Person::last_name's C-equivalent
...
char * (Person::*getNamePtmf)() = &Person::first_name; //this works - assigning the address of a member funtion to a *ptmf* (defined and assinged in one statement)
PersonGetName_t getName = (PersonGetName_t)getNamePtmf; //trying to assign a *ptmf* to a *fp* - doesn't work!!!
```


### Multiple Inheritance
The reasone why a C++ compiler doesn't allow the assignment of a member function's address to a C function pointer relates to the *multiple inheritance* feature of C++.
In C++ a class can derive from serveral base classes. The lyout of base class data attributes in the child class, is done by the compile. This may look like this example:

To clarify this, let's have a look to the following example:
![Classes](doc/classes.png)
Class `PtmfX` uses multiple inheritance to derive from class `PtmfA`, `PtmfB` and `PtmfC`. So `PtmfX` is composed out of the data attribues of `PtmfA`, `PtmfB`, `PtmfC` and its own data. This can be illustrated like this:
![PtmfX](doc/ptmfx.png)
`PtmfX` starts with the data of `PtmfC`, followed by `PtmfA`, follwoed by `PtmfB` and finally by its own data. So the `PtmfC` data has an offset of `PtmfX`s **this + 0**. `PtmfA` has an offset of **this + 16** and `PtmfC` has an offset of **this + 28**. `PtmfX`s own data starts at offset **this + 40**. Thus the following type conversion / upcasting rules apply (here: x is pointer to PtmfX)
- PtmfX pointer to PtmfA pointer: *this* + 16
- PtmfX pointer to PtmfB pointer: *this* + 28
- PtmfX pointer to PtmfC pointer: *this* + 0

`PtmfX` has also derived all the functions of the base classes.
- It shaddows the non-virtual function `PtmfB:learnB1` with its own implementation `PtmfX:learnB1`
- It overwrite virtual function `PtmfC::learnC2` with its own implementation `PtmfX:learnC2`
- It defines the pure-virtual function `PtmfC::learnC3` with its implementation `PtmfX:learnC3`
All the other derived functions will be used by `PtmfX` as they are defined in the respective base class. There will be no extra copy of those functions for `PtmfX`. That's important to know (for the following discussion). Just to clairfy that: `PtmfX::learnA` is just an alias for `PtmfA::learnA`. It is **the same** function!!!


Now with that in mind, lets look at this well working "C++ member function call" example:
```
PtmfX * x = new PtmfX();
x->learnA(); //works! spoiler: under the hood, x it casted from PtmfX * to a PtmfA * (by adding the respive offset) and then `PtmfA::learnA` is called
```

It also works when we use a pointer to a member function.
```
PtmfX * x = new PtmfX();
typedef void (PtmfX::*learnFunction_t)(); //typedef of a pointer to a member function of class PtmfX
learnFunction_t learn; //define such a pointer to member function

learn = &PtmfX::learnA; //assign
(x->*learn)(); //works! spoiler: to make that work, x (of type PtmfX *) must be converted into a PtmfA * (by adding the respective offset (16) to x)
```

Try to do it with a C function pointer + a PtmfX object would raise the problem (...which we have been waiting for so long):
```
PtmfX * x = new PtmfX();
typedef void (*learnFunction_t)(void *); //typedef of a C function pointer with a C-equivalent signature for the different learnX functions of PtmfX
learnFunction_t learn; //define such a function pointer

learn = (learnFunction_t)&PtmfX::learnA; //not possible: assign address of member function to *fp*
learn(x); //call function pointer with the respective object
```
For all of use, how doesn't see the problem yet...
The problem is, that the function we are calling - `PtmfX::learnA` which is actually `PtmfA::learnA` - expects a pointer to a `PtmfA` object. However we are passing in a pointer to a `PtmfX`. And we can see in the class diagram above, that a `PtmfX` pointer is different to a `PtmfA` pointer (because of the offset). Adding the offset (16) to the `PtmfX` pointer would make it a `PtmfA` pointer - but this isn't done when doing it the *C way* we tried. However it is done (under the hood) when using the C++ method call oder the C++ pointer to member function call.


### Type Conversion (Upcast)
On invocation of a *ptmf* an implicit upcast of the object pointer (involved in the pointer to member function call) is performed. But where does the information come, how to upcaste the object pointer. Quick answer: from the *ptmf*.

Imagine the following C++ *ptmf* exmaple:
```
PtmfX * x = new PtmfX();
typedef void (PtmfX::*learnFunction_t)(); //typedef of a pointer to a member function of class PtmfX
learnFunction_t learn; //define such a pointer to member function

learn = &PtmfX::learnA; //assign
(x->*learn)(); //as learn pointer to &PtmfX::learnA, x must be upcasted to a PtmfA pointer (by adding +16)

learn = &PtmfX::learnB; //assign
(x->*learn)(); //as learn pointer to &PtmfX::learnB, x must be upcasted to a PtmfB pointer (by adding +28)
...

(x->*learn)(); //now learn can point to any function, how does the compile know at this point, how to convert x to match the type of the function learn is pointing to???
```

If we have a closer look at a *ptmf* we can figure out, that a *ptmf* is not only one pointer. In fact a *ptmf* contains two pointers. One pointer contains the the address of the function, that shall be called (exactly as a *fp*). The other pointer contains the offset, that must be added to the object, on invocation!


### Pointer to virtual member functions
We are not at the end of this story. If the *ptmf* points to a virtual function. The content of the *ptmf* is different. In this case, it doesn't contain the address of the member function to be called. Instead it contains an offset into the vtable of the respective object to find the function that shall be called.

Example:
```
PtmfX * x = new PtmfX();
typedef void (PtmfX::*learnFunction_t)(); //typedef of a pointer to a member function of class PtmfX
learnFunction_t learn; //define such a pointer to member function

learn = &PtmfX::learnC1; //let learn point to a virtual function
(x->*learn)(); //as learn pointer to &PtmfC::learnC1,
                 //x must be upcasted to a PtmfC pointer first (in this case by adding +0)
                 //then, as learnC1 is a virtual function, the respective vtable entry must be read
                 //to get the address of the function to be called
```

If we have a closer look at a *ptmf* we can figure out, that in the case *ptmf* is pointing to a virtual member function, the first pointer inside *ptmf* contains the offset into the vtable of the upcasted object pointer. To upcast the object pointer, the offset stored within the second pointer inside *ptmf* must be added to it.

The *ptmf* must also store a flag to indicate, if it is pointing to a "normal" member function or to a virtual member function - as this defines the meaning of the two pointers inside the *ptmf* and the conversion rules that must be applied.


If a clever C programmer considers all that stuff, I am sure a solution can be found to implement a target and compiler specific solution (a non portable solution) to even call C++ member functions with a {void *, *fp*}-tupel.



## Appendix
Attached application, compiled with a 32-MinGW GCC on a Windows maschine, results in the following output:

```
__________PtmfA__________
this => 0063fef4
sizeof(PtmfA) => 12
----------DATA-----------
&this->a1 => 0063fef4 (this + 0)
&this->a2 => 0063fef8 (this + 4)
&this->a3 => 0063fefc (this + 8)
----------PTMF-----------
&PtmfA::learnA => 00000000:004013e4


__________PtmfB__________
this => 0063fee8
sizeof(PtmfA) => 12
----------DATA-----------
&this->b1 => 0063fee8 (this + 0)
&this->b2 => 0063feec (this + 4)
&this->b3 => 0063fef0 (this + 8)
----------PTMF-----------
&PtmfB::learnB  => 00000000:004014c4
&PtmfB::learnB1 => 00000000:004015d8


__________PtmfC__________
this => 0063feb4
sizeof(PtmfA) => 16  <-- as PtmfC has virtual functions, it takes 4 bytes more because of the invisible vtable pointer
----------DATA-----------
&this->c1 => 0063feb8 (this + 4)   <-- c1 starts at offset 4, because at the first 4 bytes, there is the vtable pointer
&this->c2 => 0063febc (this + 8)
&this->c3 => 0063fec0 (this + 12)
----------PTMF-----------
&PtmfC::learnC  => 00000000:004015ee
&PtmfC::learnC1 => 00000000:00000001
&PtmfC::learnC2 => 00000000:00000005
&PtmfC::learnC3 => 00000000:00000009


__________PtmfX__________
this => 0063feb4
sizeof(PtmfX) => 52
----------OFFSETS-----------
(PtmfA *)this => 0063fec4 (this + 16)  <-- PtmfA data starts at offset 0x10
(PtmfB *)this => 0063fed0 (this + 28)  <-- PtmfB data starts at offset 0x1C
(PtmfC *)this => 0063feb4 (this + 0)   <-- PtmfC data starts at offset 0x1C
----------DATA-----------
&this->a1 => 0063fec4 (this + 16)
&this->a2 => 0063fec8 (this + 20)
&this->a3 => 0063fecc (this + 24)
&this->b1 => 0063fed0 (this + 28)
&this->b2 => 0063fed4 (this + 32)
&this->b3 => 0063fed8 (this + 36)
&this->c1 => 0063feb8 (this + 4)
&this->c2 => 0063febc (this + 8)
&this->c3 => 0063fec0 (this + 12)
&this->x1 => 0063fedc (this + 40)   <-- the PtmfX data follows the data of the base classes
&this->x2 => 0063fee0 (this + 44)
&this->x3 => 0063fee4 (this + 48)
----------CODE-----------
&PtmfX::learnA  => 00000010:004013e4
&PtmfX::learnB  => 0000001c:004014c4
&PtmfB::learnB1 => 0000001c:004015d8   <-- base class's learnB1 (PtmfB::learnB1)
&PtmfX::learnB1 => 00000000:0040179c   <-- child class's learnB1 (PtmfX::learnB1)
&PtmfX::learnC  => 00000000:004015ee
&PtmfX::learnC1 => 00000000:00000001   <-- virtual functions are diffrent
&PtmfX::learnC2 => 00000000:00000005   <-- virtual functions are diffrent
&PtmfX::learnC3 => 00000000:00000009   <-- virtual functions are diffrent
&PtmfX::learnX  => 00000000:004017e4
&PtmfX::learnX1 => 00000000:0000000d   <-- virtual functions are diffrent
                                   ^
                              ^    +-- bit 0 indicates whether the ptmf points to a virtual member function or not
                        ^     +-- this is either the function address of the offset into the vtable
                        +-- this is the offset thas must be added to the object for the respective type cast (upcast)
```



## Other resources

https://isocpp.org/wiki/faq/pointers-to-members
https://shaharmike.com/cpp/vtable-part1/
https://shaharmike.com/cpp/vtable-part2/
https://ww2.ii.uj.edu.pl/~kapela/pn/cpp_vtable.html
