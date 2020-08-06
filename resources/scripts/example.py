class Example(object):
	def __init__(self):
		self.a = 5
	
	def start(self):
		self.a = 100
		print("Init self.a = {}".format(self.a))
		
	def update(self):
		print("Example self.a = {}".format(self.a))

print("Python: Hello World!")