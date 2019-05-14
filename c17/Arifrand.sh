case $MACHTYPE in (x86_64*) r=64;; esac
while
./Arifexp$r -idle -rand all Arifrand >Arifrand.1
./Arifexp$r -idle -rand 0 -exp 40 -hexo Arifrand.1 >Arifrand.2
./Arifexp$r -idle -check -par 2 -div0 -v -hexi Arifrand.2 >Arifrand.wrk
res=$?
[ $res -le 0 ]
do
echo $res>>Arifrand.res
done
echo Look for NO MATCH in Arifrand.bad
echo "$res (failed)">>Arifrand.res
cp Arifrand.wrk Arifrand.bad
cp Arifrand.2 Arifrand.b2
cp Arifrand.1 Arifrand.b1
echo FAIL!
