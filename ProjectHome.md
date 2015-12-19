PocoCapsule is an _Inversion of Control_ (IoC) container, a.k.a _Dependency Injection_ (DI) framework (see article _[IoC vs DI](http://www.pocomatic.com/docs/whitepapers/ioc-vs-di)_), as well as a _Domain Specific Modeling_ (DSM) engine (see article _[DSM in IoC](http://www.pocomatic.com/docs/whitepapers/dsm)_). The main goals of PocoCapsule are to support declarative assemblying/deploying/configuring of component-based C/C++ applications and also to provide a platform that can easily be used even by domain users to quickly build other component frameworks in a descriptive modeling approach.

  * PocoCapsule is _non-invasive_ and enforces neither a proprietary component programming model (such as their base classes or a set of pre-defined methods) nor any component-container couplings (such as registry or context lookup). It accepts **_container-agnostic_** _plain-old C/C++ objects_ (**_POCO_**) as components. Examples of such components include instances of arbitrary C++ classes, template classes (such as all STL container classes), K&R structs, arrays, and even functions (as function pointers).

  * PocoCapsule supports almost all useful IoC invocation scenarios regardless the numbers of their value/reference inject arguments (in various types, aggregated types, and even user defined types). These invocations include:
    * Constructors (and destructors in _lifecycle_ control) of arbitrary C++ classes/structs/unions/arrays.
    * Global C or C++ functions, operators, and variables.
    * Non-static member functions and operators of C++ classes/structs.
    * Static member functions of C++ classes/structs.
    * Non-static data members of C++ classes/structs.
    * Static data members of C++ classes/structs.
    * Union field accesses.
    * Array (as well as C++ classes/structs with index operator) indexed accesses.

  * PocoCapsule supports component **_lifecycle_** control with any of the following methods specified by user:
    * constructors of C++ classes/structs/unions/arrays
    * global or static factory methods
    * non-static factory methods of factory-beans
    * global or static ref duplicate methods
    * non-static duplicate methods of the beans themselves
    * global or static ref release/destroy methods (and destructors)
    * non-static ref release/destroy methods of destroy-beans.
    * non-static release/destroy methods of the beans themselves.

  * PocoCapsule ensures **_[type safety](Type_Safety_of_PocoCapsule.md)_**. All type conversions are validated statically in building dynamic invocation proxies. The dynamic reflection engine only uses proxies that exactly match the signatures of IoC invocations.

  * PocoCapsule supports **_[dynamic wiring](http://www.pocomatic.com/docs/cpp-examples/basic-ioc/dynamic-wiring)_** that allows application to be deployed without immediately being wired with dependent services. These services can be wired or rewired to applications transparently after or upon they (these services) become available or being replaced.

  * PocoCapsule seamlessly supports user-defined **_[domain specific modeling](http://www.pocomatic.com/docs/whitepapers/dsm)_** schemas through model transformations. These DSM schemas describe applications architectures at higher abstraction levels. This mechanism not only largely avoids problems of the verbose, error-prone, and primitive core IoC schema but also makes the PocoCapsule a framework that can be used by domain users to quickly and easily build other user-defined or committee-designed component frameworks. Several such DSM frameworks are presented in PocoCapsule out-of-the-box or from third parties, such as:
    * **_[PocoCapsule for WebServices](http://www.pocomatic.com/docs/whitepapers/webservices)_**: supports wiring and deployment of component-based _**WebServices**_ applications. It also includes an implementation of the _**OpenSOA's SCA**_ (now _OASIS OpenCSA_) assembly container ([SCA as a DSM](http://www.pocomatic.com/docs/whitepapers/webservices/#sca-as-a-dsm)). See the [article](http://www.pocomatic.com/docs/whitepapers/webservices) and [examples](http://www.pocomatic.com/docs/cpp-examples/#webservices).
    * **_[PocoCapsule for robotic systems](http://www.pocomatic.com/docs/whitepapers/rtc)_**: supports wiring and configuring of _**robotic component**_ applications. It also includes an assembly model for _**OMG-RTC**_ applications. See the [article](http://www.pocomatic.com/docs/whitepapers/rtc) and examples in [user-defined component model](http://www.pocomatic.com/docs/cpp-examples/basic-ioc/robotic-vehicle) and [OMG-RTC component model](http://www.pocomatic.com/docs/cpp-examples/corba/rtc).
    * **_[PocoCapsule for SDR](http://www.pocomatic.com/docs/whitepapers/sdr)_**: supports wiring and configuring of _**software defined radio**_ (_**SDR**_) waveform applications. It also includes an implementation of the assembly container of _**JTRS-SCA**_core framework (CF). See the [article](http://www.pocomatic.com/docs/whitepapers/sdr) and examples in both [non-CORBA non-JTRS-SCA models](http://www.pocomatic.com/docs/cpp-examples/basic-ioc/sdr) and [JTRS-SCA (CF) component and assembly models](http://www.pocomatic.com/docs/cpp-examples/corba/jtrs-sca).
    * **_[PocoCapsule for OMG-DDS](http://www.pocomatic.com/docs/whitepapers/dds)_**: A deployment model for component based _OMG Data Distribution Service_ (**_OMG-DDS_**) applications. See the [example](http://www.pocomatic.com/docs/whitepapers/dds).
    * **_[PocoCapsule for CORBA](http://www.pocomatic.com/docs/whitepapers/corba)_**: (available from [Pocomatic Software](http://www.pocomatic.com)) supports assembly and deployment of component-based _**CORBA**_ server and _**OMG-Event/Notification**_ applications. See [examples](http://www.pocomatic.com/docs/cpp-examples/#corba)

  * PocoCapsule is portable to almost all Unix/Windows and realtime embedded operating systems (such as VxWorks and Symbian OS). The 1.0 source code can be built on Linux, Solaris, Windows, and VxWorks out-of-the-box.