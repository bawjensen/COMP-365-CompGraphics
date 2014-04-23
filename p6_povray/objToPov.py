def main():
	vertices = []
	normals = []
	faces = []

	for line in open('test.obj', 'r'):
		line = line.split()

		if not line:
			continue

		if line[0] == 'v':
			vertices.append(line[1:])

		elif line[0] == 'vn':
			normals.append(line[1:])

		elif line[0] == 'f':
			faces.append(line[1:])

	vertices = [[float(value) * 10 for value in vertex] for vertex in vertices]
	faces = [[int(index)-1 for index in face] for face in faces]
	normals = [[float(value) for value in normal] for normal in normals]

	print vertices
	print faces
	print normals

	if not normals:
		vertexFaces = [0] * len(vertices)
		for face in faces:
			for vertexIndex in face:
				vertexFaces[vertexIndex] = face

		for i in xrange(len(vertexFaces)):
			face = vertexFaces[i]

			p1, p2, p3 = face[0], face[1], face[2]

			v1, v2 = [], []

			for i in xrange(len(p1)):
				v1.append(p2[i] - p1[i])
				v2.append(p3[i] - p1[i])

	vertices = [[str(value) for value in vertex] for vertex in vertices]
	faces = [[str(index) for index in face] for face in faces]
	normals = [[str(value) for value in normal] for normal in normals]


	with open('test.inc', 'w') as outFile:
		# Opening
		outFile.write('mesh2 {\n')

		# Vertices
		outFile.write('\tvertex_vectors {\n')
		outFile.write('\t\t' + str(len(vertices)) + ',\n')

		for vertex in vertices:
			outFile.write('\t\t<' + ','.join(vertex) + '>,\n')

		outFile.write('\t}\n')

		# Faces
		outFile.write('\tface_indices {\n')
		outFile.write('\t\t' + str(len(faces)) + ',\n')

		for face in faces:
			outFile.write('\t\t<' + ','.join(face) + '>,\n')

		outFile.write('\t}\n')

		if (normals):
			# Normals
			outFile.write('\tface_indices {\n')
			outFile.write('\t\t' + str(len(normals)) + ',\n')

			for normal in normals:
				outFile.write('\t\t<' + ','.join(normal) + '>,\n')

			outFile.write('\t}\n')

		outFile.write('}')

if __name__ == '__main__':
	main()