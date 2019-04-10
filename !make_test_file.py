template_path = '!template.cpp'
pre = 'test'
max_i = 3

with open(template_path, 'r') as file:
    code = file.read()
    for i in range(max_i):
        with open('{}_{}.cpp'.format(pre, chr(65 + i)), mode='w') as file:
            file.write(code)
