case $MACHTYPE in (x86_64*) r=64;; esac
while
echo 1000000001+100\*>Millrand.1
./Arifexp$r -idle -rand Millrand >>Millrand.1
./Arifexp$r -idle Millrand.1 >Millrand.2
./Miller$r -idle -scan 50 Millrand.2 >Millrand.wrk
res=$?
[ $res -le 10 ]
do
echo $res>>Millrand.res
done
echo Look for NO MATCH in Millrand.bad
echo "$res (failed)">>Millrand.res
cp Millrand.wrk Millrand.bad
cp Millrand.2 Millrand.b2
cp Millrand.1 Millrand.b1
echo FAIL!
