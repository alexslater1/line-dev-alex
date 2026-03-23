rm ../doc/javadoc -rf
set -e
mvn clean compile -q
mkdir -p ../doc/javadoc
javadoc -d ../doc/javadoc \
           -cp "$(mvn dependency:build-classpath -Dmdep.outputFile=/dev/stdout -q):target/classes" \
           -encoding UTF-8 \
           -charset UTF-8 \
           -sourcepath src/main/kotlin \
           -subpackages jline \
           -exclude jline.Scratch \
           2>/dev/null;
