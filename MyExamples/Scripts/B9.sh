n=10
m=100
# This script produces B9(10,100). Change values on n and m to get B9(n,m).
echo "system:ad94_fig10

clock:1:x
clock:1:y
event:a
event:b"

for ((i = 1 ; i <= n ; i++)); do
	echo "event:e$i"
done

echo "process:P
location:P:q0{initial:}"

for ((i = 1 ; i <= n ; i++)); do
	echo "location:P:q1$i{}
location:P:q2$i{}
location:P:qp$i{}
location:P:q3$i{}"
done

for ((i = 1 ; i <= n ; i++)); do
	echo "location:P:r1$i{}
location:P:r2$i{}
location:P:r3$i{}
location:P:r4$i{}"
done

for ((i = 1 ; i <= n ; i++)); do
	echo "edge:P:q0:q1$i:e$i{}[push:a1$i]
edge:P:q1$i:q2$i:a{}[push:a2$i]
edge:P:q2$i:qp$i:a{do: x=0 : provided: x>=1}[]
edge:P:qp$i:q2$i:a{provided: y<=$m}[]
edge:P:q2$i:q3$i:b{}[push:a3$i]
edge:P:q3$i:q0:a{}[push:a$i]"
done

echo "edge:P:q0:r11:a{}[pop:a1<=2]
edge:P:r11:r21:a{}[pop:a31<=2]
edge:P:r21:r31:a{}[pop:a21<=2]
edge:P:r31:r41:a{}[pop:a11<=2]"

for ((i = 2 ; i <= n ; i++)); do
	echo "edge:P:r4$((i-1)):r1$i:a{}[pop:a$i<=2]
edge:P:r1$i:r2$i:a{}[pop:a3$i<=2]
edge:P:r2$i:r3$i:a{}[pop:a2$i<=2]
edge:P:r3$i:r4$i:a{}[pop:a1$i<=2]"
done