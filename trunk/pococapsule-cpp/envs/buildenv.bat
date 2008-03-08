REM Build Tools

set MKS_DIR=D:\PROGRA~1\MKSTOO~1

set SCM_TOOL_DIR=C:\PROGRA~1\Subversion\bin
set PLATFORM=Windows_NT


set POCOCAPSULE_DIR=D:/work/svn/pococapsule/prod/cpp
set VBROKERDIR=D:/Borland/install/vb70d
set ACE_ROOT=D:/Work/Vendor/ACE_Wrappers
REM set TAO_ROOT=%ACE_ROOT%/TAO
set TAO_ROOT=D:\Work\install\tao
set WASPC_HOME=D:/Install/systinet-server-cpp-6.5-win-vc-71
set XERCES_DIR=D:/work/xerces-c-src_2_7_0
set BUILD_TYPE=debug

set JAVA_HOME=C:/PROGRA~1/jdk/jdk1.5.0_10

set JRE_CLIENT_DIR=%JAVA_HOME%/lib
set JRE_INSTALL_DIR=%JAVA_HOME%
set CLASSPATH=%POCOCAPSULE_DIR%/lib/pocotools.jar

set PATH=%POCOCAPSULE_DIR%\bin;%VBROKERDIR%\bin;%XERCES_DIR%\bin;%WASPC_HOME%\bin;%TAO_ROOT%\bin;%TAO_ROOT%\lib;D:\borland\bin;%JAVA_HOME%\bin;%MKS_DIR%\mksnt;%SCM_TOOL_DIR%;D:\Work\bin

vcvars32.bat
