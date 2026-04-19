echo "system:ad94_fig10

clock:1:x
clock:1:y

event:a
event:b

process:P
location:P:q0{initial:}"
# This script produces B5(5000,100). Change constants in lines 12, 14, 24, 31, and 38-40, in order to make arbirtary B5(x,y).
m=100

for i in {1..5000}
do
	echo "location:P:q$i{}
location:P:qp$i{}"
done

echo "location:P:fin{}"

echo "edge:P:q0:q1:a{}[push:a]"

for i in {1..2499}
do
	echo "edge:P:q$i:qp$i:a{provided:x>=1 : do: x=0}[]
edge:P:qp$i:q$i:a{provided:y<=$m}[]
edge:P:qp$i:q$((i+1)):b{do: x=0 ; y=0}[push:a]"
done

for i in {2500..4999}
do
	echo "edge:P:q$i:qp$i:a{provided:x>=1 : do: x=0}[]
edge:P:qp$i:q$i:a{provided:y<=$m}[]
edge:P:qp$i:q$((i+1)):b{do: x=0 ; y=0}[pop:a<=2]"
done

echo "edge:P:q5000:qp5000:a{provided:x>=1 : do: x=0}[]
edge:P:qp5000:q5000:a{provided:y<=$m}[]
edge:P:q5000:fin:b{}[]
"