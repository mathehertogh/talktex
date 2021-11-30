import ctypes as ct 

GENERATOR_PATH=libcompiler_latex_generator.so
LATEX_MAX_SIZE = 1048576 #1MB should do for now right?

class Generator:
    def __init__(self):
        self.lib = ct.cdll.LoadLibrary(GENERATOR_PATH)
        
        #Function that converts a token string into latex
        self.gen = self.lib.convert_and_return
        self.gen.restype = ct.c_char_p
        self.gen.argtypes = [ct.c_char_p, ct.c_char_p, ct.c_size_t]
        
        #Function that generates a header for a latex document
        self.header = self.lib.talktex_header
        self.header.argtypes = [ct.c_char_p, ct.size_t]
        
        #Function that generates a footer for a latex document
        self.footer = self.lib.talktex_footer
        self.footer.argtypes = [ct.c_char_p, ct.size_t]
        
        
    '''Returns whether a conversion to LaTeX from token_string succeeded
    and if it did, also returns the resulting LaTeX string.'''
    def generate_latex_string(self, token_string):
        latex_buffer = ct.create_string_buffer(LATEX_MAX_SIZE)
        success = self.gen(token_string, latex_string, ct.sizeof(latex_buffer))
        
        #Only return the latex string if the parsing was actually successful
        if success:
            return success, latex_string.value
        else:
            return success, _
            
            
    '''Returns whether a conversion to LaTeX from token_string succeeded
    and if it did, also returns the resulting full LaTeX document.'''
    def generate_latex_doc(self, token_string):
        #Generate the header string
        header_buffer = ct.create_string_buffer(LATEX_MAX_SIZE)
        self.header(header_buffer, ct_sizeof(header_buffer))
        header_string = header_buffer.value
        
        #Generate the footer string
        footer_buffer = ct.create_string_buffer(LATEX_MAX_SIZE)
        self.footer(footer_buffer, ct.sizeof(footer_buffer))
        footer_string = footer_buffer.value
        
        #If appliccable, put everything together and return
        success, latex_string = generate_latex_string(token_string)
        if success:
            return success, header_string + latex_string + footer_string
        else:
            return success, _
        
        
