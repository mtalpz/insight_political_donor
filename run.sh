echo ""
echo "===================="
echo "Building "
echo "--------------------"
cd src
make
echo ""
echo "===================="
echo "Running"
echo "--------------------"
./bin/find_political_donors $1
cd ..
echo ""
