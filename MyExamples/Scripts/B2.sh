echo "system:ad94_fig10

clock:1:x
clock:1:y

event:a
event:b

process:P
location:P:q0{initial:}
location:P:q1{}"
#This script produces B2(1000). Change the constants in lines 13, 19, 21, and 25 in order to make arbitrary B2(x).
for i in {1..1001}
do
	echo "location:P:r$i{}"
done
echo "location:P:q2{}
edge:P:q0:q1:a{provided:x>=1 : do: x=0}[]
edge:P:q1:q0:a{provided:y<=1000}[push:a]
edge:P:q0:r1:b{}[pop:a<=2]"
for i in {1..1000}
do
	echo "edge:P:r$i:r$((i+1)):a{}[pop:a<=2]"
done
echo "edge:P:r1001:q2:a{}[]
"
