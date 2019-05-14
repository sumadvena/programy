case $MACHTYPE in (x86_64*) r=64;; esac
./Arifexp$r -high -size -check -v Arif >Arif.wrk
