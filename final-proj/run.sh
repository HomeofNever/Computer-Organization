#!/bin/bash

python3 p1.py NF test/p1-input01.txt > temp/p1-output01.txt
python3 p1.py NF test/p1-input02.txt > temp/p1-output02.txt
python3 p1.py F test/p1-input03.txt > temp/p1-output03.txt
python3 p1.py F test/p1-input04.txt > temp/p1-output04.txt
python3 p1.py NF test/p1-input05.txt > temp/p1-output05.txt
python3 p1.py NF test/p1-input06.txt > temp/p1-output06.txt
python3 p1.py NF test/p1-input07.txt > temp/p1-output07.txt
python3 p1.py F test/p1-input08.txt > temp/p1-output08.txt
python3 p1.py F test/p1-input09.txt > temp/p1-output09.txt

diff test/p1-output01.txt temp/p1-output01.txt > temp/diff-1.txt
diff test/p1-output02.txt temp/p1-output02.txt > temp/diff-2.txt
diff test/p1-output03.txt temp/p1-output03.txt > temp/diff-3.txt
diff test/p1-output04.txt temp/p1-output04.txt > temp/diff-4.txt
diff test/p1-output05.txt temp/p1-output05.txt > temp/diff-5.txt
diff test/p1-output06.txt temp/p1-output06.txt > temp/diff-6.txt
diff test/p1-output07.txt temp/p1-output07.txt > temp/diff-7.txt
diff test/p1-output08.txt temp/p1-output08.txt > temp/diff-8.txt
diff test/p1-output09.txt temp/p1-output09.txt > temp/diff-9.txt