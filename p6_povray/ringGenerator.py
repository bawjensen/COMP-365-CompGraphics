from math import pi, cos, sin

def main():
	subDivisions = 4

	originalPoints = [ [1.5, 0], [2, 0.5], [2.5, 0], [2, -0.5] ]

	stepAmt = (2 * pi) / subDivisions
	i = 0

	pointListIndex = 0
	pointList = []
	faceList = []

	maxIndex = subDivisions * 4

	for subDiv in xrange(subDivisions): # use a blank variable name because it's not being used
		for j in xrange(4):
			pointList.append( [originalPoints[j][0] * cos(i), originalPoints[j][1], originalPoints[j][0] * sin(i)] )
		
		# First side of square torus
		faceList.append( [(subDiv * 4 + 0) % maxIndex,  (subDiv * 4 + 1) % maxIndex, (subDiv * 4 + 5) % maxIndex] )
		faceList.append( [(subDiv * 4 + 0) % maxIndex,  (subDiv * 4 + 5) % maxIndex, (subDiv * 4 + 4) % maxIndex] )

		# Second side of square torus
		faceList.append( [(subDiv * 4 + 1) % maxIndex,  (subDiv * 4 + 2) % maxIndex, (subDiv * 4 + 6) % maxIndex] )
		faceList.append( [(subDiv * 4 + 1) % maxIndex,  (subDiv * 4 + 6) % maxIndex, (subDiv * 4 + 5) % maxIndex] )

		# Third side of square torus
		faceList.append( [(subDiv * 4 + 2) % maxIndex,  (subDiv * 4 + 3) % maxIndex, (subDiv * 4 + 7) % maxIndex] )
		faceList.append( [(subDiv * 4 + 2) % maxIndex,  (subDiv * 4 + 7) % maxIndex, (subDiv * 4 + 6) % maxIndex] )

		# Fourth side of square torus
		faceList.append( [(subDiv * 4 + 3) % maxIndex,  (subDiv * 4 + 0) % maxIndex, (subDiv * 4 + 4) % maxIndex] )
		faceList.append( [(subDiv * 4 + 3) % maxIndex,  (subDiv * 4 + 4) % maxIndex, (subDiv * 4 + 7) % maxIndex] )

		i += stepAmt



	# print
	# print pointList
	# print
	# print faceList

	pointList = [[str(val) for val in subList] for subList in pointList]
	faceList = [[str(val) for val in subList] for subList in faceList]

	with open('out.inc', 'w') as outFile:
		outFile.write('mesh2 {\n')
		outFile.write('\tvertex_vectors {\n')
		outFile.write('\t\t' + str(len(pointList)) + ',\n')

		for point in pointList:
			outFile.write('\t\t<' + ', '.join(point) + '>,\n')

		outFile.write('\t}\n')
		outFile.write('\tface_indices {\n')
		outFile.write('\t\t' + str(len(faceList)) + ',\n')

		for face in faceList:
			outFile.write('\t\t<' + ', '.join(face) + '>,\n')

		outFile.write('\t}\n')

		outFile.write('\ttexture {\n')
		outFile.write('\t\tfinish {\n')
		outFile.write('\t\t\tspecular 1\n')
		outFile.write('\t\t\troughness 0.001\n')
		outFile.write('\t\t\tambient 0\n')
		outFile.write('\t\t\tdiffuse 0\n')
		outFile.write('\t\t\treflection 0.1\n')
		outFile.write('\t\t}\n')
		outFile.write('\t\tpigment { color rgbf<1.0, 1.0, 1.0, 1.0> }\n')
		outFile.write('\t}\n')
		outFile.write('\tinterior {\n')
		outFile.write('\t\tior 1.5\n')
		outFile.write('\t\tcaustics 1\n')
		outFile.write('\t}\n')
		outFile.write('\trotate 135*z\n')
		outFile.write('\tscale 10\n')
		outFile.write('\ttranslate <0, 18, 0>\n')

		outFile.write('}')

if __name__ == '__main__':
	main()