1. Untar the source code (pococapsule-cpp-src.tar) into a directory
(e.g. pococapsule-cpp). for instance:

> % tar -xvf pococapsule-cpp-src.tar

2. Set the POCOCAPSULE\_DIR environment to the full path of this directory. For instance:

> % setenv POCOCAPSULE\_DIR /home/kjin101/pococapsule-cpp

3. Make sure JAVA\_HOME environment variable is set to a JDK (1.4 and up) installation directory. For instance:

> % setenv JAVA\_HOME /usr/lib/jvm/java-1.5.0-sun-1.5.0.08

4. Set the JRE\_INSTALL\_DIR and JRE\_CLIENT\_DIR environment variables. For instance:

> % setenv JRE\_INSTALL\_DIR ${JAVA\_HOME}

> % setenv JRE\_CLIENT\_DIR ${JRE\_INSTALL\_DIR}/jre/lib/i386/client

5. For platforms other than linux (32bites) and windows, find the installed JVM runtime library (i.e. libjvm.so) location and modify the CLNT\_JVM marco in the src/xslreaders/xslreaders.C and src/tools/launchers/launcher.C accordingly.

6. Install the Apache Xerces and set the XERCES\_DIR to the
installed directory. For instance:

> % setenv XERCES\_DIR /home/kjin101/xerces-cpp

7. (**optional**) Install HP Systinet WASP and set the WASP\_HOME to the installed directory. If you don't want to build the WebServices helper for Systinet WASP, remove it from the pococapsule-cpp/src/Makefile.

8. Change to the $POCOCAPSULE\_CPP/src directory and run the make or gmake commend. for instance:
> % cd $POCOCAPSULE\_CPP/src
> % make

9. As a result, the PocoCapsule runtime libraries, utility jar files,
user header files, and XML/DTD/XSLT source files will be copyed to the
${POCOCAPSULE\_DIR}/lib, include, and resources directories.