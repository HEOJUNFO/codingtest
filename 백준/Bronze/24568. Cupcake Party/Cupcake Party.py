# Read inputs
R = int(input().strip())
S = int(input().strip())

# Calculate total cupcakes
total_cupcakes = R * 8 + S * 3

# Since there are 28 students and at least 28 cupcakes
left_over = total_cupcakes - 28

# Print the number of cupcakes left over
print(left_over)
