SRC_DIR=../../../apps/calcjob
RESULTS_DIR=./results/
cloc --csv --exclude-dir build --found $RESULTS_DIR/found.txt --out $RESULTS_DIR/cloc_metrics.csv $SRC_DIR