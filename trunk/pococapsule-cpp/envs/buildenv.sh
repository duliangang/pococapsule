export XERCES_DIR=/work/xerces-c-src_2_7_0
export VBROKERDIR=/work/vb70
export POCOCAPSULE_DIR=/svn/pococapsule/prod/cpp
export POCOCAPSULE_HOME=$POCOCAPSULE_DIR
export TAO_ROOT=/opt/tao
export WASPC_HOME=/opt/systinet/server_cpp65
export LD_LIBRARY_PATH=$XERCES_DIR/lib:$VBROKERDIR/lib:$POCOCAPSULE_DIR/lib:$WASPC_HOME/lib:$TAO_ROOT/lib:.
export JAVA_HOME=/opt/jdk1.5.0_08
export CLASSPATH=$POCOCAPSULE_HOME/lib/pocotools.jar

export PATH=$VBROKERDIR/bin:$JAVA_HOME/bin:$TAO_ROOT/bin:$WASPC_HOME/bin:$PATH:.
export OSAGENT_PORT=14328
