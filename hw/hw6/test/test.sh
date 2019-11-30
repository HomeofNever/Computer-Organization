#!/usr/bin/zsh

PREFIX=hw6
SETNUM=(1 2 4)
METHODS=(LRU Belady)
INPUT=(01 02 03 04)
for n in "${SETNUM[@]}"
do
    for m in "${METHODS[@]}"
    do
      for f in "${INPUT[@]}"
      do
        echo "${n}-${m} input${f}.txt"
        echo "----------------------------------"
        ../cmake-build-debug/hw6 "${n}" "$m" "${PREFIX}-input${f}.txt" > output.txt
        echo "diff: output.txt ${PREFIX}-${n}-${m}-output${f}.txt"
        diff output.txt "${PREFIX}-${n}-${m}-output${f}.txt"
        echo "----------------------------------"
      done
    done
done