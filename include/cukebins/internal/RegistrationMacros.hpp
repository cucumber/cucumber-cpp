#ifndef CUKEBINS_REGISTRATIONMACROS_HPP_
#define CUKEBINS_REGISTRATIONMACROS_HPP_

// ************************************************************************** //
// **************            OBJECT NAMING MACROS              ************** //
// ************************************************************************** //

#ifdef __COUNTER__
#define CUKE_GEN_OBJECT_NAME_ BOOST_JOIN(CukeObject, __COUNTER__)
#else
// Use a counter to be incremented every time cukebins is included
// in case this does not suffice (possible with multiple files only)
#define CUKE_GEN_OBJECT_NAME_ BOOST_JOIN(CukeObject, __LINE__)
#endif

// ************************************************************************** //
// **************                 CUKE OBJECTS                 ************** //
// ************************************************************************** //

#define CUKE_OBJECT_(class_name, parent_class, registration_fn) \
class class_name : public parent_class {                                  \
public:                                                                   \
    void body();                                                      \
private:                                                                  \
    static const int cukeRegId;                                           \
};                                                                        \
const int class_name ::cukeRegId = registration_fn ;                      \
void class_name ::body()                                              \
/**/

#endif /* CUKEBINS_REGISTRATIONMACROS_HPP_ */
