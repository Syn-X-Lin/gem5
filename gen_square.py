from sys import argv

print argv
script, amp = argv

num1 = 100000
num2 = 300000
total = 30000000

name = 'profile/energy_prof'

def func(x, num1, num2):
	x = x % (num1 + num2)
	if x < num1:
		return 1
	else:
		return 1

t = range(total)
y = [amp * func(ti, num1, num2) for ti in t]

f = open(name, 'w')
for yi in y:
	f.write(str(yi) + '\n')
f.close()
