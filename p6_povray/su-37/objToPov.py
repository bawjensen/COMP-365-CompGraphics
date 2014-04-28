import math
import sys

usesImageForColor = [True] * 30

def calculatevertexNormals(faces, vertices):
	vertexNormals = [[] for i in xrange(len(vertices))]

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
		numvertexNormals = 0
		for normal in tempNormal:
			numvertexNormals += 1
			finalNormal[0] += normal[0]
			finalNormal[1] += normal[1]
			finalNormal[2] += normal[2]

		finalNormal[0] /= numvertexNormals
		finalNormal[1] /= numvertexNormals
		finalNormal[2] /= numvertexNormals

		vertexNormals[i] = finalNormal

	return vertexNormals
	
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
	matNameList = {}
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

	materialList.append(material)

	print len(materialList)

	return materialList, matNameDict

# ------------------------------------------------------------------------------------------------------------------------

def readFromFile(filename, usingMTL, matNameDict):
	vertices = []
	vertexNormals = []
	vertexTextures = []
	faces = []

	currentMaterial = 0

	for line in open(filename, 'r'):
		line = line.split()

		if not line:
			continue

		if line[0] == 'v':
			vertices.append(line[1:])

		elif line[0] == 'vn':
			vertexNormals.append(line[1:])

		elif line[0] == 'vt':
			vertexTextures.append(line[1:])

		elif line[0] == 'f':
			faces.append(line[1:] + [currentMaterial])

		if line[0] == 'usemtl':
			currentMaterial = matNameDict[line[1]]

	return vertices, vertexNormals, vertexTextures, faces
	
# ------------------------------------------------------------------------------------------------------------------------

def writeToFile(filename, vertices, vertexNormals, faces, normalIndices, textureIndices, materialList, usingMTL):

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
		outFile.write('\t\t' + str(len(vertexNormals)) + ',\n')

		for normal in vertexNormals:
			outFile.write('\t\t<' + ', '.join(normal) + '>,\n')

		outFile.write('\t}\n')

		# Normal Vectors
		# if materialList:
		# 	outFile.write('\ttexture_list {\n')
		# 	outFile.write('\t\t' + str(len(materialList)) + ',\n')

		# 	i = 0
		# 	for texture in materialList:
		# 		outFile.write('\t\t' + 'texture {\n')
		# 		outFile.write('\t\t\t' + 'pigment {\n')
		# 		outFile.write('\t\t\t\trgb <' + ', '.join(texture[0]) + '>\n')
		# 		outFile.write('\t\t\t}\n')
		# 		outFile.write('\t\t\t' + 'finish { //' + str(i) + '\n')
		# 		outFile.write('\t\t\t\tambient ' + texture[1] + '\n')
		# 		outFile.write('\t\t\t\tdiffuse ' + texture[2] + '\n')
		# 		outFile.write('\t\t\t\tphong ' + texture[3] + '\n')
		# 		outFile.write('\t\t\t\tphong_size ' + texture[4] + '\n')
		# 		outFile.write('\t\t\t}\n')
		# 		outFile.write('\t\t}\n')

		# 		i += 1

		# 	outFile.write('\t}\n')

		# Faces
		outFile.write('\tface_indices {\n')
		outFile.write('\t\t' + str(len(faces)) + ',\n')

		for face in faces:
			# if usingMTL:
			# 	outFile.write('\t\t<' + ', '.join(face[:-1]) + '>, ' + face[-1] + ',\n')
			# else:
			outFile.write('\t\t<' + ', '.join(face[:-1]) + '>,\n')

		outFile.write('\t}\n')

		# Normal Indices
		outFile.write('\tnormal_indices {\n')
		outFile.write('\t\t' + str(len(normalIndices)) + ',\n')

		for normalIndex in normalIndices:
			outFile.write('\t\t<' + ', '.join(normalIndex) + '>,\n')

		outFile.write('\t}\n')

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
	filename = 'su-37'
	usingMTL = True
	matNameDict = {}
	materialList = []
	if usingMTL:
		materialList, matNameDict = parseMTL('su-37.mtl')

	vertices, vertexNormals, vertexTextures, faces = readFromFile(filename+'.obj', usingMTL, matNameDict)

	# vertices = [[float(value) for value in vertex] for vertex in vertices]
	# vertexNormals = [[float(value) for value in normal] for normal in vertexNormals]
	# vertexTextures = [[float(value) for value in textureCoord] for textureCoord in vertexTextures]

	normalIndices = False

	normalIndices = [[int(index.split('/')[2])-1 for index in face[:-1]] for face in faces]
	textureIndices = [[int(index.split('/')[2])-1 for index in face[:-1]] for face in faces]
	faces = [[int(index.split('/')[0])-1 for index in face[:-1]] + [face[-1]] for face in faces]

	# vertices = [[str(value) for value in vertex] for vertex in vertices]
	faces = [[str(index) for index in face] for face in faces]
	# vertexNormals = [[str(value) for value in normal] for normal in vertexNormals]

	normalIndices = [[str(index) for index in normalIndex] for normalIndex in normalIndices]
	textureIndices = [[str(index) for index in textureIndex] for textureIndex in textureIndices]


	faces = triangulate(faces, usingMTL)
	normalIndices = triangulate(normalIndices, False)
	textureIndices = triangulate(textureIndices, False)

	writeToFile('test.inc', vertices, vertexNormals, faces, normalIndices, textureIndices, materialList, usingMTL)

# ------------------------------------------------------------------------------------------------------------------------

if __name__ == '__main__':
	main()