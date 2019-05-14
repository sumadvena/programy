case $MACHTYPE in (x86_64*) r=64 e=0;; esac
./Arifexp$r -high -size -time -mhz 2000 -rep 10$e Arif3 >Arif3.wrk
