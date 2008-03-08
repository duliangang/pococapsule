all: opt

opt:
	javac -O -classpath ../../.. *.java
	cp ../resources/* ../../../../../resources
	cd ../../..; jar -cvf ../../lib/pocotools.jar com/pocomatic/tools/*.class com/pocomatic/resources/*; 

debug:
	javac -g -classpath ../../.. *.java
	cp ../resources/* ../../../../../resources
	cd ../../..; jar -cvf ../../lib/pocotools.jar com/pocomatic/tools/*.class com/pocomatic/resources/*

clean:
	rm -f *.class 
