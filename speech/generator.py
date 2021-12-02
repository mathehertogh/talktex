import ctypes as ct 

GENERATOR_PATH="../build/src/latex-generator/libcompiler_latex_generator.so"
LATEX_MAX_SIZE = 1048576 #1MB should do for now right?

class Generator:
	def __init__(self):
		self.lib = ct.cdll.LoadLibrary(GENERATOR_PATH)
		
		#Function that converts a token string into latex
		self.texify = self.lib.texify
		self.texify.restype = ct.c_bool
		self.texify.argtypes = [ct.c_char_p, ct.c_char_p, ct.c_size_t]
		
		#Function that generates a header for a latex document
		self.header = self.lib.talktex_header
		self.header.argtypes = [ct.c_char_p, ct.c_size_t]
		
		#Function that generates a footer for a latex document
		self.footer = self.lib.talktex_footer
		self.footer.argtypes = [ct.c_char_p, ct.c_size_t]
		
		
	'''Returns whether a conversion to LaTeX from token_string succeeded
	and if it did, also returns the resulting LaTeX string.'''
	def generate_latex_string(self, token_string):
		c_latex_buffer = ct.create_string_buffer(LATEX_MAX_SIZE)
		c_token_string = ct.c_char_p(token_string.encode('utf-8'))
		success = self.texify(c_token_string, c_latex_buffer, ct.sizeof(c_latex_buffer))
		latex_string = c_latex_buffer.value.decode('utf-8')
		#Only return the latex string if the parsing was actually successful
		if success:
			return success, latex_string
		else:
			return success, ""
			
			
	'''Returns whether a conversion to LaTeX from token_string succeeded
	and if it did, also returns the resulting full LaTeX document.'''
	def generate_latex_doc(self, token_string):
		#Generate the header string
		c_header_buffer = ct.create_string_buffer(LATEX_MAX_SIZE)
		self.header(c_header_buffer, ct_sizeof(c_header_buffer))
		header_string = header_buffer.value.decode('utf-8')
		
		#Generate the footer string
		c_footer_buffer = ct.create_string_buffer(LATEX_MAX_SIZE)
		self.footer(c_footer_buffer, ct.sizeof(c_footer_buffer))
		footer_string = c_footer_buffer.value.decode('utf-8')
		
		#If appliccable, put everything together and return
		success, latex_string = generate_latex_string(token_string)
		if success:
			return success, header_string + latex_string + footer_string
		else:
			return success, ""
		
		
