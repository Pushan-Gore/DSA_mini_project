READ ME FILE

**** Pushan Gore  ****
**** 111503023    ****
**** Compression  ****

Project : LZW Compression ( My Version )

	Compressing:

	The Project uses the LZW compression algorithm (My modified version) to compress a "single"* file.
	I have used my modified version of LZW algorithm (similar to LZ77) for this project.
	The algorithm reads characters from the file and writes them to the dictionary.
	
	Compression algorithm is:
   	BEGIN:
		s = next input character
		while not EOF
			{ c = next input character
			  if s + c exists in dictionary 
				s = s + c
			  else {
				output the code for s;
				add string s + c to the dictionary with a new code
				s = c
			  }
			}
		output the code for s
   	END

	Check "./project --help" for more information.
	The program takes a single file for compression and compresses it into a '.mtz' format file.(With same name)
	Example: 
		./project -c example
		>> example.mtz will be created.

	
	Decompressing or extracting:

	Extraction algorithm is :
 	  BEGIN:
		s = NILL
   		while not EOF {
			k = next input code
			entry = dictionary entry for k
			//exception case
			if(entry == NULL)
				entry = s + s[0]
			output entry
			if(s != NILL) 
				add string s + entry[0] to dictionary with a new code
			s = entry
		}
	   END
 
	Check "./project --help" for more information.
	The program takes a single file for decompressing or extracting. The file should have a '.mtz' extension.
	The outputed file will have the same name without the '.mtz' extension.
	Example:
		./project -x example.mtz
		>> example will be created.
	
		
	*(Multiple files are not compressed. See the 'expected final project' section.)	

Expected Final Project:
	The final project should compress and extract MULTIPLE files (compressed into a single file)


