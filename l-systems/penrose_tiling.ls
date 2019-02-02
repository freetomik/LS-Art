# https://10klsystems.wordpress.com/examples/
# Penrose tiling

[L-System]
name=Penrose tiling
axiom=[N]++[N]++[N]++[N]++[N]
angle=36
numRules=4
rule0=M>OA++PA----NA[-OA----MA]++
rule1=N>+OA--PA[---MA--NA]+
rule2=O>-MA++NA[+++OA++PA]-
rule3=P>--OA++++MA[+PA++++NA]--NA
iterations=5
lineLength=16
closed=0
