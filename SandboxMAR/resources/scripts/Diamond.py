import MAREnginePy as mar

class Diamond(mar.Entity):	
	def __init__(self):
		mar.Entity.__init__(self)
		self.light_changer = 0.0
	
	def start(self):
		self.light_changer = 0.05
		
	def update(self):
		self.light.constant += self.light_changer
		self.light_changer += 0.05
		
		if self.light.constant >= 1.0:
			self.light_changer = -1.0
		else:
			self.light_changer = 0.05
		
		

	
	

