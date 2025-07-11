cppcheck --enable=all --inconclusive --xml --xml-version=2 \
    -i=../../../apps/calcjob/build/ \
    ../../../apps/calcjob/ 2> results/report.xml