for file in batteries/03-ct-1000*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done