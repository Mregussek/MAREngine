class Example(object):
	def __init__(self):
		self.a = 5
	
	def start(self):
		self.a = 100
		
	def update(self):
		print("Example self.a = {}".format(self.a))

print("Python: Hello World!")