#ifndef CUKE_REGISTRATIONMACROS_HPP_
#define CUKE_REGISTRATIONMACROS_HPP_

// ************************************************************************** //
// **************            OBJECT NAMING MACROS              ************** //
// ************************************************************************** //

// from https://www.boost.org/doc/libs/1_84_0/boost/config/helper_macros.hpp
#define CUKE_JOIN(X, Y) CUKE_DO_JOIN(X, Y)
#define CUKE_DO_JOIN(X, Y) CUKE_DO_JOIN2(X, Y)
#define CUKE_DO_JOIN2(X, Y) X##Y

#ifndef CUKE_OBJECT_PREFIX
    #define CUKE_OBJECT_PREFIX CukeObject
#endif

#ifdef __COUNTER__
    #define CUKE_GEN_OBJECT_NAME_ CUKE_JOIN(CUKE_OBJECT_PREFIX, __COUNTER__)
#else
    // Use a counter to be incremented every time cucumber-cpp is included
    // in case this does not suffice (possible with multiple files only)
    #define CUKE_GEN_OBJECT_NAME_ CUKE_JOIN(CUKE_OBJECT_PREFIX, __LINE__)
#endif

// ************************************************************************** //
// **************                 CUKE OBJECTS                 ************** //
// ************************************************************************** //

#define CUKE_OBJECT_(class_name, parent_class, registration_fn, args)                     \
    class class_name : public parent_class {                                              \
    public:                                                                               \
        void body() override { return invokeWithArgs(*this, &class_name::bodyWithArgs); } \
        void bodyWithArgs args;                                                           \
                                                                                          \
    private:                                                                              \
        static const int cukeRegId;                                                       \
    };                                                                                    \
    const int class_name ::cukeRegId = registration_fn;                                   \
    void class_name ::bodyWithArgs args /**/

#endif /* CUKE_REGISTRATIONMACROS_HPP_ */
