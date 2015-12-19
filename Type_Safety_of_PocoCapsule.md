## Type Safety in C++ ##

Type safety in C++ means language type systems are able to detect and prohibit invalid type conversions. Common type operations, that are relevant to using PocoCapsule, are summarized in the following table:

| **type operation** | **valid conversion** | **note** | **type check** |
|:-------------------|:---------------------|:---------|:---------------|
| implicit conversion | up a type hierarchy  | static and safe  | compile time   |
| dynamic\_cast      | within a type hierarchy | safe, but relies on RTTI | runtime        |
| static\_cast       | within a type hierarchy | unsafe on down and horizontal casts | compile time, but limited (1) |
| C-style or reinterpret\_cast | any to any           | unsafe (2) | no             |
| C-style or reinterpret\_cast to/from opaque | any to any           | unsafe and error prone (3) | no             |

  1. Many invalid type casts of using static\_cast could not be reported by compiler. For instance, static\_cast across type hierarchies, static\_cast from a base class implementation (that does not support the subclass) to a subclass reference, etc..
  1. C-style type cast and reinterpret\_cast operations allow any types to be converted to any other types. These casts include conversions up, down, and horizontal within type hierarchies, as well as across hierarchies. For valid conversions with in type hierarchies, these casts generate correct results. However, these casts do not detect or prohibit invalid down and horizontal casts within hierarchies and wrong type casts that across hierarchies.
  1. C-style type cast and reinterpret\_cast to/from opaque allow any pointer types to be converted to opaques (such as void pointers, char or uchar pointers, integers) and then casted back to pointers of any other types. Like direct C-style or reinterpret\_cast, these indirect (through opaque) casts do not detect or prohibit invalid end-to-end conversions neither. And worse than that, such an indirect conversion completely bypasses the C++ language type system. Therefore, it could generate incorrect results even in valid type conversions involving multi-inheritance, unless the opaque is casted back to the exactly original converted-from pointer type without moving up or down in its type hierarchy.

## Type Safety in PocoCapsule ##

Instead of dictating a single predefined type hierarchy (i.e. a common base class), PocoCapsule supports heterogeneous user-defined type hierarchies. Namely, supported components are plain-old C/C++ objects (POCOs) of various user-defined types that do not share a single common base type. Examples of these POCO types include, for instance, classes without any base class, classes derived from various different user-defined base classes, K&R plain-old data (POD) structs, arrays,  and function types (in form of function pointers). Components of these types are internally manipulated and even returned as opaques. PocoCapsule ensures type casts to/from these opaques made by its IoC container in performing dependency injections are always valid. PocoCapsule also supplies component type information at runtime to help applications ensure they are making valid type casts on component reference opaques returned from directory (i.e. application context) lookup.

Type safety in **_dependency-injection_** scenarios is ensured by the following two rules in PocoCapsule:

  * IoC proxies only apply implicit type conversions (for non-opaques).
  * IoC proxies use C-style type casts only to convert bean reference opaques back to their exactly original casted-from types (which are types declared in application descriptions).

These two rules ensure 100% type safety in _dependency injection_ usage scenarios where dependency references are injected as IoC invocation input parameters. Type conversion errors will be reported by C++ compilers on building proxies.

Type safety in **_directory-lookup_** scenarios is ensured by either of the following two rules in casting a bean reference opaque retrieved from a PocoCapsule bean directory (application context):

  * An user should specify the bean's type-id on retrieving its reference opaque from the directory. PocoCapsule compares this type-id with the bean's original casted-from type-id (which is the _class_ attribute declared in application description) and only returns the opaque when the comparison exactly matchs or returns NULL on failure. User should first cast the returned opaque pointer to the specified type before carrying out further casts (such as dynamic\_cast). For example:
```
  Foo* ref = (Foo*)ctxt->getBean("my-foo-bean", "Foo");
 if( ref == NULL ) {
    // error: either bean does not exists or it mismatches with the type Foo.
    ...
 } 

 // further casts
 Foo2* ref2 = ref;
 Foo3* ref3 = dynamic_cast<Foo3*>(ref);
 ...
```

  * The user should retrieve the bean's type-id from the directory (use the getBeanPtrTypeId() method on the application context). This is the pointer type id of the original declared type. Before perform further casts, User should first cast the opaque pointer to a pointer type that matches bean's pointer type id. For example:
```
  Foo* ref = (Foo*)ctxt->getBean("my-foo-bean");
 if( ref == NULL ) {
   // error: bean does not exist ... 
   ... 
 }
 
 const char* ptr_type_id = ctxt->getBeanPtrTypeId("my-foo-bean");
 if( strcmp(ptr_type_id, "Foo*") && strcmp(ptr_type_id, "Foo_ptr") ) {
    // error: the bean's type mismatches with the type Foo. 
    ... 
 } 

 // further casts
 Foo2* ref2 = ref;
 Foo3* ref3 = dynamic_cast<Foo3*>(ref);
 ... 
```

## Miscellaneous ##
  * Type safety is not about binary compability.
  * PocoCapsule type system treats C++ typedef'ed types as distinct types.
  * Implicit type conversions in PocoCapsule are not performed by PocoCapsule type system dynamically, but utilize C++ type system in the generated proxies.

