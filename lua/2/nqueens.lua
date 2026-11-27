-- these are the row indices that have been used
-- consider replacing this with a bitset on an integer
rows = {}

tmp_count = 0

--[[
This looks at all the diagonals in the current
state of the board and if any of the queens on
the board are on the same diagonal, then this
returns false. A return value of true from
this function does not mean it is a solution,
but a return value of false guarantees it is
not a solution.
--]]
function is_possible_solution(a)
	for c1, r1 in ipairs(a) do
		for c2 = c1 + 1, #a do
			r2 = a[c2]
			dy = math.abs(r1 - r2)
			dx = math.abs(c1 - c2)
			if dy == dx then
				return false
			end
		end
	end

	return true
end

--[[
Returns the number of solutions for a board of size n x n with n queens on it
such that no two queens are attacking each other.
--]]
function solve(a, n)
	col = #a + 1 -- the one-based index of the column

	tmp_count = tmp_count + 1

	if is_possible_solution(a) == false then
		return 0
	end

	if col > n then
		for x = 1, #a do
			io.write(a[x])
			if x < #a then
				io.write(" ")
			end
		end
		io.write("\n")
		return 1
	end

	solutions = 0
	for row = 1, n do
		if rows[row] == false then
			rows[row] = true
			table.insert(a, row)
			solutions = solutions + solve(a, n)
			rows[row] = false -- unset the row
			table.remove(a) -- remove the last column in a
		end
	end

	return solutions
end

io.write("Enter the size of chess board: ")
n = io.read("*n")
io.write("\n")

-- rows holds the rows that are used
for row = 1, n do
	rows[row] = false
end

number_of_solutions = solve({}, n)
io.write("There are ", number_of_solutions, " solutions for ", n, " queens\n")
io.write("tmp_count = ", tmp_count, "\n")