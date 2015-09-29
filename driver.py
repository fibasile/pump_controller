
import serial
import time

class Driver():

	def __init__(self, port):
		self.port = port
		self.tty = None
		self.status = 'idle'
		if not self.initSerial():
			raise "Error"

	def initSerial(self):
		self.tty = serial.Serial(self.port, 9600)
		if not self.tty.isOpen():
		    self.tty.open()
		    if not self.tty.isOpen():
		    	return False
		while True:
			 line = self.tty.readline()
			 if line.count('Ready')>0:
				self.status = 'ready'
				return True
		self.status = 'error'
		return False

	def run(self,direction,step_per_s):
		print 'Run %d %f' % (direction,step_per_s)
		self.status = 'running'
		self.tty.write('RUN %d %f\r\n' % (direction,step_per_s))
		
	def halt(self):
		self.status = 'stopped'
		self.tty.write('HALT\r\n')

	def stop(self):
		self.status = 'stopped'
		self.tty.write('STOP\r\n')

	def check(self):
		self.tty.write('HELLO\r\n')


	def checkStatus(self):
		return self.status

	def close(self):
		self.tty.close()


if __name__ == '__main__':

	DEVICE = '/dev/ttyACM0'
	d = Driver(DEVICE)
	d.run(1,20)
	time.sleep(10)
	d.stop()
	d.close()

