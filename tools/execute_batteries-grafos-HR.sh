for file in batteries/02-ct*.yaml
do
    if [[ -f $file ]]; then
        python3 execution-battery.py 1 < $file
    fi
done