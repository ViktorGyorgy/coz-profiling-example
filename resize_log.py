lines_number = 100_000
with open("access_all.log") as input_file:
    head = [next(input_file) for _ in range(lines_number)]

with open("access.log", "w") as output_file:
    for line in head:
        output_file.write(line)