echo "Starting subscribers ..."

for a in 1 2 3 4 5 6 7 8 9 10;
do
	syncsub &
done

echo "Starting publisher ..."
syncpub