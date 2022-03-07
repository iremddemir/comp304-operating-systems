Problem 1:
p1a:
  compile: gcc -o p1a p1a.c
  run: ./p1a 2
       ./p1a 3
       ./p1a 5
p1b:
  compile: gcc -o p1b p1b.c
  run: ./p1b pwd
       ./p1b ps f
p1c:
  compile: gcc -o p1c p1c.c
  run: ./p1b &
       ps -l