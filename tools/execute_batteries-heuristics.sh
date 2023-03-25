for file in batteries/ct-BA*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done

for file in batteries/ct-BI*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done

for file in batteries/ct-ER*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done

for file in batteries/ct-WA*.yaml
do
    if [[ -f $file ]]; then
        nohup python3 execution-battery.py 1 < $file
    fi
done

