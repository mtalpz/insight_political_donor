echo ""
echo "=============================="
echo "Setting up folders (if needed)"
echo "=============================="
mkdir output src/logs src/obj src/bin
echo "===================="
echo "Building "
echo "--------------------"
cd src
make
echo ""
echo "===================="
echo "Running"
echo "--------------------"
./bin/find_political_donors
cd ..
echo ""
