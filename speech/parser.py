from generator import Generator

class TokenString:
	def __init__(self):
		self.string_content = ""
		
	def append(self, text):
		self.string_content = (self.string_content + text)
	
	def get(self):
		return self.string_content
		   
	def clear(self):
		self.string_content = ""

class Parser:
	def __init__(self, break_threshold=1.0):
		self.string = TokenString()
		self.generator = Generator()
		self.break_token = "end "
		self.break_threshold = break_threshold
	  
	def add_tokens(self, metadata):
		word = ""
		transcripts = metadata.transcripts
		transcript = transcripts[0]
		tokens = transcript.tokens
		last_token_space = False
		last_token_time=0
		
		#Process the list of tokens
		for token in tokens:
			self.string.append(token.text)
			if token.text == ' ':
				if (token.start_time-last_token_time) > self.break_threshold:
					self.string.append(self.break_token)
			last_token_time = token.start_time
			
		#add newline
		self.string.append("\n")
			
	def finalize(self):
		self.string.append(" ")
	
	def get_latex_string(self):
		success, output = self.generator.generate_latex_string(self.string.get())
		if success:
			return output
		else:
			return "ERROR: Input is not valid LaTeX. Input: " + self.string.get()
	
	def get_latex_doc(self):  
		success, output = self.generator.generate_latex_doc(self.string.get())
		if success:
			return output
		else:
			return "ERROR: Input is not valid LaTeX"
	
	def clear(self):
	  self.token_string.clear()
