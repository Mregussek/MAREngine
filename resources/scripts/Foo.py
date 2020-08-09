import MAREnginePy as mar

class Foo(mar.Entity):	
	def __init__(self):
		mar.Entity.__init__(self)
		self.a = 0
	
	def start(self):
		self.a = 0.1
		
	def update(self):
		self.color.texture.x = mar.trig.cosine(mar.trig.toRadians(self.a))
		self.color.texture.y = mar.trig.sine(mar.trig.toRadians(self.a))
		self.color.texture.z = mar.trig.sine(mar.trig.toRadians(self.a))
		self.transform.center.x = mar.trig.sine(mar.trig.toRadians(2.0 * self.a))
		self.a += 0.7

	
	