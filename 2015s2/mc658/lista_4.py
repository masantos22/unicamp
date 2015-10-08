def sub_sum(S,R,t,my_t,i):
	if t == my_t:
		for k,v in enumerate(R):
			if v == 1:
				print S[k],
		print 

	elif i < len(S):
		for value in range(i,len(S)):
			if my_t + S[value] <= t and sum(S[i:]) >= t - my_t:
				R_with = R[:]
				R_with[value] = 1
				sub_sum(S,R_with,t,my_t+S[value],value+1)

			#sub_sum(S,R,t,my_t,value+1)

	return


S = [11,8,7,5,4,3,2]
print "Target :",
print sum(S)/2
sub_sum(S,[0] * len(S),sum(S)/2,0,0)
