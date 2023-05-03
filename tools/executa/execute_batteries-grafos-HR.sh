for file in batteries/02-ct*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done
