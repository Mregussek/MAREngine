import MAREnginePy as mar

class Example(mar.Entity):	
	def __init__(self):
		mar.Entity.__init__(self)
		self.a = 0
	
	def start(self):
		self.a = 0.1
		
	def update(self):
		self.color.texture.x = 0.5 * mar.trig.sine(mar.trig.toRadians(0.5 * self.a))
		self.color.texture.y = 0.5 * mar.trig.cosine(mar.trig.toRadians(self.a))
		self.color.texture.z = 0.5 * mar.trig.sine(mar.trig.toRadians(self.a))
		self.transform.center.y = 0.5 * mar.trig.sine(mar.trig.toRadians(2.0 * self.a))
		self.a += 0.5

	
	