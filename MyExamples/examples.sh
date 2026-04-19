#!bin/sh

args=$#
controller="sim"

if [[ $args > 0 ]]
then
	controller=$1
fi

# echo "B1"
tchecker -pdta $controller B1.txt
# echo "B2(5)"
tchecker -pdta $controller B2_5.txt
# echo "B2(10)"
tchecker -pdta $controller B2_10.txt
# echo "B2(100)"
tchecker -pdta $controller B2_100.txt
# echo "B2(1000)"
tchecker -pdta $controller B2_1000.txt
# echo "B3(4,3)"
tchecker -pdta $controller B3_4_3.txt
# echo "B3(3,4)"
tchecker -pdta $controller B3_3_4.txt
# echo "B4"
tchecker -pdta $controller B4.txt
# echo "B5(100,10)"
tchecker -pdta $controller B5_100_10.txt
# echo "B5(100,100)"
tchecker -pdta $controller B5_100_100.txt
# echo "B5(100,1000)"
tchecker -pdta $controller B5_100_1000.txt
# echo "B5(1000,100)"
tchecker -pdta $controller B5_1000_100.txt
# echo "B5(5000,100)"
tchecker -pdta $controller B5_5000_100.txt
# echo "B6(4,5,100)"
tchecker -pdta $controller B6_4_5_100.txt
# echo "B6(4,5,1000)"
tchecker -pdta $controller B6_4_5_1000.txt
# echo "B6(4,5,10000)"
tchecker -pdta $controller B6_4_5_10000.txt
# echo "B6(5,4,100)"
tchecker -pdta $controller B6_5_4_100.txt
# echo "B6(5,4,1000)"
tchecker -pdta $controller B6_5_4_1000.txt
# echo "B6(5,4,10000)"
tchecker -pdta $controller B6_5_4_10000.txt
# echo "B6(500,501,100)"
tchecker -pdta $controller B6_500_501_100.txt
# echo "B6(501,500,100)"
tchecker -pdta $controller B6_501_500_100.txt
# echo "B7"
tchecker -pdta $controller B7.txt
# echo "B8"
tchecker -pdta $controller B8.txt
# echo "B9(10,10)"
tchecker -pdta $controller B9_10_10.txt
# echo "B9(50,10)"
tchecker -pdta $controller B9_50_10.txt
# echo "B9(100,10)"
tchecker -pdta $controller B9_100_10.txt
# echo "B9(10,20)"
tchecker -pdta $controller B9_10_20.txt
# echo "B9(10,50)"
tchecker -pdta $controller B9_10_50.txt
# echo "B9(10,100)"
tchecker -pdta $controller B9_10_100.txt
# echo "B10"
tchecker -pdta $controller B10.txt