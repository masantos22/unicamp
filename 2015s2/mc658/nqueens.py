import numpy as np

def can_place(board,i,j,n):
	if max(board[i]) > 0 or max(board[:,j]) > 0:
		return False

	for k,l in queens_spots(board,n):
		if abs(i-k) == abs(j-l):
			return False

	return True

def NQueens(board,i,n):
	if i == n:
		print board
		print 
	else:
		for j in range(n):
			if can_place(board,i,j,n):
				b = np.copy(board)
				b[i][j] = 1
				NQueens(b,i+1,n)

def queens_spots(board,n):
	q = []
	for i in range(n):
		for j in range(n):
			if board[i][j] > 0:
				q.append((i,j))

	return q

n = 5
board = [[0]*n]*n
board = np.array(board)

NQueens(board,0,n)