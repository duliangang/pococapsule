set MKS_DIR=D:\PROGRA~1\MKSTOO~1

set SCM_TOOL_DIR=C:\PROGRA~1\Subversion\bin

set POCOCAPSULE_DIR=D:/work/svn/pococapsule/prod/cpp
set CPU=SIMNT
set TOOL=gnu
set VBROKERDIR=D:/Install/vbrt60_simnt
set ACE_ROOT=
set TAO_ROOT=D:/Work/Install/tao_simnt
set TAO_HOST_ROOT=D:\Work\install\tao
set WASPC_HOME=
set XERCES_DIR=
set BUILD_TYPE=debug
set PLATFORM=VxWorks

set JAVA_HOME=C:/PROGRA~1/jdk/jdk1.5.0_10

set JRE_CLIENT_DIR=%JAVA_HOME%/lib
set JRE_INSTALL_DIR=%JAVA_HOME%
set CLASSPATH=%POCOCAPSULE_DIR%/lib/pocotools.jar

set PATH=%POCOCAPSULE_DIR%\bin;%VBROKERDIR%\bin;%XERCES_DIR%\bin;%WASPC_HOME%\bin;%TAO_ROOT%\bin;%TAO_ROOT%\lib;D:\borland\bin;%JAVA_HOME%\bin;%MKS_DIR%\mksnt;%SCM_TOOL_DIR%;%TAO_HOST_ROOT%\lib

REM #D:\Tornado2.2\host\x86-win32\bin\torVars.bat
set WIND_HOST_TYPE=x86-win32
set WIND_BASE=D:/Tornado2.2
set PATH=%WIND_BASE%\host\%WIND_HOST_TYPE%\bin;%PATH%
vcvars32.bat
