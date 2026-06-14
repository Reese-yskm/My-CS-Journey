def calculate(a, b, op):
    if op=="+":
        return a+b
    if op=="-":
        return a-b
    if op=="*":
        return a*b
    if op=="/":
        return a/b
a,b=map(int,input().split())
op=input()
print(calculate(a,b,op))