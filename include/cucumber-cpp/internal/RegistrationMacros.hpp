#ifndef CUKE_REGISTRATIONMACROS_HPP_
#define CUKE_REGISTRATIONMACROS_HPP_

// ************************************************************************** //
// **************            OBJECT NAMING MACROS              ************** //
// ************************************************************************** //

#ifndef CUKE_OBJECT_PREFIX
#define CUKE_OBJECT_PREFIX CukeObject
#endif

#ifdef __COUNTER__
#define CUKE_GEN_OBJECT_NAME_ BOOST_JOIN(CUKE_OBJECT_PREFIX, __COUNTER__)
#else
// Use a counter to be incremented every time cucumber-cpp is included
// in case this does not suffice (possible with multiple files only)
#define CUKE_GEN_OBJECT_NAME_ BOOST_JOIN(CUKE_OBJECT_PREFIX, __LINE__)
#endif

// ************************************************************************** //
// **************                 CUKE OBJECTS                 ************** //
// ************************************************************************** //

#define CUKE_OBJECT_(class_name, parent_class, registration_fn, args)     \
class class_name : public parent_class {                                  \
public:                                                                   \
    void body() {                                                         \
        return invokeWithArgs(*this, &class_name::bodyWithArgs);          \
    }                                                                     \
    void bodyWithArgs args;                                               \
private:                                                                  \
    static const int cukeRegId;                                           \
};                                                                        \
const int class_name ::cukeRegId = registration_fn ;                      \
void class_name ::bodyWithArgs args                                       \
/**/

#endif /* CUKE_REGISTRATIONMACROS_HPP_ */
