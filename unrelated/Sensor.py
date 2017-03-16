"""mathematical representation i n set theory
resi = Ai intersec (U - Ai)
summation (resi) (i fr each sublist)
max , min of summation"""

#problem take universal without expanding list



def shortrange(mainl):
	##slist_exp = [range(min(i),max(i)) for i in slist]
	# result = [min([max(x) for x in mainl]), max([min(x) for x in mainl])]
	maxvals = map(lambda x : max(x) ,
	# minmax = lambda x : min(max(i)) for i in x
	result = [min(maxvals), max(minvals) for maxvals in max(x) for minvals in min(x) for x in mainl]
	# result = [[min(max(x)) , max(min(x))]  for x in mainl]
	return result

d = [[-10,15],[13,6],[14,20]]
print shortrange(d)

