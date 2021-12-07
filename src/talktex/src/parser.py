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
	def __init__(self, script_dir, break_threshold=1.0):
		self.current_string = ""
		self.current_output = ""
		self.prev_string = ""
		self.prev_output = ""
		self.generator = Generator(script_dir)
		self.break_token = "end "
		self.break_threshold = break_threshold

	def add_tokens(self, metadata):
		transcripts = metadata.transcripts
		transcript = transcripts[0]
		tokens = transcript.tokens
		last_token_space = False
		last_token_time=0

		#Process the list of tokens
		for token in tokens:
			self.current_string += token.text
			if token.text == ' ':
				if (token.start_time-last_token_time) > self.break_threshold:
					self.current_string += self.break_token
			last_token_time = token.start_time

		#add newline
		self.current_string += "\n"

	def finalize(self):
		self.current_string += " "

	def get_latex_string(self):
		success, self.current_output = self.generator.generate_latex_string(self.current_string)
		if success:
			self.prev_string = self.current_string
			self.prev_output = self.current_output
			return success, self.current_output
		else:
			print("ERROR: Input is not valid LaTeX.\nINPUT:\n" + self.current_string)
			self.current_string = self.prev_string
			self.current_output = self.prev_output
			return success, self.prev_output

	def get_latex_doc(self):
		success, latex_output = self.generator.generate_latex_doc(self.current_string)
		if success:
			return latex_output
		else:
			return "ERROR: Input is not valid LaTeX"

	def clear(self):
		self.current_string = ""
		self.current_output = ""
		self.prev_string = ""
		self.prev_output = ""
