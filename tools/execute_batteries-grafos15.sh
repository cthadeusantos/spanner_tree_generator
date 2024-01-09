for file in batteries/grafo15-bf-*.yaml
do
    if [[ -f $file ]]; then
        python3 execution-battery.py 1 < $file
    fi
done