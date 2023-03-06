for file in batteries/00*.yaml
do
    if [[ -f $file ]]; then
        python3 execution-battery.py 1 < $file
    fi
done

