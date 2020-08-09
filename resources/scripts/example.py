import MAREnginePy as mar

class Example(mar.Entity):	
	def __init__(self):
		mar.Entity.__init__(self)
		self.a = 5
	
	def start(self):
		self.a = 10
		print("start_method = {}".format(self.a))
		
	def update(self):
		transform = self.getTransform()
		center = transform.center
		center.y += 0.2
		transform.center = center

	
	