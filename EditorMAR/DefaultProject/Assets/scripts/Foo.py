import MAREnginePy as mar

def update_color(color, R, G, B):
	if color.texture.x >= 1:
		R = -0.5
	else:
		R = 0.05
		
	if color.texture.y >= 1:
		G = -0.5
	else:
		G = 0.05
		
	if color.texture.z >= 1:
		B = -0.5
	else:
		B = 0.05
		
	return (color, R, G, B)

class Foo(mar.Entity):	
	def __init__(self):
		mar.Entity.__init__(self)
		self.a = 0
		self.color_R_change = 0
		self.color_G_change = 0
		self.color_B_change = 0
	
	def start(self):
		self.a = 0.1
		self.color_R_change = 0.05
		self.color_G_change = 0.05
		self.color_B_change = 0.05
		
	def update(self):
		[self.color, self.color_R_change, self.color_G_change, self.color_B_change] = update_color(self.color, self.color_R_change, self.color_G_change, self.color_B_change)
	
		self.color.texture.x += self.color_R_change
		self.color.texture.y += self.color_G_change
		self.color.texture.z += self.color_B_change
		
		self.transform.center.x += 0.5 * mar.trig.sine(self.a)
		self.a += 0.05

	
	