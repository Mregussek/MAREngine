import MAREnginePy as mar

class Examples(mar.Entity):
	def __init__(self):
		mar.Entity.__init__(self)
		self.color_R_change = 0
		self.color_G_change = 0
		self.color_B_change = 0
		self.center_change = 0
	
	def start(self):
		self.center_change = 0.1
		self.color_R_change = 0.05
		self.color_G_change = 0.05
		self.color_B_change = 0.05
		
	def update(self):
		self.transform.center.y += 0.01 * mar.trig.sine(self.center_change)
		self.center_change += 0.02
		
		if self.color.texture.x >= 1:
			self.color_R_change = -0.5
		else:
			self.color_R_change = 0.05
			
		if self.color.texture.y >= 1:
			self.color_G_change = -0.5
		else:
			self.color_G_change = 0.05
			
		if self.color.texture.z >= 1:
			self.color_B_change = -0.5
		else:
			self.color_B_change = 0.05
			
		self.color.texture.x += self.color_R_change
		self.color.texture.y += self.color_G_change
		self.color.texture.z += self.color_B_change
		
		

	
	

