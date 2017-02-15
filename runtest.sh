#!/bin/bash

g++ src/main.cpp -o omega

./omega input 2 0.1 7 0.2 > output

F1='entradaApriori.txt'
F2='Out_Discret.arff'
F3='Out_PrecRec.txt'
F4='presage_log.txt'
F5='output'

diff ${F1} gold/${F1} && 
diff ${F2} gold/${F2} &&
diff ${F3} gold/${F3} && 
diff ${F4} gold/${F4} && 
diff ${F5} gold/${F5} && 
rm -f omega ${F1} ${F2} ${F3} ${F4} ${F5}
