import zlib
import os
import sys
import getopt
import  xml.dom.minidom

'''
use CRC polynom 4C11DB7 to calculate the total crc32
'''

OK = 0
ERR = 1
STOP = 2
RET_INVALID_ARG = 3
RET_FILE_NOT_EXIST = 4
RET_FILE_PATH_ERR = 5
RET_FILE_OPERATION_ERR = 6
RET_XML_CONFIG_ERR=7


SOFTWARE_VERSION="LiteOsV1R2C51SPC00"

def append_buffer(buffer, pos, value):
	buffer.append(value)
	
def serialize_byte(buffer, value, byte_number = 4, pos = 0, callback = append_buffer):
	if byte_number == 4:
		mask = 0xff000000
		shift_num = 24
	elif byte_number == 2:
		mask = 0x0000ff00
		shift_num = 8
	else:
		return ERR	
	
	for i in range(0, byte_number):
		callback(buffer, i + pos, ((mask & value) >> shift_num))
		mask = (mask >> 8)
		shift_num -= 8
		
	return OK
	
def write_buffer(buffer, pos, value):
	buffer[pos] = value
	
def write_byte(buffer,  pos, value, byte_number = 4):	
	serialize_byte(buffer, value, byte_number, pos, write_buffer)
	
def write_fd(fd, pos, value):
	buffer = bytes(chr(value))
	fd.write(buffer)
	
def write_fd_byte(fd, value, byte_number = 4):
	serialize_byte(fd, value, byte_number, 0, write_fd)	

	
class config_info(object):
	input_file=""
	output_file=""
	software_version=""
	def parse_args(self):
		try:
			opts, args = getopt.getopt(sys.argv[1:], 'c:i:o')
		except getopt.GetoptError as err:
			print str(err)			
			return RET_INVALID_ARG			
		
		config_file = "config.xml"
		for opt, arg in opts:
			if opt == "-c":
				config_file = arg
			elif opt == "-i":
				self.input_file = arg
			elif opt == "-o":
				self.output_file = arg
			else:
				pass
		
		if self.input_file == "":
			print "must input a source bin file"
			return RET_INVALID_ARG
			
		if self.output_file == "":
			self.output_file = self.input_file + ".out_bin"
		
		return self.__parse_xml__(config_file)
	
	def __parse_xml__(self, config_file):
		if not os.path.isfile(config_file):
			print "config xml file \"{}\" not exist".format(config_file)
			return RET_INVALID_ARG	
		
		ret = RET_XML_CONFIG_ERR
		try:
			dom = xml.dom.minidom.parse(config_file)
			root = dom.documentElement
			elements = root.getElementsByTagName('software_version')
			if len(elements) > 0:
				#print str(elements[0].firstChild.data)
				self.software_version = str(elements[0].firstChild.data)
			ret =  OK
		except Exception as err:
			print "parse xml exception %s"%(err)			
		except:
			print "parse xml exception"
		finally:
			return ret

class software_header(object):
	buffer = bytearray()
	crc = 0
	header_len = 4 + 4 + 32
	def __init__(self, config):
		file_len = os.path.getsize(config.input_file)
		serialize_byte(self.buffer, self.header_len + file_len)
		serialize_byte(self.buffer, 0)
		
		for i in range(0, 32):
			if i >= len(config.software_version) or i == 31:
				self.buffer.append(0)
			else:
				self.buffer.append(config.software_version[i])
		self.crc = zlib.crc32(bytes(buffer))
		
	def get_buffer(self):
		return bytes(self.buffer)
		
	def get_crc(self):
		return self.crc
	def get_crc_offset(self):
		return 4



		
		
		

class software_maker(object):
	def __init__(self):
		pass
	
	def __get_info(self, input_file):
		if not os.path.isfile(input_file):
			print "{0} is not a file!".format(input_file)
			return [RET_INVALID_ARG, ""]
		return [OK, SOFTWARE_VERSION]

	def make_software(self, config):			
		input_file = config.input_file
		output_file = config.output_file
		
		if not os.path.isfile(input_file):
			print "{0} is not a file!".format(input_file)
			return RET_INVALID_ARG
			
		header = software_header(config)

		crc = header.get_crc();
		ret = OK
		try:
			input_fd = open(input_file, 'rb')
			output_fd = open(output_file, 'wb')
			block_size = 4 * 1024			
			output_fd.write(header.get_buffer())
			
			
			while True:
				buffer = input_fd.read(block_size)
				if len(buffer) == 0: # EOF or file empty. return hashes	
					output_fd.seek(header.get_crc_offset())
					crc &= 0xffffffff
					write_fd_byte(output_fd, crc)
					break					
				crc = zlib.crc32(buffer, crc)				
				output_fd.write(buffer)		
			
				
		except IOError as e:
			print "IOError {0}".format(str(e))	
			ret = RET_FILE_OPERATION_ERR
			
		except  Exception as err:
			print 'except hanppen' + str(err)
			ret = RET_FILE_OPERATION_ERR

		finally:
			if not input_fd is None:
				input_fd.close()			
			if not output_fd is None:
				output_fd.close()
		print 'make software %s to %s ,length %d, crc 0x%x, software_version \"%s\", ret %d'%(input_file, output_file, os.path.getsize(input_file), crc, config.software_version, ret)
		
		return ret

def print_usage():
	print "Usage: {} [-c config_xml_file]  [-o output_file] -i input_file".format(sys.argv[0])
	

	
def main():
	config = config_info()	
	ret = config.parse_args()	
	if ret != OK:
		print "parse args err ret %d"%(ret)
		print_usage()
		return ret
	
	maker = software_maker()
	ret = maker.make_software(config)
	if ret != OK:
		print_usage()
	return ret
	
if __name__ == "__main__":
	main()
	
