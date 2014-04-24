import math
import sys

def calculateNormals(faces, vertices):
	normals = [[] for i in xrange(len(vertices))]

	vertexToFaces = [[] for i in range(len(vertices))]
	for face in faces: 					# Invert the list from [face] -> vertices into [vertex] -> faces
		for vertexIndex in face:
			vertexToFaces[vertexIndex].append(face)

	for i in xrange(len(vertexToFaces)): 	# Iterate over every vertex
		vFaces = vertexToFaces[i] 		# Faces that use this vertex

		tempNormal = []
		for face in vFaces:

			p1, p2, p3 = vertices[face[0]], vertices[face[1]], vertices[face[2]]

			v1, v2 = [], []

			for j in xrange(len(p1)):
				v1.append(p2[j] - p1[j])
				v2.append(p3[j] - p1[j])

			cross = [0]*3

			cross[0] = v1[1] * v2[2] - v1[2] * v2[1]
			cross[1] = v1[2] * v2[0] - v1[0] * v2[2]
			cross[2] = v1[0] * v2[1] - v1[1] * v2[0]

			magnitude = math.sqrt(cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2])

			if magnitude == 0:
				tempNormal.append([1, 0, 0])
			else:
				tempNormal.append([cross[0] / magnitude, cross[1] / magnitude, cross[2] / magnitude])

		finalNormal = [0]*3
		numNormals = 0
		for normal in tempNormal:
			numNormals += 1
			finalNormal[0] += normal[0]
			finalNormal[1] += normal[1]
			finalNormal[2] += normal[2]

		finalNormal[0] /= numNormals
		finalNormal[1] /= numNormals
		finalNormal[2] /= numNormals

		normals[i] = finalNormal

	return normals
	
# ------------------------------------------------------------------------------------------------------------------------

def calcAmbient(colorList):
	colorList = [float(color) for color in colorList]
	return str(max(colorList))

# ------------------------------------------------------------------------------------------------------------------------

def calcDiffuse(colorList):
	colorList = [float(color) for color in colorList]

	maxVal = max(colorList)

	return [str(color / maxVal) for color in colorList], str(maxVal)
	
# ------------------------------------------------------------------------------------------------------------------------

def calcSpecular(colorList):
	colorList = [float(color) for color in colorList]
	return str(max(colorList))

# ------------------------------------------------------------------------------------------------------------------------

def parseMTL(filename):
	materialList = []
	matNameDict = {}
	numTextures = 0

	firstMaterial = True
	material = ['0'] * 5 # Format for a material is [rgb list, ambient, diffuse, phong a.k.a specular, phong_size]

	for line in open(filename, 'r'):
		line = line.split()

		if not line:
			continue

		if line[0] == 'newmtl':
			if firstMaterial:
				firstMaterial = False
			else: # not firstMaterial
				materialList.append(material)
				material = ['0'] * 5 # Format for a material is [rgb list, ambient, diffuse, phong a.k.a specular, phong_size]


			matNameDict[line[1]] = numTextures
			numTextures += 1

		else:
			if line[0] == 'Ka':
				material[1] = calcAmbient(line[1:])
			elif line[0] == 'Kd':
				material[0], material[2] = calcDiffuse(line[1:])
			elif line[0] == 'Ks':
				material[3] = calcSpecular(line[1:])
			elif line[0] == 'Ns':
				material[4] = line[1]



	return materialList, matNameDict

# ------------------------------------------------------------------------------------------------------------------------

def readFromFile(filename, usingMTL, matNameDict):
	vertices = []
	normals = []
	faces = []
	if usingMTL:
		currentMaterial = 10

	for line in open(filename, 'r'):
		line = line.split()

		if not line:
			continue

		if line[0] == 'v':
			vertices.append(line[1:])

		elif line[0] == 'vn':
			normals.append(line[1:])

		elif line[0] == 'f':
			if usingMTL:
				faces.append(line[1:] + [currentMaterial])
			else:
				faces.append(line[1:])

		if line[0] == 'usemtl' and usingMTL:
			currentMaterial = matNameDict[line[1]]

	return vertices, normals, faces
	
# ------------------------------------------------------------------------------------------------------------------------

def writeToFile(filename, vertices, normals, faces, normalIndices, materialList, usingMTL):

	with open(filename, 'w') as outFile:
		# Opening
		outFile.write('mesh2 {\n')

		# Vertices
		outFile.write('\tvertex_vectors {\n')
		outFile.write('\t\t' + str(len(vertices)) + ',\n')

		for vertex in vertices:
			outFile.write('\t\t<' + ', '.join(vertex[:3]) + '>,\n')

		outFile.write('\t}\n')

		# Normal Vectors
		outFile.write('\tnormal_vectors {\n')
		outFile.write('\t\t' + str(len(normals)) + ',\n')

		for normal in normals:
			outFile.write('\t\t<' + ', '.join(normal) + '>,\n')

		outFile.write('\t}\n')

		# Normal Vectors
		if materialList:
			outFile.write('\ttexture_list {\n')
			outFile.write('\t\t' + str(len(materialList)) + ',\n')

			for texture in materialList:
				outFile.write('\t\t' + 'texture {\n')
				outFile.write('\t\t\t' + 'pigment {\n')
				outFile.write('\t\t\t\trgb <' + ', '.join(texture[0]) + '>\n')
				outFile.write('\t\t\t}\n')
				outFile.write('\t\t\t' + 'finish {\n')
				outFile.write('\t\t\t\tambient ' + texture[1] + '\n')
				outFile.write('\t\t\t\tdiffuse ' + texture[2] + '\n')
				if type(texture[3]) == int:
					print texture
					sys.exit()
				outFile.write('\t\t\t\tphong ' + texture[3] + '\n')
				outFile.write('\t\t\t\tphong_size ' + texture[4] + '\n')
				outFile.write('\t\t\t}\n')
				outFile.write('\t\t}\n')
				# outFile.write('\t\t<' + ', '.join(texture) + '>,\n') # TODO --------

			outFile.write('\t}\n')

		# Faces
		outFile.write('\tface_indices {\n')
		outFile.write('\t\t' + str(len(faces)) + ',\n')

		for face in faces:
			if usingMTL:
				outFile.write('\t\t<' + ', '.join(face[:-1]) + '>, ' + face[-1] + ',\n')
			else:
				outFile.write('\t\t<' + ', '.join(face) + '>,\n')

		outFile.write('\t}\n')

		if normalIndices:
			# Faces
			outFile.write('\tnormal_indices {\n')
			outFile.write('\t\t' + str(len(normalIndices)) + ',\n')

			for normalIndex in normalIndices:
				outFile.write('\t\t<' + ', '.join(normalIndex) + '>,\n')

			outFile.write('\t}\n')

		outFile.write('\tpigment { White }\n')

		outFile.write('}')

# ------------------------------------------------------------------------------------------------------------------------

def triangulate(oldList, usingMTL):
	if not oldList:
		return False

	newList = []
	for item in oldList:
		if usingMTL:
			for i in xrange(len(item)-3):
				newList.append([item[0], item[i+1], item[i+2], item[-1]])

		else:
			for i in xrange(len(item)-2):
				newList.append([item[0], item[i+1], item[i+2]])

	return newList
	
# ------------------------------------------------------------------------------------------------------------------------

def main():
	filename = 'airboat'
	usingMTL = True
	matNameDict = {}
	materialList = []
	if usingMTL:
		materialList, matNameDict = parseMTL('vp.mtl')

	# with open('ref.txt', 'w') as outFile:
	# 	for i in xrange(len(materialList)):
	# 		for matName, matNum in matNameDict.items():
	# 			if matNum == i:
	# 				name = matName
	# 		outFile.write(str(i) + ' (' + name + '): ' + materialList[i].__str__() + '\n')


	vertices, normals, faces = readFromFile(filename+'.obj', usingMTL, matNameDict)

	vertices = [[float(value) for value in vertex] for vertex in vertices]
	normals = [[float(value) for value in normal] for normal in normals]

	normalIndices = False
	if faces[0][0].find('//') != -1:
		normalIndices = [[int(index.split('//')[1])-1 for index in face] for face in faces]
		faces = [[int(index.split('//')[0])-1 for index in face] for face in faces]

	else:
		if usingMTL:
			faces = [[int(index)-1 for index in face[:-1]] + [face[-1]] for face in faces]
		else:
			faces = [[int(index)-1 for index in face] for face in faces]

	if not normals:
		normals = calculateNormals(faces, vertices)

	vertices = [[str(value) for value in vertex] for vertex in vertices]
	faces = [[str(index) for index in face] for face in faces]
	normals = [[str(value) for value in normal] for normal in normals]
	if normalIndices:
		normalIndices = [[str(index) for index in normalIndex] for normalIndex in normalIndices]


	faces = triangulate(faces, usingMTL)
	normalIndices = triangulate(normalIndices, usingMTL)

	writeToFile('test.inc', vertices, normals, faces, normalIndices, materialList, usingMTL)

# ------------------------------------------------------------------------------------------------------------------------

if __name__ == '__main__':
	main()