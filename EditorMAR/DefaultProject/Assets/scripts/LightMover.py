import MAREnginePy as mar


def check_values(x_rot, y_rot, x_cen):
	if x_rot >= 360.0:
		x_rot = 0.0
	if y_rot >= 360.0:
		y_rot = 0.0
	if x_cen >= 1.0:
		x_cen = 0.0


class LightMover(mar.Entity):	
	def __init__(self):
		mar.Entity.__init__(self)
		self.x_rotation = 0.0
		self.y_rotation = 0.0
		self.z_center = 0.0
	
	def start(self):
		self.x_rotation = 0.05
		self.y_rotation = 0.05
		self.z_center = 0.05
		
	def update(self):
		self.transform.center.z += 0.5 * mar.trig.sine(self.z_center)
		self.z_center += 0.05
		
		self.transform.angles.x += mar.trig.sine(mar.trig.toRadians(self.x_rotation))
		self.x_rotation += 5.0
		self.transform.angles.y += mar.trig.sine(mar.trig.toRadians(self.y_rotation))
		self.y_rotation += 5.0
		
		check_values(self.x_rotation, self.y_rotation, self.z_center)